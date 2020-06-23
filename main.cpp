#include <signal.h>
#include "connection.h"
#include "display.h"
#include "api.h"
#include "mode.h"
#include "settings.h"

bool interruptReceived = false;
static void interruptHandler(int signo)
{
    interruptReceived = true;
}

#define NUM_TOP_PLAYS 50
bool running;
int main(int argc, char **argv)
{
    signal(SIGTERM, interruptHandler);
    signal(SIGINT, interruptHandler);

    RGBMatrix::Options defaults;
    defaults.chain_length = 2;
    defaults.led_rgb_sequence = "RBG";
    rgb_matrix::RuntimeOptions runtime_opt;
    runtime_opt.drop_privileges = 0;
    RGBMatrix *mat = CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime_opt);
    
    Display d(mat);

    Settings settings("settings.cfg", &d);

    API a;
    std::vector<float> top = a.getUserBest(settings.getName(), NUM_TOP_PLAYS);

    Connection c;
    bool connected = c.connect();
    DataPacket *data = (DataPacket *)c.bufferAddr();

    d.setData({&data->pp, &data->hit, top});
    settings.loadModes();
    d.Start();

    while (!interruptReceived && connected)
    {
		while (!interruptReceived && c.getData());
        data->pp.maxPP = 0;
        connected = c.connect();
    }
    delete mat;
}
