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

#define M_MAX_PP m_pp_data[0]
#define M_FC_PP m_pp_data[1]
#define M_RT_PP m_pp_data[2]

#define M_3 m_hit_data[0]
#define M_1 m_hit_data[1]
#define M_5 m_hit_data[2]
#define M_MISS m_hit_data[3]

#define M_TOP_SCALE 0.25

class Display : public ThreadedCanvasManipulator {
    public:
        Display(RGBMatrix *mat);
        void setData(float*, int*);
        void addLine(float);
        void addMode(std::string);
        void setTopPlays(float*, int);
        virtual void Run();
        std::vector<std::string> modes;
    private:
        Font font;
        int id;
        float *m_pp_data;
        int *m_hit_data;
        float* m_top_plays;
        int m_plays_count;
        std::set<float> m_pp_lines;
        void drawNumbers(float num, int x, int y, Color c, bool left, int d = 0);
};
