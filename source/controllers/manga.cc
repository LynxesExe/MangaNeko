#include "manga.h"
#include "../utils/config.h"
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
    std::string&& mangaId, unsigned int chapter, unsigned int page) const
{
    std::cout << "Requested manga ID: " << mangaId << std::endl;
    std::cout << "Chapter: " << chapter << std::endl;
    std::cout << "Page: " << page << std::endl;
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    callback(resp);
}