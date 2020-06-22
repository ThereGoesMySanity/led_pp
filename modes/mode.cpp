#include "mode.h"

void FixedSizeWindow::Draw()
{
    int y0, y1;
    if (ppData->rtPP < scale * (area.height / 2))
    {
        y0 = area.y;
        y1 = scale * area.height;
    }
    else
    {
        y0 = ppData->rtPP - scale * (area.height / 2);
        y1 = ppData->rtPP + scale * (area.height / 2);
    }
    int line = (ppData->rtPP - y0) / scale;
    for (int i = 0; i < line; i++)
    {
        display->DrawLine(area.x, i, area.x + area.width, i, CURRENT_PLAY);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        int yval = (lines[i] - y0) / scale;
        if (yval >= area.y && yval < area.height)
        {
            display->DrawLine(area.x, yval, area.x + area.width, yval, LINE_COLOR);
            if (drawLineText)
            {
                if (i == 0 || (yval + display->FontHeight() + 2 < (lines[i - 1] - y0) / scale))
                {
                    display->DrawNumbers(lines[i], area.x + area.width - 4, yval + 2, LINE_COLOR, false);
                }
                else if (i == lines.size() - 1 || (yval - display->FontHeight() - 2 > (lines[i + 1] - y0) / scale))
                {
                    display->DrawNumbers(lines[i], area.x + area.width - 4, yval - 2 - display->FontHeight(), LINE_COLOR, false);
                }
            }
        }
    }
}

void ScalingWindow::Draw()
{
    float scale = ppData->rtPP / (area.height - margin);
    for (float pp : lines)
    {
        if (pp > ppData->rtPP)
        {
            scale = pp / (area.height - margin);
            break;
        }
    }
    if (integerScales) scale = (int)scale;
    for (int i = 0; i < area.height; i++)
    {
        if (scale * i < ppData->rtPP)
        {
            display->DrawLine(area.x, i, area.x + area.width, i, CURRENT_PLAY);
        }
    }
    for (int i = 0; i < lines.size(); i++)
    {
        int yval = lines[i] / scale;
        display->DrawLine(area.x, yval, area.x + area.width, yval, LINE_COLOR);
        if (drawLineText)
        {
            if (i == 0 || (yval + display->FontHeight() + 2 < lines[i - 1] / scale))
            {
                display->DrawNumbers(lines[i], area.x + area.width - 4, yval + 2, LINE_COLOR, false);
            }
            else if (i == lines.size() - 1 || (yval - display->FontHeight() - 2 > lines[i + 1] / scale))
            {
                display->DrawNumbers(lines[i], area.x + area.width - 4, yval - 2 - display->FontHeight(), LINE_COLOR, false);
            }
        }
    }
}

void FixedWindow::Draw()
{
    float scale = max / area.height;
    int fcLine = (int)(ppData->fcPP / scale);
    int rtLine = (int)(ppData->rtPP / scale);
    display->DrawLine(area.x, fcLine, area.x + area.width, fcLine, FC_LINE);
    for (int i = 0; i < rtLine; i++)
    {
        display->DrawLine(area.x, i, area.x + area.width, i, CURRENT_PLAY);
    }
}

void AccMode::Draw()
{
    int scale = hitData->hit300 + hitData->hit100 + hitData->hit50 + hitData->misses;
    if (scale == 0)
        return;
    float offset = scale * accOffset;
    int line300 = (int)((hitData->hit300 - offset) * area.height / (scale - offset));
    int line100 = (int)((hitData->hit300 + hitData->hit100 - offset) * area.height / (scale - offset));
    int line50 = (int)((hitData->hit300 + hitData->hit100 + hitData->hit50 - offset) * area.height / (scale - offset));
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
        float acc = 100.0 * (hitData->hit300 * 6 + hitData->hit100 * 2 + hitData->hit50) / (6 * scale);
        display->DrawNumbers(acc, area.x, area.y + 4, TEXT_COLOR, true);
    }
}

void PPMode::Draw() {
    display->
}