
#include <cpr/cpr.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

class clientTest {
private:
public:
  static bool testUpload(std::string imagePath) {
    try {
      if (!std::filesystem::exists(imagePath)) {
        throw std::runtime_error("ERROR: File not found at: ");
      }

      std::string APIKEY = "";
      std::string url = "https://api.imgbb.com/1/upload";

      cpr::Response res =
          cpr::Post(cpr::Url{url}, cpr::Parameters{{"key", APIKEY}},
                    cpr::Multipart{cpr::Part{"image", cpr::File{imagePath}}});

      if (res.error || res.status_code != 200) {
        std::cout << "Response text: " << res.text << std::endl;
        throw std::runtime_error("Error uploading Imgae");
      }

      std::cout << "Response:\n" << res.text << std::endl;

      return true;
    } catch (const std::exception &e) {
      std::cerr << "Exception caught: " << e.what() << std::endl;
      return false;
    }
  }
};

int main() {

  clientTest::testUpload("./img_1.jpg");

  return 0;
}

// // Simple test to verify GTest is working
// TEST(SanityCheck, BasicAssertions) {
//     EXPECT_EQ(1 + 1, 2);
//     EXPECT_TRUE(true);
// }