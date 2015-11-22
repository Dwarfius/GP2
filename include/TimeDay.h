#ifndef _TIMEDAY_H
#define _TIMEDAY_H
#include "common.h"
#include "BaseComponent.h"
class TimeDay :public BaseComponent
{
private:
	float timeScale;
	float timeMinute;
	int timeHour;
	
public:
	TimeDay();
	~TimeDay();

	void Update(float deltaTime);
	void SetTime(int hour, float minute);
	string GetTimeString();
};
#endif

