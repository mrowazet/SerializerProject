#pragma once
#include <chrono>
#include <iostream>

using namespace std::chrono;

class Stopwatch
{
	public:
		Stopwatch();
		~Stopwatch();

		void start();
		void stop();
		unsigned long getLastElapsedTime() const;

	private:
		system_clock::time_point m_startPoint;
		bool m_started = false;
		unsigned long m_elapsedTime = 0;

		unsigned long getElapsedTime();	
};

