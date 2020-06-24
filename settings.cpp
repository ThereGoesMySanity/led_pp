#include <fstream>
#include <iostream>
#include <unistd.h>
#include "settings.h"
#include "mode.h"
extern bool interruptReceived;
extern int interruptFd;
Settings::Settings(std::string file, Display* display) : d(display), file(file), modeRegex("([A-Z_]*)\\((\\d+),(\\d+),(\\d+),(\\d+),?(.*?)\\)")
{
	eventQueue = inotify_init();
	watch = inotify_add_watch(eventQueue, file.c_str(), IN_MODIFY);
	parse();
	readThread = std::thread(&Settings::readLoop, this);
}

Settings::~Settings()
{
    readThread.join();
	inotify_rm_watch(eventQueue, watch);
	close(eventQueue);
}
void Settings::readLoop()
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(eventQueue, &set);
	FD_SET(interruptFd, &set);
	int max = std::max(eventQueue, interruptFd);
	while (select(max + 1, &set, NULL, NULL, NULL) > -1 && !FD_ISSET(interruptFd, &set))
	{
		int length = read(eventQueue, buffer, 16 * sizeof(struct inotify_event));

		if (length < 0) break;
		
		if (((struct inotify_event*)buffer)->mask & IN_MODIFY)
		{
			parse();
			loadModes();
		}
	}
}
void Settings::parse()
{
    std::ifstream fs;
    fs.open(file);
	fs >> username;

    std::string mode;
    fs >> mode;
    while (!fs.eof())
    {
        if (std::regex_match(mode, modeRegex))
        {
			modes.push_back(mode);
        }
        fs >> mode;
    }
	fs.close();
}

void Settings::loadModes()
{
	if (d->data.pp == nullptr) return;
    std::smatch match;
	std::lock_guard<std::mutex> lock(d->modeLock);
	d->clearModes();
	for (std::string modeStr : modes)
	{
		std::regex_match(modeStr, match, modeRegex);
		printf("Added mode %s\n", match[1].str().c_str());
		Mode* mode = Mode::CreateMode(d, match[1],
			{ std::stoi(match[2].str()), std::stoi(match[3].str()),
			 std::stoi(match[4].str()), std::stoi(match[5].str()) },
			match[6]);
		d->addMode(mode);
	}
}
