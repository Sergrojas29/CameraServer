#pragma once
#include "CameraClient.h"
#include "cpr/session.h"
#include <algorithm>
#include <array>
#include <execution>
#include <filesystem>
#include <fstream>
#include <functional>
#include <opencv2/opencv.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// TODO: Choose where the overlay defualt loaction
inline constexpr std::string OVERLAY_FILE_LOCATION = "overlays/";
inline constexpr std::string COLLAGE_FILE_LOCATION = "collages/";

using stringOptional = std::optional<std::string>;

class CollageCreate {

  
public:
  // TODO: Parallel Computeing to be worked out

  static stringOptional portraitCollage(const std::string &imagePath,
                                        const std::string &overlayPath,
                                        SessionInfo &SessionInfo);

  static stringOptional
  templateCollage_4(const std::vector<std::string> &imagePaths,
                    const std::string &overlayPath, SessionInfo &SessionInfo);
};
