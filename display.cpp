#include "display.h"

Mode Display::modes[numModes] = MODES;

Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat) {
    font.LoadFont("rpi-rgb-led-matrix/fonts/tom-thumb.bdf");
}
void Display::Run() {
    while(running()) {
        canvas()->Fill(0,0,0);
        if(*((char*)m_pp_data) != 0) {
            for(int m = 0; m < numModes; m++) {
                int y1 = 32 * (numModes - m) / numModes - 1;
                int y2 = 32 * (numModes - m - 1) / numModes;
                switch(modes[m]) {
                    case LINES:
                        {
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
                            for(float pp : m_pp_lines) {
                                std::string ppstr = std::to_string((int)pp);
                                int hi = (int)(pp/scale);
                                DrawLine(canvas(), 64-hi, y1, 64-hi, y2, LINE_COLOR);
                                VerticalDrawText(canvas(), font, 63-hi, y1, LINE_COLOR, NULL, ppstr.c_str());
                            }
                        }
                        break;
                    case PP_IF_FC:
                        {
                            float scale = M_MAX_PP/64;
                            int fcLine = (int)(M_FC_PP/scale);
                            int rtLine = (int)(M_RT_PP/scale);
                            DrawLine(canvas(), 64-fcLine, y1, 64-fcLine, y2, FC_LINE);
                            for(int i = 0; i < rtLine; i++){
                                DrawLine(canvas(), 64-i, y1, 64-i, y2, CURRENT_PLAY);
                            }
                        }
                        break;
                    case ACC:
                        {
                            float scale = (M_3+M_1+M_5+M_MISS)/64.0;
                            int line300 = (int)(M_3/scale);
                            int line100 = (int)((M_3+M_1)/scale);
                            int line50 = (int)((M_3+M_1+M_5)/scale);
                            Color c = cMiss;
                            for(int i = 0; i < 64; i++) {
                                if(i < line300) {
                                    c = c300;
                                } else if(i < line100) {
                                    c = c100;
                                } else if(i < line50) {
                                    c = c50;
                                }
                                DrawLine(canvas(), 64-i, y1, 64-i, y2, c);
                            }
                        }
                        break;
                    case TOP_PLAYS:
                        {
                        }
                        break;
                }
            }
        }
        usleep(10000);
    }
}
void Display::addLine(float line){
    m_pp_lines.insert(line);
}
void Display::setData(float* pp, int* hit) {
    m_pp_data = pp;
    m_hit_data = hit;
}
