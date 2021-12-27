#include "stdafx.h"
#include "Game.h"

Game::Game(void)
{
	// Создание клеток
	cells = new CELL*[Size()] ;
	for (int x=0; x<Size(); x++) 
		cells[x]= new CELL[Size()] ;
	
	// Стартовая очистка и расстановка
	for (int x=0; x<Size(); x++) 
		for (int y=0; y<Size(); y++) 
			cells[x][y]=cEmpty ;
	
	for (int y=0; y<=2; y++)
		for (int x=(y+1) % 2; x<Size(); x+=2)
			cells[x][y]=cHuman2 ;
		
	for (int y=Size()-1; y>=Size()-3; y--)
		for (int x=(y+1) % 2; x<Size(); x+=2)
			cells[x][y]=cHuman1 ;
	
}

bool Game::canAttackFrom(int xs, int ys, STEP step) {
	
	// шашка
	if (isCellHumanCommon(xs,ys,step)) {
		for (int dx=-2; dx<=2; dx+=4)
			for (int dy=-2; dy<=2; dy+=4) {
				int xf = xs+dx ;
				int yf = ys+dy ;
				if ((xf>=0)&&(xf<Size())&&(yf>=0)&&(yf<Size()))
					if (isStepGrab(xs,ys,xf,yf,step)) return true ;
			}
		return false ;	
	}
	// дамка 
	else {
		for (int d=2; d<8; d++) {
			int xf = xs+d ; int yf = ys+d ;
			if ((xf>=0)&&(xf<Size())&&(yf>=0)&&(yf<Size()))
					if (isStepGrab(xs,ys,xf,yf,step)) return true ;
			xf = xs+d ; yf = ys-d ;
			if ((xf>=0)&&(xf<Size())&&(yf>=0)&&(yf<Size()))
					if (isStepGrab(xs,ys,xf,yf,step)) return true ;
			xf = xs-d ; yf = ys+d ;
			if ((xf>=0)&&(xf<Size())&&(yf>=0)&&(yf<Size()))
					if (isStepGrab(xs,ys,xf,yf,step)) return true ;
			xf = xs-d ; yf = ys-d ;
			if ((xf>=0)&&(xf<Size())&&(yf>=0)&&(yf<Size()))
					if (isStepGrab(xs,ys,xf,yf,step)) return true ;
		}
		return false ;
	}
	
}

bool Game::moveObject(int xs, int ys, int xf, int yf, STEP step, bool * isgrabbed) {
	*isgrabbed = false ;
	// Прыжок через клетку - взятие фигуры (универсальный код для дамки и шашки)
	if (abs(xs-xf)>1) {
		int dx = (xf>xs)?1:-1 ;
		int dy = (yf>ys)?1:-1 ;

		int yo = ys+dy ;
		for (int xo=xs+dx; xo!=xf; xo+=dx) {
			if (isCellEnemy(xo,yo,step)) {
				cells[xo][yo]=cEmpty ;
				*isgrabbed=true ;
			}
			yo+=dy ;
		}
	}

	// Перемещение фигуры
	cells[xf][yf]=cells[xs][ys] ;
	cells[xs][ys]=cEmpty ;
	
	// Трансформация в дамку
	if (step==sHuman1) {
		if ((yf==0)&&(cells[xf][yf]==cHuman1)) 
			cells[xf][yf]=cHuman1D ;
	}
	if (step==sHuman2) {
		if ((yf==Size()-1)&&(cells[xf][yf]==cHuman2)) 
			cells[xf][yf]=cHuman2D ;
	}
	
	return true ;
}

int Game::Size() {
	return 8 ;
}

bool Game::isValidCell(int x, int y) {
	return (x+y) % 2 == 1 ;
}

bool Game::isValidStep(int xs, int ys, int xf, int yf, STEP step, char * msg) {
	strcpy(msg,"") ;

	// Некорректная клетка
	if (!isValidCell(xf,yf)) return false ;
	// Непустая клетка
	if (getCell(xf,yf)!=cEmpty) return false ;
	// Не шашка игрока
	if (!isCellHuman(xs,ys,step)) return false ;
	if ((xs==xf)||(ys==yf)) return false ;

	// Проверка хода взятия
	if (isStepGrab(xs,ys,xf,yf,step)) return true ;

	// Проверка хода обычного
	if (isCellHumanCommon(xs,ys,step)) {
		int dy ;
		// Выбираем направление хода по доске
		if (step==sHuman1) dy=-1 ; 
		if (step==cHuman2) dy=1 ; 

		// Если клетка в сторону - ОК
		if ((xf==xs+1)&&(yf==ys+dy)) return true ;
		if ((xf==xs-1)&&(yf==ys+dy)) return true ;
	}
	// Проверка дамки
	else {
		if (abs(xs-xf)!=abs(ys-yf)) return false ;

		int dx = (xf>xs)?1:-1 ;
		int dy = (yf>ys)?1:-1 ;
		int yo = ys+dy ;
		for (int xo=xs+dx; xo!=xf; xo+=dx) {
			if (getCell(xo,yo)!=cEmpty) return false ;
			yo+=dy ;
		}
		return true ;
	}
			
	return false ;
}

bool Game::isStepGrab(int xs, int ys, int xf, int yf, STEP step) {
	
	if (!isCellHuman(xs,ys,step)) return false ;
	if (getCell(xf,yf)!=cEmpty) return false ;

	if (isCellHumanCommon(xs,ys,step)) {
		return (abs(xs-xf)==2)&&(abs(ys-yf)==2)&&isCellEnemy((xs+xf)/2,(ys+yf)/2,step) ;
	}
	else {
		if (abs(xs-xf)!=abs(ys-yf)) return false ;

		int dx = (xf>xs)?1:-1 ;
		int dy = (yf>ys)?1:-1 ;
		int yo = ys+dy ;
		int cntenemy=0 ;
		for (int xo=xs+dx; xo!=xf; xo+=dx) {
			if (isCellHuman(xo,yo,step)) return false ;
			if (isCellEnemy(xo,yo,step)) cntenemy++ ;
			yo+=dy ;
		}
		return cntenemy>0 ;
	}
}

int Game::countHuman(STEP human) {
	int cnt=0 ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++) 
			if (isCellHuman(xs,ys,human)) cnt++ ;
	return cnt ;
}

bool Game::testWin(bool * win, STEP step) {	
	// Если нет ходов, то проигрывает тот, чей сейчас ход
	if (!canStep(sHuman1)) {		
		*win=(step==sHuman2) ;
		return true ;
	}

	if (!canStep(sHuman2)) {		
		*win=(step==sHuman1) ;
		return true ;
	}
		
	// Закончились фигуры
	if ((step==sHuman1)&&(countHuman(sHuman2)==0)) {
		*win=true ;
		return true ;
	}

	if ((step==sHuman2)&&(countHuman(sHuman1)==0)) {
		*win=true ;
		return true ;
	}

	return false ;
}

bool Game::isCellHuman(int x,int y, STEP step) {
	if (step==cHuman1)
		return (getCell(x,y)==cHuman1)||(getCell(x,y)==cHuman1D) ;
	else
		return (getCell(x,y)==cHuman2)||(getCell(x,y)==cHuman2D) ;
}

bool Game::isCellHumanCommon(int x,int y, STEP step) {
	if (step==cHuman1)
		return (getCell(x,y)==cHuman1) ;
	else
		return (getCell(x,y)==cHuman2) ;
}

bool Game::isCellHumanDamka(int x,int y, STEP step) {
	if (step==cHuman1)
		return (getCell(x,y)==cHuman1D) ;
	else
		return (getCell(x,y)==cHuman2D) ;
}

bool Game::isCellEnemy(int x,int y, STEP step) {
	if (step==cHuman1)
		return (getCell(x,y)==cHuman2)||(getCell(x,y)==cHuman2D) ;
	else
		return (getCell(x,y)==cHuman1)||(getCell(x,y)==cHuman1D) ;
}

bool Game::canStep(STEP step) {	
	char msg[255] ;
	// Перебираем все комбинации ходов, оставляя только корректные	
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++)
			if (isCellHuman(xs,ys,step))
				for (int xf=0; xf<Size(); xf++)
					for (int yf=0; yf<Size(); yf++)
						if (getCell(xf,yf)==cEmpty)
							if (isValidStep(xs,ys,xf,yf,step,msg)) 
								return true ;

	return false ;	
}

CELL Game::getCell(int x, int y) {
	return cells[x][y] ;
}

Game::~Game(void)
{
}
