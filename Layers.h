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


struct Layer          // ����:
{
	QString  name;    // ��������
	QString  mratName;// ��� mrat-����� � ��������� ���. �������
	int		mratExist;// ������� mrat-����� � ���������� 
	int		 number;  // ����� ����
	int      exist;   // �������������  (0-���, 1-����)


// ����������
	int      gaz;     // ���                        (0-���, 1-�� ���, 2-5 -����.���.)	
	int      model;   // ������ ������������        (0-���, 1-�/� ���., 2-�/� ���., 3-����. � �.�.)
	int      j;		  // ��������� ���              (0-���, 1-����)
	int      sv;      // ��������� ���. ���������	(0-���, 1-����)
	int      dop;     // �������������� ���������	(0-���, 1-����)
	int      mat;     // ��������					(>0 - ������ ��������, -1 - ���������, -2 - ���������� � �.�.)

// �������� ���������
	double   ro;      // ���������                      [�/��3]
	double   t;       // �����������                    [�������]
	double   eps;     // ��������������� �������������  [SGS]
	double   mu;      // ��������� �������������        [SGS]
	double   sig;     // ������������                   [SGS]

// �������������� ���������
	double   A;       // ������������ ���       [�/����]
	double   NAt;     // ���-�� ������ � ���.   [��.]
	double   Z;       // ������� ����� ����     [��.]
	double   Evtor;   // ���� ���������         [��]
	double   NTr;     // ������������ �������   [1/��3]
};

class Layers : public QMainWindow
{
    Q_OBJECT

public:
    Layers(QWidget *parent = Q_NULLPTR);

private:
    Ui::LayersClass ui;
	void init_paths();			//��������� ����������� ����� ��� ������� ������� �������

public:
	LayListModel *LayInfoM;		// ������� ������ ��� ����� ������ � ���. � ����� � �������� ������
	MratModel *HimSostM;	// ������� ������ ��� ����� ������ � ���. � ���. ������� ����� � �������� ������
	QString FileName;			// ��� ����� *.LAY
	QString Path;				// ���� ����� �������
	QString FullName;			// ������ ��� ����� (FullName=Path+FileName)
	QString COMMENT;			// ������-����������� � �����
	QString layersName;			// ������ ��� ����� layers
	bool pechsExist = false;	// ������� ������� ��������
	bool layersExist = true;	// ������� ����� layers
	bool ok = false;			// ������ ������ "Ok"
	bool exit = false;			// ������ ������ "�����"
	Paths p;					// ����������� ���� ��� ������� ������� �������

public:
	int  ReadFile(QString name);	// ������ �� �����
	int  WriteFile(QString name);	// ������ � ����� *.LAY
	int  ReadManagerIni();          // ������ ����� <Manager_ini>
	void ReadLayers();				// ������ ����� layers
	int ReadPL(QString filename);	// ������ ����� PL
	int ReadMrat(QString filename);	// ������ mrat-�����
	int WriteMrat(QString filename);				// ������ mrat-�����
	void WriteLayers();				// ������(��������) ����� layers

public:
	int nModel;							// ���������� ������� ������������
	QString Model[N_MODEL_MAX];			// ������ ���������� ������� ������������
	QString ModelShort[N_MODEL_MAX];    // ������ �������� ���������� ������� ������������
	int     ModelNum[N_MODEL_MAX];		// ������ ������� ������������
	int     i_Sig; 						// ����� ������ ������������ � �������

	int nPP;							// ���������� ������� ���������������
	int i_nPP;							// ����� ������ �������������� � �������
	QString PPName[N_MODEL_MAX];		// ������ �������� ���������������
	int     PPNum[N_MODEL_MAX];			// ������ ���������������
	double PPCT[N_MODEL_MAX];			// ������������ ������� ���������������
	double PPION[N_MODEL_MAX];			// ���� ��������� ��������������

	int nMat;							// ���������� ����������
	double  MatRo[N_MODEL_MAX];			// ��������� ����������
	double  MatSIGMA[N_MODEL_MAX];		// ������������ ����������
	double  MatEPS[N_MODEL_MAX];		// ��������������� ������������� ����������
	double  MatMU[N_MODEL_MAX];			// ��������� ������������� ����������
	QString MatName[N_MODEL_MAX];		// ������ �������� ����������
	int     MatNum[N_MODEL_MAX];		// ������ ����������
	int		i_Mat;						// ����� ��������� � �������
	int     MatSigN[N_MODEL_MAX];		// ���������� ������� ������������ ��� ���������
	int     MatSigNum[N_MODEL_MAX][N_MODEL_MAX]; //������ ������� ������������ ��� ���������
	//*.PL ����
	int layNumPL;						// ���-�� ����� � ����� PL
	QVector<int> layersPL;				// ������ ����� � ����� PL
	// ���� ��� ��������� (0-���, 1-����)
	int     MatT[N_MODEL_MAX];	   // �����������
	int     MatJ[N_MODEL_MAX];	   // �������� ��� � �������� ��������� ����������
	int     MatPP[N_MODEL_MAX];	   // ������ ��������������
	int     MatDop[N_MODEL_MAX];	   // ������������ ���, ��� - �� ������ � ���., ������� ����� ����
	double     MatEion[N_MODEL_MAX];	   // ���� ���������
	double     MatCt[N_MODEL_MAX];	   // ������������ �������

	int nLayer;						 	   // ���������� �����
	Layer Lay[N_LAYERS_MAX + 1];			   // ������ �����
	int  INDEX[N_LAYERS_MAX + 1];			   // ��������� ������ 
	QVector<Layer> vLay;				// ������ ����� ��� �������
	// ������� �� ������� �����
	QVector<QString> vName;				// ��� ����
	QVector<int>  vNumber;				// ����� ����
	QVector<double>  vEps;				// ��������������� �������������
	QVector<double>  vMu;				// ��������� �������������
	QVector<double>  vSig;				// ������������
	QVector<double>  vRo;				// ���������
	QVector<QString> vModelShort;		// ������ ��������� ���������� ������� ��� ��������� �����
	QVector<QString> vMratName;			// ����� mrat-������
	QVector<int> vMratExist;			// ������� mrat-����� � ����������

	//������� ���. �������
	double RoMrat;						// ��������� �� mrat-�����
	QVector<QString> vElement;			//  �������� ���. ��������
	QVector<double> vElPortion;			//  ���� ���. ��������
	 //( INDEX[i] - ����� ���� � ������� Lay, �������� � ������ �� ������ ����� i)
	bool Change = false;						   // True - �������� ���� ��������, False-���
	bool MratChange = false;				// True - ��������� mrat-����� ��������, FAlse - ���
	int Demo;							   // ������ ��� ������� Demo-������
public:
	void DataUpdate(bool a); // ����� ������� � ���������� (true - ���������� � ������, false - ���������� �� �����)
	void InitData(); //	���������� �������� ������
	void PrepareList();  // ���������� ������ �� �����
	void PrepareData();  // ���������� ��������� ������ �� �����
	void PrepareSig();   // ���������� ������� ������������ �� �����
	void PrepareFields();// ���������� ��������� �����
	void CheckMrat();	// �������� �� ������� mrat-������ � ����������
	bool CheckPLFile(); // �������� ������������� PL-����� 
	void GetBasicSections(); // ������ ������� �������
	void GetFinalSections(); // ������ �������� �������
	void closeEvent(QCloseEvent* event);
// Dialog Data

	QString	m_FileName;		// ��� ��������� ����� �� ������
	QString	m_Comment;		// ������-����������� � ����� �� ������	
	QString	m_nLayer;		// ���������� ����� �� ������ (������, ��������������� nLayer)
	QString m_mratName;		// ��� mrat-����� �� ������
	QString m_composName;	// ��� ��������� �� ������ 
	int			m_List;		// ����� ���������� ������ � ������ �����
	QString	m_Name;				// ��� �������� ����
	int		m_Num;				// ����� �������� ���� � ������� Layer
	int		m_Type;				// ����� ��������� � �������
	int		m_Mat;			// ����� ���������
	int		m_Gaz;			// ����� �/� (0-���, 1-�� ���, >1 - �/�)
	double	m_Eps;			// ��������������� �������������
	double	m_Mu;			// ��������� �������������
	double	m_Sig;			// ������������
	int			m_Model;		// ����� ���������� ������ � ������ ������� ������������
	int			m_ModelNum[N_MODEL_MAX];    // ������ ������� ������������ � ������ �� ������
	int			m_Sig_Num;		// ����� ������ ������������
	double	m_Ro;			// ���������
	double	m_T;			// �����������
	int	m_j;				// ��������� ���			(0-���, 1-����)
	int	m_sv;				// ��������� ���. ��������� (0-���, 1-����)
	double	m_A;			// ������������ ���
	double	m_NAt;			// ���-�� ������ � ��������
	double	m_Z;			// ������� ����� ����
	double	m_Evtor;		// ���� ���������
	double	m_NTr;	    	// ������������ �������

	bool LayInfoListCleared = false;
	bool CombBoxItedAdded = false;
	bool MatComboBoxAdded = false;
	bool PPComboBoxAdded = false;
	bool CommentAdded = false;
	bool userChange = true; // �������� �������������

private slots:
	void OnOpenMrat();		// ��������� ���� � ���������� *.mrat
	void OnOpenMratTable();		// ��������� ���� � ���������� *.mrat �� �������
	void OnGetRoFromMrat();		// ������������� ��������� �� mrat-�����
	void OnCreateMrat();	// ������� ���� � ���������� *.mrat
	int OnChangeElem();	// ����� ���. �������� �� �������
	void OnAddElem();		// ���������� ���. ��������
	void OnDeleteElem();	// �������� ��������� ���. ��������
	int OnSaveMrat();		// ��������� mrat-����
	void OnEditCell();
	void OnChange();        // ���������� ��� ��������� ���������� ����, �����/���� ����
	void OnChangeMrat();	// ���������� ��� ��������� mrat-�����
	void OnSelchangeComboModel();
	void OnSelchangePPModel();
	void OnSelchangeMat();
	void OnSelchangeList();
	void OnOK();
	void OnCancel();
	void OnAddLayer();     // �������� ���� � ������ �����
	void OnInList();     // ������ ��������� � ����
	void OnEditLayer(); // ������ ��������� � ����
	void OnDelete();	 // ���������� ���� �� ������
};

#endif