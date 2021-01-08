#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QtGui/QWSServer>

class ScreenSaver : public QWSScreenSaver
{
public:
    ScreenSaver();
    bool save(int level);
    void restore();
    void setClosePopUpsEnabled(bool enable);
    void setScreenDimEnabled(bool enable);
    void setScreenOffEnabled(bool enable);

    enum ScreenSaverLevel {
        CLOSEPOPUPS,
        SCREENDIM,
        SCREENOFF
    };

    static void setEnable(bool flag);
private:
    void closeActiveModalWidgets();
    //void setBacklight(ushort brightness);

private:
    bool closePopUpsEnabled;
    bool screenDimEnabled;
    bool screenOffEnabled;

    static bool enable;
};

#endif // SCREENSAVER_H
