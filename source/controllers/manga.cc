#include "manga.h"

void Manga::uploadManga(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    auto resp = HttpResponse::newHttpResponse();

    drogon::MultiPartParser fileUpload;
    if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
    {
        resp->setStatusCode(k400BadRequest);
        resp->addHeader("manga-neko-error", "No file found or 0 length request received.");
        callback(resp);
    }
    for (auto& file : fileUpload.getFiles())
    {
        // TODO: Register file on the DB here!
        if (file.fileLength() > 0)
        {
            file.saveAs(file.getFileName());
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