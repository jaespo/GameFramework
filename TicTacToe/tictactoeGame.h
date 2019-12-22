#pragma once
#include "..\GameFramework\game.h"

/*
 *	A class that represents a move in a Tic Tak Toe position
 */
class CTicTacToeMove : CAbstractMove
{
public:
	typedef std::shared_ptr<CTicTacToeMove>	Yq;

	//
	// ctors
	//
	CTicTacToeMove() {}
	CTicTacToeMove(int vFile, int vRank);

	//
	// virtual function implementations
	//
	virtual void setFromStr(const std::string& rStr);
	virtual std::string toStr() const;

	//
	// setters and getters
	//
	int getRank() { return mRank; }
	int getFile() { return mFile; }
	void setRank(int vRank) { mRank = vRank; }
	void setFile(int vFile) { mFile = vFile; }

private:
	int			mRank;
	int			mFile;
};

/*
 * A class that represents a Tic Tac Toe position
 */
class CTicTacToePos : public CAbstractPos
{
public:
	static const int kNumRanks = 3;
	static const int kNumFiles = 3;
	static const char* kRankNames;		// = "123";
	static const char* kFileNames;		// = "abc";
	static const char* kPlayerNames;	// = "XO"

	typedef std::shared_ptr<CTicTacToePos>	Yq;

   	virtual void init();
	virtual void makeMove(const CAbstractMove& rMove);
	virtual bool isMoveLegal(const CAbstractMove&) const;
	virtual bool isGameOver() const;
	virtual void getLegalMoves(std::vector<CAbstractMove::Yq>& rMoveVect) const;
	virtual void initFromFen(const std::string& rFen); // throws
	virtual void toStr(std::vector<std::string>&, bool bAdorned = false) const;
	virtual std::shared_ptr<CAbstractPos> createAndCopy();

private:
	char		mBoard[kNumRanks][kNumFiles];
	int			mMovesMade;
};

