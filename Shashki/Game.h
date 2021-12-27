#pragma once

#include <vector>
using namespace std ;

// Перечисление типов ячеек
enum CELL { cHuman1,cHuman2,cHuman1D,cHuman2D,cEmpty } ;

// Перечисление типов ходов
enum STEP { sHuman1,sHuman2 } ;

// Основной класс игры
class Game
{
private:
	// Ячейки
	CELL ** cells ; 
public:
	Game(void);
	// Размер поля
	static int Size() ;
	// Корректно ли поле (разрешены черные клетки)
	static bool isValidCell(int x, int y) ;
	// Корректен ли ход
	bool isValidStep(int xs, int ys, int xf, int yf, STEP step, char * msg) ;
	// Получить ячейку
	CELL getCell(int x, int y) ;
	// Является ли клетка игрока
	bool isCellHuman(int x,int y, STEP human) ;
	// Является ли клетка противника
	bool isCellEnemy(int x,int y, STEP human) ;	
	// Число фигур игрока
	int countHuman(STEP human) ;
	// Передвинуть фигуру
	bool moveObject(int xs, int ys, int xf, int yf, STEP step, bool * isgrabbed) ;
	bool isStepGrab(int xs, int ys, int xf, int yf, STEP step) ;
	bool canAttackFrom(int xs, int ys, STEP step) ;
	// Проверка победы
	bool testWin(bool * win, STEP step) ;
	bool canStep(STEP step) ;
	bool isCellHumanCommon(int x,int y, STEP step) ;
	bool isCellHumanDamka(int x,int y, STEP step) ;
	~Game(void);    
};

