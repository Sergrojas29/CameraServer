#include "ImageEffect.h"


std::unordered_map<std::string, ImageEffect> createEffectMap(){
    std::unordered_map<std::string, ImageEffect> map;

    map["template_1"] = [](cv::Mat& overlay){
        //does something
    };

    return map;
}
