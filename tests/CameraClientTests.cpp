#include "CameraClient.h"
#include "CollageCreate.h"
#include "UploadClient.h"
#include <gtest/gtest.h>
#include <iostream>
#include <optional>

// init Camera
CameraClient TestCamera;

std::string OVERLAY_1_TEMPLATE = "imageTest/1Template.png";
std::string OVERLAY_4_TEMPLATE = "imageTest/4Template.png";

// TEST(CameraConnection, ONE_RUN_PORTRAIT_ONE_COLLAGE) {

//   ASSERT_TRUE(TestCamera.connect());
//   EXPECT_TRUE(TestCamera.isConnected());
//   TestCamera.setupFolder("TEST_ONE_SESSION");

//   EXPECT_TRUE(TestCamera.createSession());

//   EXPECT_TRUE(TestCamera.capturePhoto());
//   EXPECT_TRUE(TestCamera.capturePhoto());
//   EXPECT_TRUE(TestCamera.capturePhoto());
//   EXPECT_TRUE(TestCamera.capturePhoto());


//   for(auto&& img: TestCamera.getPhotoPaths()){
//     auto path = CollageCreate::portraitCollage(img, OVERLAY_1_TEMPLATE);
//     if(path.has_value()){
//       TestCamera.session.collagePaths.push_back(path.value());
//     }
//   }

  


//   EXPECT_TRUE(TestCamera.endSession());
// }

TEST(CameraConnection, ONE_RUN_PORTRAIT_FOUR_COLLAGE) {
  ASSERT_TRUE(TestCamera.connect());
  EXPECT_TRUE(TestCamera.isConnected());
  TestCamera.setupFolder("TEST_ONE_SESSION");

  EXPECT_TRUE(TestCamera.createSession());

  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());


  CollageCreate::templateCollage_4( TestCamera.getPhotoPaths(), OVERLAY_4_TEMPLATE, TestCamera.session);

  CollageCreate::portraitCollage(TestCamera.session.photoPaths[0], OVERLAY_1_TEMPLATE , TestCamera.session);
  CollageCreate::portraitCollage(TestCamera.session.photoPaths[1], OVERLAY_1_TEMPLATE , TestCamera.session);
  CollageCreate::portraitCollage(TestCamera.session.photoPaths[2], OVERLAY_1_TEMPLATE , TestCamera.session);

  for(auto&& collageFile : TestCamera.session.collagePaths){
    auto url = UploadClient::UploadImage(collageFile);
    if( url.has_value()){
      std::cout << url.value() << std::endl;
    }
  }

  EXPECT_TRUE(TestCamera.endSession());
}