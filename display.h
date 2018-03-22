#include <math.h>
#include <unistd.h>
#include "rpi-rgb-led-matrix/include/led-matrix.h"
#include "rpi-rgb-led-matrix/include/graphics.h"
#include "rpi-rgb-led-matrix/include/canvas.h"
#include "rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"

using namespace rgb_matrix;

const struct Color FC_LINE(200, 200, 200);
const struct Color TOP_PLAY(200,200,200);
const struct Color CURRENT_PLAY(100,100,0);

const struct Color c300(0, 200, 200);
const struct Color c100(0, 200, 0);
const struct Color c50(200, 0, 200);

#define M_MAX_PP m_data[0]
#define M_FC_PP m_data[1]
#define M_RT_PP m_data[2]

enum Mode {TOP_PP, PP_IF_FC, ACC, TOP_PLAYS};

class Display : public ThreadedCanvasManipulator {
    public:
        Display(RGBMatrix *mat);
        void setData(float* data);
        void setTopPP(float maxPP);
        virtual void Run();
    private:
        Mode mode = PP_IF_FC;
        float *m_data;
        float m_topPP = 0;
};
