#include <math.h>
#include <unistd.h>
#include "rpi-rgb-led-matrix/include/led-matrix.h"
#include "rpi-rgb-led-matrix/include/graphics.h"
#include "rpi-rgb-led-matrix/include/canvas.h"
#include "rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"

using namespace rgb_matrix;

const struct Color TOP_PLAY(200,200,200);
const struct Color CURRENT_PLAY(100,100,0);

class Display : public ThreadedCanvasManipulator {
    public:
        Display(RGBMatrix *mat);
        void setPP(float pp);
        void setMaxPP(float maxPP);
        virtual void Run();
    private:
        float pp = 0;
        float maxPP = 0;
};
