
#include "Layers.h"
#include "qfiledialog.h"
#include <fstream>
#include "qmessagebox.h"
#include <atlstr.h>
#include "qfontdatabase.h"
#include "qfile.h"
#include "qtextstream.h"
#include "PeriodicTable.h"
#include "Python_bindings.h"

using namespace std;

Layers::Layers(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    int i;
	if (qApp->arguments().last().right(3) == "LAY") // Открвыется в студии
	{
        if (ReadManagerIni() == -1)
        {
            exit = true;
            this->close();
            return;
        }
		FileName = qApp->arguments().last();
		i = ReadFile(FileName);
	}
	else // открывается вне студии
	{
		QFileDialog  dialog;
		QString filename;
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setNameFilter(tr("Layers files (*.lay)"));
		dialog.setViewMode(QFileDialog::Detail);
		if (dialog.exec() == QDialog::Accepted)
		{
			FileName = QFileInfo(dialog.selectedFiles()[0]).fileName();
			Path = QFileInfo(dialog.selectedFiles()[0]).path();
            if (ReadManagerIni() == -1)
            {
                exit = true;
                this->close();
                return;
            }
			i = ReadFile(Path + '/' + FileName);
		}
		else
		{
			ok = true;
			this->close();
			return;
		}


	}
	
	LayInfoM = new LayListModel(this);// создаем модель для связи данных с инф. о слоях с таблицей вывода
	HimSostM = new MratModel(this);
    QDir::setCurrent(Path);
	InitData();
	init_paths();
	//LayInfoM->populateData(vName,vNumber,vEps,vMu,vSig,vRo,vModelShort); // вносим данные о слоях в модель
	ui.layInfo_tv->setModel(LayInfoM); // соединяем модель с таблицей
	ui.layInfo_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.layInfo_tv->horizontalHeader()->setVisible(true);
	ui.layInfo_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    
	ui.himSost_tv->setModel(HimSostM); // соединяем модель с таблицей
	ui.himSost_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.himSost_tv->horizontalHeader()->setVisible(true);

	ui.layInfo_tv->horizontalHeader()->setStyleSheet(
		"QHeaderView::section{"
            "border-top:0px solid #D8D8D8;"
            "border-left:0px solid #D8D8D8;"
            "border-right:1px solid #D8D8D8;"
            "border-bottom: 1px solid #D8D8D8;"
            "background-color:white;"
            "padding:4px;"
        "}"
        "QTableCornerButton::section{"
            "border-top:0px solid #D8D8D8;"
            "border-left:0px solid #D8D8D8;"
            "border-right:1px solid #D8D8D8;"
            "border-bottom: 1px solid #D8D8D8;"
            "background-color:white;"
		"}");

    ui.himSost_tv->horizontalHeader()->setStyleSheet(
        "QHeaderView::section{"
        "border-top:0px solid #D8D8D8;"
        "border-left:0px solid #D8D8D8;"
        "border-right:1px solid #D8D8D8;"
        "border-bottom: 1px solid #D8D8D8;"
        "background-color:white;"
        "padding:4px;"
        "}"
        "QTableCornerButton::section{"
        "border-top:0px solid #D8D8D8;"
        "border-left:0px solid #D8D8D8;"
        "border-right:1px solid #D8D8D8;"
        "border-bottom: 1px solid #D8D8D8;"
        "background-color:white;"
        "}");

	// Размеры виджетов остаются постоянными при их скрытии 
	// для сохранения макета
	QSizePolicy sp_retain = ui.trapConc_lb->sizePolicy();
	sp_retain.setRetainSizeWhenHidden(true);
	ui.trapConc_lb->setSizePolicy(sp_retain);
	
	QSizePolicy sp_retain1 = ui.trapConc_le->sizePolicy();
	sp_retain1.setRetainSizeWhenHidden(true);
	ui.trapConc_le->setSizePolicy(sp_retain1);

	QSizePolicy sp_retain2 = ui.ionCost_lb->sizePolicy();
	sp_retain2.setRetainSizeWhenHidden(true);
	ui.ionCost_lb->setSizePolicy(sp_retain2);

	QSizePolicy sp_retain3 = ui.ionCost_le->sizePolicy();
	sp_retain3.setRetainSizeWhenHidden(true);
	ui.ionCost_le->setSizePolicy(sp_retain3);

	QSizePolicy sp_retain4 = ui.molarWeight_lb->sizePolicy();
	sp_retain4.setRetainSizeWhenHidden(true);
	ui.molarWeight_lb->setSizePolicy(sp_retain4);

	QSizePolicy sp_retain5 = ui.molarWeight_le->sizePolicy();
	sp_retain5.setRetainSizeWhenHidden(true);
	ui.molarWeight_le->setSizePolicy(sp_retain5);

	QSizePolicy sp_retain6 = ui.atomMolecule_le->sizePolicy();
	sp_retain6.setRetainSizeWhenHidden(true);
	ui.atomMolecule_le->setSizePolicy(sp_retain6);

	QSizePolicy sp_retain7 = ui.atomMolecule_lb->sizePolicy();
	sp_retain7.setRetainSizeWhenHidden(true);
	ui.atomMolecule_lb->setSizePolicy(sp_retain7);

	QSizePolicy sp_retain8 = ui.nuclearCharge_lb->sizePolicy();
	sp_retain8.setRetainSizeWhenHidden(true);
	ui.nuclearCharge_lb->setSizePolicy(sp_retain8);

	QSizePolicy sp_retain9 = ui.nuclearCharge_le->sizePolicy();
	sp_retain9.setRetainSizeWhenHidden(true);
	ui.nuclearCharge_le->setSizePolicy(sp_retain9);

	QSizePolicy sp_retain10 = ui.outsideCur_chb->sizePolicy();
	sp_retain10.setRetainSizeWhenHidden(true);
	ui.outsideCur_chb->setSizePolicy(sp_retain10);

	QSizePolicy sp_retain11 = ui.outSource_chb->sizePolicy();
	sp_retain11.setRetainSizeWhenHidden(true);
	ui.outSource_chb->setSizePolicy(sp_retain11);

	QSizePolicy sp_retain12 = ui.temperature_lb->sizePolicy();
	sp_retain12.setRetainSizeWhenHidden(true);
	ui.temperature_lb->setSizePolicy(sp_retain12);

	QSizePolicy sp_retain13 = ui.temperature_le->sizePolicy();
	sp_retain13.setRetainSizeWhenHidden(true);
	ui.temperature_le->setSizePolicy(sp_retain13);
    /////////////////////////////////////////////////////////////////////////////
    // Связь с элементами интерфейса
    //
    connect(ui.delete_pb, SIGNAL(clicked()), this, SLOT(OnChange()));
    connect(ui.inList_pb, SIGNAL(clicked()), this, SLOT(OnChange()));
    connect(ui.outsideCur_chb, SIGNAL(clicked()), this, SLOT(OnChange()));
    connect(ui.outSource_chb, SIGNAL(clicked()), this, SLOT(OnChange()));
   // connect(ui.layInfo_lw, SIGNAL(currentRowChanged(int)), this, SLOT(OnChange()));
    connect(ui.conductModel_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChange()));
    connect(ui.dielectricConst_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.magneticConst_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.conduct_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.layNumber_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.layName_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.density_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    //connect(ui.density_le, SIGNAL(editingFinished()), this, SLOT(OnChangeMrat()));
    connect(ui.temperature_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.trapConc_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.ionCost_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.molarWeight_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.atomMolecule_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.nuclearCharge_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
    connect(ui.material_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelchangeMat()));
   // connect(ui.layInfo_lw, SIGNAL(currentRowChanged(int)), this, SLOT(OnSelchangeList()));
    connect(ui.typePP_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChange()));
	connect(ui.typePP_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelchangePPModel()));
    connect(ui.conductModel_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelchangeComboModel()));
    connect(ui.save_pb, SIGNAL(clicked()), this, SLOT(OnOK()));
    connect(ui.leave_pb, SIGNAL(clicked()), this, SLOT(OnCancel()));
    connect(ui.delete_pb, SIGNAL(clicked()), this, SLOT(OnDelete()));
    connect(ui.inList_pb, SIGNAL(clicked()), this, SLOT(OnEditLayer()));
	//connect(ui.layInfo_tw, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelchangeList()));
	connect(ui.layInfo_tv->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(OnSelchangeList()));
    connect(ui.himSostOpen_pb, SIGNAL(clicked()), this, SLOT(OnOpenMrat()));
    connect(ui.himSostCreate_pb, SIGNAL(clicked()), this, SLOT(OnCreateMrat()));
    connect(ui.himSost_tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnChangeElem()));
    connect(ui.himSost_tv, SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)), this, SLOT(OnEditCell()));
	connect(ui.addEl_pb, SIGNAL(clicked()), this, SLOT(OnAddElem()));
	connect(ui.deleteEl_pb, SIGNAL(clicked()), this, SLOT(OnDeleteElem()));
	connect(ui.saveMrat_pb, SIGNAL(clicked()), this, SLOT(OnSaveMrat()));
	connect(ui.himSost_tv->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(OnEditCell()));
	connect(ui.mratName_le, SIGNAL(editingFinished()), this, SLOT(OnChangeMrat()));
	connect(ui.mratName_le, SIGNAL(editingFinished()), this, SLOT(OnChange()));
	connect(ui.composName_le, SIGNAL(editingFinished()), this, SLOT(OnChangeMrat()));
	connect(ui.layInfo_tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnOpenMratTable()));
    connect(ui.getRoFromMrat_pb, SIGNAL(clicked()), this, SLOT(OnGetRoFromMrat()));
    connect(ui.addLayer_pb, SIGNAL(clicked()), this, SLOT(OnAddLayer()));
}   


////////////////////////////////////////////////////////////////////////
// Чтение файла <Manager_ini> в переменные диалога
// Возвращаемый параметр :
//  0   - Данные считаны успешно
// -1   - Ошибка чтения
// 
int Layers::ReadManagerIni()
{
    char Buffer[255];
    QString tmp;
    QString key, value;
    int k, iModel = 0, iModelShort = 0, iPP = 0, iMat = 0, iSig;

    QString appPath = qApp->applicationDirPath();
    int index = appPath.lastIndexOf("/");
    appPath.remove(index + 1, appPath.size() + 1);

    const QString iniFilePath = appPath + "share/ini/manager.ini";

    fstream file(iniFilePath.toStdString(), ios::in | ios::_Nocreate);

    if (file.fail())
    {
        QMessageBox::warning(this, tr("Manager"),
            QString::fromLocal8Bit("Не удалось открыть файл %1\nв папке %2")
            .arg(Manager_ini)
            .arg(iniFilePath));
        //tmp = "Не удалось открыть файл "; tmp += Manager_ini; tmp += "\nв папке " + Path;
        //tmp = "Не удалось открыть файл "; tmp += Manager_ini; tmp += "\nв папке " + iniFilePath;
        //AfxMessageBox(tmp);
        return -1;  // не удалось открыть
    }

    while (!file.eof())    // до конца файла
    {
        file.getline(Buffer, 255);  // читаем строку
        tmp = QString::fromLocal8Bit(Buffer);
        k = tmp.indexOf('=');
        if (k < 1) continue;
        key = tmp.left(k);  // разбираем строку на key и value
        value = tmp.mid(k + 1);
        if (key == "LAYERSDEMO") { //demo-версия
            Demo = value.toInt();
            continue;
        }

        if (key == "SIG_N") {  //кол-во моделей проводимости
            nModel = value.toInt();
            continue;
        }

        //очередная модель проводимости
        if (key == "SIG_NUMBER") { //номер
            ModelNum[iModel] = value.toInt();
            continue;
        }

        if (key == "SIG_DESC") { //название
            Model[iModel] = value;
            iModel++;
            continue;
        }

        if (key == "SIG_DESCSHORT") {
            ModelShort[iModelShort] = value;
            iModelShort++;
            continue;
        }

        if (key == "MAT_PPN") { //кол-во моделей полупроводников
            nPP = value.toInt();
            continue;
        }

        //очередной полупроводник
        if (key == "MAT_PPNUM") { //номер
            PPNum[iPP] = value.toInt();
            continue;
        }

        if (key == "MAT_PPNAME") {//название
            PPName[iPP] = value;
            continue;
        }

		if (key == "MAT_PPCT") {//концентрация ловушек 
			PPCT[iPP] = value.toDouble();
			continue;
		}

		if (key == "MAT_PPION") {//цена ионизации
			PPION[iPP] = value.toDouble();
			iPP++;
			continue;
		}

        if (key == "MAT_N") { //кол-во материалов
            nMat = value.toInt();
            continue;
        };

        //очередной материал
        if (key == "MAT_NUMBER") { //номер
            MatNum[iMat] = value.toInt();
            continue;
        }

        if (key == "MAT_NAME") { //название
            MatName[iMat] = value;
            continue;
        }

        if (key == "MAT_SIGNUM") { //Количество моделей проводимости для материала
            MatSigN[iMat] = value.toInt();
            iSig = 0;
            continue;
        }

        if (key == "MAT_SIGNUMBER") { //Номера моделей проводимости для материала
            MatSigNum[iMat][iSig] = value.toInt();
            iSig++;
            continue;
        }

        // Поля для материала (0-нет, 1-есть)
        if (key == "MAT_T") { // температура
            MatT[iMat] = value.toDouble();
            continue;
        }

        if (key == "MAT_J") { // стронний ток и источник вторичных электронов
            MatJ[iMat] = value.toInt();
            continue;
        }

        if (key == "MAT_PP") { // модель полупроводника
            MatPP[iMat] = value.toInt();
            continue;
        }

        if (key == "MAT_DOP") { // молекулярный вес, кол-во атомов в мол., средний заряд ядра
            MatDop[iMat] = value.toInt();
            continue;
        }

        if (key == "MAT_EION") { // цена ионизации
           MatEion[iMat] = value.toDouble();
            continue;
        }

        if (key == "MAT_CT") { // концентрация ловушек
            MatCt[iMat] = value.toDouble();
            continue;
        }

		if (key == "MAT_SIGMA") { // проводимость
			MatSIGMA[iMat] = value.toDouble();
			continue;
		}

		if (key == "MAT_EPS") { // диэлектрическая проницаемость
			MatEPS[iMat] = value.toDouble();
			continue;
		}

		if (key == "MAT_MU") { // магнитная проницаемость
			MatMU[iMat] = value.toDouble();
			continue;
		}

        if (key == "MAT_RO") { //плотность
            MatRo[iMat] = value.toDouble();
            iMat++;
            continue;
        }

    };
    file.close();   // закрываем

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Чтение файла layers
//
void Layers::ReadLayers()
{
    layersName = Path + "/pechs/object/layers";
    QFile file(layersName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        // Файла layers не существует (Новый РЭМП)
        // Создаем необходимые директории
        QDir().setCurrent(Path);
        QDir().mkdir("pechs");
		QDir().setCurrent(Path + "/pechs");
		QDir().mkdir("object");
		QDir().setCurrent(Path + "/pechs/object");
		QDir().mkdir("materials");
        for (int i = 0; i <= N_LAYERS_MAX; i++)	// Для всех слоев,
        {
            if (Lay[i].exist == 1)			// которые существуют,
            {
                Lay[i].mratName = Lay[i].name;
            }
        }
        WriteLayers(); // создаем файл layers
        return;
    }
    
    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString mrat, buff;
        int i;
        in >> i;        // номер слоя
        in >> mrat;     // назавние mrat-файла
        in.readLine();  // переходим на другую строку
        Lay[i].mratName = mrat;
    }
}


/////////////////////////////////////////////////////////////////////////////
// Чтение файла *.PL (кол-во слоев и их номера)
//
int Layers::ReadPL(QString filename)
{
	QString PLfilename = FileName;
	PLfilename.replace("LAY", "PL");
	PLfilename = Path + "/" + PLfilename;
	QFile file(PLfilename);
	QTextStream in(&file);
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		while (!in.atEnd())
		{
			QString str = in.readLine();
			if (str == "<Number of layers>")
			{
				layNumPL = in.readLine().toInt();
				in.readLine();
				for (int i = 0; i < layNumPL; i++)
				{
					int laynumber;
					in >> laynumber;
					layersPL.append(laynumber);
				}
				break;
			}
		}
	}
	else
	{
		return -1;
	}
	file.close();
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Чтение mrat-файла 
//
int Layers::ReadMrat(QString filename)
{
    QString tmp, key, value, ion;
    QStringList tmpList;
    bool ionn = true;
    int k;
    double buf;



    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Manager"),
            QString::fromUtf8("Не удается прочитать файл %1:\n%2.")
            .arg(filename)
            .arg(file.errorString()));
        return -1;
    }
    QTextStream in(&file);

    vElement.clear();
    vElPortion.clear();

    /*while (!in.atEnd())
    {
        tmp = in.readLine();
        tmpList = tmp.split("\t");
        if (tmpList[0] == "[Composite]")
        {
            m_composName = tmpList[1];
        }
        if (tmpList[0] == "[Element]")
        {
            if (tmpList.size() == 4)
                vElement.append(tmpList[1] + "+");
            else
                vElement.append(tmpList[1]);

            vElPortion.append(tmpList[2].toDouble());
            

        }
		if (tmpList[0] == "[Density]")
		{
		    m_Ro = tmpList[1].toDouble();
           
		}
    }*/

    while (!in.atEnd())
    {
        if (ionn)
            in >> tmp;
        else
            tmp = ion;
        if (tmp == "[Composite]")
        {
            in >> m_composName;
            in.readLine();
        }
        if (tmp == "[Element]")
        {
            in >> tmp >> buf >> ion;
            if (ion == "1")
            {
                vElement.append(tmp + "+");
                ionn = true;
            }
            else
            {
                vElement.append(tmp);
                ionn = false;
            }

            vElPortion.append(buf);
        }
        if (tmp == "[Density]")
        {
            in >> RoMrat;
            in.readLine();
            break;
        }
    }


	userChange = false;
    HimSostM->populateData(vElement, vElPortion);
	userChange = true;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Запись mrat-файла
//
int Layers::WriteMrat(QString filename)
{

	QFile file(filename);
	file.open(QFile::WriteOnly | QFile::Text);
	QTextStream out(&file);
	m_composName = ui.composName_le->text();
	// Запись в файл .mrat
	out << "[Composite] " << m_composName << endl;
	for (int i = 0; i < vElement.size(); i++)
	{ 
        QString ion = "";
        if (vElement[i].contains("+"))
            ion = "1";

        vElement[i].remove("+");
		out << "[Element] " << vElement[i] << " " << vElPortion[i] << " " << ion << endl;
		
	}
	out << "[Density] " << ui.density_le->text() << endl;

	file.close();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Запись(создание) файла layers
//
void Layers::WriteLayers()
{
    QFile file(layersName);
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);

    for (int i = 0; i <= N_LAYERS_MAX; i++)	// Для всех слоев,
    {
        if (Lay[i].exist == 1)			// которые существуют,
        {
            out << i << " " << Lay[i].mratName << " " << Lay[i].ro << endl;
        }
    }
    file.close();

}

/////////////////////////////////////////////////////////////////////////////
// Подготовка списка слоев на экран
//
void Layers::PrepareList()
{
	
    userChange = false;
    int i, j = -1, k, l;
	vName.clear();
	vNumber.clear();
	vEps.clear(); // диэлектрическая проницаемость
	vMu.clear(); // магнитная проницаемость
	vSig.clear(); // проводимость
	vRo.clear(); // плотность
	vModelShort.clear(); // модель проводимости
    vMratExist.clear(); // наличие mrat-файла в библиотеке
    vMratName.clear(); // имя mrat-файла
    for (i = 0; i <= N_LAYERS_MAX; i++)	// Для всех слоев,
    {
        if (Lay[i].exist == 1)			// которые существуют,
        {
            j++;					   // заводим номер
		
			vName.push_back(Lay[i].name); // название слоя
			vNumber.push_back(i); // номер слоя
			vEps.push_back(Lay[i].eps); // диэлектрическая проницаемость
			vMu.push_back(Lay[i].mu); // магнитная проницаемость
			vSig.push_back(Lay[i].sig);; // проводимость
			vRo.push_back(Lay[i].ro);; // плотность
			vMratName.push_back(Lay[i].mratName); // Имена mrat файлов
            vMratExist.push_back(Lay[i].mratExist); // наличие mrat-файла в библиотеке
            l = 0;
            for (k = 1; k < nModel; k++)
                if (ModelNum[k] == Lay[i].model) l = k;

			vModelShort.push_back(ModelShort[l]);
            INDEX[j] = i;   // Определяем, на какой слой указывает текущий номер в списке			
        }
    }
	int row1 = LayInfoM->rowCount();

	LayInfoM->populateData(vName, vNumber, vMratName, vMratExist ,vEps, vMu, vSig, vRo, vModelShort); // вносим данные о слоях в модель

	m_nLayer = QString("%1").arg(nLayer);		// Записываем количество слоев в строку
    userChange = true;
}

/////////////////////////////////////////////////////////////////////////////
// Подготовка данных текущего слоя на экран
//
void Layers::PrepareData()
{
	if (nLayer == 0)
		return;
    int i;
    m_Num = INDEX[m_List];					// Номер
    m_mratName = Lay[m_Num].mratName;       // имя mrat-файла 
    m_Name = Lay[m_Num].name;				// название
    m_Gaz = Lay[m_Num].gaz - 1;				// тип п/п
    if (m_Gaz < 0) m_Gaz = 0;
    m_Mat = Lay[m_Num].mat;					// материал
    m_Type = m_Mat - 1;

    m_Eps = Lay[m_Num].eps;				// диэлектрическая проницаемость
    m_Mu = Lay[m_Num].mu;				// магнитная проницаемость
    m_Sig = Lay[m_Num].sig;				// проводимость
    m_Sig_Num = Lay[m_Num].model;			// модель проводимости

    if (Lay[m_Num].sv == 1) m_sv = 1; else m_sv = 0;	// сторонний ист. вторичных 

    m_Ro = Lay[m_Num].ro;				// плотность
    m_T = Lay[m_Num].t;				// температура
    if (Lay[m_Num].j == 1) m_j = 1; else m_j = 0;	// сторонний ток

    m_A = Lay[m_Num].A;				// молекулярный вес
    m_NAt = Lay[m_Num].NAt;				// кол-во атомов в мол.
    m_Z = Lay[m_Num].Z;				// средний заряд ядра
    m_Evtor = Lay[m_Num].Evtor;			// цена ионизации
    m_NTr = Lay[m_Num].NTr;				// концентрация ловушек

    i_Mat = 0;			// Номер материала в массиве
    for (i = 0; i < nMat; i++)
        if (MatNum[i] == m_Mat) i_Mat = i;

    i_Sig = 0;			// Номер модели проводимости в массиве
	for (i = 1; i < nModel; i++)
	{
		if (ModelNum[i] == m_Sig_Num) i_Sig = i;
	}

    if (Lay[m_Num].mratExist) // Хим. состав материала
    {
        ReadMrat("C:/Program Files/REMP/share/scripts/mat_files/" + m_mratName + ".mrat");
    }
    else
    {
        m_composName = Lay[m_Num].mratName;
        vElement.clear();
        vElPortion.clear();
        HimSostM->populateData(vElement, vElPortion);
    }

    PrepareFields();
}

/////////////////////////////////////////////////////////////////////////////
// Подготовка моделей проводимости на экран
//
void Layers::PrepareSig()
{
    userChange = false;
    int j, k, l = 0;
    m_Model = l;
    ui.conductModel_cb->clear();
    ui.conductModel_cb->addItem(Model[0]);	// 0 модель всегда есть
    m_ModelNum[l] = 0;

    for (j = 0; j < MatSigN[i_Mat]; j++)
        for (k = 1; k < nModel; k++)
            if (ModelNum[k] == MatSigNum[i_Mat][j])
            {
                ui.conductModel_cb->addItem(Model[k]);
                l++;
                m_ModelNum[l] = ModelNum[k];
                if (m_Sig_Num == ModelNum[k])	m_Model = l;// Если в списке есть модель проводимости, ставим на нее указатель
            }
    Change = false;
    userChange = true;
}

/////////////////////////////////////////////////////////////////////////////
// Включение слоя в список
//
void Layers::OnInList()
{

    DataUpdate(TRUE);		// Чтение данных с экрана
    if ((m_Num > N_LAYERS_MAX) || (m_Num < 0)) {
        QMessageBox::information(this, "Layers", QString::fromLocal8Bit("Неправильный номер слоя"), QMessageBox::Ok); return; };
    // При неправильном номере ничего не делаем

    if (Lay[m_Num].exist != 1)nLayer++; // Если слоя не было, увеличиваем количество слоев

/////////////////////////////////////////////////////////////////////////////
// Записываем данные слоя в массив
    Lay[m_Num].exist = 1;
	Lay[m_Num].number = m_Num;
    Lay[m_Num].name = m_Name;		// название	
    Lay[m_Num].mratName = m_mratName; // имя mrat-файла
    Lay[m_Num].mratExist = 1; 
    Lay[m_Num].gaz = m_Gaz;		// тип п/п
    if (MatPP[i_Mat] == 1) Lay[m_Num].gaz += 1;
    m_Mat = m_Type + 1;
    Lay[m_Num].mat = m_Mat;			// материал

    Lay[m_Num].eps = m_Eps;		// диэлектрическая проницаемость
    Lay[m_Num].mu = m_Mu;		// магнитная проницаемость
    Lay[m_Num].sig = m_Sig;		// проводимость

    m_Sig_Num = m_ModelNum[m_Model];
    Lay[m_Num].model = m_Sig_Num;		// модель проводимости
    if (m_sv == TRUE) Lay[m_Num].sv = 1; else Lay[m_Num].sv = 0;	// сторонний ист. вторичных 

    Lay[m_Num].ro = m_Ro;		// плотность
    Lay[m_Num].t = m_T;		// температура
    if (m_j == TRUE) Lay[m_Num].j = 1; else Lay[m_Num].j = 0;	// сторонний ток

    if (m_Mat == 1) Lay[m_Num].dop = 0; else Lay[m_Num].dop = 1;	// дополнительные параметры
    Lay[m_Num].A = m_A;		// молекулярный вес
    Lay[m_Num].NAt = m_NAt;		// кол-во атомов в мол.
    Lay[m_Num].Z = m_Z;		// средний заряд ядра
    Lay[m_Num].Evtor = m_Evtor;		// цена ионизации
    Lay[m_Num].NTr = m_NTr;		// концентрация ловушек

	if (MratChange) // Если менялся mrat-файл
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Сохранить изменение файла с описанием хим. состава?"),
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
		{
			if (OnSaveMrat() == -1)
			{
				userChange = false;
				ui.layInfo_tv->setCurrentIndex(LayInfoM->index(m_List, 0));
				userChange = true;
				return;
			}
		}
		else
			MratChange = false;
	}
   
    Change = FALSE;		// Не менялся
    CheckMrat();
    PrepareList();		// Обновляем  список на экран
    //DataUpdate(FALSE);	// Записываем данные на экран
    
    
}

/////////////////////////////////////////////////////////////////////////////
// Внести изменения в слой
//
void Layers::OnEditLayer()
{
    int layNumber = ui.layNumber_le->text().toInt();

    if (Lay[layNumber].exist == 0)			// которые существуют,
    {
        QMessageBox::warning(this, tr("Manager"),
            QString::fromLocal8Bit("Слоя с таким номером не существует!"));
        return;
    }
    OnInList();
}

/////////////////////////////////////////////////////////////////////////////
// Исключение слоя из списка
//
void Layers::OnDelete()
{
    QString temp;
    int i = INDEX[m_List];            // находим место слоя в массиве
    //int i = ui.layInfo_lw->currentRow();
    if (Lay[i].exist == 0) return;		// если ее и не было, ничего не делаем - для правильной работы при 0 слоев
 
    temp = QString("%1").arg(i);
    //temp = "Вы уверены, что хотите удалить слой № " + temp + " [" + Lay[i].name + "] ?";
    //if (AfxMessageBox(temp, MB_YESNO) != IDYES) return; // а надо ли уничтожать?

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Вы уверены, что хотите удалить слой № %1 [%2] ?").arg(temp).arg(Lay[i].name),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    Lay[i].exist = 0;					// уничтожаем	
    nLayer--;						// уменьшаем количество слоев
	
	if ((m_List - 1) >= 0)
		m_List--;
	else
		m_List = 0;
	
    PrepareList();		// Обновляем список слоев на экран
    PrepareData();		// Обновляем остальные данные на экране
    DataUpdate(FALSE);	// Записываем данные на экран
}

////////////////////////////////////////////////////////////////////////////
// Подготовка начальных данных
//
void Layers::InitData()
{
    m_FileName = "";
    m_Comment = "";
    m_nLayer = "";
    m_List = 0;
    m_Model = 0;
    m_Name = "";
    m_Num = 0;
    m_Eps = 1.0;
    m_Mu = 1.0;
    m_Sig = 0.0;
    m_Ro = 1.0;
    m_T = Const_T;
    m_j = 0;
    m_Gaz = 0;
    m_sv = 0;
    m_A = Const_A;
    m_NAt = Const_NAt;
    m_Z = Const_Z;
    m_Evtor = Const_Evtor;
    m_NTr = Const_NTr;
    Demo = -1; // По умолчанию demo-версия

    ////////////////////////////////////////////////////////////////////////////
// Подготовка типов вещества на экране
    m_Type = 0;
	MatComboBoxAdded = true;
    ui.material_cb->clear();	// Очистка списка
    for (int i = 0; i < nMat; i++)	// Для всех материалов
    {
        ui.material_cb->addItem(MatName[i]);
        if (MatNum[i] == Lay[m_Num].mat) m_Type = i; // Если в списке есть текущий материал, ставим на него указатель
    }
	MatComboBoxAdded = false;
    /////////////////////////////////////////////////////////////////////////////
    // Подготовка типов п/п на экране
    m_Gaz = 0;
	PPComboBoxAdded = true;
    ui.typePP_cb->clear();		// Очистка списка
    for (int i = 0; i < nPP; i++)	// Для всех п/п
    {
        
        ui.typePP_cb->addItem(PPName[i]);
       
        if (PPNum[i] == Lay[m_Num].gaz) m_Gaz = i; // Если в списке есть текущий п/п, ставим на него указатель
    }
	PPComboBoxAdded = false;
    /////////////////////////////////////////////////////////////////////////////
    PrepareList();		// Подготовка списка слоев на экран
    PrepareData();		// Подготовка остальных данных на экран

   m_FileName = FileName; // Имя входного файла
   m_Comment = COMMENT;   // Строка-комментарий
   ui.comment_le->setText(m_Comment);
   DataUpdate(FALSE);
   ReadPL(FileName);
}

/////////////////////////////////////////////////////////////////////////////
// Активность полей на экране
//
void Layers::PrepareFields()
{
    PrepareSig();

    // Поля для материала (0-нет, 1-есть)
    if (MatT[i_Mat] == 0)		// температура
    {
        ui.temperature_lb->hide();
        ui.temperature_le->hide();
    }
    else
    {
        ui.temperature_lb->show();
        ui.temperature_le->show();
    }
    if (MatJ[i_Mat] == 0)		// стoронний ток и источник вторичных электронов
    {
        ui.outsideCur_chb->hide();
        ui.outSource_chb->hide();
    }
    else
    {
        ui.outsideCur_chb->show();
        ui.outSource_chb->show();
    }
    if ((MatPP[i_Mat] == 0) || (m_Model != 3) )		// модель полупроводника
    {
        ui.typePP_cb->hide();
        ui.typePP_lb->hide();
    }
    else
    {
        ui.typePP_cb->show();
        ui.typePP_lb->show();
        ui.typePP_cb->setCurrentIndex(m_Gaz);
    }
    if (MatDop[i_Mat] == 0)		// молекулярный вес, кол - во атомов в мол., средний заряд ядра
    {
        ui.molarWeight_lb->hide();
        ui.molarWeight_le->hide();
        ui.nuclearCharge_lb->hide();
        ui.nuclearCharge_le->hide();
        ui.atomMolecule_lb->hide();
        ui.atomMolecule_le->hide();
    }
    else
    {
        ui.molarWeight_lb->show();
        ui.molarWeight_le->show();
        ui.nuclearCharge_lb->show();
        ui.nuclearCharge_le->show();
        ui.atomMolecule_lb->show();
        ui.atomMolecule_le->show();
    }
    if (MatEion[i_Mat] == 0)		// цена ионизации
    {
        ui.ionCost_lb->hide();
        ui.ionCost_le->hide();
    }
    else
    {
        ui.ionCost_lb->show();
        ui.ionCost_le->show();
    }
    if (MatCt[i_Mat] == 0)		// концентрация ловушек
    {
        ui.trapConc_lb->hide();
        ui.trapConc_le->hide();
    }
    else
    {
        ui.trapConc_lb->show();
        ui.trapConc_le->show();
    }
}

/////////////////////////////////////////////////////////////
// Проверка на наличие mrat-файлов в библиотеке
//
void Layers::CheckMrat()
{
    QString appPath = qApp->applicationDirPath();
    int index = appPath.lastIndexOf("/");
    appPath.remove(index + 1, appPath.size() + 1);

    const QString matFilesFolder = appPath + "share/scripts/mat_files";

    QDir matFiles("C:/Program Files/REMP/share/scripts/mat_files");
    QStringList matFileList = matFiles.entryList(QStringList() << "*.mrat" << "*.MRAT", QDir::Files);
    

    for (int i = 0; i <= N_LAYERS_MAX; i++)	// Для всех слоев,
    {
        if (Lay[i].exist == 1)			// которые существуют,
        {
            int mratexist = 0;
            for each (QString matfile in matFileList)
            {
                if (matfile.contains(Lay[i].mratName + ".mrat"))
                    mratexist = 1;
            }
            Lay[i].mratExist = mratexist;
        }
    }
}

/////////////////////////////////////////////////////////////
// Проверка на соответствие кол-ва слоев и их номеров в файлах .LAY и .PL
//
bool Layers::CheckPLFile()
{
	if (vNumber.count() != layersPL.count())
		return false;
	for (int i = 0; i < vNumber.count(); i++)
	{
		if (vNumber[i] != layersPL[i])
			return false;
	}
    return true;
}

/////////////////////////////////////////////////////////////
// Расчет базовых сечений
//
void Layers::GetBasicSections()
{
    PythonBinds::calcDistribution(p);
}

/////////////////////////////////////////////////////////////
// Расчет итоговых сечений
//
void Layers::GetFinalSections()
{
    PythonBinds::getDistribution(p);
}


/////////////////////////////////////////////////////////////
// Обмен данными с итерфейсом (true - записываем с экрана, false - записываем на экран)
//
void Layers::DataUpdate(bool a)
{
    if (a)
    {
        //FileName = ui.fileName_le->text();
       // m_Comment = ui.comment_le->text();
        m_nLayer = ui.layCount_la->text();
		m_List = ui.layInfo_tv->currentIndex().row();

        m_Name = ui.layName_le->text();
        m_Num = ui.layNumber_le->text().toInt();
        m_Type = ui.material_cb->currentIndex();
        m_mratName = ui.mratName_le->text();
        m_composName = ui.composName_le->text();

        m_Eps = ui.dielectricConst_le->text().toFloat();
        m_Mu = ui.magneticConst_le->text().toFloat();
        m_Sig = ui.conduct_le->text().toFloat();
        m_Ro = ui.density_le->text().toFloat();

        m_Model = ui.conductModel_cb->currentIndex();
        if (m_Model == -1) // в случае когда в списке слоев не выбран слой
            m_Model = 0;

        m_T = ui.temperature_le->text().toFloat();

        m_j = ui.outsideCur_chb->isChecked();
        m_sv = ui.outSource_chb->isChecked();

        m_Gaz = ui.typePP_cb->currentIndex();

        m_A = ui.molarWeight_le->text().toFloat();
        m_NAt = ui.atomMolecule_le->text().toFloat();
        m_Z = ui.nuclearCharge_le->text().toFloat();
        m_Evtor = ui.ionCost_le->text().toFloat();
        m_NTr = ui.trapConc_le->text().toFloat();
    }
    else
    {
        userChange = false;
        ui.fileName_le->setText(FileName);
        //ui.comment_le->setText(m_Comment);
        ui.layCount_la->setText(m_nLayer);
		//ui.layInfo_tv->setCurrentIndex(LayInfoM->index(m_List, 0));

        ui.layName_le->setText(m_Name);
        ui.layNumber_le->setText(QString("%1").arg(m_Num));
        ui.material_cb->setCurrentIndex(m_Type);
        ui.mratName_le->setText(m_mratName);
        ui.composName_le->setText(m_composName);

        ui.dielectricConst_le->setText(QString("%1").arg(m_Eps));
        ui.magneticConst_le->setText(QString("%1").arg(m_Mu));
        ui.conduct_le->setText(QString("%1").arg(m_Sig));
        ui.density_le->setText(QString("%1").arg(m_Ro));
        ui.conductModel_cb->setCurrentIndex(m_Model);

        ui.temperature_le->setText(QString("%1").arg(m_T));

        ui.outsideCur_chb->setChecked(m_j);
        ui.outSource_chb->setChecked(m_sv);

        ui.typePP_cb->setCurrentIndex(m_Gaz);

        ui.molarWeight_le->setText(QString("%1").arg(m_A));
        ui.atomMolecule_le->setText(QString("%1").arg(m_NAt));
        ui.nuclearCharge_le->setText(QString("%1").arg(m_Z));
        ui.ionCost_le->setText(QString("%1").arg(m_Evtor));
        ui.trapConc_le->setText(QString("%1").arg(m_NTr));
        userChange = true;
    }
}

////////////////////////////////////////////////////////////////////////
//Установка необходимых путей для запуска расчета сечений
//
void Layers::init_paths()
{
	QString appPath = qApp->applicationDirPath();
	QDir appDir(appPath);
	appDir.cdUp();
	appPath = appDir.path();
	p.execDir = appPath;
	p.mat = appPath + "/.." + "/share/scripts/mat_files";
	p.par = appPath + +"/.." "/share/scripts/xrb_parameters.ini";
	p.home = appPath + +"/.." "/share/scripts";
	p.proj = Path;
	p.rmp = Path + "/" + FileName.replace("LAY", "LTB");
	p.lay = layersName;
	p.tab = Path + "/pechs/materials";
}

////////////////////////////////////////////////////////////////////////
// Чтение входного файла в переменные диалога
// Возвращаемый параметр i - номер строки, где произошла ошибка чтения
// i=0   - Данные прочитаны успешно
// i=-1  - Ошибка чтения обработана данной подпрограммой
// 
int  Layers::ReadFile(QString name)
{
    char szBuffer[150];
    char buf[100];
    int i = 0, j, k;
    double  tmp;

    fstream file;
    file.open(name.toStdString(), ios::in | ios::_Nocreate);	// открываем
    if (file.fail())
    {
        QMessageBox::warning(this, tr("Manager"),
            QString::fromLocal8Bit("Не удалось открыть файл c описанием слоев %1\nв папке %2")
            .arg(FileName)
            .arg(Path));
       // AfxMessageBox("Не удалось открыть файл c описанием слоев " + FileName + "\nв папке " + Path);
        return -1;
    }

    for (j = 0; j < N_LAYERS_MAX; j++)
    {
        Lay[j].exist = 0;                        //задается не существование слоя
        Lay[j].mratExist = 0;                   //задается не существование mrat-файла в библиотеке
    }
    ////////////////////////////////////////////////////////////////////////
    //  Чтение шапки файла
    file.getline(szBuffer, 255); i++; if (file.fail()) return i;	// чтение строки-комментария к файлу
    COMMENT = QString::fromUtf8(szBuffer);
    file.getline(szBuffer, 255); i++; if (file.fail()) return i;	//пропуск строки
    file >> nLayer;                   if (file.fail()) return i;	//число слоев
    file.getline(szBuffer, 255); i++; if (file.fail()) return i;	//до конца строки

    if (nLayer > N_LAYERS_MAX)
    {
        QMessageBox::warning(this, tr("Manager"),
            QString::fromLocal8Bit("Слишком много слоев \nв файле c описанием слоев %1\nв папке %2")
            .arg(FileName)
            .arg(Path));
        return -1;
    }

    ////////////////////////////////////////////////////////////////////////
    //  Чтение каждого слоя
    for (j = 0; j < nLayer; j++)
    {
        file.getline(szBuffer, 255); i++; if (file.fail()) return i;
        file.getline(szBuffer, 255); i++; if (file.fail()) return i;		//пропуск 2 строк
        file >> k;                        if (file.fail()) return i;	// номер слоя
        file >> buf;                      if (file.fail()) return i;	// имя слоя в буфер
        file.getline(szBuffer, 255); i++; if (file.fail()) return i;		//до конца строки
		Lay[k].number = k;						 // задается номер слоя
        Lay[k].exist = 1;                        //задается существование слоя
        Lay[k].name = buf;                       //записывается имя слоя в массив
// индикаторы
        file.getline(szBuffer, 255); i++;   if (file.fail()) return i;		//пропуск строки
        file >> Lay[k].gaz;                 if (file.fail()) return i;  // (газ/не газ)
        if (Lay[k].gaz <= 1) Lay[k].gaz = 0;
        if (Lay[k].gaz > 1)  Lay[k].gaz -= 1;
        file >> Lay[k].model;               if (file.fail()) return i;	// модель проводимости 
        file >> Lay[k].j;		            if (file.fail()) return i;	// сторонний ток
        file >> Lay[k].sv;                  if (file.fail()) return i;	// сторонний ист. вторичных 
        file >> Lay[k].dop;                 if (file.fail()) return i;	// дополнительные параметры
        file.getline(szBuffer, 255); i++;   if (file.fail()) return i;		//до конца строки
// основные параметры
        file.getline(szBuffer, 255); i++; if (file.fail()) return i;		//пропуск строки
        file >> tmp;			          if (file.fail()) return i;	// давление
        Lay[k].mat = 1;											// материал
        if ((tmp >= MatNum[0]) && (tmp <= MatNum[nMat - 1])) Lay[k].mat = int(tmp);
        file >> Lay[k].ro;          if (file.fail()) return i;	// плотность
        file >> Lay[k].t;           if (file.fail()) return i;	// температура
        file >> Lay[k].eps;         if (file.fail()) return i;	// диэлектрическая проницаемость
        file >> Lay[k].mu;          if (file.fail()) return i;	// магнитная проницаемость
        file >> Lay[k].sig;         if (file.fail()) return i;	// проводимость 
        file.getline(szBuffer, 255); i++; if (file.fail()) return i;		//до конца строки
// дополнительные параметры
        if (Lay[k].dop == 1)
        {
            file.getline(szBuffer, 255); i++; if (file.fail()) return i;		//пропуск строки
            file >> Lay[k].A;           if (file.fail()) return i;	// молекулярный вес
            file >> Lay[k].NAt;         if (file.fail()) return i;	// кол-во атомов в молекуле
            file >> Lay[k].Z;           if (file.fail()) return i;	// средний заряд ядра
            file >> Lay[k].Evtor;       if (file.fail()) return i;	// цена ионизации
            if (Lay[k].gaz > 0)
            {
                file >> Lay[k].NTr;     if (file.fail()) return i;	// концентрация ловушек
            }
            file.getline(szBuffer, 255); i++; if (file.fail()) return i;		//до конца строки
        }
    };
    file.close();   // закрываем

////////////////////////////////////////////////////////////////////////
//  Определение материала по модели проводимости
//  Присвоение значений по умолчанию
    for (k = 0; k < N_MODEL_MAX; k++)
    {
        if (Lay[k].exist == 1)
        {
            for (j = 0; j < nMat; j++)
                for (i = 0; i < MatSigN[j]; i++)
                    if (MatSigNum[j][i] == Lay[k].model)Lay[k].mat = MatNum[j];

            int i_Mat = 0;			// Номер материала в массиве
            for (i = 0; i < nMat; i++)
                if (MatNum[i] == Lay[k].mat) i_Mat = i;

            //  Присвоение значений по умолчанию
            if (MatT[i_Mat] == 0)		Lay[k].t = Const_T;
            if (MatJ[i_Mat] == 0)    {Lay[k].j = 0; Lay[k].sv = 0; }
            if (MatPP[i_Mat] == 0)		Lay[k].gaz = 0;
            if (MatDop[i_Mat] == 0) { Lay[k].A = Const_A;  Lay[k].Z = Const_Z;  Lay[k].NAt = Const_NAt; }
            if (MatEion[i_Mat] == 0)		Lay[k].Evtor = Const_Evtor;
            if (MatCt[i_Mat] == 0)		Lay[k].NTr = Const_NTr;
        }
    }

    ReadLayers(); // Читаем файл layers
    CheckMrat();
    return  0;
}

////////////////////////////////////////////////////////////////////////
// Запись переменных диалога в файл
// Возвращаемый параметр i :
// i=0   - Данные записаны успешно
// i=-1  - Ошибка записи
// 
int  Layers::WriteFile(QString name)
{
	m_Comment = ui.comment_le->text();
    int i;

    fstream file;
    file.open(name.toStdString(), ios::out); if (file.fail()) return -1;// открываем

////////////////////////////////////////////////////////////////////////
//  Запись шапки файла
    file << m_Comment.toStdString();                 file << "\n"; // строка-комментарий
    //file << "<Количество слоев>";    file << "\n";
    file << "<Number of layers>";    file << "\n";
    file << nLayer;            file << "\n"; //число слоев

////////////////////////////////////////////////////////////////////////
//  Запись всех слоев
    for (i = 0; i <= N_LAYERS_MAX; i++)
    {
        if (Lay[i].exist == 1) // только для существующих слоев
        {
            file << "\n";
            //file << "<Номер, название слоя>"; file << "\n";
            file << "<Number, layer name>"; file << "\n";
            file << i;                        file << "   ";	// номер слоя
            file << Lay[i].name.toStdString();        file << "\n";		// название слоя
        // индикаторы
            //file << "п/п №, мод. провод. №, стор.ток(0-нет,1-есть), стор.ист.втор.эл.(0-нет,1-да), доп.параметры(0-нет,1-да)>";    file << "\n";
            file << "<p/p N, conduct. N, ext. J (0-no,1-yes), ext. sec. el. (0-no,1-yes), extra opt. (0-no,1-yes)>";    file << "\n";
            //if (pDlg->Lay[i].gaz > 0) pDlg->Lay[i].gaz += 1;
            file << Lay[i].gaz + 1;       file << "     "; // газ
            file << Lay[i].model;       file << "     "; // модель проводимости
            file << Lay[i].j;			  file << "     "; // сторонний ток
            file << Lay[i].sv;          file << "     "; // сторонний ист. вторичных 
            file << Lay[i].dop;         file << "\n";    // дополнительные параметры
        // основные параметры
            //file << "<давление в слое(атм.), плотн.(г/см3), темп.[Цельсий], диэл.прон., магн. прон., проводимость>";    file << "\n";
            file << "<material(N), density (g/cm**3), temp.[C], diel. const, magnetic perm., conductivity>";    file << "\n";
            file << Lay[i].mat;		  file << "     ";  // материал
            file << Lay[i].ro;          file << "     ";  // плотность
            file << Lay[i].t;           file << "     ";  // температура
            file << Lay[i].eps;         file << "     ";  // диэлектрическая проницаемость 
            file << Lay[i].mu;          file << "     ";  // магнитная проницаемость
            file << Lay[i].sig;         file << "\n";     // проводимость
        // дополнительные параметры
            if (Lay[i].dop == 1)
            {
                if (Lay[i].gaz > 0)
                {
                    //file << "<молекулярный вес[г/моль], кол-во атомов в мол.[шт.], средний заряд ядра[эл.], цена ион.[эВ], конц. ловушек[1/см3]>";    file << "\n";
                    file << "<molecular weight[g/mole], atoms in molecule[], average core charge[el.], ionization cost[eV], conc. of traps[1/cm**3]>";    file << "\n";
                }
                else
                {
                    //file << "<молекулярный вес[г/моль], кол-во атомов в мол.[шт.], средний заряд ядра[эл.], цена ион.[эВ]>";    file << "\n";
                    file << "<molecular weight[g/mole], atoms in molecule[], average core charge[el.], ionization cost[eV]>";    file << "\n";
                }
                file << Lay[i].A;           file << "     ";  // молекулярный вес
                file << Lay[i].NAt;         file << "     ";  // кол-во атомов в молекуле
                file << Lay[i].Z;           file << "     ";  // средний заряд ядра
                if (Lay[i].gaz > 0)
                {
                    file << Lay[i].Evtor;   file << "     ";  // цена ионизации
                    file << Lay[i].NTr;     file << "\n";     // концентрация ловушек
                }
                else
                {
                    file << Lay[i].Evtor;   file << "\n";     // цена ионизации
                }
            }
            if (file.fail()) return -1;
        }
    }

    file << "\n";
    file.close();// закрываем

    ////////////////////////////////////////////////////////////////////////
    QString name2 = name;
    name2.replace(".LAY", ".LTB");
    name2.toUpper();
    file.open(name2.toStdString(), ios::out | ios::trunc); if (file.fail()) return -1;// открываем
    ////////////////////////////////////////////////////////////////////////
    //  Запись всех слоев
    for (i = 0; i <= N_LAYERS_MAX; i++)
    {
        if (Lay[i].exist == 1) // только для существующих слоев
        {
            file << i;                        file << "\t";	// номер слоя
            file << Lay[i].name.toStdString();        file << "\t"; 	// название слоя
            file << Lay[i].ro;          file << "\n";    // плотность
            if (file.fail()) return -1;
        }
    }
    file.close();// закрываем

    return	0;
}

/////////////////////////////////////////////////////////////////////////////
// Создаем файл с композитом *.mrat
//
void Layers::OnCreateMrat()
{
    ui.mratName_le->setText("MATERIAL");
    ui.composName_le->setText("MATERIAL");
    vElement.clear();
    vElPortion.clear();
    HimSostM->populateData(vElement, vElPortion);
    Change = TRUE;
	MratChange = true;
}

/////////////////////////////////////////////////////////////////////////////
// выбор хим. элемента из таблицы
//
int Layers::OnChangeElem()
{
    int row; // строка
    int column; // столбец
    int k;
    row = ui.himSost_tv->currentIndex().row();
    column = ui.himSost_tv->currentIndex().column();
    if (column != 0)
        return 0;
    PeriodicTable dlg;
    dlg.exec();
    vElement[row] = dlg.element;
    HimSostM->populateData(vElement, vElPortion);
	MratChange = true;
    if (dlg.element == "NONE")
        return -1;
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Добавление хим. элемента
//
void Layers::OnAddElem()
{
	vElement.push_back("NONE");
	vElPortion.push_back(0);
	HimSostM->populateData(vElement, vElPortion);
	QModelIndex currentIndex = HimSostM->index(vElement.size() - 1, 0);
	ui.himSost_tv->setCurrentIndex(currentIndex);
    if (OnChangeElem() == -1) {
        vElement.removeLast();
        vElPortion.removeLast();
        HimSostM->populateData(vElement, vElPortion);
    }
	MratChange = true;
}

/////////////////////////////////////////////////////////////////////////////
// Удаление хим. элемента
//
void Layers::OnDeleteElem()
{
	int row; // строка
	int column; // столбец
	int k;
	row = ui.himSost_tv->currentIndex().row();
	column = ui.himSost_tv->currentIndex().column();
	if (column != 0)
		return;
	vElement.remove(row);
	vElPortion.remove(row);
	HimSostM->populateData(vElement, vElPortion);
	MratChange = true;
}

/////////////////////////////////////////////////////////////////////////////
// Сохранить mrat-файл
//
int Layers::OnSaveMrat()
{
	// Проверка суммы долей хим. элементов
	
	double sum = 0;
	for (int i = 0; i < vElPortion.size(); i++)
	{
		sum += vElPortion[i];
	}
	if (abs(1 - sum) > 0.00000001)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("Композиты"), QString::fromLocal8Bit("Сумма массовых долей хим. элементов в композите не равна 1!"), QMessageBox::Ok); return -1;
	}

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Сохранить mrat-файл с плотностью [%1] ?").arg(ui.density_le->text()),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return -1;



	m_mratName = ui.mratName_le->text();
	QString FileName = m_mratName;
	QDir::setCurrent("C:/Program Files/REMP/share/scripts/mat_files");
	QString FullName = QFileDialog::getSaveFileName(this, tr("Save File"), FileName, tr("mrat (*.mrat)")); // Выбор места сохранения и имени файла

	if (FullName.isEmpty() || FullName.isNull()) { // при отмене диалога сохранения
		return -1;
	}
	if (WriteMrat(FullName) == 0)
		MratChange = false;
    OnInList();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// При изменении параметров слоя
//
void Layers::OnChange()
{
	if(userChange)
        Change = true;
}

/////////////////////////////////////////////////////////////////////////////
// При изменении mrat-файла
//
void Layers::OnChangeMrat()
{
	if (userChange)
		MratChange = true;
}

/////////////////////////////////////////////////////////////////////////////
// Выбор новой модели проводимости
//
void Layers::OnSelchangeComboModel()
{
    if(userChange)
	{
		if (ui.material_cb->currentIndex() == 3)
		{
			int i;
			m_Sig_Num = m_ModelNum[ui.conductModel_cb->currentIndex()];
			i_Sig = 0;			// Номер модели проводимости в массиве
			for (i = 1; i < nModel; i++)
				if (ModelNum[i] == m_Sig_Num) i_Sig = i;

			if (!((ui.conductModel_cb->currentIndex() == 3) || (ui.conductModel_cb->currentIndex() == 2)))		// модель полупроводника
			{
				ui.typePP_cb->hide();
				ui.typePP_lb->hide();
				m_NTr = MatCt[i_Mat];
				ui.trapConc_le->setText(QString("%1").arg(m_NTr));
			}
			else
			{
				ui.typePP_cb->show();
				ui.typePP_lb->show();
				ui.typePP_cb->setCurrentIndex(m_Gaz);
				m_NTr = PPCT[m_Gaz];
				ui.trapConc_le->setText(QString("%1").arg(m_NTr));

			}
		}

    Change = TRUE;}
}

/////////////////////////////////////////////////////////////////////////////
// Выбор новой модели полупроводника
//
void Layers::OnSelchangePPModel()
{
	if (userChange) {
		m_Gaz = ui.typePP_cb->currentIndex();
		m_NTr = PPCT[m_Gaz];
		m_Evtor = PPION[m_Gaz];
		ui.trapConc_le->setText(QString("%1").arg(m_NTr));
		ui.ionCost_le->setText(QString("%1").arg(m_Evtor));

		Change = TRUE;
	}

}

/////////////////////////////////////////////////////////////////////////////
// Выбор нового материала
//
void Layers::OnSelchangeMat()
{
    if(userChange) {
    int i;
    DataUpdate(TRUE);
    m_Type = ui.material_cb->currentIndex();
    m_Mat = m_Type + 1;					// материал
    i_Mat = 0;			// Номер материала в массиве
    for (i = 0; i < nMat; i++)
        if (MatNum[i] == m_Mat) i_Mat = i;

    PrepareFields();

    m_Eps = MatEPS[i_Mat];
    m_Mu = MatMU[i_Mat];
    m_Sig = MatSIGMA[i_Mat];
    m_Ro = MatRo[i_Mat];
    m_T = Const_T;
    m_j = FALSE;
    m_Gaz = 0;
    m_sv = FALSE;
    m_A = Const_A;
    m_NAt = Const_NAt;
    m_Z = Const_Z;
    m_Evtor = MatEion[i_Mat];
    m_NTr = MatCt[i_Mat];

    DataUpdate(FALSE);	// Запись данных на экран
    Change = TRUE; }
}

/////////////////////////////////////////////////////////////////////////////
// Выбор нового слоя в списке
//
void Layers::OnSelchangeList()
{
	if (userChange) {
		
		if (MratChange) // Если менялся mrat-файл
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Сохранить изменение файла с описанием хим. состава?"),
				QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes)
			{
				if (OnSaveMrat() == -1)
				{
					userChange = false;
					ui.layInfo_tv->setCurrentIndex(LayInfoM->index(m_List, 0));
					userChange = true;
					return;
				}
			}
			else
				MratChange = false;
		}
		QString temp;
		m_List = ui.layInfo_tv->currentIndex().row();
		if (m_List == -1) // в случае когда в списке слоев не выбран слой 
			m_List = 0;
		if (Change)	// Если что-то изменялось
		{
			m_Num = ui.layNumber_le->text().toInt();
			if ((m_Num > N_LAYERS_MAX) || (m_Num < 0)) {
				QMessageBox::information(this, "Layers", QString::fromLocal8Bit("Неправильный номер слоя"), QMessageBox::Ok);
				
				return;
			};
			// При неправильном номере ничего не делаем
			DataUpdate(TRUE);	// Чтение данных с экрана
			//temp = QString("%1").arg( m_Num);
			
			if (Lay[m_Num].exist == 1)	// для существующего слоя
				temp = QString::fromLocal8Bit("Внести изменения для слоя №%1 [%2]?").arg(m_Num).arg(m_Name);
			else					// для нового слоя
				temp = QString::fromLocal8Bit("Включить в список слой №%1 [%2]?").arg(m_Num).arg(m_Name);


			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, " ", temp,
				QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes)
				OnInList();
            
			//if (AfxMessageBox(temp, MB_YESNO) == IDYES) OnInList(); // записываем
		}

		
		PrepareData();	    // Обновление данных на экран
		DataUpdate(FALSE);	// Запись данных на экран
		Change = FALSE;		// Не менялся
	}
}

/////////////////////////////////////////////////////////////////////////////
// Редактирование в ячейках таблицы с хим. элементами
//
void Layers::OnEditCell()
{
    if (userChange)
    {
        int row; // строка
        int column; // столбец
        int k;
        row = ui.himSost_tv->currentIndex().row();
        column = ui.himSost_tv->currentIndex().column();
        if (column == 0)
            return;
        /*QModelIndex index = ui.himSost_tv->model()->index(row, column);
        ui.himSost_tv->setCurrentIndex(index);*/
		vElPortion = HimSostM->lm_ElPortion;
		MratChange = true;

    }
}

/////////////////////////////////////////////////////////////////////////////
// Открываем файл с композитом *.mrat
//
void Layers::OnOpenMrat()
{
    // Диалог для выбора файла *.mrat
    QFileDialog  dialog;
    QString MFileName;
    QString MPath;
    dialog.setDirectory("C:/Program Files/REMP/share/scripts/mat_files");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("MRAT files (*.mrat)"));
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec() == QDialog::Accepted)
    {
        MFileName = QFileInfo(dialog.selectedFiles()[0]).fileName();
        MPath = QFileInfo(dialog.selectedFiles()[0]).path();
    }
    else
    {
        return;
    }

    QDir::setCurrent(MPath);
    if (ReadMrat(MPath + '/' + MFileName) == -1)
        return;
    ui.mratName_le->setText(MFileName.remove(".mrat"));
    ui.composName_le->setText(m_composName);
	//ui.density_le->setText(QString("%1").arg(m_Ro));
	MratChange = false;
    Change = TRUE;
	//OnInList();
}

/////////////////////////////////////////////////////////////////////////////
// Открываем файл с композитом *.mrat из таблицы
//
void Layers::OnOpenMratTable()
{
	int row; // строка
	int column; // столбец
	int k;
	row = ui.layInfo_tv->currentIndex().row();
	column = ui.layInfo_tv->currentIndex().column();
	if (column != 2)
		return;
	if (row != m_List)
		OnSelchangeList();
	OnOpenMrat();
    OnInList();
}



/////////////////////////////////////////////////////////////////////////////
// Устанавливаем плотность из mrat - файла
//
void Layers::OnGetRoFromMrat()
{
    ui.density_le->setText(QString("%1").arg(RoMrat));
    OnInList();
}


/////////////////////////////////////////////////////////////////////////////
// Выход без сохранения данных
//
void Layers::OnCancel()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Выйти без сохранения?"),
        QMessageBox::Yes | QMessageBox::No);
    if (/*(Demo == DEMOCONST)*/ (reply == QMessageBox::No)) return;
    // если нет, то возвращаемся в диалог
   // OnCancel();	// если да - заканчиваем работу
    exit = true;
    this->close();
}

/////////////////////////////////////////////////////////////////////////////
// Добавить слой в список слоев
//
void Layers::OnAddLayer()
{
    int layNumber = ui.layNumber_le->text().toInt();
   
        if (Lay[layNumber].exist == 1)			// которые существуют,
        {
                QMessageBox::warning(this, tr("Manager"),
                    QString::fromLocal8Bit("Невозможно добавить! Слой с таким номером уже существует!"));
                return;
        }
    OnInList();
}

/////////////////////////////////////////////////////////////////////////////
// Выход с сохранением данных
//
void Layers::OnOK()
{
   
    OnSelchangeList();

    /////////////////////////////////////////////////////////////////////////////
    // Чтение имени выходного файла из диалога открытия файла


    FullName = QFileDialog::getSaveFileName(this, tr("Save File"),FileName, tr("LAY (*.LAY)"));
    if (FullName.size() == 0) return;
	int fn = FullName.lastIndexOf('/');
	FileName = FullName.right(FullName.size() - fn - 1);
    /////////////////////////////////////////////////////////////////////////////
    // Запись результатов в файл
    if (WriteFile(FullName) == 0)	// удачная
    {
        QMessageBox::information(this, "Layers", QString::fromLocal8Bit("Данные успешно сохранены в файл %1\n в папке %2").arg(FileName).arg(FullName.left(fn)), QMessageBox::Ok);
        fstream file;
        file.open(LAYTMPNAME, ios::out);
        file << FileName.toStdString();  file << "\n";
        file.close();
    }
       
	WriteLayers(); // Сохраняем файл layers

	// Расчет сечений(распределений)
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Запустить расчет сечений?"),
		QMessageBox::Yes | QMessageBox::No);
	
	if (reply == QMessageBox::Yes)
	{

        //if (CheckPLFile()) //Соответствие PL и LAY файлов
        //    GetFinalSections(); // Запуск расчета итоговых сечений
        //else 
        //{
        //    QMessageBox::warning(this, tr("Manager"),
        //        QString::fromLocal8Bit("Файлы с описанием слоев(LAY) и описанием взаимодействия частиц со слоями(PL) не соответствуют(разное кол-во слоев, разные номера слоев)!\nИтоговые сечения не рассчитаны!"));
        //    return;
        //}

        PythonBinds::start_interpreteter();
        PythonBinds::initiateInterpritater(p.home.toStdString());

		int status_calc_distr = PythonBinds::calcDistribution(p); // Запуск расчета базовых сечений
        if (!status_calc_distr)
		    int status_get_distr = PythonBinds::getDistribution(p); // Запуск расчета базовых сечений
	}

    PythonBinds::finalize_interpreteter();
    ok = true;
    this->close();	//конец работы
}

void Layers::closeEvent(QCloseEvent* event)
{
    if (!(ok || exit)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, " ", QString::fromLocal8Bit("Вы уверены, что хотите завершить работу без сохранения результатов?"),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            event->ignore();
        if (reply == QMessageBox::Yes)
            event->accept();
    }
    if (ok || exit)
        event->accept();
}



