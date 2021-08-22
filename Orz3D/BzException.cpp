#include "BzException.h"
#include "sstream"


BzException::BzException(int line, const char* file) noexcept
	: line(line), file(file)
{}


const char* BzException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}


const char* BzException::GetType() const noexcept
{
	return "Bz Exception";
}

int BzException::GetLine() const noexcept
{
	return line;
}


const std::string& BzException::GetFile() const noexcept
{
	return file;
}


std::string BzException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}