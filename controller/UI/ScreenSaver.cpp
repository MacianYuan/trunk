#include <deviceinfo/tq2440.h>
#include "controller/UI/ScreenSaver.h"
#include "controller/Base/NormalTools.h"

bool ScreenSaver::enable = true;

ScreenSaver::ScreenSaver() : closePopUpsEnabled(true), screenDimEnabled(true), screenOffEnabled(true) {
    restore();
}

bool ScreenSaver::save(int level) {
    if (!enable)
    {
        return false;
    }
    switch (level) {
        case CLOSEPOPUPS:
            if (closePopUpsEnabled) {
                closeActiveModalWidgets();
            }
            return true;
        case SCREENDIM:
            if (screenDimEnabled) {
                backlight(50);
            }
            return true;
        case SCREENOFF:
            if (screenOffEnabled) {
                backlight(0);
            }
            return true;
        default:
            return false;
    }
}

void ScreenSaver::restore() {
    backlight(100);
}

void ScreenSaver::setClosePopUpsEnabled(bool enabled) {
    closePopUpsEnabled = enabled;
}

void ScreenSaver::setScreenDimEnabled(bool enabled) {
    screenDimEnabled = enabled;
}

void ScreenSaver::setScreenOffEnabled(bool enabled) {
    screenOffEnabled = enabled;
}

void ScreenSaver::setEnable(bool flag)
{
    enable = flag;
    if (!enable)
    {   //如果禁用屏保，则一直打开背光灯
        backlight(100);
    }
}

void ScreenSaver::closeActiveModalWidgets() {
    NTG()->ClosePopupWindow();
}
