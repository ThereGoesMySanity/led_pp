#ifndef _a_DISPLAY_H
#define _a_DISPLAY_H
#include <set>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <string>
#include <math.h>
#include <unistd.h>
#include "rpi-rgb-led-matrix/include/led-matrix.h"
#include "rpi-rgb-led-matrix/include/graphics.h"
#include "rpi-rgb-led-matrix/include/canvas.h"
#include "rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"
using namespace rgb_matrix;

const struct Color FC_LINE(200, 200, 200);
const struct Color LINE_COLOR(200,200,200);
const struct Color TEXT_COLOR(255,255,255);
const struct Color CURRENT_PLAY(100,100,0);

const struct Color c300(0, 200, 200);
const struct Color c100(0, 200, 0);
const struct Color c50(200, 0, 200);
const struct Color cMiss(200, 0, 0);

typedef struct DataPacket {
    PPData pp;
    HitData hit;
};
enum PPDataArr { MAX, FC, RT };
static std::unordered_map<std::string,int> const pdaTable = {{"MAX", 0}, {"FC", 1}, {"RT", 2}};

typedef struct OsuData {
    PPData* pp;
    HitData* hit;
    std::vector<float> topPlays;
} OsuData;
typedef struct PPData {
    float maxPP;
    float fcPP;
    float rtPP;
} PPData;
typedef struct HitData {
    int hit300;
    int hit100;
    int hit50;
    int misses;
} HitData;

#define M_TOP_SCALE 0.25

class Display : public ThreadedCanvasManipulator {
    public:
        Display(RGBMatrix *mat);
        void setData(OsuData);
        void addLine(float);
        void addMode(std::string name, Rectangle area, std::string args);
        void setTopPlays(float*, int);
        virtual void Run();
        Canvas* Canvas() { return canvas(); };
        std::vector<Mode *> modes;
        void DrawNumbers(Font *font, float num, int x, int y, Color c, bool left, int d = 0);
        void DrawNumbers(float num, int x, int y, Color c, bool left, int d = 0);
        void DrawText(Font *font, std::string text, int x, int y, Color c, bool left);
        void DrawLine(int x0, int y0, int x1, int y1, Color c);
        int FontHeight() { return font.CharacterWidth('0'); }
    private:
        Font font;
        int id;
        OsuData data;
        float* m_top_plays;
        int m_plays_count;
        std::set<float> m_pp_lines;
};
#endif