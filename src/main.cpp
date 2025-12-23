#include <iostream>
#include <fstream>
#include <sstream>
#include "base64.hpp"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <gif-input> <html-output>" << std::endl;
        return 1;
    }

    std::string base64;
    std::string htmlFilename;

    int returnCode = readArguments(argc, argv, &base64, &htmlFilename);
    if (returnCode != 0)
    {
        return returnCode;
    }
    std::vector<uint8_t> image = parseB64(base64);

    // Get image information (format, dimensions, file paths)
    ImageInfo info = getImageInfo(image, argv[1], htmlFilename);

    // Write the decoded image to file
    std::ofstream imgOut(info.imageFilePath, std::ios::out | std::ios::binary);
    imgOut.write(reinterpret_cast<const char *>(image.data()), image.size());
    imgOut.close();

    // Create HTML with the image info
    std::string html = createHtml(info);

    std::ofstream out(htmlFilename, std::ios::out | std::ios::binary);
    out << html;
    return 0;
}
