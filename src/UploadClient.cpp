#include "UploadClient.h"
#include "SessionInfo.h"
#include <optional>


// TODO make this function parallel with openMP but watch out for race condition on push_back
bool UploadClient::UploadImages(SessionInfo& session) {
  try {

    std::string APIKEY = "f19e946fe179e9fb2da37e8ec4126157";
    std::string url = "https://api.imgbb.com/1/upload";


    for (auto& filePath : session.collagePaths) {
    
      cpr::Response res =
      cpr::Post(cpr::Url{url}, cpr::Parameters{{"key", APIKEY}},
        cpr::Multipart{cpr::Part{"image", cpr::File{filePath}}});
        
        if (res.error || res.status_code != 200) {
          std::cout << "Response text: " << res.text << std::endl;
          throw std::runtime_error("Error uploading Image");
        }

        PhotoURLs urlData;
        auto data = json::parse(res.text);
        
        urlData.url = data["data"]["url"];
        urlData.urlThumb =data["data"]["thumb"]["url"];
        urlData.timestamp = data["data"]["time"];
        urlData.url_viewer =data["data"]["url_viewer"];

        session.urlData.push_back(urlData);
        
      }

    return true;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }
};


  bool UploadClient::saveToJSON(const std::string &filename, const SessionInfo &newEntry) {

    json data;

    // Read existing file
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
      inFile >> data;
      inFile.close();
    } else {
      data = json::array();
    }

    // add new photo
    data.push_back(newEntry);

    // Write back
    std::ofstream outFile(filename);
    outFile << data.dump(4);
    outFile.close();
    return true;
  }
