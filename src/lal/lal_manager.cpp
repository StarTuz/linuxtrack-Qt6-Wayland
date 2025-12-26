#include "lal_manager.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace lal {

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
  // TODO: Implement actual check
  // Check if expected destination files exist in
  // ~/.local/share/linuxtrack/lal/<id>/
  (void)id;
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
