#include "lal_manager.h"
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>

namespace fs = std::filesystem;

namespace lal {

static std::string shellQuote(const std::string &s) {
  std::string out = "'";
  for (char c : s) {
    if (c == '\'') {
      out += "'\\''";
    } else {
      out += c;
    }
  }
  out += "'";
  return out;
}

static fs::path linuxtrackConfigDir() {
  const char *xdg = std::getenv("XDG_CONFIG_HOME");
  if (xdg && xdg[0] != '\0') {
    return fs::path(xdg) / "linuxtrack";
  }
  const char *home = std::getenv("HOME");
  if (!home) {
    return {};
  }
  return fs::path(home) / ".config/linuxtrack";
}

static fs::path executableSibling(const char *name) {
  std::array<char, 4096> buf{};
  ssize_t len = readlink("/proc/self/exe", buf.data(), buf.size() - 1);
  if (len <= 0) {
    return {};
  }
  buf[static_cast<size_t>(len)] = '\0';
  return fs::path(buf.data()).parent_path() / name;
}

static fs::path findLtrExtractor() {
  fs::path sibling = executableSibling("ltr_extractor");
  if (!sibling.empty() && fs::exists(sibling)) {
    return sibling;
  }
  fs::path buildSibling = executableSibling("../ltr_extractor");
  if (!buildSibling.empty() && fs::exists(buildSibling)) {
    return buildSibling;
  }
  return fs::path("ltr_extractor");
}

static bool installTrackIrFirmware(const std::string &archivePath,
                                   const fs::path &installDir) {
  fs::path selected(archivePath);
  fs::path trackIrDir;

  if (selected.filename() == "TrackIR5.exe") {
    trackIrDir = selected.parent_path();
  } else if (fs::exists(selected.parent_path() / "TrackIR5.exe")) {
    trackIrDir = selected.parent_path();
  } else {
    const char *home = std::getenv("HOME");
    if (home) {
      fs::path wineTrackIr =
          fs::path(home) /
          ".wine/drive_c/Program Files (x86)/TrackIR5";
      if (fs::exists(wineTrackIr / "TrackIR5.exe")) {
        trackIrDir = wineTrackIr;
      }
    }
  }

  if (trackIrDir.empty()) {
    std::cerr << "LAL: TrackIR installer bundles cannot be decoded directly "
                 "yet; install TrackIR with Wine first, then select "
                 "TrackIR5.exe from the installed TrackIR5 directory."
              << std::endl;
    return false;
  }

  fs::path exe = trackIrDir / "TrackIR5.exe";
  fs::path sgl = trackIrDir / "sgl.dat";
  if (!fs::exists(exe) || !fs::exists(sgl)) {
    std::cerr << "LAL: Missing TrackIR5.exe or sgl.dat in " << trackIrDir
              << std::endl;
    return false;
  }

  fs::create_directories(installDir);
  fs::copy_file(exe, installDir / "TrackIR5.exe",
                fs::copy_options::overwrite_existing);
  fs::copy_file(sgl, installDir / "sgl.dat",
                fs::copy_options::overwrite_existing);

  fs::path cfg = linuxtrackConfigDir() / "tir_firmware";
  fs::create_directories(cfg);

  fs::path tirViews = trackIrDir / "TIRViews.dll";
  if (fs::exists(tirViews)) {
    fs::copy_file(tirViews, cfg / "TIRViews.dll",
                  fs::copy_options::overwrite_existing);
  }

  fs::path extractor = findLtrExtractor();
  std::string cmd = shellQuote(extractor.string()) + " --extract " +
                    shellQuote(exe.string()) + " " + shellQuote(sgl.string());
  std::cout << "LAL Executing: " << cmd << std::endl;
  if (std::system(cmd.c_str()) != 0) {
    return false;
  }

  fs::path gamedata = cfg / "gamedata.txt";
  return fs::exists(gamedata) && fs::file_size(gamedata) > 0;
}

LALManager &LALManager::instance() {
  static LALManager instance;
  return instance;
}

bool LALManager::loadManifest(const std::string &manifestPath) {
  try {
    std::ifstream f(manifestPath);
    if (!f.is_open())
      return false;

    manifestJson = nlohmann::json::parse(f);

    assets.clear();
    for (const auto &item : manifestJson["assets"]) {
      AssetDefinition def;
      def.id = item["id"];
      def.name = item["name"];
      def.vendor = item.value("vendor", "Unknown");
      def.version = item["version"];
      def.description = item.value("description", "");
      def.critical = item.value("critical", false);

      for (const auto &src : item["sources"]) {
        AssetSource s;
        s.type = src["type"];
        s.url = src.value("url", "");
        s.sha256 = src.value("sha256", "");
        s.filename = src.value("filename", "");
        def.sources.push_back(s);
      }

      if (item.contains("extraction")) {
        def.extractionTool = item["extraction"].value("tool", "7z");
        if (item["extraction"].contains("map")) {
          for (auto &[key, val] : item["extraction"]["map"].items()) {
            def.fileMap[key] = val;
          }
        }
      } else {
        def.extractionTool = "7z"; // Default
      }

      assets[def.id] = def;
    }
    return true;
  } catch (const std::exception &e) {
    std::cerr << "LAL Manifest Error: " << e.what() << std::endl;
    return false;
  }
}

std::vector<std::string> LALManager::getAssetIds() const {
  std::vector<std::string> ids;
  for (const auto &pair : assets) {
    ids.push_back(pair.first);
  }
  return ids;
}

AssetDefinition LALManager::getAssetDefinition(const std::string &id) const {
  auto it = assets.find(id);
  if (it != assets.end()) {
    return it->second;
  }
  return AssetDefinition();
}

AssetStatus LALManager::getAssetStatus(const std::string &id) const {
  const char *home = std::getenv("HOME");
  if (!home) return AssetStatus::MISSING;
  
  fs::path installDir = fs::path(home) / ".local/share/linuxtrack/lal" / id;
  
  // Check if directory exists and has files
  if (fs::exists(installDir) && fs::is_directory(installDir)) {
    std::error_code ec;
    auto it = fs::directory_iterator(installDir, ec);
    if (!ec && it != fs::end(it)) {
      return AssetStatus::INSTALLED;  // Has at least one file
    }
  }
  return AssetStatus::MISSING;
}

bool LALManager::verifyAsset(const std::string &id) {
  (void)id;
  // TODO: Hash check installed files
  return false;
}

bool LALManager::installAssetFromArchive(const std::string &id,
                                         const std::string &archivePath) {
  auto it = assets.find(id);
  if (it == assets.end())
    return false;

  // Define destination: ~/.local/share/linuxtrack/lal/<id>/
  const char *home = std::getenv("HOME");
  if (!home)
    return false;

  fs::path installDir = fs::path(home) / ".local/share/linuxtrack/lal" / id;
  fs::create_directories(installDir);

  if (id == "tir_firmware_v5") {
    return installTrackIrFirmware(archivePath, installDir);
  }

  std::string tool = it->second.extractionTool;
  if (tool.empty())
    tool = "7z";

  return extractFiles(tool, archivePath, installDir.string());
}

bool LALManager::checkHash(const std::string &filePath,
                           const std::string &expectedHash) {
  // Stub: Always return true for prototype unless hash is "PENDING"
  if (expectedHash.find("PENDING") != std::string::npos)
    return true;
  return true;
}

bool LALManager::extractFiles(const std::string &tool,
                              const std::string &archivePath,
                              const std::string &destDir) {
  std::string cmd;
  if (tool == "7z") {
    // 7z e: extract
    // -y: assume yes
    // -o: output directory (must use -o{dir} no space for some 7z versions, but
    // 7z uses -o{dir})
    cmd = "7z e -y \"-o" + destDir + "\" \"" + archivePath +
          "\" > /dev/null 2>&1";
  } else if (tool == "tar") {
    cmd = "tar -xf \"" + archivePath + "\" -C \"" + destDir + "\"";
  } else {
    std::cerr << "LAL: Unknown tool " << tool << std::endl;
    return false;
  }

  std::cout << "LAL Executing: " << cmd << std::endl;
  int res = std::system(cmd.c_str());
  return (res == 0);
}

} // namespace lal
