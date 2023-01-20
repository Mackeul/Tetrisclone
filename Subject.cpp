#pragma once
#include "Subject.h"
#include "Observer.h"
#include "Log.h"

void Subject::notify(Event event) {

	for (Observer* observer : m_Observers) {
		observer->onNotify(event);
	}
}

void Subject::addObserver(Observer* observer) {

	m_Observers.push_back(observer);
	LOG_INFO("Observer added.");

}

void Subject::removeObserver(Observer* observer) {

	// Used the std::remove function to find the observer in the vector (even if multiple times) 
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer),m_Observers.end());

	LOG_INFO("Observer removed.");

}