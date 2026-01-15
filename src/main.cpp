#include <iostream>
#include "crow.h"
#include "CameraClient.h"
#include "ServerRoutes.h"




int main() {


    //Initialize Server
    crow::SimpleApp app;

    //Register Routes
    registerRoutes(app);

    // 4. Run
    std::cout << "Server starting on port 8080..." << std::endl;
    app.port(8080).multithreaded().run();

    return 0;
}