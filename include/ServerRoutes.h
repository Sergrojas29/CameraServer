#pragma once
#include "crow.h"
#include "CameraClient.h"
#include <nlohmann/json.hpp>

// inline header only
inline void registerRoutes(crow::SimpleApp &app)
{

    // Status Check
    CROW_ROUTE(app, "/api/connect")([]()
                                   {
    nlohmann::json j;
    j["service"] = "CameraServer";
    j["isConnected"] = "True";

    CameraClient myMainCamera;
    
    myMainCamera.setupFolder("Test2");
    myMainCamera.connect();
    myMainCamera.capturePhoto();
    myMainCamera.capturePhoto();
    myMainCamera.capturePhoto();

             
    

    crow::response res(j.dump());
    res.add_header("Access-Control-Allow-Origin", "*"); 
    return res;
 });

    // Take Photo
    CROW_ROUTE(app, "/api/takePhoto")([]()
                                      {

        nlohmann::json j;
        j["service"] = "CameraServer";
        j["isConnected"] = "True";

    std::cout << "Call to Take Photo\n";

    crow::response res(j.dump());
    res.add_header("Access-Control-Allow-Origin", "*"); 
    
    return res; });

    // Create Collage
    CROW_ROUTE(app, "/api/createCollage")([]()
                                          {
        nlohmann::json j;
        j["service"] = "CameraServer";
        j["isConnected"] = "True";
        
        std::cout << "Call to Create Collage\n";
    crow::response res(j.dump());
    res.add_header("Access-Control-Allow-Origin", "*"); 
    
    return res; });

    // print
    CROW_ROUTE(app, "/api/print")([]()
                                  {
        nlohmann::json j;
        j["service"] = "CameraServer";
        j["isConnected"] = "true";


        std::cout << "Call to print\n";

    crow::response res(j.dump());
    
    // This allows your browser (port 5500) to read the response
    res.add_header("Access-Control-Allow-Origin", "*"); 
    
    return res; });

    CROW_ROUTE(app, "/api/disconnect")([]()
                                       {
        nlohmann::json j;
        j["service"] = "CameraServer";
        j["isConnected"] = "false";

        std::cout << "Call to Disconnect\n";
    crow::response res(j.dump());
    
    // This allows your browser (port 5500) to read the response
    res.add_header("Access-Control-Allow-Origin", "*"); 
    
    return res; });
}