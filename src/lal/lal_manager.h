#ifndef LAL_MANAGER_H
#define LAL_MANAGER_H

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace lal {

enum class AssetStatus { MISSING, DOWNLOADING, INSTALLED, CORRUPT, UNKNOWN };

struct AssetSource {
  std::string type; // "url", "manual_download"
  std::string url;
  std::string sha256;
  std::string filename;
};

struct AssetDefinition {
  std::string id;
  std::string name;
  std::string vendor;
  std::string version;
  std::string description;
  bool critical;
  std::vector<AssetSource> sources;
  // ... extraction logic details
};

class LALManager {
public:
  static LALManager &instance();

  // Initialization
  bool loadManifest(const std::string &manifestPath);

  // Query
  std::vector<std::string> getAssetIds() const;
  AssetDefinition getAssetDefinition(const std::string &id) const;
  AssetStatus getAssetStatus(const std::string &id) const;

  // Actions
  bool verifyAsset(const std::string &id);
  bool installAssetFromArchive(const std::string &id,
                               const std::string &archivePath);

private:
  LALManager() = default;
  ~LALManager() = default;
  LALManager(const LALManager &) = delete;
  LALManager &operator=(const LALManager &) = delete;

  std::map<std::string, AssetDefinition> assets;
  nlohmann::json manifestJson;

  // Validates SHA256 of a file
  bool checkHash(const std::string &filePath, const std::string &expectedHash);

  // Runs external extraction tool (7z, tar)
  bool extractFiles(const std::string &tool, const std::string &archivePath,
                    const std::string &destDir);
};

} // namespace lal

#endif // LAL_MANAGER_H
