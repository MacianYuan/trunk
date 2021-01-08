
#ifndef DOWNLOADPAGE_H
#define DOWNLOADPAGE_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QTextBrowser>
#include <QtCore/QObject>

class DownloadPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DownloadPage(QWidget* parent = 0);
    void initializePage();

public:
    QLabel* label;
    QProgressBar* progressBar;
    QTextBrowser* information;
    bool complete;
signals:
    void beforeBack(void);
protected:
    bool isComplete() const;
    void cleanupPage(void);
};

#endif //DOWNLOADPAGE_H
