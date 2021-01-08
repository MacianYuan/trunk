#ifndef IMFRAME_H
#define IMFRAME_H

#include <QtGui/QWSInputMethod>

class InputWidgetNum;
class InputWidgetH;
class InputWidgetV;

class IMFrame : public QWSInputMethod
{
    Q_OBJECT
public:
    explicit IMFrame();
	void updateHandler(int type);

private slots:
    void sendContent(const QString &);

private:
    InputWidgetNum *inputWidgetNum;
	InputWidgetV *inputWidgetV;
	InputWidgetH *inputWidgetH;
};

///---------------------------------------------

enum InputTypeUL {
	IS_UPPER = -1,
	IS_LOWER = 1
};

enum InputTypeDefine {
	NUM_INPUT_FOR_REGISTER,
	NUM_INPUT_FOR_USE_HOST,
    ALL_INPUT_H,
    ALL_INPUT_V,
    ALL_INPUT_V_setup
};

class InputType {
public:
	static int getFlag();
	static void setFlag(int flag);
	static int getUppper();
	static void setUppper(int uppper);

private:
	static int flag;
	static int uppper;
};

#endif // IMFRAME_H
