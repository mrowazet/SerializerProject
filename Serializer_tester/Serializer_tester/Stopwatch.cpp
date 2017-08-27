#include "Stopwatch.h"

Stopwatch::Stopwatch()
{

}

Stopwatch::~Stopwatch()
{

}

void Stopwatch::start()
{
	m_started = true;
	std::cout << "Stopwatch started. \n";
	m_startPoint = system_clock::now();
}

void Stopwatch::stop()
{
	if (m_started)
	{
		m_elapsedTime = getElapsedTime();
		std::cout << "Stopwatch stopped. Elapsed time: " << m_elapsedTime << "ms \n";
		
		m_started = false;
	}
	else
	{
		std::cout << "Stopwatch not started. \n";
	}
}

unsigned long Stopwatch::getElapsedTime()
{
	auto currentTime = system_clock::now();
	auto elapsedTime = currentTime - m_startPoint;
	auto miliseconds = duration_cast<milliseconds>(elapsedTime).count();

	return miliseconds;
}

unsigned long Stopwatch::getLastElapsedTime() const
{
	return m_elapsedTime;
}
