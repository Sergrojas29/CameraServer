#include "ImageEffect.h"
#include "SessionInfo.h"
#include <opencv2/core/mat.hpp>

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

std::unordered_map<std::string, ImageEffect> createEffectMap() {
  std::unordered_map<std::string, ImageEffect> map;

  map["PRT_1_SPC_EVENT_FATALE-VERMILIONSKII"] = [](SessionInfo &session) {
    std::string overlayPath =
        OVERLAY_FILE_LOCATION + "/PRT_1_SPC_EVENT_FATALE-VERMILIONSKII.png";

    #pragma omp parallel for
    for (auto &&file : session.photoPaths) {
      // load Images
      cv::Mat img = cv::imread(file);
      cv::Mat overImg = cv::imread(overlayPath, cv::IMREAD_UNCHANGED);

      // Reduce Img Size
      cv::Mat resizeImg;
      int height = overImg.rows;
      int width = overImg.cols;
      cv::Size newSize(width - 200, height - 200);
      cv::resize(img, resizeImg, newSize);

      // BlendImage
      applyOverlay(resizeImg, overImg, cv::Point(100, 100));

      std::string OutputPath = session.save_path +"/" +session.generateUUID() + ".jpg";

      cv::imwrite(OutputPath, overImg);

      session.collagePaths.push_back(OutputPath);
    }

    return true;
  };

  return map;
}
