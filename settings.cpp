#include <fstream>
#include <iostream>
#include "settings.h"
#include "interrupt.h"
#include "mode.h"
Settings::Settings(std::string file, Display* display) : display(display), file(file), readThread(&Settings::read, this), modeRegex("([A-Z_]*)\\((\\d+),(\\d+),(\\d+),(\\d+),?(.*?)\\)");
{
	eventQueue = inotify_init();
	watch = inotify_add_watch(eventQueue, file.c_str(), IN_MODIFY);
	parse();
}

Settings::~Settings()
{
	inotify_rm_watch(eventQueue, watch);
	close(eventQueue);
}
void Settings::read()
{
	while (!interruptReceived)
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
    std::string name;
    fs >> name;

    std::string mode;
    file >> mode;
    while (!file.eof())
    {
        if (std::regex_match(mode, match, modeRegex))
        {
			modes.push_back(mode);
        }
        file >> mode;
    }
	fs.close();
}

void Settings::loadModes()
{
    std::smatch match;
	d->modeLock.lock();
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
	d->modeLock.unlock();
}
