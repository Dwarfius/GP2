#ifndef _TIMEDAY_H
#define _TIMEDAY_H
#include "common.h"
#include "BaseComponent.h"
#include "Font.h"
#include <iomanip>

class TimeDay : public BaseComponent
{
private:
	float timeScale;
	float timeMinute;
	int timeHour;
	Font* font;
	
public:
	TimeDay(Font* f);
	~TimeDay();

	void Update(float deltaTime);
	void OnRender(Camera *camera);

	void SetTime(int hour, float minute);
	string GetTimeString();
};
#endif

