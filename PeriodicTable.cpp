#include "PeriodicTable.h"
#include "QFile.h"
#include "QTextStream.h"
#include "QMessageBox.h"
#include "QDir.h"

PeriodicTable::PeriodicTable(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	
	
	PeriodTableM = new PeriodicTableModel(this); // Создаем модель 
	ui.periodicTable_tv->setModel(PeriodTableM);//Привязка QTableView с моделью
	ui.periodicTable_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.periodicTable_tv->horizontalHeader()->setVisible(false); //Скрываем хэдер

	if (readElements() == -1) { // Читаем фал с информацией о хим. элементах (символ и номер)
		this->close();
		return;
	}
	PeriodTableM->populateData(name, number); // Добавляем вектора в модель


	
	connect(ui.periodicTable_tv->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(OnElementSelected()));

}

PeriodicTable::~PeriodicTable()
{
}

/////////////////////////////////////////////////////////////////////////////
// Функции считывающая файл с инф. и хим. элементах
//
int PeriodicTable::readElements()
{
	QString appPath = qApp->applicationDirPath();
	QDir dir(appPath);
	dir.cdUp();
	QString elements_f = dir.path() + "/share/scripts/elements";
	QFile file(elements_f);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Manager"),
			QString::fromLocal8Bit("Не удается прочитать файл %1:\n%2.")
			.arg("PeriodTable.txt")
			.arg(file.errorString()));
		return -1;
	}
	QTextStream in(&file);

	QStringList IonElements = { "O", "N" };// Список элементов для которых возможно установить ионизацию

	while (true) // Записываем информацию из файла в вектора (имя и номер)
	{
		int a;
		QString b;
		in >> a;
		in >> b;
		number.append(a);
		name.append(b);

		if (IonElements.contains(b))
		{
			number.append(a);
			name.append(b + "+");
		}
		in.readLine();
		if (in.atEnd())
			break;
	}
	number.append(0); 
	name.append(0);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Добавление элемента с ионизацией
//
void PeriodicTable::AddElemntPlus()
{
}

/////////////////////////////////////////////////////////////////////////////
// Выбор эдемента из таблицы
//
void PeriodicTable::OnElementSelected() {
	if (userChange)
	{
		int row; // строка
		int column; // столбец
		int vnumber; // номер в векторе

		row = ui.periodicTable_tv->currentIndex().row();
		column = ui.periodicTable_tv->currentIndex().column();

		vnumber = row*5 + column;
		element = QString("%1").arg(name[vnumber]);
		this->close();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Функции для работы с model/view
//
PeriodicTableModel::PeriodicTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

/////////////////////////////////////////////////////////////////////////////
// Добавление инф. о хим. эелементах в модель
//
void PeriodicTableModel::populateData(const QVector<QString> &Name, const QVector<int>  &Number)
{
	lm_Name.clear();
	lm_Number.clear();

	lm_Name = Name;
	lm_Number = Number;

	return;
}

/////////////////////////////////////////////////////////////////////////////
//Кол-во строк
//
int PeriodicTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 23;
}

/////////////////////////////////////////////////////////////////////////////
// Кол-во столбцов
//
int PeriodicTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 5;
}

QVariant PeriodicTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}
	if (index.column() == 0) {
		return (QString("%1:%2").arg(lm_Number[index.row()*5]).arg(lm_Name[index.row() * 5]));
	}
	else if (index.column() == 1) {
		return QString("%1:%2").arg(lm_Number[index.row() * 5 + 1]).arg(lm_Name[index.row() * 5 + 1]);
	}
	else if (index.column() == 2) {
		return QString("%1:%2").arg(lm_Number[index.row() * 5 + 2]).arg(lm_Name[index.row() * 5 + 2]);
	}
	else if (index.column() == 3) {
		return QString("%1:%2").arg(lm_Number[index.row() * 5 + 3]).arg(lm_Name[index.row() * 5 + 3]);
	}
	else if (index.column() == 4) {
		return QString("%1:%2").arg(lm_Number[index.row() * 5 + 4]).arg(lm_Name[index.row() * 5 + 4]);
	}
	return QVariant();
}

QVariant PeriodicTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
	}
	return QVariant();
}
