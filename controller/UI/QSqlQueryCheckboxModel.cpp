//
// Created by 何继胜 on 2017/5/19.
//

#include "controller/UI/QSqlQueryCheckboxModel.h"
#include "deviceinfo/enum.h"
#include <tools/IniParse.h>
#include "controller/Base/GlobalData.h"
#define CHECK_BOX_COLUMN 3

QSqlQueryCheckboxModel::QSqlQueryCheckboxModel(QObject *parent, int colCheckBox, QString title) : QAbstractTableModel(parent) {
    this->colNumberWithCheckBox = colCheckBox;
    this->title = title;
}

Qt::ItemFlags QSqlQueryCheckboxModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QAbstractItemModel::flags(index);
    }

    if (index.column() == CHECK_BOX_COLUMN) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QSqlQueryCheckboxModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    int nRow = index.row();
    int nColumn = index.column();
    const biotech::osla::Entrust &record = entrusts.entrust(nRow);

    IniParse osla(OSLA_CONFIG);
    GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";
    }

    switch (role) {
        //case Qt::TextColorRole:
        //return QColor(Qt::GlobalColor::white);
//        case Qt::TextAlignmentRole:
//            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case Qt::DisplayRole: {
            if (0 == nColumn) {
                return record.id();
            } else if (1 == nColumn) {
                return QString::fromUtf8(record.title().c_str(), record.title().length());
            } else if (2 == nColumn) {
                if (EntrustStatus::DONT_DO== record.status()) {
                    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
                          return "未执行";
                    }
                    else{
                          return "Non-execution";
                    }

//                } else if (EntrustStatus::NOW_DO== record.status()) {
//                    return "执行中";
                } else if (EntrustStatus::DONT_OVER == record.status()) {
                    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
                          return "未完成";
                    }
                    else{
                          return "Unfinished";
                    }

                }
            } else {
                return "";
            }
        }
        case Qt::CheckStateRole: {
            if (nColumn == CHECK_BOX_COLUMN) {
                return record.is_doing() > 0 ? Qt::Checked : Qt::Unchecked;
            }
        }
        default:
            return QVariant();
    }

    return QVariant();
}

int QSqlQueryCheckboxModel::rowCount(const QModelIndex &parent) const {
    return entrusts.entrust_size();
}

int QSqlQueryCheckboxModel::columnCount(const QModelIndex &parent) const {
    return 4;
}

bool QSqlQueryCheckboxModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    int nColumn = index.column();
    biotech::osla::Entrust *record = entrusts.mutable_entrust(index.row());
    switch (role) {
        case Qt::CheckStateRole: {
            if (nColumn == CHECK_BOX_COLUMN) {
                record->set_is_doing((value.toInt() == Qt::Checked ? 1 : 0));
                if (value.toInt() == Qt::Checked) {
                    checkedEntrust.insert(record->id());
                } else {
                    checkedEntrust.erase(record->id());
                }
                emit dataChanged(index, index);
                return true;
            }
        }
        default:
            return false;
    }
    return false;
}

QVariant QSqlQueryCheckboxModel::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (role) {
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case Qt::DisplayRole: {
            if (orientation == Qt::Horizontal) {

                IniParse osla(OSLA_CONFIG);
                GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
                if (GlobalData::currentLocale.isEmpty()) {
                    GlobalData::currentLocale = "en_US";
                }
                if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
                    if (section == 0)
                        return "委托ID";

                    if (section == 1)
                        return "标题";

                    if (section == 2) {
                        return "状态";
                    }

                    if (section == 3) {
                        return title;/// "";
                    }
                }
                else{

                    if (section == 0)
                        return "Entrust ID";

                    if (section == 1)
                        return "Title";

                    if (section == 2) {
                        return "Status";
                    }

                    if (section == 3) {
                        return title;/// "";
                    }
                }

            }
        }
        default:
            return QVariant();
    }

    return QVariant();
}

bool QSqlQueryCheckboxModel::getStatus(const QModelIndex &index, int *status) const
{
    if (!index.isValid()) {
        false;
    }

    int nRow = index.row();
    const biotech::osla::Entrust &record = entrusts.entrust(nRow);

    *status = record.status();

    return true;
}

void QSqlQueryCheckboxModel::updateData(biotech::osla::EntrustList recordList) {
    entrusts = recordList;
    beginResetModel();
    endResetModel();
}

std::set<int> &QSqlQueryCheckboxModel::getCheckedEntrust() {
    return checkedEntrust;
}
