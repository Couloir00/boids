#ifndef __ALL_GUI_HPP_
#define __ALL_GUI_HPP_

#include <imgui.h>

class graphicsUtils {
public:
    static void utilitiesWindow(bool& lodsEnabled, bool& fogEnabled, float& fogIntensity, float& fogRed, float& fogGreen, float& fogBlue);
};

#endif