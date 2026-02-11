#include <gtest/gtest.h>
#include "CameraClient.h"



TEST(CameraConnection, ONE_RUN){
    CameraClient TestCamera;

    ASSERT_TRUE(TestCamera.connect());
    EXPECT_TRUE(TestCamera.isConnected());
    TestCamera.setupFolder("NewTestFolder");
    
    EXPECT_TRUE(TestCamera.createSession());

    EXPECT_TRUE(TestCamera.capturePhoto());
    EXPECT_TRUE(TestCamera.capturePhoto());
    EXPECT_TRUE(TestCamera.capturePhoto());
    EXPECT_TRUE(TestCamera.capturePhoto());
    
    EXPECT_TRUE(TestCamera.endSession());

}