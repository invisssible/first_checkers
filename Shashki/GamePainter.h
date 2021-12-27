#pragma once

#include "Game.h"

// Результат клика по доске
enum CLICKRESULT { crOk, crMsg, crFinish } ;

// Класс рисования
class GamePainter
{
private:
	//Ссылка на игру
	Game * game ;
	// Размеры доски и ячейки
	int _w ;
	int _h ;
	int dw ;
	int dh ;
	int border ;
	bool rotated ;
	// Выбранная клетка
	POINT Selected ;
	// Очистка выбранной
	void ClearSelected() ;
	// Проверка выбранной
	bool isSelected() ;
public:
	GamePainter(void);
	// Установить игру для рисования
	void setGame(Game * Agame) ;
	// Вывести игру на контекст
	void drawTo(HDC hdc,int w, int h) ;	
	// Поворот
	void setRotated(bool rotated) ;
	// Щелчок по доске
	CLICKRESULT doClick(int cx,int cy, STEP step, char * msg) ;
	~GamePainter(void);
};

