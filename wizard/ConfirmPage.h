
#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>

class ConfirmPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ConfirmPage(QWidget* parent = 0);
    void initializePage();

private:
    QLabel* okLabel;
};

#endif //CONFIRMPAGE_H
