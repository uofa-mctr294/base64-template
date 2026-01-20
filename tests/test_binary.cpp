#include <gtest/gtest.h>
#include <string>
#include <array>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <filesystem>
#include <algorithm>

static int g_argc = 0;
static char **g_argv = nullptr;

// Helper: run a command and capture stdout
static std::string run_command_capture(const std::string &cmd)
{
#ifdef _WIN32
    std::array<char, 256> buffer;
    std::string result;
    FILE *pipe = _popen(cmd.c_str(), "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), (int)buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }
    _pclose(pipe);
    return result;
#else
    std::array<char, 256> buffer;
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
#endif
}

// Helper: read entire file into a string
static std::string read_file_to_string(const std::string &path)
{
    std::FILE *f = std::fopen(path.c_str(), "rb");
    if (!f)
        throw std::runtime_error("failed to open file: " + path);
    std::string content;
    std::array<char, 4096> buf;
    size_t n = 0;
    while ((n = std::fread(buf.data(), 1, buf.size(), f)) > 0)
    {
        content.append(buf.data(), n);
    }
    std::fclose(f);
    return content;
}

TEST(Base64BinaryTest, GeneratedHtmlContainsImageSize)
{
    ASSERT_GE(g_argc, 4) << "Expected: test_binary <exePath> <inputB64> <outputHtml>";
    std::string exePath = std::string(g_argv[1]);
    std::string inputB64 = std::string(g_argv[2]);
    std::string outputHtml = std::string(g_argv[3]);

    // Build the command line with proper quoting on Windows
#ifdef _WIN32
    // Convert forward slashes to backslashes for Windows paths
    std::replace(exePath.begin(), exePath.end(), '/', '\\');
    std::replace(inputB64.begin(), inputB64.end(), '/', '\\');
    std::replace(outputHtml.begin(), outputHtml.end(), '/', '\\');
    std::string inner = "\"" + exePath + "\" \"" + inputB64 + "\" \"" + outputHtml + "\"";
    std::string cmd = std::string("cmd /c ") + inner;
#else
    std::string cmd = exePath + " " + inputB64 + " " + outputHtml;
#endif

    // Run the program to generate the HTML file and capture output
    (void)run_command_capture(cmd);

    // Ensure the file exists, then read and assert the actual dimensions exist
#if __cplusplus >= 201703L
    namespace fs = std::filesystem;
    ASSERT_TRUE(fs::exists(outputHtml)) << "Output HTML not found: " << outputHtml;
#endif
    std::string html = read_file_to_string(outputHtml);
    // happy-minions.b64 is a 500x226 GIF
    EXPECT_NE(html.find("height=226"), std::string::npos) << "Missing height attribute in HTML";
    EXPECT_NE(html.find("width=500"), std::string::npos) << "Missing width attribute in HTML";
    EXPECT_NE(html.find("size: 500x226"), std::string::npos) << "Missing size text in HTML";
}

int main(int argc, char **argv)
{
    // Save argv for tests
    g_argc = argc;
    g_argv = argv;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
