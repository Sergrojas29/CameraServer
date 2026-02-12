#pragma once
#include "CameraClient.h"
#include <algorithm>
#include <array>
#include <execution>
#include <functional>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <optional>
#include <filesystem>
#include <fstream>
#include <vector>

using stringOptional = std::optional<std::string>;

class CollageCreate {
private:
  static std::string overlayFileLocation;
  static std::string collageFileLocation;

public:
  /**
   * @param
   */

  // TODO Remove any instance of this method
  static std::string SinglePortraitCollage(const std::string &imageFileName,
                                           const std::string &overlayFileName){
                                            return "NO METHOD";
                                           };

  static std::array<std::string, PHOTOS_PER_SESSION>
  createSinglePortraitCollageList(
      const std::array<std::string, PHOTOS_PER_SESSION> &imagePaths,
      const std::string &overlayFileName);

  static stringOptional portraitCollage(const std::string &imagePath,
  const std::string &overlayPath);

  static stringOptional templateCollage_4(const std::vector<std::string> &imagePaths,
  const std::string &overlayPath);
  
};
