#pragma once
#include "crow.h"
#include "CameraClient.h"
#include "CollageCreate.h"
#include <nlohmann/json.hpp>
#include <fstream>

// inline header only
inline void registerRoutes(crow::SimpleApp &app, CameraClient &cam)
{

    // Status Check
    CROW_ROUTE(app, "/api/connect")([&cam]()
                                   {
    nlohmann::json j;
    bool success = cam.connect();


    j["service"] = "CameraServer";
    j["isConnected"] = success ? "true" : "false";
    
    crow::response res(j.dump());
    res.add_header("Access-Control-Allow-Origin", "*"); 
    return res;
 });

     // setup Folder
    CROW_ROUTE(app, "/api/setupFolder/<string>")([&cam](std::string folderName){
        
        nlohmann::json data;
        data["service"] = "CameraServer";
        data["operation"] = "setupfolder";
        
        
        bool success = cam.setupFolder(folderName);
        
        data["captureStatus"] = success ? "success" : "error";
        data["file_path"] = success ? folderName : "error";

    crow::response res(data.dump());
    res.add_header("Access-Control-Allow-Origin", "*"); 
    
    return res; });

    // Take Photo
    CROW_ROUTE(app, "/api/takePhoto")([&cam]()
                                      {

        nlohmann::json j;
        j["service"] = "CameraServer";
        
        std::string file_path = cam.capturePhoto();
        
        j["captureStatus"] = (file_path == "Error") ? "Error" : "success";
        j["file_path"] = (file_path == "Error") ? "Error" : file_path;

    crow::response res(j.dump());
    res.add_header("Access-Control-Allow-Origin", "*"); 
    
    return res; });

    // Create Collage
    CROW_ROUTE(app, "/api/createCollage")([&cam]()
                                          {
        nlohmann::json j;
        j["service"] = "CameraServer";
        j["isConnected"] = "True";

        CollageCreate::SinglePortraitCollage(cam.m_save_path,"img_2.jpg","Overlay-FlamaLama.png");

          


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