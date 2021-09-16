#ifndef LAYERS
#define LAYERS

#include <QtWidgets/QMainWindow>
#include "ui_Layers.h"
#include "const.h"
#include "QCloseEvent"
#include "QAbstractTableModel"
#include "MratModel.h"
#include "LayListModel.h"
#include "Paths.h"
namespace Ui {
	class Layers;
}


struct Layer          // Слой:
{
	QString  name;    // название
	QString  mratName;// имя mrat-файла с описанием хим. состава
	int		mratExist;// наличие mrat-файла в библиотеке 
	int		 number;  // номер слоя
	int      exist;   // существование  (0-нет, 1-есть)


// индикаторы
	int      gaz;     // газ                        (0-газ, 1-не газ, 2-5 -диэл.мод.)	
	int      model;   // модель проводимости        (0-нет, 1-п/п таб., 2-п/п фор., 3-диэл. и т.д.)
	int      j;		  // сторонний ток              (0-нет, 1-есть)
	int      sv;      // сторонний ист. вторичных	(0-нет, 1-есть)
	int      dop;     // дополнительные параметры	(0-нет, 1-есть)
	int      mat;     // материал					(>0 - старое давление, -1 - проводник, -2 - диэлектрик и т.д.)

// основные параметры
	double   ro;      // плотность                      [г/см3]
	double   t;       // температура                    [Цельсий]
	double   eps;     // диэлектрическая проницаемость  [SGS]
	double   mu;      // магнитная проницаемость        [SGS]
	double   sig;     // проводимость                   [SGS]

// дополнительные параметры
	double   A;       // молекулярный вес       [г/моль]
	double   NAt;     // кол-во атомов в мол.   [шт.]
	double   Z;       // средний заряд ядра     [эл.]
	double   Evtor;   // цена ионизации         [эВ]
	double   NTr;     // концентрация ловушек   [1/см3]
};

class Layers : public QMainWindow
{
    Q_OBJECT

public:
    Layers(QWidget *parent = Q_NULLPTR);

private:
    Ui::LayersClass ui;
	void init_paths();			//Установка необходимых путей для запуска расчета сечений

public:
	LayListModel *LayInfoM;		// создаем модель для связи данных с инф. о слоях с таблицей вывода
	MratModel *HimSostM;	// создаем модель для связи данных с инф. о хим. составе слоев с таблицей вывода
	QString FileName;			// Имя файла *.LAY
	QString Path;				// Путь папки прокета
	QString FullName;			// Полное имя файла (FullName=Path+FileName)
	QString COMMENT;			// Строка-комментарий к файлу
	QString layersName;			// Полное имя файла layers
	bool pechsExist = false;	// Наличие проекта переноса
	bool layersExist = true;	// Наличие файла layers
	bool ok = false;			// Нажата кнопка "Ok"
	bool exit = false;			// Нажата кнопка "Выход"
	Paths p;					// Необходимые пути для запуска расчета сечений

public:
	int  ReadFile(QString name);	// Чтение из файла
	int  WriteFile(QString name);	// Запись в файла *.LAY
	int  ReadManagerIni();          // Чтение файла <Manager_ini>
	void ReadLayers();				// Чтение файла layers
	int ReadPL(QString filename);	// Чтение файла PL
	int ReadMrat(QString filename);	// Чтение mrat-файла
	int WriteMrat(QString filename);				// Запись mrat-файла
	void WriteLayers();				// Запись(создание) файла layers

public:
	int nModel;							// Количество моделей проводимости
	QString Model[N_MODEL_MAX];			// Массив заголовков моделей проводимости
	QString ModelShort[N_MODEL_MAX];    // Массив коротких заголовков моделей проводимости
	int     ModelNum[N_MODEL_MAX];		// Номера моделей проводимости
	int     i_Sig; 						// Номер модели проводимости в массиве

	int nPP;							// Количество моделей полупроводников
	int i_nPP;							// Номер модели полупроводника в массиве
	QString PPName[N_MODEL_MAX];		// Массив названий полупроводников
	int     PPNum[N_MODEL_MAX];			// Номера полупроводников
	double PPCT[N_MODEL_MAX];			// Концентрация ловушек полупроводников
	double PPION[N_MODEL_MAX];			// Цена ионизации полупроводника

	int nMat;							// Количество материалов
	double  MatRo[N_MODEL_MAX];			// Плотность материалов
	double  MatSIGMA[N_MODEL_MAX];		// Проводимость материалов
	double  MatEPS[N_MODEL_MAX];		// Диэлектрическая проницаемость материалов
	double  MatMU[N_MODEL_MAX];			// Магнитная проницаемость материалов
	QString MatName[N_MODEL_MAX];		// Массив названий материалов
	int     MatNum[N_MODEL_MAX];		// Номера материалов
	int		i_Mat;						// Номер материала в массиве
	int     MatSigN[N_MODEL_MAX];		// Количество моделей проводимости для материала
	int     MatSigNum[N_MODEL_MAX][N_MODEL_MAX]; //Номера моделей проводимости для материала
	//*.PL файл
	int layNumPL;						// кол-во слоев в файле PL
	QVector<int> layersPL;				// номера слоев в файле PL
	// Поля для материала (0-нет, 1-есть)
	int     MatT[N_MODEL_MAX];	   // температура
	int     MatJ[N_MODEL_MAX];	   // стронний ток и источник вторичных электронов
	int     MatPP[N_MODEL_MAX];	   // модель полупроводника
	int     MatDop[N_MODEL_MAX];	   // молекулярный вес, кол - во атомов в мол., средний заряд ядра
	double     MatEion[N_MODEL_MAX];	   // цена ионизации
	double     MatCt[N_MODEL_MAX];	   // концентрация ловушек

	int nLayer;						 	   // Количество слоев
	Layer Lay[N_LAYERS_MAX + 1];			   // Массив слоев
	int  INDEX[N_LAYERS_MAX + 1];			   // Индексный массив 
	QVector<Layer> vLay;				// Вектор слоев для выывода
	// Таблица со списком слоев
	QVector<QString> vName;				// имя слоя
	QVector<int>  vNumber;				// номер слоя
	QVector<double>  vEps;				// диэлектрическая проницаемость
	QVector<double>  vMu;				// магнитная проницаемость
	QVector<double>  vSig;				// проводимость
	QVector<double>  vRo;				// плотность
	QVector<QString> vModelShort;		// Вектор коротоких заголовков моделей для имеющихся слоев
	QVector<QString> vMratName;			// Имена mrat-файлов
	QVector<int> vMratExist;			// Наличие mrat-файла в библиотеке

	//Таблица хим. состава
	double RoMrat;						// Плотность из mrat-файла
	QVector<QString> vElement;			//  Название хим. элемента
	QVector<double> vElPortion;			//  Доля хим. элемента
	 //( INDEX[i] - номер слоя в массиве Lay, имеющего в списке на экране номер i)
	bool Change = false;						   // True - описание слоя менялось, False-нет
	bool MratChange = false;				// True - описанима mrat-файла менялось, FAlse - нет
	int Demo;							   // Пароль для запуска Demo-версии
public:
	void DataUpdate(bool a); // Обмен данными с итерфейсом (true - записываем с экрана, false - записываем на экран)
	void InitData(); //	Подготовка исходных данных
	void PrepareList();  // Подготовка списка на экран
	void PrepareData();  // Подготовка остальных данных на экран
	void PrepareSig();   // Подготовка моделей проводимости на экран
	void PrepareFields();// Подготовка видимости полей
	void CheckMrat();	// Проверка на наличие mrat-файлов в библиотеке
	bool CheckPLFile(); // Проверка совместимости PL-файла 
	void GetBasicSections(); // Расчет базовых сечений
	void GetFinalSections(); // Расчет итоговых сечений
	void closeEvent(QCloseEvent* event);
// Dialog Data

	QString	m_FileName;		// Имя выходного файла на экране
	QString	m_Comment;		// Строка-комментарий к файлу на экране	
	QString	m_nLayer;		// Количество слоев на экране (строка, соответствующая nLayer)
	QString m_mratName;		// Имя mrat-файла на экране
	QString m_composName;	// Имя композита на экране 
	int			m_List;		// Номер выбранного пункта в списке слоев
	QString	m_Name;				// Имя текущего слоя
	int		m_Num;				// Номер текущего слоя в массиве Layer
	int		m_Type;				// Номер материала в массиве
	int		m_Mat;			// Номер материала
	int		m_Gaz;			// Номер п/п (0-газ, 1-не газ, >1 - п/п)
	double	m_Eps;			// диэлектрическая проницаемость
	double	m_Mu;			// магнитная проницаемость
	double	m_Sig;			// проводимость
	int			m_Model;		// Номер выбранного пункта в списке моделей проводимости
	int			m_ModelNum[N_MODEL_MAX];    // Номера моделей проводимости в списке на экране
	int			m_Sig_Num;		// Номер модели проводимости
	double	m_Ro;			// плотность
	double	m_T;			// температура
	int	m_j;				// сторонний ток			(0-нет, 1-есть)
	int	m_sv;				// сторонний ист. вторичных (0-нет, 1-есть)
	double	m_A;			// молекулярный вес
	double	m_NAt;			// кол-во атомов в молекуле
	double	m_Z;			// средний заряд ядра
	double	m_Evtor;		// цена ионизации
	double	m_NTr;	    	// концентрация ловушек

	bool LayInfoListCleared = false;
	bool CombBoxItedAdded = false;
	bool MatComboBoxAdded = false;
	bool PPComboBoxAdded = false;
	bool CommentAdded = false;
	bool userChange = true; // изменено пользователем

private slots:
	void OnOpenMrat();		// Открываем файл с композитом *.mrat
	void OnOpenMratTable();		// Открываем файл с композитом *.mrat из таблицы
	void OnGetRoFromMrat();		// Устанавливаем плотность из mrat-файла
	void OnCreateMrat();	// Создаем файл с композитом *.mrat
	int OnChangeElem();	// выбор хим. элемента из таблицы
	void OnAddElem();		// Добавление хим. элемента
	void OnDeleteElem();	// Удаление выбраного хим. элемента
	int OnSaveMrat();		// Сохранить mrat-файл
	void OnEditCell();
	void OnChange();        // Вызывается при изменении параметров слоя, добав/удал слоя
	void OnChangeMrat();	// Вызывается при изменении mrat-файла
	void OnSelchangeComboModel();
	void OnSelchangePPModel();
	void OnSelchangeMat();
	void OnSelchangeList();
	void OnOK();
	void OnCancel();
	void OnAddLayer();     // Добавить слой в список слоев
	void OnInList();     // Внести изменения в слой
	void OnEditLayer(); // Внести изменения в слой
	void OnDelete();	 // Исключение слоя из списка
};

#endif