#ifndef INPUTWIDGETNUM_H
#define INPUTWIDGETNUM_H

#include <QtGui/QWidget>

class QWSInputMethod;

namespace Ui {
    class InputWidgetNum;
}

class InputWidgetNum : public QWidget {
    Q_OBJECT
public:
    explicit InputWidgetNum(QWSInputMethod *im);
    ~InputWidgetNum();

private:
    Ui::InputWidgetNum* ui;
};

#endif // INPUTWIDGETNUM_H
