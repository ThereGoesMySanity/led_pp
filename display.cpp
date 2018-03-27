#include "display.h"

Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat) {
    font.LoadFont("rpi-rgb-led-matrix/fonts/6x4.bdf");
}
void Display::Run() {
    while(running()) {
        canvas()->Fill(0,0,0);
        if(*((char*)m_pp_data) != 0) {
            int numModes = modes.size();
            for(int m = 0; m < numModes; m++) {
                int y1 = 32 * (numModes - m) / numModes - 1;
                int y2 = 32 * (numModes - m - 1) / numModes;
                if(modes[m] == "LINES") {
                    int scale = ceil(M_RT_PP/64);
                    for(float pp : m_pp_lines) {
                        if(pp > M_RT_PP) {
                            scale = ceil(pp/64);
                            break;
                        }
                    }
                    for(int x = 0; x < 64; x++) {
                        if(scale*x < M_RT_PP) {
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
                                drawNumbers(*pp, 66-hi, y2, LINE_COLOR, false);
                            } else {
                                drawNumbers(*pp, 58-hi, y2, LINE_COLOR, false);
                            }
                        }
                    }
                } else if(modes[m] == "PP_IF_FC") {
                    float scale = M_MAX_PP/64;
                    int fcLine = (int)(M_FC_PP/scale);
                    int rtLine = (int)(M_RT_PP/scale);
                    DrawLine(canvas(), 64-fcLine, y1, 64-fcLine, y2, FC_LINE);
                    for(int i = 0; i < rtLine; i++){
                        DrawLine(canvas(), 64-i, y1, 64-i, y2, CURRENT_PLAY);
                    }
                } else if(modes[m] == "ACC") {
                    int scale = M_3+M_1+M_5+M_MISS;
                    int line300 = (int)(M_3*64.0/scale);
                    int line100 = (int)((M_3+M_1)*64.0/scale);
                    int line50 = (int)((M_3+M_1+M_5)*64.0/scale);
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
                        float acc = 100.0*(M_3*6+M_1*2+M_5)/(6*scale);
                        int dec = M_3==scale? 0 : 1;
                        drawNumbers(acc, 58, y1, TEXT_COLOR, true, dec);
                    }
                } else if(modes[m] == "TOP_PLAYS") {
                    float x1, x2;
                    if(M_RT_PP < M_TOP_SCALE*32) {
                        x1 = 0;
                        x2 = M_TOP_SCALE * 64;
                    } else {
                        x1 = M_RT_PP-M_TOP_SCALE*32;
                        x2 = M_RT_PP+M_TOP_SCALE*32;
                    }
                    int line = (M_RT_PP-x1)/M_TOP_SCALE;
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
                                        drawNumbers(m_top_plays[i], 58-xval, y2, LINE_COLOR, false);
                                    } else if (i < m_plays_count - 1) {
                                        if(xval - 13 > (m_top_plays[i+1]-x1)/M_TOP_SCALE) {
                                            drawNumbers(m_top_plays[i], 66-xval, y2, LINE_COLOR, false);
                                        }
                                    } else {
                                        drawNumbers(m_top_plays[i], 66-xval, y2, LINE_COLOR, false);
                                    }
                                } else {
                                    drawNumbers(m_top_plays[i], 58-xval, y2, LINE_COLOR, false);
                                }
                            }
                        }
                    }
                }
            }
        }
        usleep(10000);
    }
}
void Display::drawNumbers(float num, int x, int y, Color c, bool left, int dec) {
    std::string text = std::to_string((int)num);
    std::reverse(text.begin(), text.end());
    int ypos;
    if(left) {
        ypos = y+4-text.size()*4;
    } else {
        ypos = y+4;
    }
    VerticalDrawText(canvas(), font, x, ypos, c, NULL, text.c_str());
    if(dec > 0) {
        int num2 = (num-(int)num)*pow(10, dec);
        char str[dec+1];
        sprintf(str, "%0*d", dec, num2);
        std::string text2 = std::string(str);
        std::reverse(text.begin(), text.end());
        VerticalDrawText(canvas(), font, x, ypos-text2.size()*4-2, c, NULL, text2.c_str());
        canvas()->SetPixel(x+4, ypos-5, c.r, c.g, c.b);
    }
}
void Display::addLine(float line){
    m_pp_lines.insert(line);
}
void Display::setData(float* pp, int* hit) {
    m_pp_data = pp;
    m_hit_data = hit;
}
void Display::addMode(std::string m) {
    modes.push_back(m);
}
void Display::setTopPlays(float* f, int count) {
    m_top_plays = f;
    m_plays_count = count;
}
