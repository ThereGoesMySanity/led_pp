#include "display.h"

typedef struct Rectangle {
    int x, y, width, height;
} Rectangle;

class Mode {
    public:
        Mode(Display* display, Rectangle area, PPData* ppData, HitData* hitData)
            : display(display), area(area), ppData(ppData), hitData(hitData) { }
        virtual void Draw();
    protected:
        Display* display;
        Rectangle area;
        PPData* ppData;
        HitData* hitData;
};

class AccMode : public Mode {
    public: void Draw();
    private: float accOffset = 0.9;
};

class PPMode : public Mode {
    public:
        PPMode(Display* display, Rectangle area, PPData* ppData, HitData* hitData, const char* font = "rpi-rgb-led-matrix/fonts/13x6.bdf")
            : Mode(display, area, ppData, hitData) { this->font.LoadFont(font); }
        void Draw();
    private:
        Font font;
}

class LineMode : public Mode {
    public:
        LineMode(Display* display, Rectangle area, PPData* ppData, HitData* hitData
                , std::vector<float> lines, bool drawBarText = false, bool drawLineText = true)
                : Mode(display, area, ppData, hitData), lines(lines), drawBarText(drawBarText), drawLineText(drawLineText) {}
    protected:
        std::vector<float> lines;
        bool drawBarText, drawLineText;
};

class FixedWindow : public Mode {
    public:
        FixedWindow(Display* display, Rectangle area, PPData* ppData, HitData* hitData
            , float max)
            : Mode(display, area, ppData, hitData), max(max) { }
        void Draw();
    private:
        float max;
};

class FixedSizeWindow : public LineMode {
    public:
        void Draw();
    private:
        float scale = 0.25f;
};

class ScalingWindow : public LineMode {
    public:
        void Draw();
    private:
        float margin = 4;
        bool integerScales = false;
};