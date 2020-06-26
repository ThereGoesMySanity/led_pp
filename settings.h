#ifndef __SETTINGS_H
#define __SETTINGS_H
#include <thread>
#include <regex>
#include <sys/inotify.h>
#include <tclap/CmdLine.h>
#include "api.h"
class Display;
class Settings
{
public:
	Settings(std::string file, int argc, char** argv);
	~Settings();
	std::string getName() { return username; }
	void loadModes();
	std::vector<float> topPlays;
	Display *display = nullptr;
private:
	int eventQueue, watch;
	std::string file;
	std::string username;
	std::thread readThread;
	TCLAP::CmdLine cmd;
	TCLAP::ValueArg<int> playcount;
	std::vector<std::string> modes;
	std::regex modeRegex;
	API api;
	void readLoop();
	void parse();
};

#endif