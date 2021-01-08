#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H


#include <QtGui/QDialog>

class QAbstractItemModel;
class QLabel;
class QPushButton;
class QxtScheduleView;

class ScheduleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScheduleDialog(QWidget *parent = 0);
    void setDataModel(QAbstractItemModel *model);
    void setCellInterval(const int minutes);

protected:
    void showEvent(QShowEvent *);

private slots:
    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();
    void onVScrollBarValueChanged(int value);
    void onHScrollBarValueChanged(int value);

private:
    QLabel *infoLabel;
    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *closeButton;
    QxtScheduleView *scheduleView;
};

#endif // SCHEDULEDIALOG_H
