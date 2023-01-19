#include <drogon/HttpAppFramework.h>
int main() {
        
    drogon::app().loadConfigFile("../config/config.json").run();

    return 0;
}
