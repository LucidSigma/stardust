#include "stardust/data/Containers.h"

void* __cdecl operator new[](const std::size_t size, const char* name, const int flags, const unsigned int debugFlags, const char* file, const int line)
{
	return new std::uint8_t[size];
}

void* __cdecl operator new[](const std::size_t, const std::size_t alignment, const std::size_t size, const char* name, const int flags, const unsigned debugFlags, const char* file, const int line)
{
	return new std::uint8_t[size];
}