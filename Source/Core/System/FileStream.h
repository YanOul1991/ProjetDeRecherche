/* ======================================================================================
 *  Filestream.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include <fstream>
#include <string>
#include <iostream>

#pragma warning(disable: 4251)

class Image;

enum class FileStreamStatus : uint8
{
	ok,           // Stream operation success
	streamOpened, // A stream is already open
	failedStart,  // Could not proprely start the stream
	noStream,     // There is no open stream for the demanded action
	failedRead,   // Could not read to file
	failedWrite,  // Could not write to file
	invalidPath,  // The demanded path is invalid
	overflow,     // Attempted to read/write outside the buffer size or file object size
	unknownError  // An unknown error occured
};

class CORE_API FileStream
{
private:
	static std::ifstream s_inFile;
	static std::ofstream s_outFile;

	static std::ofstream s_outFileText;
	static std::ifstream s_inFileText;

	// No copy and instanciation
	FileStream(const FileStream&) = delete;
	FileStream(const FileStream&&) = delete;
	FileStream& operator=(const FileStream&) = delete;
	FileStream& operator=(const FileStream&&) = delete;

public:
	// Stream starts

	static FileStreamStatus startReadStream(const std::string& path);
	static FileStreamStatus startWriteStream(const std::string& path, bool append = false);

	static FileStreamStatus startInputStreamText(const std::string& path);
	static FileStreamStatus startOutputStreamText(const std::string& path);

	// Stream Read/write Default

	static FileStreamStatus write(char* data, size_t dataSize);
	static FileStreamStatus read(char* data, size_t dataSize);

	static FileStreamStatus outputText(std::string&& text);
	static FileStreamStatus inputText(std::string& text);

	// ReadWrite Seek

	static FileStreamStatus goToWrite(std::streampos pos);
	static FileStreamStatus goToRead(std::streampos pos);

	// Stream Close
	static void closeReadStream();
	static void closeWriteStream();

	static void closeInputStreamText();
	static void closeOutputStreamText();

	static void closeAllStream();

	static bool isWriteStreamOpen();
	static bool isReadStreamopen();

	static bool IsOutputStreamTextOpen();
	static bool IsInputStreamTextOpen();

	/*
	 * @brief
	 * Read the data of a JPEG file.
	 *
	 * @param filename
	 * Relative path to the image.
	*/
	static void readJpegImage(const char* filename, Image* img);

	/*
	 * @brief
	 * Read the data of a PNG file.
	 *
	 * @param filename
	 * Relative path to the image.
	*/
	static void readPngImage(const char* path, Image& img);

	/*
	 * @brief
	 * Saves an image as a JPEG.
	 * 
	 * @param filename
	 * The name of the image file to save. Must end with file type.
	 * 
	 * @param imageData
	 * Pointer to an Image class conataining the data to save.
	*/
	static void saveImage(const char* filename, Image* imageData);

	/*
	 * @brief 
	 * Saves an image as an 8-bit RGBA PNG file.
	 * 
	 * @param filename
	 * The name of the image file to save. Must end with file extension.
	 * 
	 * @param imageData
	 * A pointer to an Image class object containing the data 
	 * of the image to save.
	*/
	static void saveImageAsPng(const char* filename, Image* imageData);
};