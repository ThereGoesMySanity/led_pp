#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>
#include "connection.h"
#include "display.h"
#include "mode.h"
#include "settings.h"

bool interruptReceived = false;
int interruptFd;
int writeInterruptFd;
static void interruptHandler(int signo)
{
    interruptReceived = true;
    char buf[16] = { 1 };
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
    
    Connection c;
    Settings settings("settings.cfg", argc, argv);
    Display d(mat, &c, &settings);

    bool connected = c.connect();
    if (connected)
    {
		settings.loadModes();
		d.Start();

		while (!interruptReceived && connected)
		{
			while (!interruptReceived && c.getData());
            if (interruptReceived) break;
            *(float*)(c.bufferAddr()) = 0.f;
			connected = c.connect();
		}
    }
    d.Stop();
    mat->Clear();
    delete mat;
    close(interruptFd);
    close(writeInterruptFd);
}
