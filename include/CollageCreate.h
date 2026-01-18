#pragma once
#include "CameraClient.h"
#include <array>
#include <functional>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <execution>


class CollageCreate {
private:
  static std::string overlayFileLocation;
  static std::string collageFileLocation;

public:
  /**
   * @param
   */
  static std::string SinglePortraitCollage(const std::string &imageFileName,
                                           const std::string &overlayFileName);

  static std::array<std::string, PHOTOS_PER_SESSION> createSinglePortraitCollageList(
    const std::array<std::string, PHOTOS_PER_SESSION> &imagePaths, const std::string& overlayFileName);
};
