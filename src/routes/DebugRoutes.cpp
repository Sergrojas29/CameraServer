#include "RouteModules.h"
#include <cstddef>

crow::Blueprint makeDebugBlueprint(CameraClient &cam) {

  crow::Blueprint bp("api/debug");


  CROW_BP_ROUTE(bp, "/status")
  ([&cam]() {
    nlohmann::json data;

    data["session"]["activeSession"] = cam.session.activeSession;
    data["session"]["sessionPhotoCount"] = cam.session.sessionPhotoCount;
    data["session"]["sessionID"] = cam.session.sessionID;
    data["session"]["photoPaths"] = cam.session.photoPaths;
    data["session"]["collagePaths"] = cam.session.collagePaths;

    data["m_save_path"] = cam.m_save_path;
    data["m_connected"] = cam.isConnected();

    return responseJson200(data);
  });

  CROW_BP_ROUTE(bp, "/connect")
  ([&cam]() {
    nlohmann::json data;

    //*Call to connect
    bool success = cam.connect();
    data["service"] = "connect";
    data["isConnected"] = success ? "true" : "false";

    return responseJson200(data);
  });

  CROW_BP_ROUTE(bp, "/setTargetFolder/<string>")
  ([&cam](std::string folderName) {
    nlohmann::json data;

    //*Call to create folder or reconnect to folder
    bool success = cam.setupFolder(folderName);
    data["service"] = "setTargetFolder";
    data["isCreated"] = success ? true : false;
    data["file_path"] = success ? folderName : "error";
    data["description"] = "Call to create folder or reconnect to folder";

    return responseJson200(data);
  });


  CROW_BP_ROUTE(bp, "/testCapture")
  ([&cam]() {
    nlohmann::json data;
    bool connected = cam.isConnected();

    data["service"] = "testCapture";

    if (!connected) {
      data["isTestCapture"] = false;
      data["error"] = "Camera not connected";
      return responseJson200(data);
    }

    cam.createSession();
    bool success = cam.capturePhoto();
    cam.endSession();

    data["isTestCapture"] = true;
    data["error"] = success ? "none" : "Unable to Capture photo";

    return responseJson200(data);
  });

  CROW_BP_ROUTE(bp, "/testPrint")
  ([&cam]() {
    nlohmann::json data;

    data["service"] = "testPrint";

    bool success = cam.printSelectedPhoto("testPrint/portrait.2092.Still009.JPG");
    data["isPrinting"] = success ? true : false;
    data["error"] = success ? "none" : "Error Connecting to Printer";

    return responseJson200(data);
  });

  return bp;
}