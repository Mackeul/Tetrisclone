#include "ScoreManager.h"
#include "Event.h"
#include "Log.h"

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