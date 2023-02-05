#include "manga.h"
#include "../utils/config.h"
#include "../utils/bit7zipLibrarySingleton.h"
#include <filesystem>

using namespace alpha1;

void Manga::uploadManga(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    auto resp = HttpResponse::newHttpResponse();

    drogon::MultiPartParser fileUpload;
    if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
    {
        resp->setStatusCode(k400BadRequest);
        resp->addHeader("manga-neko-error", "No file found or 0 length request received.");
    }
    for (auto& file : fileUpload.getFiles())
    {
        // TODO: Register file on the DB here!
        // TODO: Find a better way to return errors to the user
        if (file.fileLength() > 0)
        {
            std::filesystem::path currentFilePath(MangaNeko::globalConfiguration.entryPointPath);
            currentFilePath.append(file.getFileName());
            if (std::filesystem::exists(currentFilePath))
            {
                resp->setStatusCode(k207MultiStatus);
                resp->addHeader("manga-neko-error", "A file with the same name already exists.");
            }
            else
            {
                file.saveAs(file.getFileName());
            }
        }
        else
        {
            resp->setStatusCode(k207MultiStatus);
            resp->addHeader("manga-neko-error", "One or more files is 0 length, ignoring those files.");
        }
    }

    resp->setStatusCode(k200OK);
    callback(resp);
}

void Manga::getPage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback,
    std::string&& mangaName, unsigned int chapter, unsigned int page) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);

    std::filesystem::path currentFilePath(MangaNeko::globalConfiguration.entryPointPath);
    currentFilePath.append(mangaName);
    if (std::filesystem::exists(currentFilePath))
    {
        // TODO: Temporary hardcoded to ZIP, figure out how to dela with this later
        bit7z::BitArchiveReader archive{ Bit7ZipLibrarySingleton::getBit7zLibrarySingleton(), currentFilePath, bit7z::BitFormat::Zip };
        auto archiveMetadata = archive.items();
        std::sort(archiveMetadata.begin(), archiveMetadata.end(), [](const auto& lhs, const auto& rhs)
            {
                return lhs.name() < rhs.name();
            });
        bool found = false;
        std::string itemPath;
        for (auto currentDirectory : archiveMetadata)
        {
            if (!found && currentDirectory.isDir() && currentDirectory.path().find(std::to_string(chapter)) != std::string::npos)
            {
                for (auto& archiveItem : archiveMetadata)
                {
                    if (!found && !archiveItem.isDir()
                        && archiveItem.path().find(currentDirectory.path()) != std::string::npos
                        && archiveItem.name().find(std::to_string(page)) != std::string::npos)
                    {
                        found = true;
                        itemPath = archiveItem.path();
                    }
                }
            }
        }
        if (found)
        {
            bit7z::BitFileExtractor extractor{ Bit7ZipLibrarySingleton::getBit7zLibrarySingleton(), bit7z::BitFormat::Zip };
            std::vector<bit7z::byte_t> buffer;
            extractor.extractMatching(currentFilePath, itemPath, buffer);
            resp = HttpResponse::newFileResponse(&buffer.front(), buffer.size());
            resp->setStatusCode(k200OK);
        }
        else
        {
            resp->setStatusCode(k404NotFound);
            resp->addHeader("manga-neko-error", "Manga page not found!");
        }
    }
    else
    {
        resp->setStatusCode(k404NotFound);
        resp->addHeader("manga-neko-error", "Manga entry not found.");
    }
    callback(resp);
}

void Manga::listAvailable(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    Json::Value responseRoot;
    Json::Value entriesArray(Json::arrayValue);
    for (const auto& entry : std::filesystem::directory_iterator(MangaNeko::globalConfiguration.entryPointPath))
    {
        Json::Value entryValue;
        entryValue["name"] = std::string(std::filesystem::relative(entry.path(), MangaNeko::globalConfiguration.entryPointPath));
        entriesArray.append(entryValue);
    }
    responseRoot["entries"] = entriesArray;

    Json::StreamWriterBuilder builder;
    std::string responseBody = Json::writeString(builder, responseRoot);
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(responseBody);
    resp->setStatusCode(k200OK);
    callback(resp);
}

void Manga::getMetadata(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback,
    std::string&& mangaName) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);

    std::filesystem::path currentFilePath(MangaNeko::globalConfiguration.entryPointPath);
    currentFilePath.append(mangaName);

    Json::Value mangaStructureRoot;
    Json::Value mangaListArray(Json::arrayValue);

    if (std::filesystem::exists(currentFilePath))
    {

        bit7z::BitArchiveReader archive{ Bit7ZipLibrarySingleton::getBit7zLibrarySingleton(), currentFilePath, bit7z::BitFormat::Zip };
        auto archiveMetadata = archive.items();
        std::sort(archiveMetadata.begin(), archiveMetadata.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.name() < rhs.name();
            });

        for (auto currentDirectory : archiveMetadata)
        {
            if (currentDirectory.isDir() && currentDirectory.path().find_first_of("1234567890") != std::string::npos)
            {
                Json::Value chapterEntry(Json::objectValue);
                // This won't work, we need for this to be a number, but how do we decide is chapters start from 0 or 1?
                std::string strippedChapterName = currentDirectory.name();
                strippedChapterName.erase(std::remove_if(strippedChapterName.begin(), strippedChapterName.end(), [](char c) {
                        return !std::isdigit(c);
                    }));
                chapterEntry["chapter name"] = strippedChapterName;
                // Starting from -1 since we need to take into account the directory entry
                int pagesCount = -1;
                for (auto currentPage : archiveMetadata)
                {
                    if (currentPage.path().find(currentDirectory.path()) != std::string::npos)
                    {
                        pagesCount++;
                    }
                }
                chapterEntry["pages"] = pagesCount;
                mangaListArray.append(chapterEntry);
            }
        }
    }
    else
    {
        resp->setStatusCode(k404NotFound);
        resp->addHeader("manga-neko-error", "Manga entry not found.");
    }

    mangaStructureRoot["mangas"] = mangaListArray;

    Json::StreamWriterBuilder builder;
    std::string responseBody = Json::writeString(builder, mangaStructureRoot);
    resp->setBody(responseBody);
    callback(resp);
}