#pragma once
#include <drogon/HttpController.h>
using namespace drogon;

namespace alpha1
{
    class Manga :public drogon::HttpController<Manga>
    {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Manga::uploadManga, "/upload", Post);
        METHOD_ADD(Manga::getPage, "/page/{1}/{2}/{3}", Get);
        METHOD_ADD(Manga::listAvailable, "/listAvailable", Get);
        METHOD_ADD(Manga::getMetadata, "/getMetadata/{1}", Get);
        METHOD_LIST_END
            void uploadManga(const HttpRequestPtr& req,
                std::function<void(const HttpResponsePtr&)>&& callback) const;
        void getPage(const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback,
            std::string&& mangaName, unsigned int chapter, unsigned int page) const;
        void listAvailable(const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) const;
        void getMetadata(const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback,
            std::string&& mangaName) const;
    };
}