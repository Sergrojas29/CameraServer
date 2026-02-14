#include "CameraClient.h"
#include "CollageCreate.h"
#include "SessionInfo.h"
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

  EXPECT_TRUE(TestCamera.createSession("PRT_1_SPC_EVENT_FATALE-VERMILIONSKII"));

  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
  EXPECT_TRUE(TestCamera.capturePhoto());
}

TEST(ONE_RUN, CREATE_COLLAGE) {

  EXPECT_TRUE(CollageCreate::creatCollage(TestCamera.session));
}

TEST(ONE_RUN, UPLOAD_PHOTOS) {

  EXPECT_TRUE(UploadClient::UploadImages(TestCamera.session));

  UploadClient::saveToJSON("db.json", TestCamera.session);
}

