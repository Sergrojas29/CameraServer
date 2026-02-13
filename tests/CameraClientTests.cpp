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

TEST(ONE_RUN, CONNECT) {

  ASSERT_TRUE(TestCamera.connect());
  EXPECT_TRUE(TestCamera.isConnected());
  TestCamera.setupFolder("TEST_ONE_SESSION");
}

TEST(ONE_RUN, TAKE_PHOTO) {

  EXPECT_TRUE(TestCamera.createSession());

  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
}

TEST(ONE_RUN, CREATE_COLLAGE) {

  CollageCreate::templateCollage_4(TestCamera.getPhotoPaths(),
                                   OVERLAY_4_TEMPLATE, TestCamera.session);

  CollageCreate::portraitCollage(TestCamera.session.photoPaths[0],
                                 OVERLAY_1_TEMPLATE, TestCamera.session);
  CollageCreate::portraitCollage(TestCamera.session.photoPaths[1],
                                 OVERLAY_1_TEMPLATE, TestCamera.session);
  CollageCreate::portraitCollage(TestCamera.session.photoPaths[2],
                                 OVERLAY_1_TEMPLATE, TestCamera.session);
}

TEST(ONE_RUN, UPLOAD_PHOTOS) {

  for (auto &&collageFile : TestCamera.session.collagePaths) {
    auto url = UploadClient::UploadImage(collageFile);
    UploadClient::saveToJSON("db.json", url.value());
  }

}

TEST(ONE_RUN, SAVE_TO_JSON) {}
