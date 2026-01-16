#include <iostream>
#include "crow.h"
#include "CameraClient.h"
#include "ServerRoutes.h"




int main() {

    //Init Camera Class to be permanent
    CameraClient myMainCamera; 

    //Initialize Server
    crow::SimpleApp app;

    //Register Routes
    registerRoutes(app, myMainCamera);

    // 4. Run
    std::cout << "Server starting on port 8080..." << std::endl;
    app.port(8080).multithreaded().run();

    return 0;
}