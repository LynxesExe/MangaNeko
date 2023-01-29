#include <drogon/HttpAppFramework.h>
int main() {
    drogon::app().loadConfigFile("../config/config.json").setClientMaxBodySize(2147483647).setClientMaxMemoryBodySize(10 * 1024 * 1024).run();
    return 0;
}
