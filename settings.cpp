#include <fstream>
#include <iostream>
#include <unistd.h>
#include "settings.h"
#include "mode.h"
extern int interruptFd;
Settings::Settings(std::string file, int argc, char** argv)
	: file(file), modeRegex("([A-Z_]*)\\((\\d+),(\\d+),(\\d+),(\\d+),?(.*?)\\)")
	, cmd("LED matrix pp display", ' ', "0.5")
	, playcount("t","top-plays","Number of top plays to get",false,50,"int",cmd)
{
    cmd.parse(argc, argv);

	eventQueue = inotify_init();
	watch = inotify_add_watch(eventQueue, file.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);
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
	char buffer[16 * sizeof(struct inotify_event)];
    fd_set set;
    FD_ZERO(&set);
    FD_SET(eventQueue, &set);
	FD_SET(interruptFd, &set);
	int max = std::max(eventQueue, interruptFd);
	while (select(max + 1, &set, NULL, NULL, NULL) > -1)
	{
		if (FD_ISSET(interruptFd, &set)) break;

		int length = read(eventQueue, buffer, 16 * sizeof(struct inotify_event));

		if (length < 0) break;

		parse();
		loadModes();

		if (((struct inotify_event*)buffer)->mask & IN_IGNORED) watch = inotify_add_watch(eventQueue, file.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);
		FD_ZERO(&set);
		FD_SET(eventQueue, &set);
		FD_SET(interruptFd, &set);
	}
}
void Settings::parse()
{
    std::ifstream fs;
    fs.open(file);
	std::string uname;
	fs >> uname;
	if (uname.compare(username) != 0)
	{
		username = uname;
		topPlays = api.getUserBest(username, playcount);
		std::sort(topPlays.begin(), topPlays.end());
	}
	
	modes.clear();
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
	if (display == nullptr) return;
    std::smatch match;
	std::lock_guard<std::mutex> lock(display->modeLock);
	display->clearModes();
	for (std::string modeStr : modes)
	{
		std::regex_match(modeStr, match, modeRegex);
		printf("Added mode %s\n", match[1].str().c_str());
		Mode* mode = Mode::CreateMode(display, match[1],
			{ std::stoi(match[2].str()), std::stoi(match[3].str()),
			 std::stoi(match[4].str()), std::stoi(match[5].str()) },
			match[6]);
		display->addMode(mode);
	}
}
