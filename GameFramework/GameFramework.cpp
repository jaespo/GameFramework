// GameFramework.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "game.h"

/*
 * Utilty to read a move from the console
 */
void CGameRunner::readMove(CAbstractMove& rMove, const CAbstractPos& rPos)
{
	char pzBuf[80];
	bool bDone = false;

	printBoard(rPos);
	while (!bDone)
	{
		std::cout << "Player " << rPos.getWhoseTurn() << "'s move? ";
		std::cin.getline(pzBuf, sizeof(pzBuf));
		try
		{
			rMove.setFromStr(pzBuf);
			if (rPos.isMoveLegal(rMove))
				bDone = true;
			else
				std::cout << "Illegal move!" << std::endl;
		}
		catch (std::string vErr)
		{
			std::cout << vErr << std::endl;
		}
	}
}

/*
 * Plays a game at the text console
 */
void CGameRunner::runGame(
    int					vPlayerNum,
	CAbstractStrategy&	rStrat,
	CAbstractPos&		rPos,
	CPlayHistory&		rPlayHist)
{
	CAbstractMove::Yq	qMove = mrMoveFactory.createNewMove();
	CAbstractMove::Yq	qStratMove = mrMoveFactory.createNewMove();

	while (!rPos.isGameOver())
	{
		if (vPlayerNum == rPos.getWhoseTurn())
		{
			//
			//	The player's turn
			//
			readMove(*qMove, rPos);
			rPos.makeMove(*qMove);
			rPlayHist.add(*qMove, rPos.createAndCopy());
		}
		else
		{
			//
			//	The provided strategy's turn
			//
			rStrat.getMove(*qStratMove, rPos);
            std::cout << "The computer plays " << qStratMove->toStr() << std::endl;
			rPos.makeMove(*qStratMove);
			rPlayHist.add(*qStratMove, rPos.createAndCopy());
		}
	}
	//
	// Game is over
	//
	printBoard(rPos);
	if (rPos.getResult() == 1.0)
		std::cout << "Player 0 wins" << std::endl;
	else if (rPos.getResult() == -1.0)
		std::cout << "Player 1 wins" << std::endl;
	else
		std::cout << "Drawn" << std::endl;
	std::cout << "Hit RETURN to exit";
	char ch;
	std::cin.get(ch);
}

//
//	Print the board
//
void CGameRunner::printBoard(const CAbstractPos& rPos)
{
	std::vector<std::string> vPosStr;
	rPos.toStr(vPosStr, true);
	for (std::vector<std::string>::iterator vIt = vPosStr.begin();
		vIt != vPosStr.end(); ++vIt)
	{
		std::cout << *vIt << std::endl;
	}
}

/*
 * A strategy that prompts the user to determine the move the strategy
 * will play.
 */
void CSelfPlayStrategy::getMove(
	CAbstractMove&			rMove,
	const CAbstractPos&		rPos)
{
	CGameRunner::readMove(rMove, rPos);
}


