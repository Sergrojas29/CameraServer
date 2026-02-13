
#include "CollageCreate.h"
#include "CameraClient.h"
#include <algorithm>
#include <array>
#include <netdb.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

void applyOverlay(cv::Mat &background, cv::Mat &overlay, cv::Point point) {
  int rows = background.rows;
  int cols = background.cols;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      // Calculate position in in the destination (overlay) image
      int overlayX = point.x + x;
      int overlayY = point.y + y;

      // Check bounds to avoid out-of-range access
      if (overlayX < 0 || overlayX >= overlay.cols || overlayY < 0 ||
          overlayY >= overlay.rows) {
        continue;
      }

      // Get references to both pixels
      cv::Vec4b &overlayPixel = overlay.at<cv::Vec4b>(overlayY, overlayX);
      cv::Vec3b &bgPixel = background.at<cv::Vec3b>(y, x);

      float alpha = overlayPixel[3] / 255.0f;

      // blend
      for (int c = 0; c < 3; c++) {
        overlayPixel[c] =
            (overlayPixel[c] * alpha) + (bgPixel[c] * (1.0f - alpha));
      }

      // IMPORTANT : the pixel is no longer transparent.
      overlayPixel[3] = 255;
    }
  }
}

stringOptional CollageCreate::portraitCollage(const std::string &imagePath,
                                              const std::string &overlayPath,
                                              SessionInfo &SessionInfo) {
  try {
    // Create Object Path Objecst
    std::filesystem::path imageObj(imagePath);
    std::filesystem::path overlayObj(overlayPath);

    // load Images
    cv::Mat img = cv::imread(imageObj.string());
    cv::Mat overImg = cv::imread(overlayObj.string(), cv::IMREAD_UNCHANGED);

    if (overImg.channels() < 4) {
      throw std::runtime_error("Overlay Image doesn't have an alpha layer");
    }

    // Reduce Img Size
    cv::Mat resizeImg;
    int height = overImg.rows;
    int width = overImg.cols;
    cv::Size newSize(width - 200, height - 200);
    cv::resize(img, resizeImg, newSize);

    // BlendImage
    applyOverlay(resizeImg, overImg, cv::Point(100, 100));

    // TODO Create UUID for Blended Image
    std::string OutputPath =
        SessionInfo.save_path + "/" + imageObj.stem().string() + "blended.jpg";
    bool successs = cv::imwrite(OutputPath, overImg);

    if (!successs) {
      throw std::runtime_error("Unable to write Overlay Image");
    }

    // UpdateSession Collage Paths
    SessionInfo.collagePaths.push_back(OutputPath);

    return OutputPath;

  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return std::nullopt;
  }
}

stringOptional
CollageCreate::templateCollage_4(const std::vector<std::string> &imagePaths,
                                 const std::string &overlayPath,
                                 SessionInfo &SessionInfo) {
  try {
    std::vector<cv::Point> locations = {
        cv::Point(100, 100), cv::Point(2050, 100), cv::Point(100, 3050),
        cv::Point(2050, 3050)};

    // Get Overlay file Object
    std::filesystem::path overlayObj(overlayPath);
    // TODO file checker

    // Create cv Matrix for overlay png
    cv::Mat overImg = cv::imread(overlayObj.string(), cv::IMREAD_UNCHANGED);

    int index = 0;
    for (auto &&file : imagePaths) {
      // Get img file object
      std::filesystem::path imageObj(file);
      cv::Mat img = cv::imread(imageObj.string());

      // Reduce Img Size
      cv::Mat resizeImg;
      int height = (overImg.rows / 2) - 150;
      int width = (overImg.cols / 2) - 150;
      cv::Size newSize(width, height);
      cv::resize(img, resizeImg, newSize);

      applyOverlay(resizeImg, overImg, locations[index]);
      index++;
    }

    // Write Blended Image template4
    //  TODO Create UUID for blended Image
    std::string OutputPath =
        SessionInfo.save_path + "/" + SessionInfo.sessionID + "4blended.jpg";
    bool successs = cv::imwrite(OutputPath, overImg);

    if (!successs) {
      throw std::runtime_error("Unable to write Overlay Image");
    }

    // UpdateSession Collage Paths
    SessionInfo.collagePaths.push_back(OutputPath);

    return OutputPath;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return std::nullopt;
  };
}
