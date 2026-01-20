#include "RouteModules.h"
#include <stdexcept>

crow::Blueprint makeSessionBlueprint(CameraClient &cam) {

  crow::Blueprint bp("api/session");

  CROW_BP_ROUTE(bp, "/startSession")
  ([&cam]() {
    nlohmann::json data;

    data["service"] = "startSession";

    bool success = cam.createSession();
    data["isSessionStart"] = success ? true : false;

    return responseJson200(data);
  });

  // POST
  // Set Session Setting - Collage
  CROW_BP_ROUTE(bp, "/sessionSettings")
      .methods(crow::HTTPMethod::POST)([&cam](const crow::request &req) {
        nlohmann::json data;
        try {
          data = nlohmann::json::parse(req.body);

          if (!data.contains("CollageTemplate")) {
            throw std::runtime_error("Missing fields 'CollageTemplate'");
          }

          cam.session.CollageTemplate =
              data["CollageTemplate"].get<std::string>();

          nlohmann::json response;
          response["status"] = "success";
          response["received_CollageTemplate"] = cam.session.CollageTemplate;

          return crow::response(200, response.dump());

        } catch (const std::exception &e) {
          nlohmann::json errorJson;
          errorJson["error"] = "Invalid JSON format";
          std::cerr << "Exception caught: " << e.what() << std::endl;
          return crow::response(400, errorJson.dump());
        }
      });

  CROW_BP_ROUTE(bp, "/capturePhoto")
  ([&cam]() {
    nlohmann::json data;

    data["service"] = "capturePhoto";

    bool success = cam.capturePhoto();
    data["isCaptured"] = success ? true : false;
    return responseJson200(data);
  });


  CROW_BP_ROUTE(bp, "/endSession")
  ([&cam]() {
    nlohmann::json data;

    data["service"] = "endSession";
    bool success = cam.endSession();
    data["isEnded"] = success ? true : false;
    return responseJson200(data);
  });

  return bp;
}