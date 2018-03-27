#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "json.hpp"
class API {
    public:
        API();
        float* getUserBest(std::string player, int count);
        void pushData(char data);
    private:
        std::string data;
        CURL *c;
};
