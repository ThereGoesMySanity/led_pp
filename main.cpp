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
    
    Display d(mat);
    //TODO: get pp from osu!api
    d.setMaxPP(270);
    d.Start();

    float *pp;
    while(connected) {
        while(c.getPP(pp)){
            d.setPP(pp);
        }
        connected = c.connect();
    }
    delete mat;
}
