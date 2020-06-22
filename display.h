#include <set>
#include <vector>
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
        void setData(PPData*, HitData*);
        void addLine(float);
        void addMode(std::string);
        void setTopPlays(float*, int);
        virtual void Run();
        Canvas* Canvas() { return canvas(); };
        std::vector<std::string> modes;
        void DrawNumbers(float num, int x, int y, Color c, bool left, int d = 0);
        void DrawLine(int x0, int y0, int x1, int y1, Color c);
        int FontHeight() { return font.CharacterWidth('0'); }
    private:
        Font font;
        int id;
        PPData *m_pp_data;
        HitData *m_hit_data;
        float* m_top_plays;
        int m_plays_count;
        std::set<float> m_pp_lines;
};
