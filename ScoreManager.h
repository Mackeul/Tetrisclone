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
	int checkForHighscore();
	void getPlayerName(int mapheight, int mapwidth);
	void handleInput(WPARAM input);

private:

	int m_Score;
	std::string m_Name;
	int m_ScorePosition = 0;
	DisplayManager* m_dm;
	std::vector<HighScore> HighScores;
	void sortHighScores() { std::sort(std::begin(HighScores), std::end(HighScores), std::greater()); }

};



