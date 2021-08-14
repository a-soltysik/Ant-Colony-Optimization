#pragma once

#include <fstream>

class FileException : public std::exception
{
private:
    std::ios::iostate state;
    std::string sourceFileName;
    std::string message;
    mutable std::string whatBuffer;
    uint32_t line;

    std::string getState() const noexcept;
public:
    FileException(uint32_t line, const char* sourceFileName,
                  const std::ios_base::iostate& state, const std::string& message) noexcept
            : line(line),  sourceFileName(sourceFileName), state(state), message(message) {}
    FileException(FileException&&) = default;
    FileException(const FileException&) = default;
    FileException& operator=(const FileException&) = default;
    virtual ~FileException() = default;

    const char* what() const noexcept override;
};

#define MAKE_FILE_EXCEPTION(ios, msg) FileException(__LINE__, __FILE__, (ios).rdstate(), msg)
#define FILE_THROW_IF_FAILED(ios, msg) if (!(ios).good()) throw MAKE_FILE_EXCEPTION(ios, msg)