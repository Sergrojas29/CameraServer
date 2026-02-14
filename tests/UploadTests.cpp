// #include "CameraClient.h"
// #include "CollageCreate.h"
// #include "SessionInfo.h"
// #include "UploadClient.h"
// #include <gtest/gtest.h>
// #include <stdexcept>
// #include <string>
// #include <vector>

// std::vector<std::string> paths = {
//     "imageTest/DSC08494.jpg", "imageTest/DSC08495.jpg",
//     "imageTest/DSC08496.jpg", "imageTest/DSC08505.jpg",
//     "imageTest/DSC08729.JPG",
// };

// CameraClient TestCam;
// TEST(Session_Testing, RUNONE) {
//     TestCam.connect();
    
//     TestCam.createSession("PRT_1_SPC_EVENT_FATALE-VERMILIONSKII");
//     TestCam.session.photoPaths = paths;
//     CollageCreate::creatCollage(TestCam.session);
//     UploadClient::UploadImages(TestCam.session);
//     UploadClient::saveToJSON("db.json", TestCam.session);
//     TestCam.endSession();


    
// }

