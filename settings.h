#ifndef __SETTINGS_H
#define __SETTINGS_H
#include "display.h"
#include <thread>
class Settings
{
public:
	Settings(std::string file, Display* display);
	~Settings();
	std::string getName() { return username; }
	void loadModes();
private:
	int eventQueue, watch;
	std::thread readThread;
	char buffer[16 * sizeof(struct inotify_event)];
	std::string file;
	std::string username;
	std::vector<std::string> modes;
	Display* d;
    std::regex modeRegex("([A-Z_]*)\\((\\d+),(\\d+),(\\d+),(\\d+),?(.*?)\\)");
	void read();
	void parse();
};

#endif