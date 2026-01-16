/* ======================================================================================
 *  Filestream.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Object/Image/Image.h"
#include "Core/Types/Color.h"
#include "ThirdParty/libjpeg/turbojpeg.h"
#include "ThirdParty/libpng/libpng18/png.h"
#include "Core/System/FileStream.h"

#include <fstream>
#include <string>
#include <vector>

using string = std::string;

std::ifstream FileStream::s_inFile{};
std::ofstream FileStream::s_outFile{};

std::ifstream FileStream::s_inFileText{};
std::ofstream FileStream::s_outFileText{};

// Create an input stream to a file path.
FileStreamStatus FileStream::startReadStream(const std::string &path)
{
  // If an input filestream is already open
  // return streamOpened status
  if (s_inFile.is_open()) 
    return FileStreamStatus::streamOpened;

  // Start an input file stream
  // and return operation status
  s_inFile.open(path, std::ios::binary);

  return s_inFile.is_open() ? FileStreamStatus::ok : FileStreamStatus::failedStart;
}

// Create an output stream to a file path.
FileStreamStatus FileStream::startWriteStream(const std::string &path, bool append)
{
  // If an output filestream is already open
  // return a streamOpened status
  if (s_outFile.is_open()) 
    return FileStreamStatus::streamOpened;

  // Add append bit flag if append param is set to true
  // this allows to write to a binary file without 
  // having its contented deleted
  if (append == true) 
    s_outFile.open(path, std::ios::binary | std::ios::app);
  else 
    s_outFile.open(path, std::ios::binary);

  // Return operation status
  return s_outFile.is_open() ? FileStreamStatus::ok : FileStreamStatus::failedStart;
}

// Output data to the currently opened file output stream.
FileStreamStatus FileStream::write(char *data, size_t dataSize)
{ 
  // If no output stream has been opened
  // cancel and return noStream status
  if (!s_outFile.is_open()) 
    return FileStreamStatus::noStream;

  // Output data
  s_outFile.write(data, dataSize);

  // Return operation status
  return s_outFile.good() ? FileStreamStatus::ok : FileStreamStatus::unknownError;
}

// Input data from the currently opened input stream.
FileStreamStatus FileStream::read(char *data, size_t dataSize)
{
  // If no input stream has been started
  // cancel and return noStream status
  if (!s_inFile.is_open()) 
    return FileStreamStatus::noStream;

  // Input data
  s_inFile.read(data, dataSize);

  // Return operation status
  return s_inFile.good() ? FileStreamStatus::ok : FileStreamStatus::unknownError;
}

// Moves the file output stream cursor to a specific byte in a binary file.
FileStreamStatus FileStream::goToWrite(std::streampos pos)
{
  // If no output stream has been opened
  // cancel and return noStream status
  if (!s_outFile.is_open()) 
    return FileStreamStatus::noStream;

  // Place cursor at position
  s_outFile.seekp(pos, std::ios::beg);

  // Return operation status
  return FileStreamStatus::ok;
}

// Move the cursor to a specific location in the currently opened input stream.
FileStreamStatus FileStream::goToRead(std::streampos pos)
{
  // If no input stream has been started
  // cancel and return noStream status
  if (!s_inFile.is_open()) 
    return FileStreamStatus::noStream;

  // Place cursor at position
  s_inFile.seekg(pos, std::ios::beg);

  // Return operation status
  return FileStreamStatus::ok;
}

// Close the input stream.
void FileStream::closeReadStream()
{
  if (s_inFile.is_open()) 
    s_inFile.close();
}

// Close the output stream
void FileStream::closeWriteStream()
{
  if (s_outFile.is_open()) 
    s_outFile.close();
}

// Close both input and output streams.
void FileStream::closeAllStream()
{
  closeReadStream();
  closeWriteStream();
  closeInputStreamText();
  closeOutputStreamText();
}

// Check if the input file stream is currently open.
bool FileStream::isWriteStreamOpen()
{
  return s_outFile.is_open();
}

// Check if the
bool FileStream::isReadStreamopen()
{
  return s_inFile.is_open();
}

/*
  Text Streams Declarations
*/

// Start a Text input stream
FileStreamStatus FileStream::startInputStreamText(const std::string& path)
{
  if (s_inFileText.is_open())
    return FileStreamStatus::streamOpened;

  s_inFileText.open(path);

  return s_inFileText.is_open() ? FileStreamStatus::ok : FileStreamStatus::failedStart;
}

// Start a text output stream
FileStreamStatus FileStream::startOutputStreamText(const std::string& path)
{
  if (s_outFileText.is_open())
    return FileStreamStatus::streamOpened;

  s_outFileText.open(path);

  return s_outFileText.is_open() ? FileStreamStatus::ok : FileStreamStatus::failedStart;
}

// Input a line from a text file
FileStreamStatus FileStream::inputText(string& text)
{
  if (!s_inFileText.is_open())
    return FileStreamStatus::noStream;

  std::getline(s_inFileText, text);

  return FileStreamStatus::ok;
}

// Output text into a strin stream
FileStreamStatus FileStream::outputText(string&& text)
{
  if (!s_outFileText.is_open())
    return FileStreamStatus::noStream;

  s_outFileText << text;

  return FileStreamStatus::ok;
}

void FileStream::closeInputStreamText()
{
  if(s_inFileText.is_open())
    s_inFileText.close();
}

void FileStream::closeOutputStreamText()
{
  if(s_outFileText.is_open())
    s_outFileText.close();
}

bool FileStream::IsInputStreamTextOpen()
{
  return s_inFileText.is_open();
}

bool FileStream::IsOutputStreamTextOpen()
{
  return s_outFileText.is_open();
}

void FileStream::readJpegImage(const char* filename, Image* img)
{
  FILE* file{};
  fopen_s(&file, filename, "rb");

  if (!file) {
    printf("Failed to load image %s", filename);
    return;
  }

  // Get the size of the binary file
  fseek(file, 0, SEEK_END);
  int imageSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Load raw data to std::vector then close file stream
  std::vector<unsigned char> jpegData(imageSize);
  fread(jpegData.data(), 1, imageSize, file);
  fclose(file);

  tjhandle handle = tjInitDecompress();

  int imgWidth{};
  int imgHeight{};
  int imgSubSample{};
  int imgColorSpace{};

  // Get jpeg header data
  tjDecompressHeader3(handle, jpegData.data(), imageSize, &imgWidth, &imgHeight, &imgSubSample, &imgColorSpace);

  //std::vector<unsigned char> pixelData(imgWidth * imgHeight * 3);

  printf("Image Resolution from reader: %dx%d\n", imgWidth, imgHeight);

  img->width = imgWidth;
  img->height = imgHeight;
  img->pixels = new op::color::SColor[imgWidth * imgHeight];

  tjDecompress2(handle, jpegData.data(), imageSize, reinterpret_cast<uint8*>(img->pixels), imgWidth, 0, imgHeight, TJPF_RGBA, 0);

  tjDestroy(handle);

  /*
  std::vector<op::color::ColorRgb>colors(10);

  for (int i = 0; i < colors.size(); i++)
  {
    unsigned char* cursor = &pixelData[i * 3];

    colors[i].r = (float)((int)cursor[0]);
    colors[i].g = (float)((int)cursor[1]);
    colors[i].b = (float)((int)cursor[2]);
    colors[i].a = 0;
  }
  */
}

void FileStream::readPngImage(const char* path, Image& img)
{
  FILE* file{};
  fopen_s(&file, path, "rb");

  if (!file) {
    printf("Could not read the follwing file: %s", path);
    return;
  }

  /**
   * Read the signature at the given path paramter.
   * If the signature does not correspond the the PNG file
   * signature then the file is not a valid PNG format file.
  */
  png_byte header[8]{};
  fread_s(header, 8, 1, 8, file);

  if (png_sig_cmp(header, 1, 8) != 0) {
    printf("[Error] The files \"%s\" is not a valid PNG file", path);
    fclose(file);
    return;
  }


  png_struct* pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!pPng) {
    printf("[Error] An error occured while reading PNG file:(\n");;
    fclose(file);
    return;
  }

  png_info* pInfo = png_create_info_struct(pPng);
  if (!pInfo) {
    printf("[Error] An error occured while reading PNG file:(\n");;
    fclose(file);
    return;
  }

  if (setjmp(png_jmpbuf(pPng))) {
    png_destroy_read_struct(&pPng, &pInfo, 0);
    printf("[Error] An error occured while reading PNG file:(\n");;
    fclose(file);
    return;
  }

  png_init_io(pPng, file);
  png_set_sig_bytes(pPng, 8); // already red signature
  png_read_info(pPng, pInfo);

  int imgWidth                = png_get_image_width(pPng, pInfo);
  int imgHeight               = png_get_image_height(pPng, pInfo);
  unsigned char imgColorType  = png_get_color_type(pPng, pInfo);
  unsigned char imgBitDepth   = png_get_bit_depth(pPng, pInfo);

  printf("Bit depth of image %s : %d\n", path, imgBitDepth);

  /**
   * To simplyfy implementation with other modules, all PNG images will
   * be converted to 8-bit RGBA.
  */

  if (imgBitDepth == 16) {
    png_set_strip_16(pPng);
  }

  if (imgColorType == PNG_COLOR_TYPE_PALETTE) {
    png_set_palette_to_rgb(pPng);
  }

  if (imgColorType == PNG_COLOR_TYPE_GRAY && imgBitDepth < 8) {
    png_set_expand_gray_1_2_4_to_8(pPng);
  }

  if (png_get_valid(pPng, pInfo, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(pPng);
  }

  if (  imgColorType == PNG_COLOR_TYPE_RGB ||
        imgColorType == PNG_COLOR_TYPE_GRAY ||
        imgColorType == PNG_COLOR_TYPE_PALETTE) {
    png_set_filler(pPng, 0xFF, PNG_FILLER_AFTER);
  }

  if (imgColorType == PNG_COLOR_TYPE_GRAY || imgColorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
    png_set_gray_to_rgb(pPng);
  }

  png_read_update_info(pPng, pInfo);

  img.width   = imgWidth;
  img.height  = imgHeight;
  img.pixels  = new op::color::SColor[imgWidth * imgHeight * 4];

  std::vector<png_bytep> rowPtr(imgHeight);

  for (int i = 0; i < imgHeight; i++) {
    rowPtr[i] = reinterpret_cast<uint8*>(img.pixels) + (i * imgWidth * 4);
  }

  png_read_image(pPng, rowPtr.data());
  png_destroy_read_struct(&pPng, &pInfo, 0);

  // Final fclose()
  fclose(file);
}

void FileStream::saveImage(const char* filename, Image* imageData)
{
  tjhandle handle = tjInitCompress();

  if (!handle) {
    printf("There was an error while trying to save image.\n"); 
    return;
  }

  uint8*  jpegBuffer{};
  unsigned long  jpegSize{};

  int pitch = imageData->width * sizeof(op::color::SColor);

  int result = tjCompress2(
    handle,
    reinterpret_cast<const unsigned char*>(imageData->pixels),
    imageData->width,
    pitch,
    imageData->height,
    TJPF_RGBA,
    &jpegBuffer,
    &jpegSize,
    TJSAMP_444,
    80,
    TJFLAG_FASTDCT
  );

  if (result != 0) {
    printf("There was an error while trying to save image.\n");
    tjDestroy(handle);
    return;
  }

  FILE* fp{};

  fopen_s(&fp, filename, "wb");

  if (!fp) {
    printf("There was an error while trying to save image.\n");
    tjFree(jpegBuffer);
    tjDestroy(handle);
    return;
  }

  fwrite(jpegBuffer, jpegSize, 1, fp);
  fclose(fp);

  tjFree(jpegBuffer);
  tjDestroy(handle);
}


void FileStream::saveImageAsPng(const char* filename, Image* imageData) 
{
  FILE*         pFile{};
  png_struct*   pPng{};
  png_info*     pInfo{};
  uint8**       pRow{};

  // Open file in binary write mode
  fopen_s(&pFile, filename, "wb");

  if (pFile == nullptr) {
    printf("Could open a file at %s", filename);
    return;
  }

  // Initialize png structures

  pPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

  if (pPng == nullptr) {
    printf("Could not create a read struct for png.\n");
    png_destroy_write_struct(&pPng, nullptr);
    fclose(pFile);
    return;
  }

  pInfo = png_create_info_struct(pPng);

  if (pInfo == nullptr) {
    printf("Error could not create pnf info struct.\n");
    png_destroy_write_struct(&pPng, nullptr);
    fclose(pFile);
    return;
  }

  // Error handling

  if (setjmp(png_jmpbuf(pPng))) {
    printf("Error with PNG creation.\n");
    png_destroy_write_struct(&pPng, &pInfo);
    fclose(pFile);
    return;
  }

  // IO initialization
  png_init_io(pPng, pFile);

  // Set image header info

  png_set_IHDR(
    pPng,
    pInfo,
    imageData->width,
    imageData->height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  png_write_info(pPng, pInfo);

  std::vector<png_bytep> rows(imageData->height);
  for (int i = 0; i < imageData->height; i++) {
    rows[i] = reinterpret_cast<uint8*>(imageData->pixels) + (i * imageData->width * 4);
  }

  png_set_strip_16(pPng);

  png_write_image(pPng, rows.data());
  png_write_end(pPng, nullptr);

  png_destroy_write_struct(&pPng, &pInfo);
  fclose(pFile);
}