#include "manga.h"
#include "../utils/config.h"
#include "../utils/bit7zipLibrarySingleton.h"
#include <filesystem>

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