#include "connection.h"
#include "display.h"
int main(int argc, char** argv) {

    RGBMatrix::Options defaults;
    defaults.chain_length = 2;
    defaults.led_rgb_sequence = "RBG";
    rgb_matrix::RuntimeOptions runtime_opt;
    runtime_opt.drop_privileges = 0;
    RGBMatrix *mat = CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime_opt);

    Connection c;
    bool connected = c.connect();
    void *data = c.bufferAddr();

    Mode modes[2] = {ACC, LINES};

    Display d(mat, sizeof(modes)/sizeof(Mode), modes);
    //TODO: get pp from osu!api
    d.addLine(100);
    d.addLine(200);
    d.addLine(270);
    d.addLine(500);
    d.addLine(850);
    d.setData((float*)data, (int*)data + 3);
    d.Start();

    while(connected) {
        while(c.getData()) ;
        ((float*)data)[0] = 0;
        connected = c.connect();
    }
    delete mat;
}
