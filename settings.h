#ifndef __SETTINGS_H
#define __SETTINGS_H
#include <thread>
#include <regex>
#include <sys/inotify.h>
#include "display.h"
class Settings
{
public:
	Settings(std::string file, Display* display);
	~Settings();
	std::string getName() { return username; }
	void loadModes();
private:
	int eventQueue, watch;
	std::string file;
	std::string username;
	std::thread readThread;
	std::vector<std::string> modes;
	Display* d;
	std::regex modeRegex;
	void readLoop();
	void parse();
};

#endif