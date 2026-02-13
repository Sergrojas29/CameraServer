#include "CollageCreate.h"
#include "CameraClient.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>


// TEST(CollageCreate, RUNONE) {
//   std::string expectedPath = "imageTest/DSC08729blended.jpg";
//   std::string imgPath = "imageTest/DSC08729.JPG";
//   std::string overPath = "imageTest/1Template.png";

//   stringOptional outputPath = CollageCreate::portraitCollage(imgPath, overPath);

//   EXPECT_EQ(outputPath, expectedPath);
// }

// TEST(CollageCreate, RUNONE_TEMPLATE4) {
//   std::string expectedPath = "imageTest/4blended.jpg";
//   std::string overPath = "imageTest/4Template.png";

//   std::vector<std::string> imgPaths = {
//       "imageTest/_MG_0043.JPG",
//       "imageTest/_MG_0044.JPG",
//       "imageTest/_MG_0045.JPG",
//       "imageTest/_MG_0046.JPG",
//   };

//   stringOptional outputPath = CollageCreate::templateCollage_4(imgPaths, overPath);

//   EXPECT_EQ(outputPath, expectedPath);
// }