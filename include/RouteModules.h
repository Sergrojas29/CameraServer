#pragma once
#include "crow.h"
#include "CameraClient.h"
#include "CollageCreate.h"
#include "nlohmann/json.hpp"


// We declare that these factory functions exist

inline crow::response responseJson200(nlohmann::json& data){
    crow::response res;
    res.body = data.dump();
    res.code = 200; // Standard OK
    res.set_header("Content-Type", "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
    return res;
}

crow::Blueprint makeDebugBlueprint(CameraClient& cam);
crow::Blueprint makeSessionBlueprint(CameraClient& cam);
crow::Blueprint makeCollageBlueprint(CameraClient& cam);
// crow::Blueprint makeMediaBlueprint(CameraClient& cam);

// crow::Blueprint makeOutputBlueprint();