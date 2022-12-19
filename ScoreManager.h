#pragma once
#include "Observer.h"

class ScoreManager : public Observer
{
public:
	ScoreManager();

	virtual void onNotify(Event event) override;
	void updateScore();
	int getScore();
	void resetScore();

private:

	int m_Score;

};

