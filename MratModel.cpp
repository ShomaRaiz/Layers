#include "MratModel.h"

/////////////////////////////////////////////////////////////////////////////
// Добавление инф. о хим. составе (mrat-файл)
//

MratModel::MratModel(QObject* parent) : QAbstractTableModel(parent)
{
}

void MratModel::populateData(const QVector<QString>& Element, const QVector<double>& ElPortion)
{
    int addRows = Element.size() - MaxRow;
    for (int i = 0; i < addRows; i++)
    {
        addRow();
        MaxRow++;
    }

    if (MaxRow > Element.size())
    {
        int position = Element.size() - 1;
        if (position < 0)
        {
            position = 0;

        }
        removeRows(position, MaxRow - Element.size());
        MaxRow = Element.size();
    }
    lm_Element.clear();
    lm_ElPortion.clear();

    lm_Element = Element;
    lm_ElPortion = ElPortion;



    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(this->rowCount(), this->columnCount());
    emit dataChanged(topLeft, bottomRight, { Qt::DisplayRole });
    return;
}

void MratModel::addRow()
{
    beginInsertRows(index(rowCount(), 0), rowCount(), rowCount());
    endInsertRows();
}

void MratModel::deleteRow()
{
    int a = rowCount();
    beginRemoveRows(QModelIndex(), rowCount(), rowCount() + 1);

    endRemoveRows();
}

bool MratModel::removeRows(int position, int rows, const QModelIndex& index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    endRemoveRows();
    return true;
}

int MratModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return lm_Element.size() + 1;
}

int MratModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

double MratModel::Sum(QVector<double> vdouble)
{
    double sum = 0;
    for each (double var in vdouble)
    {
        sum += var;
    }
    return sum;
}

QVariant MratModel::data(const QModelIndex& index, int role) const
{
    int a = index.row();
    switch (role)
    {
    case Qt::DisplayRole:
        if (index.column() == 0) {
            if (index.row() < lm_Element.size())
                return lm_Element[index.row()];
            else// if (index.row() == lm_Element.size())
                return QString::fromLocal8Bit("Сумма");
        }
        else if (index.column() == 1) {
            if (index.row() < lm_Element.size())
                return lm_ElPortion[index.row()];
            else //if (index.row() == lm_Element.size())
                return Sum(lm_ElPortion);
        }

    case Qt::EditRole:
        if (index.column() == 0) {
            return (QString("%1").arg(lm_Element[index.row()]));
        }
        else if (index.column() == 1) {
            return (QString("%1").arg(lm_ElPortion[index.row()]));
        }
    }
    return QVariant();
}

QVariant MratModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString::fromLocal8Bit("Хим. элемент");
        }
        else if (section == 1) {
            return  QString::fromLocal8Bit("Доля");
        }
    }
    return QVariant();
}

bool MratModel::setData(const QModelIndex& index,
    const QVariant& value, int role)
{
    if (index.isValid() && role == Qt::EditRole && index.column() == 1) {

        lm_ElPortion.replace(index.row(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags MratModel::flags(const QModelIndex& index) const
{
    if (!index.isValid() || index.column() == 0)
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}