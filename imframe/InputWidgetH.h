#ifndef INPUTWIDGETH_H
#define INPUTWIDGETH_H

#include <QtGui/QWidget>

class QWSInputMethod;

namespace Ui {
class InputWidgetH;
}

class InputWidgetH : public QWidget
{
    Q_OBJECT
public:
    explicit InputWidgetH(QWSInputMethod *im);
    ~InputWidgetH();

private:
    Ui::InputWidgetH *ui;
};


#endif // INPUTWIDGETH_H
