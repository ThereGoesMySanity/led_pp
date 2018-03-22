#include "display.h"
Display::Display(RGBMatrix *mat) : ThreadedCanvasManipulator(mat) {

}
void Display::Run() {
    int scale = ceil(maxPP/64);
    while(running()) {
        canvas()->Fill(0,0,0);
        for(int x = 0; x < 64; x++) {
            if(scale*x < pp) {
                DrawLine(canvas(), 64-x, 0, 64-x, 31, CURRENT_PLAY);
            }
        }
        int hi = (int)(maxPP/scale);
        DrawLine(canvas(), 64-hi, 0, 64-hi, 31, TOP_PLAY);
        usleep(10000);
    }
}
void Display::setMaxPP(float max){
    this->maxPP = max;
}
void Display::setPP(float p) {
    this->pp = p;
}
