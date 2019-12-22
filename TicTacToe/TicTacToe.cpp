// TicTacToe.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "tictactoeGame.h"
#include <iostream>

int main()
{
	std::cout << "TicTacToe (C) 2019 by Jeffery A Esposito" << std::endl;

	TDerivedMoveFactory<CTicTacToeMove>	rMoveFactory;
	CGameRunner							vGameRunner(rMoveFactory);
	CSelfPlayStrategy					vStrat;
	CTicTacToePos						vPos;
	CPlayHistory						vHist;

	vPos.init();
	vGameRunner.runGame(0, vStrat, vPos, vHist);

}

