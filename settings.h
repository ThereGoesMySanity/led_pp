#ifndef __SETTINGS_H
#define __SETTINGS_H
#include <thread>
#include <regex>
#include <sys/inotify.h>
#include <tclap/CmdLine.h>
#include "display.h"
#include "api.h"
class Settings
{
public:
	Settings(std::string file, Display* display, int argc, char** argv);
	~Settings();
	std::string getName() { return username; }
	void loadModes();
	std::vector<float> topPlays;
private:
	int eventQueue, watch;
	std::string file;
	std::string username;
	std::thread readThread;
	TCLAP::CmdLine cmd;
	TCLAP::ValueArg<int> playcount;
	std::vector<std::string> modes;
	Display* d;
	std::regex modeRegex;
	API api;
	void readLoop();
	void parse();
};

#endif