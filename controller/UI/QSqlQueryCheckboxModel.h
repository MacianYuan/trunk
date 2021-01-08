//
// Created by 何继胜 on 2017/5/19.
//

#ifndef OSLA_NEW_QSQLQUERYCHECKBOXMODEL_H
#define OSLA_NEW_QSQLQUERYCHECKBOXMODEL_H

#include <set>
#include <QtSql/QtSql>
#include <protobuf/message.pb.h>
#include <enum.h>

class QSqlQueryCheckboxModel : public QAbstractTableModel  {
Q_OBJECT
public:
    QSqlQueryCheckboxModel(QObject* parent, int colCheckBox, QString title);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool getStatus(const QModelIndex &index, int *status) const;

    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void updateData(biotech::osla::EntrustList recordList);
    std::set<int>& getCheckedEntrust();

private:
    int colNumberWithCheckBox;
    QString title;
    biotech::osla::EntrustList entrusts;
    std::set<int> checkedEntrust;
};


#endif //OSLA_NEW_QSQLQUERYCHECKBOXMODEL_H
