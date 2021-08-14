#include "FileException.h"

const char* FileException::what() const noexcept
{
    whatBuffer = std::string()
            .append("[Source File] ").append(sourceFileName)
            .append("\n[Line] ").append(std::to_string(line))
            .append("\n[IO State] ").append(getState())
            .append("\n[Message] ").append(message);
    return whatBuffer.c_str();
}

std::string FileException::getState() const noexcept
{
    std::string msg;
    if (state & std::ios::eofbit)
        msg += "<EOF>";
    if (state & std::ios::badbit)
        msg += "<BAD>";
    if (state & std::ios::failbit)
        msg += "<FAIL>";
    msg += " was set.";
    return msg;
}