#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>

#include "ScoreManager.h"
#include "Event.h"
#include "Log.h"
#include "Tile.h"

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

	for (int x = 0; x < mapwidth; x++) {
		for (int y = mapheight / 4; y < mapheight; y++) {
			m_dm->DrawTile(x, y, Tile::GREY);
		}
	}

	m_dm->Print(1, mapheight / 4, "HIGHSCORES");

	int i = 0;
	for (auto it = HighScores.begin(); it < HighScores.end(); it++) {
		i++;
		std::stringstream aString;
		aString << it->score << " " << it->name; // not enough room atm for time... << " " << it->time;
		m_dm->Print(1, mapheight / 4 + i, aString.str());
	}

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

bool ScoreManager::checkForHighscore() {

	HighScore minHighScore = *std::min_element(HighScores.begin(), HighScores.end());

	return this->m_Score > minHighScore.score;
}

void ScoreManager::getPlayerName() {
	// input player name so we can write it in highscores.
	std::string playerName;

	std::cout << "Enter name:" << std::endl;
	std::cin >> playerName;
}