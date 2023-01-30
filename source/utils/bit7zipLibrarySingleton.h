#pragma once
#include <bit7z/bitfileextractor.hpp>
class Bit7ZipLibrarySingleton {
public:

	const static bit7z::Bit7zLibrary& getBit7zLibrarySingleton()
	{
		const static bit7z::Bit7zLibrary lib{ "../dependencies/7z.so" };
		return lib;
	}
};