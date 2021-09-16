#ifndef MRATMODEL_H
#define MRATMODEL_H

#include <QtWidgets/QMainWindow>
#include "const.h"
#include "QAbstractTableModel"

class MratModel : public QAbstractTableModel // модель для представления информации о хим. составе материала
{
	Q_OBJECT

public:
	MratModel(QObject* parent = 0);

	void populateData(const QVector<QString>& Element, const QVector<double>& ElPortion);
	void addRow();
	void deleteRow();
	bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	static double Sum(QVector<double> vdouble);

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value,
		int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex& index) const;
public:
	QVector<QString> lm_Element;// имя хим. элемента
	QVector<double>  lm_ElPortion;// доля хим. элемента
	int MaxRow = 0;
};
#endif // !1