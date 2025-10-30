# Small Game Engine Project

## Example of intput/output a std::vector into a binary file
```cpp
    std::vector<int32> dataWrite = { 33, 43444, 434343, 4, 389, 433333333 };
    std::vector<int32> dataRead{};
    size_t dataSize = dataWrite.size();

    FileStream::startWriteStream("_binFileTest.oefile");
    FileStream::write(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
    FileStream::write(reinterpret_cast<char*>(dataWrite.data()), dataSize * sizeof(int32));
    FileStream::closeAllStream();

    FileStream::startReadStream("_binFileTest.oefile");
    FileStream::read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));

    dataRead.resize(dataSize);
    FileStream::read(reinterpret_cast<char*>(dataRead.data()), dataSize * sizeof(int32));
    FileStream::closeAllStream();

    std::stringstream ss;
    ss << "Binary value data : \n";

    for(const int32 i : dataRead) {
      ss << "  " << i << '\n';
    }

    FileStream::startOutputStreamText("_TestTextOutput.txt");
    FileStream::outputText(ss.str());
    FileStream::closeOutputStreamText();
```