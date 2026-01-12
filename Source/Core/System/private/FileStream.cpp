/* ======================================================================================
 *  Filestream.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Color/Color.h"
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

void FileStream::readJpegImage(const char* filename)
{
  FILE* image{};
  fopen_s(&image, filename, "rb");

  if (!image) {
    //printf("Failed to load image %s", filename);
    return;
  }

  // Get the size of the binary file
  fseek(image, 0, SEEK_END);
  int imageSize = ftell(image);
  fseek(image, 0, SEEK_SET);

  // Load raw data to std::vector then close file stream
  std::vector<unsigned char> jpegData(imageSize);
  fread(jpegData.data(), 1, imageSize, image);
  fclose(image);

  tjhandle handle = tjInitDecompress();

  int imgWidth{};
  int imgHeight{};
  int imgSubSample{};
  int imgColorSpace{};

  // Get jpeg header data
  tjDecompressHeader3(handle, jpegData.data(), imageSize, &imgWidth, &imgHeight, &imgSubSample, &imgColorSpace);

  std::vector<unsigned char> pixelData(imgWidth * imgHeight * 3);
  tjDecompress2(handle, jpegData.data(), imageSize, pixelData.data(), imgWidth, 0, imgHeight, TJPF_RGB, 0);

  tjDestroy(handle);

  // Destroy handle
  //printf("Loaded image: %s\n", filename);
  //printf("--- Image dimensions  : %dx%d\n", imgWidth, imgHeight);
  //printf("--- Image subsampling : %d\n", imgSubSample);
  //printf("--- Image color space : %d\n", imgColorSpace);

  //printf("First 10 piexls data: \n");

  std::vector<op::color::ColorRgb>colors(10);

  for (int i = 0; i < colors.size(); i++)
  {
    unsigned char* cursor = &pixelData[i * 3];

    colors[i].r = (float)((int)cursor[0]);
    colors[i].g = (float)((int)cursor[1]);
    colors[i].b = (float)((int)cursor[2]);
    colors[i].a = 0;
  }

  //for (int i = 0; i < colors.size(); i++)
  //{
  //  printf("--- [%3d] 0x%X%X%X\n", (i + 1), (int)colors[i].r, (int)colors[i].g, (int)colors[i].b);
  //}
}

void FileStream::readPngImage(const char* path)
{
  FILE* file{};
  fopen_s(&file, path, "rb");

  if (!file)
  {
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
  if (!pPng)
  {
    printf("[Error] An error occured while reading PNG file:(\n");;
    fclose(file);
    return;
  }

  png_info* pInfo = png_create_info_struct(pPng);
  if (!pInfo)
  {
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

  /**
   * To simplyfy implementation with other modules, all PNG images will
   * be converted to 8-bit RGBA.
  */

  if (imgBitDepth == 16) 
    png_set_strip_16(pPng);

  if (imgColorType == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(pPng);

  if (imgColorType == PNG_COLOR_TYPE_GRAY && imgBitDepth < 8)
    png_set_expand_gray_1_2_4_to_8(pPng);

  if (png_get_valid(pPng, pInfo, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(pPng);

  if (imgColorType == PNG_COLOR_TYPE_RGB || imgColorType == PNG_COLOR_TYPE_GRAY || imgColorType == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(pPng, 0xFF, PNG_FILLER_AFTER);

  if (imgColorType == PNG_COLOR_TYPE_GRAY || imgColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(pPng);

  png_read_update_info(pPng, pInfo);

  std::vector<unsigned char> pixels(imgWidth * imgHeight * 4);
  std::vector<png_bytep> rowPtr(imgHeight);

  for (int i = 0; i < imgHeight; i++) {
    rowPtr[i] = pixels.data() + (i * imgWidth * 4);
  }
  png_read_image(pPng, rowPtr.data());
  png_destroy_read_struct(&pPng, &pInfo, 0);

  //printf("Sucessfully read the data of %s\n", path);
  //printf("--- Image resolution  : %dx%d\n", imgWidth, imgHeight);
  //printf("--- Image color type  : %d\n", imgColorType);
  //printf("--- Image bit depth   : %d\n", imgBitDepth);

  //printf("Pixels data:\n");

  //std::vector<op::color::ColorRgb> colors(10);

  //for (size_t i = 0; i < colors.size(); i++)
  //{
  //  unsigned char* cursor = &pixels[i * 4];

  //  colors[i].r = (float)((int)cursor[0]);
  //  colors[i].g = (float)((int)cursor[1]);
  //  colors[i].b = (float)((int)cursor[2]);
  //  colors[i].a = (float)((int)cursor[3]);
  //}

  //printf("Pixel Data:\n");

  //for (int i = 0; i < colors.size(); i++)
  //{
  //  printf("--- Pixel at index %2d: 0x%X%X%X%X\n", i, (int)colors[i].r, (int)colors[i].g, (int)colors[i].b, (int)colors[i].a);
  //}

  // Final fclose()
  fclose(file);
}