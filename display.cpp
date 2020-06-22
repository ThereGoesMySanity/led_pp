#include "display.h"
#include "mode.h"
#include <sstream>
#include <string>

Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat)
{
    font.LoadFont("rpi-rgb-led-matrix/fonts/6x4.bdf");
}
Display::~Display()
{
    for(Mode* m : modes) delete m;
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
                /*
                if(modes[m] == "LINES") {
                    int scale = floor(m_pp_data->rtPP/64) + 1;
                    for(float pp : m_pp_lines) {
                        if(pp > m_pp_data->rtPP) {
                            scale = floor(pp/64) + 1;
                            break;
                        }
                    }
                    for(int x = 0; x < 64; x++) {
                        if(scale*x < m_pp_data->rtPP) {
                            DrawLine(canvas(), 64-x, y1, 64-x, y2, CURRENT_PLAY);
                        }
                    }
                    auto last = std::prev(m_pp_lines.end());
                    for(std::set<float>::iterator pp = m_pp_lines.begin(); pp != m_pp_lines.end(); ++pp) {
                        int hi = (int)(*pp/scale);
                        DrawLine(canvas(), 64-hi, y1, 64-hi, y2, LINE_COLOR);
                        //only draw text if it doesn't overlap with the next line
                        bool text = true;
                        if(pp != last) {
                            std::set<float>::iterator next = ++pp;
                            pp--;
                            if( hi + 7 > (int)(*(next)/scale)) {
                                text = false;
                            }
                        }
                        if(hi < 64 && text) {
                            if(hi + 6 > 64) {
                                DrawNumbers(*pp, 66-hi, y2, LINE_COLOR, false);
                            } else {
                                DrawNumbers(*pp, 58-hi, y2, LINE_COLOR, false);
                            }
                        }
                    }
                } else if(modes[m] == "PP_IF_FC") {
                    float scale = m_pp_data->maxPP/64;
                    int fcLine = (int)(m_pp_data->fcPP/scale);
                    int rtLine = (int)(m_pp_data->rtPP/scale);
                    DrawLine(canvas(), 64-fcLine, y1, 64-fcLine, y2, FC_LINE);
                    for(int i = 0; i < rtLine; i++){
                        DrawLine(canvas(), 64-i, y1, 64-i, y2, CURRENT_PLAY);
                    }
                } else if(modes[m] == "ACC") {
                    int scale = m_hit_data->hit300+m_hit_data->hit100+m_hit_data->hit50+m_hit_data->misses;
                    int line300 = (int)(m_hit_data->hit300*64.0/scale);
                    int line100 = (int)((m_hit_data->hit300+m_hit_data->hit100)*64.0/scale);
                    int line50 = (int)((m_hit_data->hit300+m_hit_data->hit100+m_hit_data->hit50)*64.0/scale);
                    Color c = c300;
                    for(int i = 0; i < 64; i++) {
                        if(i < line300) {
                            c = c300;
                        } else if(i < line100) {
                            c = c100;
                        } else if(i < line50) {
                            c = c50;
                        } else if(scale > 0) {
                            c = cMiss;
                        }
                        DrawLine(canvas(), 63-i, y1, 63-i, y2, c);
                    }
                    if(scale > 0) {
                        float acc = 100.0*(m_hit_data->hit300*6+m_hit_data->hit100*2+m_hit_data->hit50)/(6*scale);
                        int dec = m_hit_data->hit300==scale? 0 : 1;
                        DrawNumbers(acc, 58, y1, TEXT_COLOR, true, dec);
                    }
                } else if(modes[m] == "TOP_PLAYS") {
                    float x1, x2;
                    if(m_pp_data->rtPP < M_TOP_SCALE*32) {
                        x1 = 0;
                        x2 = M_TOP_SCALE * 64;
                    } else {
                        x1 = m_pp_data->rtPP-M_TOP_SCALE*32;
                        x2 = m_pp_data->rtPP+M_TOP_SCALE*32;
                    }
                    int line = (m_pp_data->rtPP-x1)/M_TOP_SCALE;
                    for(int i = 0; i < line; i++) {
                        DrawLine(canvas(), 63-i, y1, 63-i, y2, CURRENT_PLAY);
                    }
                    for(int i = 0; i < m_plays_count; i++) {
                        int xval = (m_top_plays[i]-x1)/M_TOP_SCALE;
                        if(xval > 0 && xval <= 64) {
                            DrawLine(canvas(), 64-xval, y1, 64-xval, y2, LINE_COLOR);
                            if(xval + 7 <= 64) {
                                if(i > 0) { 
                                    if(xval + 7 < (m_top_plays[i-1]-x1)/M_TOP_SCALE) {
                                        DrawNumbers(m_top_plays[i], 58-xval, y2, LINE_COLOR, false);
                                    } else if (i < m_plays_count - 1) {
                                        if(xval - 13 > (m_top_plays[i+1]-x1)/M_TOP_SCALE) {
                                            DrawNumbers(m_top_plays[i], 66-xval, y2, LINE_COLOR, false);
                                        }
                                    } else {
                                        DrawNumbers(m_top_plays[i], 66-xval, y2, LINE_COLOR, false);
                                    }
                                } else {
                                    DrawNumbers(m_top_plays[i], 58-xval, y2, LINE_COLOR, false);
                                }
                            }
                        }
                    }
                }
                */
            }
        }
        usleep(10000);
    }
}
void Display::DrawLine(int x0, int y0, int x1, int y1, Color c)
{
    rgb_matrix::DrawLine(canvas(), 63 - y0, 31 - x0, 63 - y1, 31 - x1, c);
}
void Display::DrawNumbers(float num, int x, int y, Color c, bool left, int dec = 0) { DrawNumbers(&font, num, x, y, c, left, dec); }

void Display::DrawNumbers(Font *font, float num, int x, int y, Color c, bool left, int dec = 0)
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
    Mode* mode;
    if (name.compare("ACC_BAR") == 0) {
        float accOffset = 0.9f;
        if(std::getline(ss, arg, ','))
        {
            accOffset = std::stof(arg);
        }
        mode = new AccMode(this, r, &data, accOffset);
    } else if (name.compare("PP_TEXT") == 0) {
        int type = 0;
        if(std::getline(ss, arg, ','))
        {
            type = pdaTable.at(arg);
        }
        std::string font = "rpi-rgb-led-matrix/fonts/13x6.bdf";
        if(std::getline(ss, arg, ','))
        {
            font = arg;
        }
        mode = new PPMode(this, r, &data, type, font.c_str());
    } else if(name.compare("FIXED_WIN_BAR") == 0) {
        int maxType = 0;
        if(std::getline(ss, arg, ','))
        {
            maxType = fwmtTable.at(arg);
        }
    } else if (name.compare("FIXED_SIZE_BAR") == 0) {

    } else if (name.compare("SCALING_BAR") == 0) {

    }
    if(mode != nullptr) {
        modes.push_back(mode);
        delete mode;
    }
}
void Display::setTopPlays(float *f, int count)
{
    m_top_plays = f;
    m_plays_count = count;
}
