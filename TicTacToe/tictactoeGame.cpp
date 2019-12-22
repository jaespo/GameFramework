#pragma once
#include "stdafx.h"
#include <string>
#include "tictactoeGame.h"
#include "..\GameFramework\misc.h"

//
//	static instantiations and initializations
//
const char* CTicTacToePos::kRankNames = "123";
const char* CTicTacToePos::kFileNames = "abc";
const char* CTicTacToePos::kPlayerNames = "XO";

/*
 * ctor for a tic tac toe move
 */
CTicTacToeMove::CTicTacToeMove(int vFile, int vRank)
{
	mFile = vFile;
	mRank = vRank;
}

/*
 * sets a move from a string.  
 *
 * @param rStr
 *		specified as a file and rank, such as "b2".
 * @thows an error string
 */
void CTicTacToeMove::setFromStr(const std::string& rStr)
{
    if ( rStr.length() != 2 ) 
	{ 
		throw rStr + ": Invalid move, length must be 2";
	}

	//
	//	Determine the move's file
	//
	const char* pCh = strchr(CTicTacToePos::kFileNames, rStr[0]);
	if ( pCh == NULL )
	{
		throw rStr + ": Invalid move, file must be in " 
			+ CTicTacToePos::kFileNames;
	}
	int file = pCh - CTicTacToePos::kFileNames;

	//
	//	Determine the move's rank
	//
	pCh = strchr(CTicTacToePos::kRankNames, rStr[1]);
	if (pCh == NULL)
	{
		throw rStr + ": Invalid move, rank must be in "
			+ CTicTacToePos::kRankNames;
	}
	int rank = pCh - CTicTacToePos::kRankNames;

	mRank = rank;
	mFile = file;
}

/*
 * @returns the move as a file and rank, for example "b3"
 */
std::string CTicTacToeMove::toStr() const
{
	std::string vRetval(1, CTicTacToePos::kFileNames[mFile] );
	vRetval.append(1, CTicTacToePos::kRankNames[mRank]);
	return vRetval;
}

/*
 * Initialize the board's starting position
 */
void CTicTacToePos::init() 
{
	mMovesMade = 0;
	for (int vRank = 0; vRank < kNumRanks; ++vRank)
		for (int vFile = 0; vFile < kNumFiles; ++vFile)
			mBoard[vRank][vFile] = '_';
}

/*
 * Make the move
 *
 * @param rMove 
 *		the move
 */
void CTicTacToePos::makeMove(const CAbstractMove& rMove) 
{
	CTicTacToeMove& rMv = (CTicTacToeMove&)rMove;
	mBoard[rMv.getRank()][rMv.getFile()] = kPlayerNames[getWhoseTurn()];

	//
	//	Check to see if the move wins
	//
	if ( (mBoard[0][rMv.getFile()] == mBoard[1][rMv.getFile()] && 
		  mBoard[0][rMv.getFile()] == mBoard[2][rMv.getFile()]) ||
		 (mBoard[rMv.getRank()][0] == mBoard[rMv.getRank()][1] &&
		  mBoard[rMv.getRank()][0] == mBoard[rMv.getRank()][2]) ||
		 (mBoard[1][1] != '_' && mBoard[0][0] == mBoard[1][1] && mBoard[0][0] == mBoard[2][2] ) || 
     	 (mBoard[1][1] != '_' && mBoard[0][2] == mBoard[1][1] && mBoard[0][2] == mBoard[2][0]) )
	{
		setResult((getWhoseTurn() == 0) ? 1 : -1);
	}
	setWhoseTurn(1 - getWhoseTurn());
	++mMovesMade;
}

/*
 * @returns a flag indicating whether or not the move is legal
 * in this position.
 */
bool CTicTacToePos::isMoveLegal(const CAbstractMove& rMove) const
{
	CTicTacToeMove& rMv = (CTicTacToeMove&)rMove;
	return mBoard[rMv.getRank()][rMv.getFile()] == '_';
}

/*
 * @returns a flag indicating whether or not the game is over
 */
bool CTicTacToePos::isGameOver() const 
{ 
	return getResult() != 0.0 || mMovesMade == ( kNumFiles * kNumRanks ); 
}

/*
 * Generate a list of legal moves
 *
 * @param rMoveVect
 *		the vector of legal moves in this position
 */
void CTicTacToePos::getLegalMoves(
	std::vector<CAbstractMove::Yq>& rMoveVect) const
{
	rMoveVect.erase(rMoveVect.begin(), rMoveVect.end());
	for ( int vRank = 0; vRank < kNumRanks; ++vRank )
		for (int vFile = 0; vFile < kNumFiles; ++vFile)
		{
			if (mBoard[vRank][vFile] == '_')
			{
				rMoveVect.push_back(CAbstractMove::Yq(
					(CAbstractMove*)new CTicTacToeMove( vFile, vRank )));
			}
		}
}

/*
 * Initialize a board from a fen, which is a 10 character string,
 * the first character is whose move, then the others are X or O or
 * _
 *
 * @param rFen
 *		is a string that contains the 10 character fen.
*/
void CTicTacToePos::initFromFen(const std::string& rFen)
{
	//
	//	Validate the length of the FEN
	//
	if (rFen.length() != 10)
		throw std::string("Fen string length must be 10");
	init();
	switch (rFen[0])
	{
	case 'X': setWhoseTurn(0); break;
	case '0': setWhoseTurn(1); break;
	default: throw std::string("Invalid whoseTurn indicator, must be X or O");
	}
	int vFenIx = 1;
	for (int vRank = 0; vRank < kNumRanks; ++vRank)
	{
		for (int vFile = 0; vFile < kNumFiles; ++vFile)
		{
			switch (rFen[vFenIx++])
			{
			case 'X':
				mBoard[vRank][vFile] = 'X';
				++mMovesMade;
				break;
			case '0':
				mBoard[vRank][vFile] = 'O';
				++mMovesMade;
				break;
			case '_':
				mBoard[vRank][vFile] = '_';
				break;
			default:
				throw std::string("Invalid board square indicator, must be X or O or _");
			}
		}
	}
}

/*
 * Convert the tic tac position to a printable string
 *
 *    a b c
 *  +-------+ 
 * 1| X     |
 * 2|   O   |
 * 3|     X |
 *  +-------+
 */
void CTicTacToePos::toStr(std::vector<std::string>& str, bool bAdorned ) const
{
	str.erase(str.begin(), str.end());
	if (bAdorned)
	{
		str.push_back(std::string("   a b c"));
		str.push_back(std::string(" +-------+"));
	}
	for ( int vRank = 0; vRank < kNumRanks; ++vRank )
	{
		std::string s;
		if (bAdorned) 
		{
			s.append(::fmtStr(vRank + 1) + "| ");
		}
		for (int vFile = 0; vFile < kNumFiles; ++vFile)
		{
			if (mBoard[vRank][vFile] == '_')
				s.append("  ");
			else
				s.append(std::string(&mBoard[vRank][vFile], 1) + " ");
		}
		if (bAdorned)
		{
			s.append("|");
		}
		str.push_back(s);
	}
	if (bAdorned)
	{
		str.push_back(std::string(" +-------+"));
	}
}

/*
 * Allocates and makes a copy of 'this' and returns a shared pointer to it
 */
std::shared_ptr<CAbstractPos> CTicTacToePos::createAndCopy()
{
	return std::shared_ptr<CAbstractPos>(new CTicTacToePos(*this));
}




