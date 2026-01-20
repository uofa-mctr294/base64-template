#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <cstdio>
#include <filesystem>
#include "base64.hpp"

// Helper: read entire file into a string
static std::string read_file_to_string(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}


TEST(Base64UnitTest, ParseB64DecodesToRawBytes)
{
    std::string input = "QUJD"; // base64 for "ABC"
    std::vector<uint8_t> bytes = parseB64(input);
    ASSERT_EQ(bytes.size(), 3u);
    EXPECT_EQ(std::string(bytes.begin(), bytes.end()), std::string("ABC", 3));
}

TEST(Base64UnitTest, CreateHtmlContainsFilenameAndSize)
{
    // Create an ImageInfo struct with test data
    ImageInfo info;
    info.imageFileName = "unit_image.gif";
    info.imageFilePath = "/some/path/unit_image.gif";
    info.width = 1;
    info.height = 1;
    std::string html = createHtml(info);
    EXPECT_NE(html.find("src=\"" + info.imageFileName + "\""), std::string::npos);
    EXPECT_NE(html.find("File: " + info.imageFileName), std::string::npos);
    EXPECT_NE(html.find("height=1"), std::string::npos);
    EXPECT_NE(html.find("width=1"), std::string::npos);
    EXPECT_NE(html.find("size: 1x1"), std::string::npos);
}

TEST(Base64UnitTest, ReadArgumentsReadsInputAndSetsOutput)
{
    namespace fs = std::filesystem;
    std::string inPath = "unit_input.b64";
    std::string outPath = "unit_out.html";
    {
        std::ofstream out(inPath, std::ios::binary);
        out << "TESTB64";
    }

    // Prepare argv
    std::vector<std::string> args = {"prog", inPath, outPath};
    std::vector<char *> argv;
    argv.reserve(args.size());
    for (auto &s : args)
        argv.push_back(const_cast<char *>(s.c_str()));

    std::string base64;
    std::string htmlFilename;
    int rc = readArguments((int)argv.size(), argv.data(), &base64, &htmlFilename);
    ASSERT_EQ(rc, 0);
    EXPECT_EQ(base64, "TESTB64");
    EXPECT_EQ(htmlFilename, outPath);
    std::remove(inPath.c_str());
}
