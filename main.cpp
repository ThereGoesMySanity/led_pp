#include <signal.h>
#include <sys/socket.h>
#include "connection.h"
#include "display.h"
#include "api.h"
#include "mode.h"
#include "settings.h"

bool interruptReceived = false;
int interruptFd;
static void interruptHandler(int signo)
{
    interruptReceived = true;
    char buf[16] = { 1 };
    write(interruptFd, &buf, 16);
}

#define NUM_TOP_PLAYS 50
bool running;
int main(int argc, char **argv)
{
    struct sigaction int_handler = {};
    int_handler.sa_handler = interruptHandler;
    int_handler.sa_flags = 0;
    sigemptyset(&int_handler.sa_mask);
    sigaction(SIGINT, &int_handler, 0);
    //siginterrupt(SIGINT, 1);

    interruptFd = socket(AF_UNIX, SOCK_STREAM, 0);

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
    if (connected)
    {
		DataPacket *data = (DataPacket *)c.bufferAddr();

		d.setData({&data->pp, &data->hit, top});
		settings.loadModes();
		d.Start();

		while (!interruptReceived && connected)
		{
			while (!interruptReceived && c.getData());
            if (interruptReceived) break;
			data->pp.maxPP = 0;
			connected = c.connect();
		}
    }
    d.Stop();
    mat->Clear();
    delete mat;
}
