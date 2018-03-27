#include "connection.h"
#include "display.h"
#include "api.h"

#define NUM_TOP_PLAYS 50

int main(int argc, char** argv) {
    RGBMatrix::Options defaults;
    defaults.chain_length = 2;
    defaults.led_rgb_sequence = "RBG";
    rgb_matrix::RuntimeOptions runtime_opt;
    runtime_opt.drop_privileges = 0;
    RGBMatrix *mat = CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime_opt);

    Display d(mat);

    std::ifstream file;
    file.open("settings.cfg");
    std::string name;
    file >> name;
    std::string mode;
    file >> mode;
    while(!file.eof()) {
        printf("Added mode %s\n", mode.c_str());
        d.addMode(mode);
        file >> mode;
    }

    API a;
    float *pp = a.getUserBest(name, NUM_TOP_PLAYS);
    d.setTopPlays(pp, NUM_TOP_PLAYS);
    d.addLine(100);
    d.addLine(200);
    d.addLine(*pp);
    d.addLine(500);
    d.addLine(850);


    Connection c;
    bool connected = c.connect();
    void *data = c.bufferAddr();

    d.setData((float*)data, (int*)data + 3);
    d.Start();

    while(connected) {
        while(c.getData()) ;
        ((float*)data)[0] = 0;
        connected = c.connect();
    }
    delete mat;
}
