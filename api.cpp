#include "api.h"

size_t getData(char *buf, size_t size, size_t nmeb, void *up)
{
    API *a = (API *)up;
    int c;
    for (c = 0; c < size * nmeb; c++)
    {
        a->pushData(buf[c]);
    }
    return c;
}

API::API()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    c = curl_easy_init();
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, &getData);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, this);
}
void API::pushData(char d)
{
    data.push_back(d);
}

std::vector<float> API::getUserBest(std::string player, int limit)
{
    std::vector<float> vals;
    data.clear();
    std::string key;
    std::ifstream f;
    f.open("key.dat");
    if (!f.good())
    {
        printf("Cannot find key.dat\n");
        exit(1);
    }
    f >> key;
    std::stringstream url;
    url << "https://osu.ppy.sh/api/get_user_best";
    url << "?k=" << key;
    url << "&u=" << player;
    url << "&limit=" << limit;
    curl_easy_setopt(c, CURLOPT_URL, url.str().c_str());
    CURLcode res = curl_easy_perform(c);
    if (res == CURLE_OK)
    {
        auto result = nlohmann::json::parse(data);
        for (auto a : result)
        {
            std::string pp = a.value("pp", "0.0");
            vals.push_back(std::stof(pp));
        }
    }
    else
    {
        printf("Error fetching top plays\n");
    }
    return vals;
}
