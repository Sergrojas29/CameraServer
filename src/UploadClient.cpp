#include "UploadClient.h"
#include "SessionInfo.h"
#include <optional>



std::optional<PhotoURLs> UploadClient::UploadImage(const std::string imagePath) {
  try {
    if (!std::filesystem::exists(imagePath)) {
      throw std::runtime_error("ERROR: File not found at: " + std::filesystem::absolute(imagePath).string());
    }

    std::string APIKEY = "f19e946fe179e9fb2da37e8ec4126157";
    std::string url = "https://api.imgbb.com/1/upload";

    cpr::Response res =
        cpr::Post(cpr::Url{url}, cpr::Parameters{{"key", APIKEY}},
                  cpr::Multipart{cpr::Part{"image", cpr::File{imagePath}}});

    if (res.error || res.status_code != 200) {
      std::cout << "Response text: " << res.text << std::endl;
      throw std::runtime_error("Error uploading Image");
    }

    auto Json = nlohmann::json::parse(res.text);

    PhotoURLs data;
    data.url = Json["data"]["url"];
    data.urlThumb =Json["data"]["thumb"]["url"];
    data.timestamp = Json["data"]["time"];
    data.url_viewer =Json["data"]["url_viewer"];


    return data;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return std::nullopt;
  }
};


  bool UploadClient::saveToJSON(const std::string &filename, const PhotoURLs &newEntry) {

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
