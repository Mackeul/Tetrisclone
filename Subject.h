#pragma once
#include <vector>
#include "Observer.h"

class Subject {
private:
	std::vector<Observer*> m_Observers;

protected:
	void notify(Event event);

public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);
};