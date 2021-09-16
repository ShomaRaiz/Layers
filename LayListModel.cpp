#include "LayListModel.h"

/////////////////////////////////////////////////////////////////////////////
// Функции для работы с model/view для таблицы слоев
//
LayListModel::LayListModel(QObject* parent) : QAbstractTableModel(parent)
{
}

/////////////////////////////////////////////////////////////////////////////
// Добавление инф. о слоях в модель
//
void LayListModel::populateData(const QVector<QString>& Name, const QVector<int>& Number, const QVector<QString>& MratName, const QVector<int>& MratExist, const QVector<double>& Eps, const QVector<double>& Mu, const QVector<double>& Sig, const QVector<double>& Ro, const QVector<QString>& model)
{
	int rows = this->rowCount();
	if (rows > Name.size())
	{
		int position = Name.size() - 1;
		if (position < 0)
		{
			position = 0;

		}
		removeRows(position, 1);
	}
	if (rows < Name.size())
	{
		addRow();
	}



	lm_Name.clear();
	lm_Eps.clear();
	lm_Mu.clear();
	lm_Number.clear();
	lm_Ro.clear();
	lm_Sig.clear();
	lm_model.clear();
	lm_MratName.clear();
	lm_MratExist.clear();

	lm_Name = Name;
	lm_Eps = Eps;
	lm_Mu = Mu;
	lm_Number = Number;
	lm_Ro = Ro;
	lm_Sig = Sig;
	lm_model = model;
	lm_MratName = MratName;
	lm_MratExist = MratExist;

	QModelIndex topLeft = createIndex(0, 0);
	QModelIndex bottomRight = createIndex(this->rowCount(), this->columnCount());
	emit dataChanged(topLeft, bottomRight, { Qt::DisplayRole });
	return;
}

void LayListModel::addRow()
{
	beginInsertRows(index(rowCount(), 0), rowCount(), rowCount());
	endInsertRows();
}

void LayListModel::deleteRow()
{
	beginRemoveRows(index(rowCount(), 0), rowCount(), rowCount());
	endRemoveRows();
}

bool LayListModel::removeRows(int position, int rows, const QModelIndex& index)
{
	Q_UNUSED(index);
	beginRemoveRows(QModelIndex(), position, position + rows - 1);
	endRemoveRows();
	return true;
}

int LayListModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return lm_Name.size();
}

int LayListModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return 8;
}

QVariant LayListModel::data(const QModelIndex& index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		if (index.column() == 0) {
			return lm_Name[index.row()];
		}
		else if (index.column() == 1) {
			return lm_Number[index.row()];
		}
		else if (index.column() == 2) {
			return lm_MratName[index.row()];
		}
		else if (index.column() == 3) {
			return lm_Eps[index.row()];
		}
		else if (index.column() == 4) {
			return lm_Mu[index.row()];
		}
		else if (index.column() == 5) {
			return lm_Sig[index.row()];
		}
		else if (index.column() == 6) {
			return lm_Ro[index.row()];
		}
		else if (index.column() == 7) {
			return lm_model[index.row()];
		}
	case Qt::BackgroundRole:
		if (index.column() == 2 && lm_MratExist[index.row()])
			return QBrush(Qt::green);
		if (index.column() == 2 && !lm_MratExist[index.row()])
			return QBrush(Qt::red);
		break;
	}
	return QVariant();
}

QVariant LayListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		if (section == 0) {
			return QString::fromLocal8Bit("Название");
		}
		else if (section == 1) {
			return  QString::fromLocal8Bit("Номер");
		}
		else if (section == 2) {
			return QString::fromLocal8Bit("Имя файла\nс хим. составом");
		}
		else if (section == 3) {
			return QString(QChar(0xb5, 0x03));
		}
		else if (section == 4) {
			return QString(QChar(0xbc, 0x03));
		}
		else if (section == 5) {
			return QString(QChar(0xc3, 0x03));
		}
		else if (section == 6) {
			return QString(QChar(0xc1, 0x03));
		}
		else if (section == 7) {
			return QString::fromLocal8Bit("Модель\nпроводимости");
		}
	}
	return QVariant();
}

Qt::ItemFlags LayListModel::flags(const QModelIndex& index) const
{
	if (index.column() == 2)
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index);
}