#ifndef PRERIODIC_TABLES
#define PRERIODIC_TABLES

#include <QWidget>
#include <QDialog.h>
#include "QAbstractTableModel"

#include "ui_PeriodicTable.h"


class PeriodicTableModel : public QAbstractTableModel // модель для представления периодической таблицы
{
	Q_OBJECT

public:
	PeriodicTableModel(QObject *parent = 0);

	void populateData(const QVector<QString> &Name, const QVector<int>  &Number);
	void addRow();
	void deleteRow();
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
	QVector<QString> lm_Name;// имя элемента
	QVector<int>  lm_Number;// номер элемента


};

class PeriodicTable : public QDialog, public Ui::PeriodicTable
{
	Q_OBJECT

public:
	PeriodicTable(QWidget *parent = Q_NULLPTR);
	~PeriodicTable();
public:
	PeriodicTableModel *PeriodTableM; //модель для связи данных о хим. элементах с таблицой на экране

	bool userChange = true; // изменения вводятся пользователем (true - Да, fasle - Нет)
	QVector<int> number; // Номера хим. элементов
	QVector<QString> name; // Имена хим. элементов
	QString element = "NONE"; //Выбраный элемент из таблицы
public:
	int readElements(); // Чтение файла с инф о хим. элементах
	void AddElemntPlus(); // Добавление элемента с ионизацией
private slots:
	void OnElementSelected();
private:
	Ui::PeriodicTable ui;
};

#endif