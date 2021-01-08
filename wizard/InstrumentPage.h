
#ifndef INSTRUMENTPAGE_H
#define INSTRUMENTPAGE_H

#include <QtGui/QWizardPage>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>

class InstrumentPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit InstrumentPage(QWidget* parent = 0);
    void initializePage();
    void addInstrument(const QString& label, int data);
    int selectedInstrumentId() const;
    QString selectedInstrumentName() const;
protected:
    bool isComplete() const;
    void cleanupPage(void);
private slots:
    void currentInstrumentChange();

private:
    QLabel* label;
    QListWidget* listWidget;
    int currentInstrumentId;
    QString currentInstrumentName;
};

#endif //INSTRUMENTPAGE_H
