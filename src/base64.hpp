#pragma once

#include <string>
#include <vector>
#include <stdint.h>

struct ImageInfo
{
    std::string imageFilePath; // Full path where image should be written
    std::string imageFileName; // Just the filename for HTML
    int width;
    int height;
};

int readArguments(int argc, char **argv, std::string *base64, std::string *htmlFilename);

std::vector<uint8_t> parseB64(const std::string base64);

ImageInfo getImageInfo(const std::vector<uint8_t> &image, const std::string &inputFilename, const std::string &htmlOutputPath);

std::string createHtml(const ImageInfo &info);