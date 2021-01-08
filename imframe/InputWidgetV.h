#ifndef INPUTWIDGETV_H
#define INPUTWIDGETV_H

#include <QtGui/QWidget>

class QWSInputMethod;

namespace Ui {
    class InputWidgetV;
}

class InputWidgetV : public QWidget {
    Q_OBJECT
public:
    explicit InputWidgetV(QWSInputMethod *im);
    ~InputWidgetV();
private:
    Ui::InputWidgetV *ui;
};


#endif // INPUTWIDGETV_H
