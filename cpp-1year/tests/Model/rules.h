#ifndef RULES_H_
#define RULES_H_

#include "snowwchess.h"

class Rules {
	
	public:
		Rules();

	public: //getters
		const MOVERULES& getMoveRules(int figureId) const;
		const FigureData& getFigureData(int figureId) const;
                std::string getPlayerData(int playerId) const;
		const FIGURES_DATA& getAllFiguresData() const;
		const FIGURES& getInitFigures(int playerId) const;
		std::string getRulesName() const;
		int getFirstTurnPlayer() const;
		int getSpecialFigure(int player) const;
		int getBoardSizeX() const;
		int getBoardSizeY() const;
		const CastleRule& getCastleRule(int dx,int dy, int player) const;

	public: //setters
		void setPlayerData(int playerId, std::string name);
		void setSpecialFigure(int player, int figureId);
		void setFirstTurnPlayer(int firstTurnId);
		void setBoardSize(int sizex, int sizey);
		void setRulesName(std::string rulesName);
		void setInitFigure(int playerId, const Figure& figure);
		void setMoveRule(int figureId, const MoveRule& moverule);
		void setFigureData(int figureId, const FigureData& figuredata);
		void setCastleRule(const CastleRule& castlerule);

	public: //sort operator()
		bool operator()(const Figure& figure1,const Figure& figure2) {
			 return myFiguresData[figure1.id].weight > myFiguresData[figure2.id].weight;
		}

	private:

		FIGURES_RULES myMoveRules;
		FIGURES_DATA myFiguresData;
		PLAYERS_DATA myPlayersData;
		FIGURES myInitFigures[2];
		CASTLERULES myCastlesRules;
		std::string myRulesName;
		int myFirstTurn;
		int mySpecialFigureId[2];
		int myBoardSizeX, myBoardSizeY;

};

#endif /* RULES_H_ */
