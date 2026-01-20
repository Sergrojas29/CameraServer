#include <iostream>

#include "crow.h"
#include "CameraClient.h"
#include "ServerRoutes.h"
#include "RouteModules.h"



int main() {

    //Init Camera Class to be permanent
    CameraClient myMainCamera; 

    //Initialize Server
    crow::SimpleApp app;

    // Debug Routes /debug/
    crow::Blueprint debugBP = makeDebugBlueprint(myMainCamera);
    app.register_blueprint(debugBP);

    // Debug Routes /session/
    crow::Blueprint sessionBP = makeSessionBlueprint(myMainCamera);
    app.register_blueprint(sessionBP);

    // Debug Routes /session/
    crow::Blueprint collageBP = makeSessionBlueprint(myMainCamera);
    app.register_blueprint(collageBP);

    //Register Routes
    registerRoutes(app, myMainCamera);

    // 4. Run
    std::cout << "Server starting on port 8080..." << std::endl;
    app.port(8080).multithreaded().run();

    return 0;
}