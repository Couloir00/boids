#include "App/App.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    runTestsAndCheckGPU(argc, argv);

    // Actual app
    App app(1920, 1080);
    app.init();
    app.update();

    return 0;
}