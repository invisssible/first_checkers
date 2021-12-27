#pragma once

#include <vector>
using namespace std ;

// ������������ ����� �����
enum CELL { cHuman1,cHuman2,cHuman1D,cHuman2D,cEmpty } ;

// ������������ ����� �����
enum STEP { sHuman1,sHuman2 } ;

// �������� ����� ����
class Game
{
private:
	// ������
	CELL ** cells ; 
public:
	Game(void);
	// ������ ����
	static int Size() ;
	// ��������� �� ���� (��������� ������ ������)
	static bool isValidCell(int x, int y) ;
	// ��������� �� ���
	bool isValidStep(int xs, int ys, int xf, int yf, STEP step, char * msg) ;
	// �������� ������
	CELL getCell(int x, int y) ;
	// �������� �� ������ ������
	bool isCellHuman(int x,int y, STEP human) ;
	// �������� �� ������ ����������
	bool isCellEnemy(int x,int y, STEP human) ;	
	// ����� ����� ������
	int countHuman(STEP human) ;
	// ����������� ������
	bool moveObject(int xs, int ys, int xf, int yf, STEP step, bool * isgrabbed) ;
	bool isStepGrab(int xs, int ys, int xf, int yf, STEP step) ;
	bool canAttackFrom(int xs, int ys, STEP step) ;
	// �������� ������
	bool testWin(bool * win, STEP step) ;
	bool canStep(STEP step) ;
	bool isCellHumanCommon(int x,int y, STEP step) ;
	bool isCellHumanDamka(int x,int y, STEP step) ;
	~Game(void);    
};

