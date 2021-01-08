#include <QtGui/QInputMethodEvent>
#include <QtGui/QWSServer>
#include <QtGui/QLineEdit>

#include "imframe/imframe.h"
#include "imframe/InputWidgetNum.h"
#include "imframe/InputWidgetV.h"
#include "imframe/InputWidgetH.h"

int InputType::flag = InputTypeDefine::NUM_INPUT_FOR_REGISTER;
int InputType::uppper = InputTypeUL::IS_LOWER;

IMFrame::IMFrame() {
    inputWidgetNum = new InputWidgetNum(this);
    inputWidgetH = new InputWidgetH(this);
    inputWidgetV = new InputWidgetV(this);
}

void IMFrame::sendContent(const QString &newcontent) {
    // send key event

    if (newcontent == "bs") {
        QWSServer::sendKeyEvent(0x01000003, Qt::Key_Backspace, Qt::NoModifier, true, false);
        //inputWidgetNum->setGeometry(461, 131, 151, 201);
    } else if (newcontent == "bs1") {
        QWSServer::sendKeyEvent(0x01000003, Qt::Key_Backspace, Qt::NoModifier, true, false);
        //inputWidgetNum->setGeometry(461, 131, 151, 201);
    } else if (newcontent == "bthda") {
        InputType::setUppper(-1 * InputType::getUppper());
        if (InputType::getFlag() == InputTypeDefine::ALL_INPUT_V) {
            inputWidgetV->close();
            inputWidgetV = new InputWidgetV(this);
            inputWidgetV->setGeometry(300, 20, 315, 452);
            inputWidgetV->show();
        } else if (InputType::getFlag() == InputTypeDefine::ALL_INPUT_H) {
            inputWidgetH->close();
            inputWidgetH = new InputWidgetH(this);
            inputWidgetH->setGeometry(40, 200, 551, 200);
            inputWidgetH->show();
        }  else if (InputType::getFlag() == InputTypeDefine::ALL_INPUT_V_setup) {
            inputWidgetV->close();
            inputWidgetV = new InputWidgetV(this);
            inputWidgetV->setGeometry(250, 0, 315, 452);
            inputWidgetV->show();
        }
    } else {
        sendCommitString(newcontent);
    }
}

void IMFrame::updateHandler(int type) {
    QWidget *currentFocusWidget = new QWidget;
    currentFocusWidget = QApplication::focusWidget();

    switch (type) {
        case QWSInputMethod::FocusOut:
            inputWidgetNum->hide();
            inputWidgetH->hide();
            inputWidgetV->hide();
            break;

        case QWSInputMethod::FocusIn:
            if (qobject_cast<QLineEdit *>(currentFocusWidget)) {
                if (InputType::getFlag() == InputTypeDefine::NUM_INPUT_FOR_REGISTER) {
                    //inputWidgetNum->setGeometry(350, 20, 252, 452);
                    inputWidgetNum->setGeometry(361, 131, 151, 201);
                    inputWidgetNum->show();
                } else if (InputType::getFlag() == InputTypeDefine::NUM_INPUT_FOR_USE_HOST) {
                    inputWidgetNum->setGeometry(461, 131, 151, 201);
                    inputWidgetNum->show();
                } else if (InputType::getFlag() == InputTypeDefine::ALL_INPUT_V) {
                    inputWidgetV->setGeometry(325, 20, 315, 452);
                    inputWidgetV->show();
                } else if (InputType::getFlag() == InputTypeDefine::ALL_INPUT_H) {
                    inputWidgetH->setGeometry(40, 200, 551, 200);
                    inputWidgetH->show();
                } else if(InputType::getFlag() == InputTypeDefine::ALL_INPUT_V_setup){
                    inputWidgetV->setGeometry(250, 0, 315, 452);
                    inputWidgetV->show();
                }

            }
            break;

        default:
            break;
    }

    QWSInputMethod::updateHandler(type);
}

int InputType::getFlag() {
    return flag;
}

void InputType::setFlag(int flag) {
    InputType::flag = flag;
}

int InputType::getUppper() {
    return uppper;
}

void InputType::setUppper(int uppper) {
    InputType::uppper = uppper;
}
