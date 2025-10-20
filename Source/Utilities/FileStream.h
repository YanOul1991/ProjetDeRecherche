#ifndef OPT_FILEIO_H  
#define OPT_FILEIO_H  

#include "_optglobal.h"
#include <fstream>

// MARK FOR DLL

#ifdef DLL_IO
  #define OPTD_FILEIO __declspec(dllexport)
#else
  #define OPTD_FILEIO __declspec(dllimport)
#endif

namespace opt::fileio
{
  struct OPTD_FILEIO FileMetaHeader
  {
    const uint32_t version;
    const uint32_t objCount;
    char signature[4];
  };

  enum class OPTD_FILEIO FileStreamStatus : uint8_t
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

  class OPTD_FILEIO FileStream
  {
  private:
    static std::ifstream s_inFile;
    static std::ofstream s_outFile;

    // No copy and instanciation
    FileStream(const FileStream&) = delete;
    FileStream(const FileStream&&) = delete;
    FileStream& operator=(const FileStream&) = delete;
    FileStream& operator=(const FileStream&&) = delete;

  public:
    // Stream starts

    static FileStreamStatus startReadStream(const std::string& path);
    static FileStreamStatus startWriteStream(const std::string& path, bool append = false);

    // Stream Read/write Default

    static FileStreamStatus write(char* data, size_t dataSize);
    static FileStreamStatus read(char* data, size_t dataSize);

    // ReadWrite Seek

    static FileStreamStatus goToWrite(std::streampos pos);
    static FileStreamStatus goToRead(std::streampos pos);

    // Stream Close

    static void closeReadStream();
    static void closeWriteStream();
    static void closeAllStream();
    static bool isWriteStreamOpen();
    static bool isReadStreamopen();
  };
}

#endif
