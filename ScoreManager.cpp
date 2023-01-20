#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <time.h>
#include <stdio.h>

#include "ScoreManager.h"
#include "Event.h"
#include "Log.h"
#include "Tile.h"
#include "Keys.h"

#define HIGHSCORES_FILE "Highscores.json"


static void to_json(nlohmann::json& j, const ScoreManager::HighScore& hs) {

	j = nlohmann::json{ {"score", hs.score}, {"name",hs.name}, {"time",hs.time} };
}

static void from_json(const nlohmann::json& j, ScoreManager::HighScore& hs) {
}

ScoreManager::ScoreManager(DisplayManager* dm)
	: m_Score(0),
	  m_dm(dm) {

}

void ScoreManager::onNotify(Event event) {

	switch (event) {
		case Event::ROW_CLEARED:
			LOG_INFO("Score Manager notified row cleared.");
			updateScore();
			break;
	}

}

void ScoreManager::updateScore() {

	m_Score += 10;

}

int ScoreManager::getScore() {

	return m_Score;
}

void ScoreManager::resetScore() {

	m_Score = 0;
}

void ScoreManager::showHighScores(int mapwidth, int mapheight) {

	int starty = mapheight / 4;
	int startx = TILESIZE;

	m_dm->Print(startx, starty, "HIGHSCORES");

	int i = 0;
	for (auto it = HighScores.begin(); it < HighScores.end(); it++) {
		i++;
		m_dm->Print(startx, starty + i, it->score);
		m_dm->Print(startx + 5, starty + i, it->name);
	}

	m_dm->Print(startx, mapheight - 2, "PRESS F1");
	m_dm->Print(startx, mapheight - 1, "TO CONTINUE");

}

void ScoreManager::loadHighScores() {

	std::stringstream logString;

	{
		using namespace nlohmann;

		HighScores.clear();

		json j;

		try {
			std::ifstream inFile(HIGHSCORES_FILE);
			j = json::parse(inFile);
		}
		catch (json::parse_error& exceptionError) {
			logString.str("");
			logString << "parse error at byte " << exceptionError.byte << std::endl;
			LOG_ERROR(logString.str());
		}

		auto hs = j.at("highscores");

		for (auto it = hs.begin(); it < hs.end(); it++) {
			HighScore aHighScore;

			aHighScore.score = it->at("score");
			aHighScore.name = it->at("name");
			aHighScore.time = it->at("time");

			HighScores.push_back(aHighScore);

		}

	}

	sortHighScores();
}

void ScoreManager::writeHighScores() {

	{
		using namespace nlohmann;

		HighScores.resize(10);   // Top 10 only.

		json j = { {"highscores",HighScores} };

		std::ofstream outFile(HIGHSCORES_FILE);
		outFile << j;

		if (!outFile) {
			std::stringstream logString;
			logString.str("");
			logString << "ofstream write failure for highscores." << std::endl;
			LOG_ERROR(logString.str());
		}
	}
}

int ScoreManager::checkForHighscore() {

	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	std::ostringstream stm;
	stm << std::put_time(&timeinfo, "%m-%d-%Y %H:%M:%S");

	int i = 0;
	for (auto aHighScore : HighScores) {
		if (m_Score > aHighScore.score) {
			HighScore newHighScore = { m_Score,"",stm.str()};
			HighScores.insert(HighScores.begin() + i, newHighScore );
			break;
		}
		++i;
	}

	m_ScorePosition = i+1;

	return m_ScorePosition;
}

void ScoreManager::getPlayerName(int mapheight, int mapwidth) {

	int starty = mapheight / 4;
	int startx = TILESIZE;

	HighScores.at(m_ScorePosition-1).name = m_Name;

	m_dm->Print(startx, starty, "HIGHSCORES");

	starty++;

	int i = 0;
	for (auto it = HighScores.begin(); it < HighScores.end(); it++) {
		m_dm->Print(startx, starty + i, it->score);
		m_dm->Print(startx + 5, starty + i, it->name);
		i++;
	}

	m_dm->Print(startx, mapheight - 2, "ENTER NAME");
}

void ScoreManager::handleInput(WPARAM input) {

	if (input >= '0' && input <= 'Z') {
		char a = (char)input;
		m_Name.push_back(a);
	}
}