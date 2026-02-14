#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <functional>
#include <unordered_map>



// TODO: Choose where the overlay defualt loaction
inline constexpr std::string OVERLAY_FILE_LOCATION = "overlays/";
inline constexpr std::string COLLAGE_FILE_LOCATION = "collages/";

using ImageEffect = std::function<void(cv::Mat&)>;


std::unordered_map<std::string, ImageEffect> createEffectMap();








