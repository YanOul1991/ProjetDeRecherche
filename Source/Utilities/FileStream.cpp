#include "fileio.h"

std::ifstream opt::fileio::FileStream::s_inFile{};
std::ofstream opt::fileio::FileStream::s_outFile{};

// Create an input stream to a file path.
opt::fileio::FileStreamStatus opt::fileio::FileStream::startReadStream(const std::string &path)
{
  if (s_inFile.is_open()) return FileStreamStatus::streamOpened;
  s_inFile.open(path, std::ios::binary);
  return s_inFile.is_open() ? FileStreamStatus::ok : FileStreamStatus::failedStart;
}

// Create an output stream to a file path.
opt::fileio::FileStreamStatus opt::fileio::FileStream::startWriteStream(const std::string &path, bool append)
{
  if (s_outFile.is_open()) return FileStreamStatus::streamOpened;
  if (append == true) s_outFile.open(path, std::ios::binary | std::ios::app);
  else s_outFile.open(path, std::ios::binary);
  return s_outFile.is_open() ? FileStreamStatus::ok : FileStreamStatus::failedStart;
}

// Output data to the currently opened file output stream.
opt::fileio::FileStreamStatus opt::fileio::FileStream::write(char *data, size_t dataSize)
{ 
  if (!s_outFile.is_open()) return FileStreamStatus::noStream;
  s_outFile.write(data, dataSize);
  return s_outFile.good() ? FileStreamStatus::ok : FileStreamStatus::unknownError;
}

// Input data from the currently opened input stream.
opt::fileio::FileStreamStatus opt::fileio::FileStream::read(char *data, size_t dataSize)
{
  if (!s_inFile.is_open()) return FileStreamStatus::noStream;
  s_inFile.read(data, dataSize);
  return s_inFile.good() ? FileStreamStatus::ok : FileStreamStatus::unknownError;
}

// Move the cursor to a specific location in the currently opened output stream.
opt::fileio::FileStreamStatus opt::fileio::FileStream::goToWrite(std::streampos pos)
{
  if (!s_outFile.is_open()) return FileStreamStatus::noStream;
  s_outFile.seekp(pos, std::ios::beg);
  return FileStreamStatus::ok;
}

// Move the cursor to a specific location in the currently opened input stream.
opt::fileio::FileStreamStatus opt::fileio::FileStream::goToRead(std::streampos pos)
{
  if (!s_inFile.is_open()) return FileStreamStatus::noStream;
  s_inFile.seekg(pos, std::ios::beg);
  return FileStreamStatus::ok;
}

// Close the input stream.
void opt::fileio::FileStream::closeReadStream()
{
  if (s_inFile.is_open()) s_inFile.close();
}

// Close the output stream
void opt::fileio::FileStream::closeWriteStream()
{
  if (s_outFile.is_open()) s_outFile.close();
}

// Close both input and output streams.
void opt::fileio::FileStream::closeAllStream()
{
  closeReadStream();
  closeWriteStream();
}

// Check if the input file stream is currently open.
bool opt::fileio::FileStream::isWriteStreamOpen()
{
  return s_outFile.is_open();
}

// Check if the
bool opt::fileio::FileStream::isReadStreamopen()
{
  return s_inFile.is_open();
}