#pragma once
#include <string>
#include <ctime>

#include "Observer.h"
#include "DisplayManager.h"
#include "nlohmann/json.hpp"

class ScoreManager : public Observer
{

public:
	struct HighScore {
		int score;
		std::string name;
		std::string time;

		// "<" operator so the sort can work.  std::less()
		bool operator<(const HighScore& right) {
			return score < right.score;
		}

		// ">" operator so the sort can work.  std::greater()
		bool operator>(const HighScore& right) {
			return score > right.score;
		}
	};

	ScoreManager(DisplayManager* m_dm);

	virtual void onNotify(Event event) override;
	void updateScore();
	int getScore();
	void resetScore();
	void loadHighScores();
	void showHighScores(int mapwidth, int mapheight);
	void writeHighScores();
	bool checkForHighscore();
	void getPlayerName();

private:

	int m_Score;
	DisplayManager* m_dm;
	std::vector<HighScore> HighScores;
	void sortHighScores() { std::sort(std::begin(HighScores), std::end(HighScores), std::greater()); }

};



