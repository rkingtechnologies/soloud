#ifndef RKT_SOLOUD_DEMOS_COMMON_ASSET_MANAGER_H_
#define RKT_SOLOUD_DEMOS_COMMON_ASSET_MANAGER_H_

#include <filesystem>
#include <optional>

namespace demo_asset_manager {

std::optional<std::filesystem::path> FindDemoAssetsDir(bool use_cache = true);

}

#endif  // RKT_SOLOUD_DEMOS_COMMON_ASSET_MANAGER_H_