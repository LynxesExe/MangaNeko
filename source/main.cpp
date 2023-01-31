#include <drogon/HttpAppFramework.h>
#include "utils/config.h"
#include "utils/bit7zipLibrarySingleton.h"
#include <bit7z/bitfileextractor.hpp>
#include <filesystem>

int main() {
    //bit7z::Bit7zLibrary lib{ "../dependencies/7z.so" };

    std::filesystem::path p("/tmp");
    std::cout << "Path: " << p.c_str() << std::endl;

    const bit7z::Bit7zLibrary& lib = Bit7ZipLibrarySingleton::getBit7zLibrarySingleton();
    std::cout << "Config path: " << MangaNeko::globalConfiguration.configFilePath << std::endl;
    std::cout << "Entrypoint : " << MangaNeko::globalConfiguration.entryPointPath << std::endl;
    bit7z::BitFileExtractor extractor{ lib, bit7z::BitFormat::Zip };
    extractor.extract("/tmp/manganeko/uploads/Drag-On Dragoon Utahime Five Manga.cbz", "/tmp/manganeko/cache");
    drogon::app().loadConfigFile(MangaNeko::globalConfiguration.configFilePath).run();
    return 0;
}
