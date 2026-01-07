
#include "Core/System/FileStream.h"
#include <fstream>
#include <string>

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