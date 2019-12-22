#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "misc.h"

//
//	An abstract class that represents a move in a position
//
class CAbstractMove
{
public:
	static CAbstractMove					kNullMove;
	typedef std::shared_ptr<CAbstractMove>	Yq;

	virtual void setFromStr(const std::string& rStr) = 0;
	virtual std::string toStr() const = 0;
};

/*
 * Used to create a new move
 */
class CMoveFactory
{
public:
	virtual CAbstractMove::Yq createNewMove() = 0;
};

/*
 * Derived class that does the actual move creation 
 */
template<class TMove> class TDerivedMoveFactory : public CMoveFactory
{
public:
	virtual CAbstractMove::Yq createNewMove()
	{
		return std::shared_ptr<CAbstractMove>((CAbstractMove*)new TMove);
	}
};

//
//	An abstract class that represents a position
//
class CAbstractPos
{
public:
	typedef std::shared_ptr<CAbstractPos>	Yq;

	virtual void init() = 0;
	virtual void makeMove(const CAbstractMove& rMove) = 0; // throws 
	virtual bool isMoveLegal(const CAbstractMove& ) const = 0;
	virtual bool isGameOver() const = 0;
	virtual void getLegalMoves(std::vector<CAbstractMove::Yq>& rMoveVect) const = 0;
	virtual std::shared_ptr<CAbstractPos> createAndCopy() = 0;
	
	int getWhoseTurn() const { return mWhoseTurn; }
	void setWhoseTurn(int whoseTurn) { mWhoseTurn = whoseTurn; }
	double getResult() const { return mResult; }

	virtual void initFromFen(const std::string& rFen) = 0; // throws 
	virtual void toStr(std::vector<std::string>&, bool bAdorned = false) const = 0;

protected:
	void setResult(int vResult) { mResult = vResult; }

private:		
	int			mWhoseTurn = 0;
	double		mResult = 0;
};
 
//
//	A move and the position that results from making that move
//
struct SMoveToPos
{
	typedef std::shared_ptr<SMoveToPos>	Yq;

	SMoveToPos(CAbstractMove& rMove, CAbstractPos::Yq qPos)
		: mrMove(rMove), mqPos(qPos) {}
	SMoveToPos()
		: mrMove(CAbstractMove::kNullMove), mqPos(NULL) {}

	CAbstractMove&		mrMove;
	CAbstractPos::Yq	mqPos;
};

//
//  A sequence of positions and moves
//
class CPlayHistory
{
public:
	void add(
		CAbstractMove&			rMove,
		CAbstractPos::Yq		qPos)
	{
		mMoveToPosVect.push_back(SMoveToPos::Yq(new SMoveToPos(rMove, qPos)));
	}

private:
	std::vector<SMoveToPos::Yq>		mMoveToPosVect;
};

/*
 * Base class of a strategy, which accepts a position and decides which
 * move to make.
*/
class CAbstractStrategy
{
public:
	virtual void getMove(
		CAbstractMove&			rMove,
		const CAbstractPos&		rPos) = 0;
};

/*
 * A strategy which prompts the console for a move
 */
class CSelfPlayStrategy: public CAbstractStrategy
{
public:
	virtual EXPORT void getMove(
		CAbstractMove&			rMove,
		const CAbstractPos&		rPos);
};

/*
 * Simple way to run a game from the command prompt
 */
class CGameRunner
{
public:
	CGameRunner(CMoveFactory& rMoveFactory)
		: mrMoveFactory(rMoveFactory) {}

	EXPORT void runGame(
		int					vPlayerNum,
		CAbstractStrategy&	rStrat,
		CAbstractPos&		rPos,
		CPlayHistory&		rPlayHist);

	static void readMove(CAbstractMove& rMove, const CAbstractPos& rPos);

private:
	static void printBoard(const CAbstractPos& rPos);

private:
	CMoveFactory&		mrMoveFactory;

};
