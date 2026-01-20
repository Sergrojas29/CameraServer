#include "RouteModules.h"


crow::Blueprint makeCollageBlueprint(CameraClient &cam) {

  crow::Blueprint bp("api/collage");

  //TODO : Add more routes to collage ie special or 3 point Collage

  //FUNC Create 4 Photo Collage
  CROW_BP_ROUTE(bp, "/createTemplate4")
  ([&cam]() {
    nlohmann::json data;

    data["service"] = "startSession";

    bool success = cam.createSession();
    data["isSessionStart"] = success ? true : false;

    return responseJson200(data);
  });

  return bp;
}