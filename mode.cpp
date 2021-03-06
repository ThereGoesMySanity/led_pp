#include "mode.h"
#include <sstream>
Mode* Mode::CreateMode(Display* d, std::string name, Rectangle r, std::string args)
{
    std::stringstream ss(args);
    std::string arg;
    Mode* mode = nullptr;
    if (name.compare("ACC_BAR") == 0)
    {
        float accOffset = 0.9f;
        if (std::getline(ss, arg, ','))
        {
            accOffset = std::stof(arg);
        }
        mode = new AccMode(d, r, &d->data, accOffset);
    }
    else if (name.compare("PP_TEXT") == 0)
    {
        int type = RT;
        if (std::getline(ss, arg, ','))
        {
            type = pdaTable.at(arg);
        }
        std::string font = "../rpi-rgb-led-matrix/fonts/13x6.bdf";
        if (std::getline(ss, arg, ','))
        {
            font = arg;
        }
        mode = new PPMode(d, r, &d->data, type, font.c_str());
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
        mode = new FixedWindow(d, r, &d->data, maxType, customMax);
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
        mode = new FixedSizeWindow(d, r, &d->data, &d->settings->topPlays, scale, locked, drawBarText, drawLineText);
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
        mode = new ScalingWindow(d, r, &d->data, &d->settings->topPlays, margin, integerScales, drawBarText, drawLineText);
    }
    return mode;
}

void FixedSizeWindow::Draw()
{
    int y0;
    if (data->pp->rtPP < scale * (area.height / 2))
    {
        y0 = 0;
    }
    else if (locked)
    {
        y0 = floor(data->pp->rtPP / (scale * area.height)) * (scale * area.height);
    }
    else
    {
        y0 = data->pp->rtPP - scale * (area.height / 2);
    }
    LineMode::DrawLines(y0, scale);
}

void ScalingWindow::Draw()
{
    float scale = data->pp->rtPP / (area.height - margin);
    for (float pp : *lines)
    {
        if (pp > data->pp->rtPP)
        {
            scale = pp / (area.height - margin);
            break;
        }
    }
    if (integerScales) scale = (int)scale;
    DrawLines(0, scale);
}

void LineMode::DrawLines(int y0, float scale)
{
    int line = (data->pp->rtPP - y0) / scale;
    std::vector<float> lines(*this->lines);
    for (int i = 0; i < line; i++)
    {
        display->DrawLine(area.x, area.y + i, area.x + area.width - 1, area.y + i, CURRENT_PLAY);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        int yval = (lines[i] - y0) / scale;
        if (yval >= 0 && yval < area.height)
        {
            Color color = LINE_COLOR;
            if (lines[i] < data->pp->rtPP) color = Color(255, 255, 160);
            display->DrawLine(area.x, area.y + yval, area.x + area.width - 1, area.y + yval, color);
            if (i == 0 || (yval - display->FontHeight() - 2 > (lines[i - 1] - y0) / scale))
            {
                display->DrawNumbers(lines[i], area.x + area.width - 2, area.y + yval - 2, LINE_COLOR, false);
            }
            else if (i == lines.size() - 1 || (yval + display->FontHeight() + 2 < (lines[i + 1] - y0) / scale))
            {
                display->DrawNumbers(lines[i], area.x + area.width - 2, area.y + yval + display->FontHeight() + 2, LINE_COLOR, false);
            }
        }
    }
}

float FixedWindow::max()
{
    switch (maxType) {
        case MAX_PP: return data->pp->maxPP;
        case TOP_PLAY: return display->settings->topPlays[0];
        case CUSTOM: return customMax;
    }
    return 0;
}
void FixedWindow::Draw()
{
    float scale = max() / area.height;
    int fcLine = (int)(data->pp->fcPP / scale);
    int rtLine = (int)(data->pp->rtPP / scale);
    display->DrawLine(area.x, fcLine, area.x + area.width - 1, fcLine, FC_LINE);
    for (int i = 0; i < rtLine; i++)
    {
        display->DrawLine(area.x, area.y + i, area.x + area.width - 1, area.y + i, CURRENT_PLAY);
    }
}

void AccMode::Draw()
{
    int scale = data->hit->hit300 + data->hit->hit100 + data->hit->hit50 + data->hit->misses;
    if (scale == 0)
        return;
    float offset = scale * accOffset;
    int line300 = (int)((data->hit->hit300 - offset) / (scale - offset) * area.height);
    int line100 = (int)((data->hit->hit300 + data->hit->hit100 - offset) / (scale - offset) * area.height);
    int line50 = (int)((data->hit->hit300 + data->hit->hit100 + data->hit->hit50 - offset) / (scale - offset) * area.height);
    Color c = c300;
    for (int i = 0; i < area.height; i++)
    {
        if (i < line300)
        {
            c = c300;
        }
        else if (i < line100)
        {
            c = c100;
        }
        else if (i < line50)
        {
            c = c50;
        }
        else
        {
            c = cMiss;
        }
        display->DrawLine(area.x, area.y + i, area.x + area.width - 1, area.y + i, c);
    }
    if (scale > 0)
    {
        float acc = 100.0 * (data->hit->hit300 * 6 + data->hit->hit100 * 2 + data->hit->hit50) / (6 * scale);
        display->DrawNumbers(acc, area.x, area.y + 4, TEXT_COLOR, true);
    }
}

void PPMode::Draw() {
    char buf[6] = {0};
    sprintf(buf, "%ipp", (int)(((float *)data->pp)[ppType]));
    std::string ppText(buf);
    int offset = (area.width - ppText.size() * font.height()) / 2;
    display->DrawText(&font, ppText, area.x + offset, area.y + area.height - 2, c, true);
}
