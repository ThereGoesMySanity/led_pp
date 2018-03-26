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

enum Mode {LINES, PP_IF_FC, ACC, TOP_PLAYS};

class Display : public ThreadedCanvasManipulator {
    public:
        Display(RGBMatrix *mat, int num, Mode* m);
        void setData(float*, int*);
        void addLine(float);
        void addMode(Mode);
        virtual void Run();
        std::vector<Mode> modes;
    private:
        Font font;
        int id;
        float *m_pp_data;
        int *m_hit_data;
        std::set<float> m_pp_lines;
};
