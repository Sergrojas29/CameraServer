#pragma once
#include "CameraClient.h"
#include "ImageEffect.h"

#include "SessionInfo.h"
#include "cpr/session.h"
#include <algorithm>
#include <array>
#include <execution>
#include <filesystem>
#include <fstream>
#include <functional>
#include <opencv2/opencv.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <netdb.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>



using stringOptional = std::optional<std::string>;

class CollageCreate {

  
public:

  inline static const auto myFilters = createEffectMap();

  static bool creatCollage(SessionInfo& session);


  
  // TODO: Parallel Computeing to be worked out

  static stringOptional portraitCollage(const std::string &imagePath,
                                        const std::string &overlayPath,
                                        SessionInfo &SessionInfo);

  static stringOptional
  templateCollage_4(const std::vector<std::string> &imagePaths,
                    const std::string &overlayPath, SessionInfo &SessionInfo);
};
