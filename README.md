# base64 decoder and image display

## Introduction

You will often have to interact with data of various formats depending on the application,
and for the sake of compuatational and storage efficiency those formats will often not be
human readable.

Base64 is an encoding format to turn bytes (8 bit numbers) onto ASCII text
characters and back.

HTML is a standardized document format meant to dicate and describe the content of webpages. It is often used alongside CSS, which determines stylization, and Javascript which provides scripting and programming capabilites.

## Requirements

Parse a base64 encoded file and make a webpage with the resulting image.

Write a library with 4 functions. These functions are defined in [base64.hpp](src/base64.hpp).

`readArguments` will accept the program arguments. The first program argument is the filepath to the file containing the base64 text. It may be relative or absolute, and the contents of the file will be returned as a string via the base64 argument. The second program argument will be the desired output file for the html file.

`parseB64` will parse an base64 string and produce the corresponding byte stream

`getImageInfo` will call an image info library and retrieve the image's width, height, and a suitable file extension. It will combine the file extension with the original filepath of the base64 data to get a filename for the output image.

`createHtml` will take the image info a create an HTML page that can display the image. Here is a template for the page:

```html
    <!DOCTYPE html>
    <html>
      <body>
        <h1>Image Title</h1>
        <img src="imageFilename.jpg" alt="image caption" height=100 width=100>
        <p>File: imageFilename.jpg size: 100x100</p>
      </body>
    </html>
```

## Testing Requirements

- For the sake of testing we require that all your source files are placed in `src`.
- We also require that nothing in `tests/test_base64.cpp` and `src/base64.hpp` is removed. You may feel free to add to these files or add additional files to these directory, as long as testing continues to build and run. Any additional changes may require you to make changes to `CMakeLists.txt`
- We expect that your html page mostly matches the html provided. We will be searching it to find some key values/tags

## Hints

- To learn about base 64 encoding, you can use sources such as wikipedia or [understanding_base64](
<https://www.sunshine2k.de/articles/coding/base64/understanding_base64.html>)

- It is suggested that you use the following third-party dependency to obtain the width and height of the image:
[ImageInfo](https://github.com/xiaozhuai/imageinfo.git)

## Building

In the guessing-game lab you were given build instructions similar to the ones below.

```shell
cmake -S . -B build -DIMAGEINFO_INCLUDE_DIR=<imageinfo>
cmake --build build --config Debug
build\Debug\base64.exe images\minions\happy-minions.b64 output\index.html
```

where <imageinfo> is the place where you've checked out ImageInfo

Example:

```shell
cmake -S . -B build -DIMAGEINFO_INCLUDE_DIR=extern/imageinfo/include
```

## Testing

[Build](#building)

```shell
ctest --test-dir build -C Debug
Add -V for verbose testing
```
