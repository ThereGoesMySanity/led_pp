#include "mode.h"
#include <algorithm>
void FixedSizeWindow::Draw()
{
    int y0 = (int)std::min(0.f, data->pp->rtPP - scale * (area.height / 2));
    DrawLines(y0, scale);
}

void ScalingWindow::Draw()
{
    float scale = data->pp->rtPP / (area.height - margin);
    for (float pp : lines)
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
    for (int i = 0; i < line; i++)
    {
        display->DrawLine(area.x, area.y + i, area.x + area.width - 1, area.y + i, CURRENT_PLAY);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        int yval = (lines[i] - y0) / scale;
        if (yval < area.height)
        {
            display->DrawLine(area.x, area.y + yval, area.x + area.width - 1, area.y + yval, LINE_COLOR);
            if (drawLineText)
            {
                if (i == 0 || (yval + display->FontHeight() + 2 < (lines[i - 1] - y0) / scale))
                {
                    display->DrawNumbers(lines[i], area.x + area.width - 2, area.y + yval + 2, LINE_COLOR, false);
                }
                else if (i == lines.size() - 1 || (yval - display->FontHeight() - 2 > (lines[i + 1] - y0) / scale))
                {
                    display->DrawNumbers(lines[i], area.x + area.width - 2, area.y + yval - 2 - display->FontHeight(), LINE_COLOR, false);
                }
            }
        }
    }
}

float FixedWindow::max()
{
    switch (maxType) {
        case MAX_PP: return data->pp->maxPP;
        case TOP_PLAY: return data->topPlays[0];
        case CUSTOM: return customMax;
    }
    return 0;
}
void FixedWindow::Draw()
{
    float scale = max() / area.height;
    int fcLine = (int)(data->pp->fcPP / scale);
    int rtLine = (int)(data->pp->rtPP / scale);
    display->DrawLine(area.x, fcLine, area.x + area.width, fcLine, FC_LINE);
    for (int i = 0; i < rtLine; i++)
    {
        display->DrawLine(area.x, i, area.x + area.width, i, CURRENT_PLAY);
    }
}

void AccMode::Draw()
{
    int scale = data->hit->hit300 + data->hit->hit100 + data->hit->hit50 + data->hit->misses;
    if (scale == 0)
        return;
    float offset = scale * accOffset;
    int line300 = (int)((data->hit->hit300 - offset) * area.height / (scale - offset));
    int line100 = (int)((data->hit->hit300 + data->hit->hit100 - offset) * area.height / (scale - offset));
    int line50 = (int)((data->hit->hit300 + data->hit->hit100 + data->hit->hit50 - offset) * area.height / (scale - offset));
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
        display->DrawLine(area.x, area.y + i, area.x + area.width, area.y + i, c);
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
    display->DrawText(&font, ppText, area.x, area.y, c, true);
}