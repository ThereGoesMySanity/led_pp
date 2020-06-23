#ifndef _a_MODE_H
#define _a_MODE_H
#include "display.h"

typedef struct Rectangle
{
	int x, y, width, height;
} Rectangle;

class Mode
{
public:
	Mode(Display* display, Rectangle area, OsuData* data)
		: display(display), area(area), data(data) {}
	virtual void Draw() = 0;

protected:
	Display* display;
	Rectangle area;
	OsuData* data;
};
class AccMode : public Mode
{
public:
	AccMode(Display* display, Rectangle area, OsuData* data, float accOffset)
		: Mode(display, area, data), accOffset(accOffset) {}
	virtual void Draw();

private:
	float accOffset;
};

class PPMode : public Mode
{
public:
	PPMode(Display* display, Rectangle area, OsuData* data, int ppType, const char* font)
		: Mode(display, area, data), ppType(ppType) {
		this->font.LoadFont(font);
	}
	virtual void Draw();

private:
	int ppType;
	Font font;
	Color c = Color(200, 200, 200);
};

class LineMode : public Mode
{
public:
	LineMode(Display* display, Rectangle area, OsuData* data, std::vector<float> lines, bool drawBarText = false, bool drawLineText = true)
		: Mode(display, area, data), lines(lines), drawBarText(drawBarText), drawLineText(drawLineText)
	{
		std::sort(lines.begin(), lines.end());
	}
	virtual void Draw() = 0;

protected:
	std::vector<float> lines;
	bool drawBarText, drawLineText;
};
enum FixedWindowMaxTypes
{
	MAX_PP,
	TOP_PLAY,
	CUSTOM
};
static std::unordered_map<std::string, int> const fwmtTable = { {"MAX_PP", 0}, {"TOP_PLAY", 1}, {"CUSTOM", 2} };
class FixedWindow : public Mode
{
public:
	FixedWindow(Display* display, Rectangle area, OsuData* data, int maxType, float customMax)
		: Mode(display, area, data), maxType(maxType), customMax(customMax) {}
	virtual void Draw();

private:
	float max();
	int maxType;
	float customMax;
};

class FixedSizeWindow : public LineMode
{
public:
	FixedSizeWindow(Display* display, Rectangle area, OsuData* data, std::vector<float> lines, float scale, bool drawBarText, bool drawLineText)
		: LineMode(display, area, data, lines, drawBarText, drawLineText), scale(scale) {}
	virtual void Draw();

private:
	float scale;
};

class ScalingWindow : public LineMode
{
public:
	ScalingWindow(Display* display, Rectangle area, OsuData* data, std::vector<float> lines, int margin, bool integerScales, bool drawBarText, bool drawLineText)
		: LineMode(display, area, data, lines, drawBarText, drawLineText), margin(margin), integerScales(integerScales) {}
	virtual void Draw();

private:
	int margin;
	bool integerScales;
};
#endif