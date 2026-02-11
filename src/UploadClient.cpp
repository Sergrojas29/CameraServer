#include "UploadClient.h"
#include <optional>



std::optional<std::string> UploadClient::UploadImage(const std::string imagePath) {
  try {
    if (!std::filesystem::exists(imagePath)) {
      throw std::runtime_error("ERROR: File not found at: " + std::filesystem::absolute(imagePath).string());
    }

    std::string APIKEY = "";
    std::string url = "https://api.imgbb.com/1/upload";

    cpr::Response res =
        cpr::Post(cpr::Url{url}, cpr::Parameters{{"key", APIKEY}},
                  cpr::Multipart{cpr::Part{"image", cpr::File{imagePath}}});

    if (res.error || res.status_code != 200) {
      std::cout << "Response text: " << res.text << std::endl;
      throw std::runtime_error("Error uploading Image");
    }

    auto imageUrl = nlohmann::json::parse(res.text)["data"]["url"];


    return imageUrl;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return std::nullopt;
  }
};
