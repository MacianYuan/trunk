#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtCore/QDateTime>
#include <QtGui/QHBoxLayout>
#include <QtGui/QScrollBar>
#include <QtGui/QAbstractScrollArea>
#include <QtGui/QAbstractItemView>
#include "controller/UI/ScheduleDialog.h"
#include "3rdparty/qxt/qxtscheduleview.h"

ScheduleDialog::ScheduleDialog(QWidget *parent) : QDialog(parent)
{
    setStyleSheet("font-size: 18px;");
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Booking in the latest week"));//tr("最近一周预约")

    infoLabel = new QLabel(this);
    upButton = new QPushButton(tr("∧"), this);
    downButton = new QPushButton(tr("∨"), this);
    leftButton = new QPushButton(tr("＜"), this);
    rightButton = new QPushButton(tr("＞"), this);
    closeButton = new QPushButton(tr("Close"), this);//关闭
    upButton->setFocusPolicy(Qt::NoFocus);
    downButton->setFocusPolicy(Qt::NoFocus);
    leftButton->setEnabled(false);
    leftButton->setFocusPolicy(Qt::NoFocus);
    rightButton->setFocusPolicy(Qt::NoFocus);
    closeButton->setFocusPolicy(Qt::NoFocus);

    scheduleView = new QxtScheduleView(this);
    QDateTime now = QDateTime::currentDateTime();
    scheduleView->setDateRange(now.date(),now.date().addDays(6));
    //scheduleView->setCurrentZoomDepth(60, Qxt::Minute);
    //scheduleView->setZoomStepWidth(30, Qxt::Minute);
    scheduleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scheduleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scheduleView->setFrameStyle(QFrame::StyledPanel);

    QHBoxLayout *footer = new QHBoxLayout;
    footer->addWidget(infoLabel);
    footer->addStretch();
    footer->addWidget(upButton);
    footer->addWidget(downButton);
    footer->addStretch();
    footer->addWidget(leftButton);
    footer->addWidget(rightButton);
    footer->addStretch();
    footer->addWidget(closeButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(scheduleView);
    layout->addLayout(footer);

    setLayout(layout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(upButton, SIGNAL(clicked()), this, SLOT(scrollUp()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(scrollDown()));
    connect(leftButton, SIGNAL(clicked()), this, SLOT(scrollLeft()));
    connect(rightButton, SIGNAL(clicked()), this, SLOT(scrollRight()));
    connect(scheduleView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVScrollBarValueChanged(int)));
    connect(scheduleView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHScrollBarValueChanged(int)));
}

void ScheduleDialog::setDataModel(QAbstractItemModel *model)
{
    scheduleView->setModel(model);
    infoLabel->setText(tr("There was %1 appointment in the last week").arg(model->rowCount()));//最近一周共有%1个预约
}

void ScheduleDialog::setCellInterval(const int minutes)
{
    scheduleView->setCurrentZoomDepth(minutes, Qxt::Minute);
}

void ScheduleDialog::showEvent(QShowEvent *)
{
    // 设置日程表从8:00pm开始显示
    int steps = 8 * 60 / scheduleView->currentZoomDepth(Qxt::Minute);
    scheduleView->verticalScrollBar()->setValue(scheduleView->verticalScrollBar()->singleStep() * steps);
    //scheduleView->verticalScrollBar()->setValue(scheduleView->verticalScrollBar()->maximum() / 2);
}

void ScheduleDialog::scrollUp()
{
    scheduleView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
}

void ScheduleDialog::scrollDown()
{
    scheduleView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
}

void ScheduleDialog::scrollLeft()
{
    scheduleView->horizontalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
}

void ScheduleDialog::scrollRight()
{
    scheduleView->horizontalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
}

void ScheduleDialog::onVScrollBarValueChanged(int value)
{
    upButton->setEnabled(value != scheduleView->verticalScrollBar()->minimum());
    downButton->setEnabled(value != scheduleView->verticalScrollBar()->maximum());
}

void ScheduleDialog::onHScrollBarValueChanged(int value)
{
    leftButton->setEnabled(value != scheduleView->horizontalScrollBar()->minimum());
    rightButton->setEnabled(value != scheduleView->horizontalScrollBar()->maximum());
}
