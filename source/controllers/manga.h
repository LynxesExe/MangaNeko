#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
class Manga:public drogon::HttpController<Manga>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Manga::uploadManga, "/upload", Post);
    METHOD_LIST_END
    void uploadManga(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback) const;
};
