#include "display.h"
Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat) {

}
void Display::Run() {
    while(running()) {
        canvas()->Fill(0,0,0);
        switch(mode) {
            case TOP_PP:
                {
                    int scale = ceil(m_topPP/64);
                    for(int x = 0; x < 64; x++) {
                        if(scale*x < M_RT_PP) {
                            DrawLine(canvas(), 64-x, 0, 64-x, 31, CURRENT_PLAY);
                        }
                    }
                    int hi = (int)(m_topPP/scale);
                    DrawLine(canvas(), 64-hi, 0, 64-hi, 31, TOP_PLAY);
                }
                break;
            case PP_IF_FC:
                {
                    float scale = M_MAX_PP/64;
                    int fcLine = (int)(M_FC_PP/scale);
                    int rtLine = (int)(M_RT_PP/scale);
                    DrawLine(canvas(), 64-fcLine, 0, 64-fcLine, 31, FC_LINE);
                    for(int i = 0; i < rtLine; i++){
                        DrawLine(canvas(), 64-i, 0, 64-i, 31, CURRENT_PLAY);
                    }
                }
                break;
            case ACC:
                {}
                break;
            case TOP_PLAYS:
                {}
                break;
        }
        usleep(10000);
    }
}
void Display::setTopPP(float max){
    this->m_topPP = max;
}
void Display::setData(float *d) {
    this->m_data = d;
}
