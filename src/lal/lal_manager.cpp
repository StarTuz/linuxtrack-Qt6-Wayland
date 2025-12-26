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
  (void)id;
  (void)archivePath;
  // TODO: Implement 7z extraction logic
  return false;
}

bool LALManager::checkHash(const std::string &filePath,
                           const std::string &expectedHash) {
  (void)filePath;
  (void)expectedHash;
  return false; // Stub
}

bool LALManager::extractFiles(const std::string &tool,
                              const std::string &archivePath,
                              const std::string &destDir) {
  (void)tool;
  (void)archivePath;
  (void)destDir;

  // Stub implementation for 7z
  // In production this would use QProcess or fork/exec
  // std::string cmd = "7z e -y -o" + destDir + " " + archivePath;
  // int res = std::system(cmd.c_str());
  return false;
}

} // namespace lal
