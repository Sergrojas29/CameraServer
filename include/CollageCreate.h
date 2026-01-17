#pragma once
#include <string>
#include <opencv2/opencv.hpp>   
#include <stdexcept>

class CollageCreate
{
private:
    static std::string overlayFileLocation;
    static std::string collageFileLocation;
    
public:
    /**
     * @param 
     */
    static std::string  SinglePortraitCollage(
        const std::string& save_path,
        const std::string& imageFileName,
        const std::string& overlayFileName);

};


