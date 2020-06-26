#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>
#include <tclap/CmdLine.h>
#include "connection.h"
#include "display.h"
#include "api.h"
#include "mode.h"
#include "settings.h"

bool interruptReceived = false;
int interruptFd;
int writeInterruptFd;
static void interruptHandler(int signo)
{
    interruptReceived = true;
    char buf[16] = { 1 };
    printf("Interrupt handler\n");
    write(writeInterruptFd, buf, 16);
}

bool running;
int main(int argc, char **argv)
{
    struct sigaction int_handler = {};
    int_handler.sa_handler = interruptHandler;
    int_handler.sa_flags = 0;
    sigemptyset(&int_handler.sa_mask);
    sigaction(SIGINT, &int_handler, 0);

    int pipefds[2];
    pipe(pipefds);
    interruptFd = pipefds[0];
    writeInterruptFd = pipefds[1];

    RGBMatrix::Options defaults;
    defaults.chain_length = 2;
    defaults.led_rgb_sequence = "RBG";
    rgb_matrix::RuntimeOptions runtime_opt;
    runtime_opt.drop_privileges = 0;
    RGBMatrix *mat = CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime_opt);
    
    Display d(mat);

    Settings settings("settings.cfg", &d);

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
    close(interruptFd);
    close(writeInterruptFd);
}
