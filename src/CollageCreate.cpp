
#include "CollageCreate.h"
#include "CameraClient.h"
#include "ImageEffect.h"
#include <stdexcept>



// stringOptional CollageCreate::portraitCollage(const std::string &imagePath,
//                                               const std::string &overlayPath,
//                                               SessionInfo &SessionInfo) {
//   try {
//     // Create Object Path Objecst
//     std::filesystem::path imageObj(imagePath);
//     std::filesystem::path overlayObj(overlayPath);

//     // load Images
//     cv::Mat img = cv::imread(imageObj.string());
//     cv::Mat overImg = cv::imread(overlayObj.string(), cv::IMREAD_UNCHANGED);

//     if (overImg.channels() < 4) {
//       throw std::runtime_error("Overlay Image doesn't have an alpha layer");
//     }

//     // Reduce Img Size
//     cv::Mat resizeImg;
//     int height = overImg.rows;
//     int width = overImg.cols;
//     cv::Size newSize(width - 200, height - 200);
//     cv::resize(img, resizeImg, newSize);

//     // BlendImage
//     applyOverlay(resizeImg, overImg, cv::Point(100, 100));

//     // TODO Create UUID for Blended Image
//     std::string OutputPath =
//         SessionInfo.save_path + "/" + imageObj.stem().string() + "blended.jpg";
//     bool successs = cv::imwrite(OutputPath, overImg);

//     if (!successs) {
//       throw std::runtime_error("Unable to write Overlay Image");
//     }

//     // UpdateSession Collage Paths
//     SessionInfo.collagePaths.push_back(OutputPath);

//     return OutputPath;

//   } catch (const std::exception &e) {
//     std::cerr << "Exception caught: " << e.what() << std::endl;
//     return std::nullopt;
//   }
// }

// stringOptional
// CollageCreate::templateCollage_4(const std::vector<std::string> &imagePaths,
//                                  const std::string &overlayPath,
//                                  SessionInfo &SessionInfo) {
//   try {
//     std::vector<cv::Point> locations = {
//         cv::Point(100, 100), cv::Point(2050, 100), cv::Point(100, 3050),
//         cv::Point(2050, 3050)};

//     // Get Overlay file Object
//     std::filesystem::path overlayObj(overlayPath);
//     // TODO file checker

//     // Create cv Matrix for overlay png
//     cv::Mat overImg = cv::imread(overlayObj.string(), cv::IMREAD_UNCHANGED);

//     int index = 0;
//     for (auto &&file : imagePaths) {
//       // Get img file object
//       std::filesystem::path imageObj(file);
//       cv::Mat img = cv::imread(imageObj.string());

//       // Reduce Img Size
//       cv::Mat resizeImg;
//       int height = (overImg.rows / 2) - 150;
//       int width = (overImg.cols / 2) - 150;
//       cv::Size newSize(width, height);
//       cv::resize(img, resizeImg, newSize);

//       applyOverlay(resizeImg, overImg, locations[index]);
//       index++;
//     }

//     // Write Blended Image template4
//     //  TODO Create UUID for blended Image
//     std::string OutputPath =
//         SessionInfo.save_path + "/" + SessionInfo.sessionID + "4blended.jpg";
//     bool successs = cv::imwrite(OutputPath, overImg);

//     if (!successs) {
//       throw std::runtime_error("Unable to write Overlay Image");
//     }

//     // UpdateSession Collage Paths
//     SessionInfo.collagePaths.push_back(OutputPath);

//     return OutputPath;
//   } catch (const std::exception &e) {
//     std::cerr << "Exception caught: " << e.what() << std::endl;
//     return std::nullopt;
//   };
// }


bool CollageCreate::creatCollage(SessionInfo& session){
  try {
    if (!myFilters.contains(session.imageEffect)) {
      throw std::runtime_error("Filter DOES NOT EXSIST : " + session.imageEffect);
    }
    bool successs = myFilters.at(session.imageEffect)(session);
    return successs;
  } catch (const std::exception& e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }; 
}