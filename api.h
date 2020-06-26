#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "json.hpp"
#ifndef __API_H
#define __API_H
class API {
    public:
        API();
        std::vector<float> getUserBest(std::string player, int count);
        void pushData(char data);
    private:
        std::string data;
        CURL *c;
};
#endif