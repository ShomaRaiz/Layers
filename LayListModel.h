#ifndef LAYLISTMODEL_H
#define LAYLISTMODEL_H

#include <QtWidgets/QMainWindow>
#include "const.h"
#include "QAbstractTableModel"

class LayListModel : public QAbstractTableModel // модель для представления информации о слое в таблице
{
	Q_OBJECT

public:
	LayListModel(QObject* parent = 0);

	void populateData(const QVector<QString>& Name, const QVector<int>& Number, const QVector<QString>& MratName, const QVector<int>& MratExist, const QVector<double>& Eps, const QVector<double>& Mu, const QVector<double>& Sig, const QVector<double>& Ro, const QVector<QString>& model);
	void addRow();
	void deleteRow();
	bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const;
private:
	//Layer lm_layInfo[N_LAYERS_MAX + 1];
	QVector<QString> lm_Name;// имя слоя
	QVector<int>  lm_Number;// номер слоя
	QVector<double>  lm_Eps; // диэлектрическая проницаемость
	QVector<double>  lm_Mu; // магнитная проницаемость
	QVector<double>  lm_Sig; // проводимость
	QVector<double>  lm_Ro; // плотность
	QVector<QString> lm_model; // модели проводимости
	QVector<QString> lm_MratName; // имена mrat файлов
	QVector<int> lm_MratExist; // наличие mrat-файла в библиотеке



};

#endif