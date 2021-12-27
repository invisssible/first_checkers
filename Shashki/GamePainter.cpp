#include "stdafx.h"
#include "GamePainter.h"

GamePainter::GamePainter(void)
{
	game = NULL ;
	ClearSelected() ;
	rotated = false ;
}

void GamePainter::ClearSelected() {
	Selected.x=-1 ;
	Selected.y=-1 ;
}

bool GamePainter::isSelected() {
	return (Selected.x!=-1) ;
}

CLICKRESULT GamePainter::doClick(int cx,int cy, STEP step, char * msg) {
	POINT clicked ;
	bool find = false ;

	for (int x=0; x<game->Size(); x++) 
		for (int y=0; y<game->Size(); y++) 
			if ((cx>border+x*dw)&&(cy>border+y*dh)&&(cx<border+(x+1)*dw)&&(cy<border+(y+1)*dh)) {
				clicked.x = x ;
				clicked.y = y ;
				find = true ;
			}

	if (!find) return crOk ;

	if (rotated) {
		clicked.x=game->Size()-1-clicked.x ;
		clicked.y=game->Size()-1-clicked.y ;
	}

	if (game->isCellHuman(clicked.x,clicked.y,step)) {
		Selected = clicked ;
		return crOk ;
	}

	if (isSelected()) {
		char msg2[255] ;
		if (game->isValidStep(Selected.x,Selected.y,clicked.x,clicked.y,step,msg2)) {
			bool grabbed ;
			game->moveObject(Selected.x,Selected.y,clicked.x,clicked.y,step,&grabbed) ;
			if ((grabbed)&&(game->canAttackFrom(clicked.x,clicked.y,step))) {
				Selected = clicked ;
				return crOk ;
			}
			else {
				ClearSelected() ;
				return crFinish ;
			}
		}
		else {
			if (strlen(msg2)>0) {
				strcpy(msg,msg2) ;
				return crMsg ;
			}
			else
				return crOk ;
		}
	}

	return crOk ;
}

void GamePainter::drawTo(HDC hdc,int w, int h) {
	_w = w ;
	_h = h ;

	border = 10 ;
	dw = (w-border*3)/game->Size() ;
	dh = (h-border*3-50)/game->Size() ;

	COLORREF gray = RGB(80,80,80) ;
	COLORREF black = RGB(0,0,0) ;
	COLORREF white = RGB(255,255,255) ;
	COLORREF white2 = RGB(200,200,200) ;
	COLORREF black2 = RGB(60,60,60) ;
	COLORREF green = RGB(100,255,100) ;

	HBRUSH brushGray = CreateSolidBrush(gray) ;
	HBRUSH brushBlack = CreateSolidBrush(black) ;
	HBRUSH brushWhite = CreateSolidBrush(white) ;
	HBRUSH brushBlack2 = CreateSolidBrush(black2) ;
	HBRUSH brushWhite2 = CreateSolidBrush(white2) ;
	HBRUSH brushGreen = CreateSolidBrush(green) ;
	
	HPEN pen = CreatePen(PS_SOLID, 1,gray) ;
	HPEN penBlack2 = CreatePen(PS_SOLID, 1,black2) ;
	HPEN penWhite2 = CreatePen(PS_SOLID, 1,white2) ;
	
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, brushGray);
	HPEN OldPen = (HPEN)SelectObject(hdc, pen);

	for (int x=0; x<game->Size(); x++) 
		for (int y=0; y<game->Size(); y++) {
			int vx = x ; int vy = y ;
			if (rotated) {
				vx=game->Size()-1-x ;
				vy=game->Size()-1-y ;
			}

			// Рисование клетки, если нужно - с фоном, но обязательно правильного цвета
			if (game->isValidCell(x,y)) SelectObject(hdc, brushGray); else SelectObject(hdc,OldBrush) ;
			if ((Selected.x==x)&&(Selected.y==y)) SelectObject(hdc,brushGreen) ;
			Rectangle(hdc,border+vx*dw,border+vy*dh,border+(vx+1)*dw,border+(vy+1)*dh) ;
			
			if (game!=NULL) {
				// Вывод фигур
				if (game->getCell(x,y)==cHuman2) {
					SelectObject(hdc,brushBlack) ;
					Ellipse(hdc,border+vx*dw+3,border+vy*dh+3,border+(vx+1)*dw-3,border+(vy+1)*dh-3) ;
				}
				if (game->getCell(x,y)==cHuman1) {
					SelectObject(hdc,brushWhite) ;
					Ellipse(hdc,border+vx*dw+3,border+vy*dh+3,border+(vx+1)*dw-3,border+(vy+1)*dh-3) ;
				}
				if (game->getCell(x,y)==cHuman2D) {
					SelectObject(hdc,brushBlack) ;
					Ellipse(hdc,border+vx*dw+3,border+vy*dh+3,border+(vx+1)*dw-3,border+(vy+1)*dh-3) ;
					SelectObject(hdc,brushBlack2) ;
					SelectObject(hdc,penBlack2) ;
					Ellipse(hdc,border+vx*dw+9,border+vy*dh+9,border+(vx+1)*dw-9,border+(vy+1)*dh-9) ;
				}
				if (game->getCell(x,y)==cHuman1D) {
					SelectObject(hdc,brushWhite) ;
					Ellipse(hdc,border+vx*dw+3,border+vy*dh+3,border+(vx+1)*dw-3,border+(vy+1)*dh-3) ;
					SelectObject(hdc,brushWhite2) ;
					SelectObject(hdc,penWhite2) ;
					Ellipse(hdc,border+vx*dw+9,border+vy*dh+9,border+(vx+1)*dw-9,border+(vy+1)*dh-9) ;
					SelectObject(hdc,pen) ;
				}

			}
		}

	SelectObject(hdc,OldBrush) ;
	DeleteObject(brushGray) ;
	DeleteObject(brushBlack) ;
	DeleteObject(brushWhite) ;
	DeleteObject(brushBlack2) ;
	DeleteObject(brushWhite2) ;
	SelectObject(hdc,OldPen) ;
	DeleteObject(pen) ;
	DeleteObject(penWhite2) ;
	DeleteObject(penBlack2) ;

}

void GamePainter::setGame(Game * Agame) {
	game = Agame ;
}

GamePainter::~GamePainter(void)
{
}

void GamePainter::setRotated(bool rotated) {
	this->rotated = rotated ;
}