#pragma once
#include "game.h"

//
//	An class that represents a move in a Tic Tak Toe position
//
class CTicTacToeMove : CAbstractMove
{
public:
	typedef std::shared_ptr<CTicTacToeMove>	Yq;

	virtual void setFromStr(const std::string& rStr);
	virtual std::string toStr() const;
};

//
//	An class that represents a Tic Tac Toe position
//
class CTicTacToePos: CAbstractPos
{
public:
	typedef std::shared_ptr<CTicTacToePos>	Yq;

	virtual void init();
	virtual void makeMove(const CAbstractMove& rMove); // throws  
	virtual bool isMoveLegal(const CAbstractMove&) const;
	virtual YVal result() const;
	virtual bool isGameOver();
	virtual std::vector<CAbstractMove::Yq> getLegalMoves() const;

	virtual void initFromFen(const std::string& rFen); // throws 
	virtual std::string toStr() const;
};

