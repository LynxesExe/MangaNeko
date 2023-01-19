#include "TestCtrl.h"
void TestCtrl::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
   std::cout << "REQUEST RECEIVED!" << std::endl;
   auto resp=HttpResponse::newHttpResponse();
   resp->setStatusCode(k200OK);
   resp->setContentTypeCode(CT_TEXT_HTML);
   resp->setBody("Hello World from Drogon!");
   callback(resp);
   //write your application logic here
}
