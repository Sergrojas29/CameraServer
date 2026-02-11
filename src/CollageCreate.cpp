
#include "CollageCreate.h"
#include "CameraClient.h"
#include <algorithm>
#include <array>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <vector>

std::string removeExten(const std::string &filename) {
  std::regex rex(R"(^(?:.*\/)?([^/]+)\..*$)");
  std::string result = std::regex_replace(filename, rex, "$1");
  return result;  
}




void applyOverlay(cv::Mat &background, const cv::Mat &overlay) {

  int rows = overlay.rows;
  int cols = overlay.cols;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      cv::Vec4b overlayPixel = overlay.at<cv::Vec4b>(y, x);

      float alpha = overlayPixel[3] / 255.0f;

      if (alpha < 0.1)
        continue;

      cv::Vec3b &bgPixel = background.at<cv::Vec3b>(y, x);

      // blend
      for (int c = 0; c < 3; c++) {
        bgPixel[c] = (overlayPixel[c] * alpha) + (bgPixel[3] * (1.0f - alpha));
      }
    }
  }
}






std::string
CollageCreate::SinglePortraitCollage(const std::string &imageFileName,
                                     const std::string &overlayFileName) {

  std::string outputPath = COLLAGE_FILE_LOCATION +
                           removeExten(overlayFileName) +
                           removeExten(imageFileName) + ".png";


  // std::cout << "outputPath: " << outputPath << std::endl;
  // std::cout << "imageFileName: " << imageFileName << std::endl;
  // std::cout << "overlayFileName: " << overlayFileName << std::endl;

  cv::Mat originalImage = cv::imread(imageFileName);
  cv::Mat overlayImage =
      cv::imread(OVERLAY_FILE_LOCATION + overlayFileName, cv::IMREAD_UNCHANGED);

  try {
    if (originalImage.empty())
      throw std::runtime_error(
          "Original Image could not be read. Check File path");
    if (overlayImage.empty())
      throw std::runtime_error(
          "Overlay Image could not be read. Check File path");

    int height = overlayImage.rows;
    int width = overlayImage.cols;

    // (W, H)
    cv::Size newSize(width, height);
    cv::Mat resizeImage;
    cv::resize(originalImage, resizeImage, newSize);

    applyOverlay(resizeImage, overlayImage);

    bool successs = cv::imwrite(outputPath, resizeImage);

    if (!successs)
      throw std::runtime_error("Error: Could NOT save image");

    std::cout << "Success! Save resized image" << std::endl;
    return outputPath;

  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return "Error";
  }
}

std::array<std::string, 4> CollageCreate::createSinglePortraitCollageList(
    const std::array<std::string, PHOTOS_PER_SESSION> &imagePaths,
    const std::string &overlayFileName) {

  std::cout << "Processing " << PHOTOS_PER_SESSION
            << " images with parallel STL...\n";
  std::array<std::string, 4> results = {};

  std::transform(std::execution::par, imagePaths.begin(), imagePaths.end(),
                 results.begin(), [&overlayFileName](const std::string &img) {
                   return CollageCreate::SinglePortraitCollage(img,
                                                               overlayFileName);
                 });

  return results;
}
