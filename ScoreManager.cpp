#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

#include "nlohmann/json.hpp"

#include "ScoreManager.h"
#include "Event.h"
#include "Log.h"

#define HIGHSCORES_FILE "Highscores.json"


ScoreManager::ScoreManager()
	: m_Score(0) {

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

void ScoreManager::showHighScores(DisplayManager* dm) {

}

void ScoreManager::loadHighScores() {

	std::stringstream logString;

	{
		using namespace nlohmann;

		HighScores.clear();

		ordered_json j;

		try {
			std::ifstream inFile(HIGHSCORES_FILE);
			j = ordered_json::parse(inFile);
		}
		catch (ordered_json::parse_error& exceptionError) {
			logString.str("");
			logString << "parse error at byte " << exceptionError.byte << std::endl;
			LOG_ERROR(logString.str());
		}

		auto h = j.at("highscores");

		for (auto it = j.begin(); it < j.end(); it++) {
			HighScore aHighScore;

			aHighScore.time = it->at("time");
			aHighScore.name = it->at("name");
			aHighScore.score = it->at("score");

			HighScores.push_back(aHighScore);

		}

	}
}

void ScoreManager::writeHighScores() {

	std::stringstream logString;

	{
		using namespace nlohmann;

		ordered_json j;
		// Setup json data.
		// Sort highscores highest at top.  Equal scores sort by date oldest at top.

		try {
			std::ofstream outFile(HIGHSCORES_FILE);
			// Write json to outFile.
		}
		catch (ordered_json::parse_error& exceptionError) {
			logString.str("");
			logString << "parse error at byte " << exceptionError.byte << std::endl;
			LOG_ERROR(logString.str());
		}

		auto h = j.at("highscores");

	}


}