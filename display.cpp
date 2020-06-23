#include "display.h"
#include "mode.h"
#include <sstream>
#include <string>
extern bool interruptReceived;

Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat)
{
    font.LoadFont("rpi-rgb-led-matrix/fonts/6x4-4.bdf");
}
Display::~Display()
{
    clearModes();
}
void Display::Run()
{
    while (!interruptReceived && running())
    {
        canvas()->Fill(0, 0, 0);
        if (data.pp->maxPP != 0)
        {
            modeLock.lock();
            int numModes = modes.size();
            for (int m = 0; m < numModes; m++)
            {
                modes[m]->Draw();
            }
            modeLock.unlock();
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

void Display::clearModes() { for (Mode* m : modes) delete m; modes.clear(); }

void Display::addMode(Mode* mode)
{
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
