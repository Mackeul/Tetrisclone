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
	void showHighScores(DisplayManager* dm);
	void writeHighScores();

private:

	int m_Score;

	struct HighScore {
		std::string time;
		std::string name;
		int score;
	};

	std::vector<HighScore> HighScores;
};

