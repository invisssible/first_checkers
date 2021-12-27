#pragma once

#include "Game.h"

// ��������� ����� �� �����
enum CLICKRESULT { crOk, crMsg, crFinish } ;

// ����� ���������
class GamePainter
{
private:
	//������ �� ����
	Game * game ;
	// ������� ����� � ������
	int _w ;
	int _h ;
	int dw ;
	int dh ;
	int border ;
	bool rotated ;
	// ��������� ������
	POINT Selected ;
	// ������� ���������
	void ClearSelected() ;
	// �������� ���������
	bool isSelected() ;
public:
	GamePainter(void);
	// ���������� ���� ��� ���������
	void setGame(Game * Agame) ;
	// ������� ���� �� ��������
	void drawTo(HDC hdc,int w, int h) ;	
	// �������
	void setRotated(bool rotated) ;
	// ������ �� �����
	CLICKRESULT doClick(int cx,int cy, STEP step, char * msg) ;
	~GamePainter(void);
};

