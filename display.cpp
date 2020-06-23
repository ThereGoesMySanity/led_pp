#include "display.h"
#include "mode.h"
#include <sstream>
#include <string>

Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat)
{
    font.LoadFont("rpi-rgb-led-matrix/fonts/6x4-4.bdf");
}
Display::~Display()
{
    for (Mode *m : modes)
        delete m;
}
void Display::Run()
{
    while (running())
    {
        canvas()->Fill(0, 0, 0);
        if (data.pp->maxPP != 0)
        {
            int numModes = modes.size();
            for (int m = 0; m < numModes; m++)
            {
                modes[m]->Draw();
            }
        }
        usleep(10000);
    }
}
void Display::DrawLine(int x0, int y0, int x1, int y1, Color c)
{
    rgb_matrix::DrawLine(canvas(), 63 - y0, 31 - x0, 63 - y1, 31 - x1, c);
}
void Display::DrawNumbers(float num, int x, int y, Color c, bool left, int dec) { DrawNumbers(&font, num, x, y, c, left, dec); }

void Display::DrawNumbers(Font *font, float num, int x, int y, Color c, bool left, int dec)
{
    DrawText(font, std::to_string((int)num), x, y, c, left);
}

void Display::DrawText(Font *font, std::string text, int x, int y, Color c, bool left)
{
    std::reverse(text.begin(), text.end());
    int xpos = x - font->height();
    if (left)
    {
        xpos += font->height() * text.size();
    }
    VerticalDrawText(canvas(), *font, 63 - y, 31 - xpos, c, NULL, text.c_str());
}
void Display::addLine(float line)
{
    m_pp_lines.insert(line);
}
void Display::setData(OsuData data)
{
    this->data = data;
}
void Display::addMode(std::string name, Rectangle r, std::string args)
{
    std::stringstream ss(args);
    std::string arg;
    Mode *mode;
    if (name.compare("ACC_BAR") == 0)
    {
        float accOffset = 0.9f;
        if (std::getline(ss, arg, ','))
        {
            accOffset = std::stof(arg);
        }
        mode = new AccMode(this, r, &data, accOffset);
    }
    else if (name.compare("PP_TEXT") == 0)
    {
        int type = RT;
        if (std::getline(ss, arg, ','))
        {
            type = pdaTable.at(arg);
        }
        std::string font = "rpi-rgb-led-matrix/fonts/13x6.bdf";
        if (std::getline(ss, arg, ','))
        {
            font = arg;
        }
        mode = new PPMode(this, r, &data, type, font.c_str());
    }
    else if (name.compare("FIXED_WIN_BAR") == 0)
    {
        int maxType = 0;
        if (std::getline(ss, arg, ','))
        {
            maxType = fwmtTable.at(arg);
        }
        float customMax = 0;
        if (maxType == CUSTOM && std::getline(ss, arg, ','))
        {
            customMax = std::stof(arg);
        }
        mode = new FixedWindow(this, r, &data, maxType, customMax);
    }
    else if (name.compare("FIXED_SIZE_BAR") == 0)
    {
        float scale = 4;
        if (std::getline(ss, arg, ','))
        {
            scale = std::stof(arg);
        }
        bool locked = false;
        if (std::getline(ss, arg, ','))
        {
            locked = arg.compare("true") == 0;
        }
        bool drawBarText = false;
        if (std::getline(ss, arg, ','))
        {
            drawBarText = arg.compare("true") == 0;
        }
        bool drawLineText = true;
        if (std::getline(ss, arg, ','))
        {
            drawLineText = arg.compare("true") == 0;
        }
        mode = new FixedSizeWindow(this, r, &data, data.topPlays, scale, locked, drawBarText, drawLineText);
    }
    else if (name.compare("SCALING_BAR") == 0)
    {
        int margin = 4;
        if (std::getline(ss, arg, ','))
        {
            margin = std::stoi(arg);
        }
        bool integerScales = false;
        if (std::getline(ss, arg, ','))
        {
            integerScales = arg.compare("true") == 0;
        }
        bool drawBarText = false;
        if (std::getline(ss, arg, ','))
        {
            drawBarText = arg.compare("true") == 0;
        }
        bool drawLineText = true;
        if (std::getline(ss, arg, ','))
        {
            drawLineText = arg.compare("true") == 0;
        }
        mode = new ScalingWindow(this, r, &data, data.topPlays, margin, integerScales, drawBarText, drawLineText);
    }
    if (mode != nullptr)
    {
        modes.push_back(mode);
    }
}
void Display::setTopPlays(float *f, int count)
{
    m_top_plays = f;
    m_plays_count = count;
}
