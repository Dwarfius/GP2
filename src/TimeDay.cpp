#include "TimeDay.h"



TimeDay::TimeDay()
{
	timeHour = 0;
	timeMinute = 0;
	timeScale = 1;
}


TimeDay::~TimeDay()
{
}

void TimeDay::Update(float deltaTime)
{
	timeMinute += (deltaTime * timeScale);

	if (timeMinute > 59.99)
	{
		timeMinute = 0;
		timeHour++;
		if (timeHour > 23) timeHour = 0;
	}
}

void TimeDay::SetTime(int hour, float minute = 0)
{
	timeHour = hour;
	timeMinute = minute;
	if (timeHour < 0 || timeMinute < 0) {
		timeHour = 0;
		timeMinute = 0;
	}
}

string TimeDay::GetTimeString()
{
	/*stringstream ss;
	ss.width = 2;
	ss << timeHour;
	ss.fill('0');
	ss << ":";
	ss << timeMinute;
	ss.fill('0');
	return ss.str();*/
	return string();
}

