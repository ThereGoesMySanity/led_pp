#include <regex>
#include "connection.h"
#include "display.h"
#include "api.h"
#include "mode.h"

#define NUM_TOP_PLAYS 50

int main(int argc, char **argv)
{
    RGBMatrix::Options defaults;
    defaults.chain_length = 2;
    defaults.led_rgb_sequence = "RBG";
    rgb_matrix::RuntimeOptions runtime_opt;
    runtime_opt.drop_privileges = 0;
    RGBMatrix *mat = CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime_opt);

    std::ifstream file;
    file.open("settings.cfg");
    std::string name;
    file >> name;

    API a;
    std::vector<float> top = a.getUserBest(name, NUM_TOP_PLAYS);

    Display d(mat);

    std::regex regex("([A-Z_]*)\\((\\d+),(\\d+),(\\d+),(\\d+),?(.*?)\\)");
    std::string mode;
    file >> mode;
    std::smatch match;
    while (std::regex_match(mode, match, regex))
    {
        printf("Added mode %s\n", match[1]);
        d.addMode(match[1],
                  {std::stoi(match[2]), std::stoi(match[3]),
                   std::stoi(match[4]), std::stoi(match[5])},
                  match[6]);
        file >> mode;
    }


    Connection c;
    bool connected = c.connect();
    DataPacket *data = (DataPacket *)c.bufferAddr();

    d.setData({&data->pp, &data->hit, top});
    d.Start();

    while (connected)
    {
        while (c.getData())
            ;
        data->pp.maxPP = 0;
        connected = c.connect();
    }
    delete mat;
}
