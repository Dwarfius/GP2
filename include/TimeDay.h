#ifndef _TIMEDAY_H
#define _TIMEDAY_H
#include "common.h"
#include "BaseComponent.h"
#include "Font.h"
#include <iomanip>

class TimeDay : public BaseComponent
{
private:
	float timeMinute;
	int timeHour;
	Font* font;
	float timeScale;

public:
	TimeDay(Font* f);
	~TimeDay();

	void Update(float deltaTime);
	void OnRender(Camera *camera);

	int GetHour() { return timeHour; }
	float GetMinute() { return timeMinute; }

	void SetTime(int hour, float minute);
	string GetTimeString();
};
#endif

