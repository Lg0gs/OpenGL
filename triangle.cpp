#include "classes/Triangle.h"
using namespace std;

int main() {
    Triangle t;
    t.setupSDLWindow();
    t.setupGLContext();
    t.setupGlad();
    t.setupVertextSpecification();
    t.startLoop();
    t.quitApp();

    return 0;
}