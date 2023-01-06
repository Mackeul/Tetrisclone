#pragma once
#include <string>
#include <ctime>

#include "Observer.h"
#include "DisplayManager.h"

class ScoreManager : public Observer
{
public:
	ScoreManager();

	virtual void onNotify(Event event) override;
	void updateScore();
	int getScore();
	void resetScore();
	void loadHighScores();
	void showHighScores(DisplayManager* dm, int mapwidth, int mapheight);
	void writeHighScores();

private:

	int m_Score;

	struct HighScore {		
		int score;
		std::string name;
		std::string time;
	};

	std::vector<HighScore> HighScores;
};

