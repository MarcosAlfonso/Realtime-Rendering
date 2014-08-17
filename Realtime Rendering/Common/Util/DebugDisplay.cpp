#include "DebugDisplay.h"
//#include "text2D.h"

DebugDisplay::DebugDisplay(glm::vec2 _pos, bool _isTimed)
{
	pos = _pos;
	isTimed = _isTimed;
	strings.reserve(100);
}

void DebugDisplay::addDebug(std::string str, float age)
{
	DebugEntry * debug = new DebugEntry(str, glfwGetTime(), age);
	strings.push_back(*debug);
	count++;
}

void DebugDisplay::addDebug(std::string str)
{
	DebugEntry * debug = new DebugEntry(str, -1, -1);
	strings.push_back(*debug);
	count++;
}

void DebugDisplay::Draw()
{
	for (int i = 0; i < count; i++)
	{
		printText2D(strings[i].str.c_str(), pos.x, pos.y-(i*26), 26);

		if (isTimed && glfwGetTime() - strings[i].birthTime > strings[i].age)
		{
			strings.erase(strings.begin() + i);
			count--;
		}
	}

	if (!isTimed)
	{
		strings.clear();
		count = 0;
	}
}