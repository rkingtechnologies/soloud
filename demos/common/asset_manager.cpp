#include "asset_manager.h"

#include <array>
#include <print>
#include <deque>

namespace demo_asset_manager {

std::optional<std::filesystem::path> FindDemoAssetsDir(bool use_cache) {
  static std::optional<std::filesystem::path> cached_path = std::nullopt;

  if (use_cache && cached_path != std::nullopt) {
    return cached_path;
  }

  const static std::array<std::filesystem::path, 33> possible_structures = {
  "assets",
  "demos/assets",
  "soloud/demos/assets",
  "external/soloud/demos/assets",
  "extern/soloud/demos/assets",
  "third_party/soloud/demos/assets",
  "deps/soloud/demos/assets",
  "dependencies/soloud/demos/assets",
  "vendor/soloud/demos/assets",

  "build/demos/assets",
  "build/soloud/demos/assets",
  "build/external/soloud/demos/assets",
  "build/extern/soloud/demos/assets",
  "build/third_party/soloud/demos/assets",
  "build/deps/soloud/demos/assets",
  "build/dependencies/soloud/demos/assets",
  "build/vendor/soloud/demos/assets",

  "bin/demos/assets",
  "bin/soloud/demos/assets",
  "bin/external/soloud/demos/assets",
  "bin/extern/soloud/demos/assets",
  "bin/third_party/soloud/demos/assets",
  "bin/deps/soloud/demos/assets",
  "bin/dependencies/soloud/demos/assets",
  "bin/vendor/soloud/demos/assets",

  "install/demos/assets",
  "install/soloud/demos/assets",
  "install/external/soloud/demos/assets",
  "install/extern/soloud/demos/assets",
  "install/third_party/soloud/demos/assets",
  "install/deps/soloud/demos/assets",
  "install/dependencies/soloud/demos/assets",
  "install/vendor/soloud/demos/assets",
};

  std::deque<std::filesystem::path> possible_paths;

  constexpr int back_dir_levels = 5;

  // Add relative paths based on current working directory.
  std::filesystem::path prefix_path = ".";

  for (int i = 0; i <= back_dir_levels; ++i) {
    for (const auto& structure : possible_structures) {
      possible_paths.push_back(prefix_path / structure);
    }
    prefix_path /= "..";
  }


#ifdef SOLOUD_DEMO_ASSETS_DIR  // from cmake define
  possible_paths.push_front(SOLOUD_DEMO_ASSETS_DIR);
#endif

  for (const auto& path : possible_paths) {
    std::println("Checking for demo assets in: {}", path.string());

    if (!std::filesystem::exists(path)) [[likely]] {
      continue;
    }

    if (!std::filesystem::exists(path / "soloud_assets_stamp.txt"))
      [[unlikely]] {
      continue;
    }

    std::println("Found demo assets dir: {}", path.string());

    cached_path = std::filesystem::canonical(path);

    return cached_path;
  }
  return std::nullopt;
}

}  // namespace demo_asset_manager