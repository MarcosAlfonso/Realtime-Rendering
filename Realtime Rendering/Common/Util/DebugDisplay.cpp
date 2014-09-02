#include "DebugDisplay.h"
//#include "text2D.h"

DebugDisplay::DebugDisplay(glm::vec2 _pos, bool _isTimed)
{
	pos = _pos;
	isTimed = _isTimed;
	strings.reserve(100);
}

DebugDisplay::~DebugDisplay()
{
}

void DebugDisplay::addDebug(std::string str, float lifespan)
{
	strings.push_back(str);
	lifespans.push_back(lifespan);
	birthtimes.push_back(glfwGetTime());
	count++;
}

void DebugDisplay::addDebug(std::string str)
{
	strings.push_back(str);
	lifespans.push_back(-1);
	birthtimes.push_back(-1);
	count++;
}

void DebugDisplay::Draw()
{
	for (int i = 0; i < count; i++)
	{
		printText2D(strings[i].c_str(), pos.x, pos.y-(i*26), 25);

		if (isTimed && glfwGetTime() - birthtimes[i] > lifespans[i])
		{
			strings.erase(strings.begin() + i);
			lifespans.erase(lifespans.begin() + i);
			birthtimes.erase(birthtimes.begin() + i);
			count--;
		}
	}

	if (!isTimed)
	{
		strings.clear();
		lifespans.clear();
		birthtimes.clear();
		count = 0;
	}
}