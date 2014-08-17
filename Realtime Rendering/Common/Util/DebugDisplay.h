#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "text2D.h"

class DebugEntry
{

public:
	std::string str;
	double birthTime;
	float age;
	DebugEntry(std::string _str, double _birthTime, float _age)
	{
		str = _str;
		birthTime = _birthTime;
		age = _age;
	}

};

class DebugDisplay
{
	
public:
	DebugDisplay();
	DebugDisplay(glm::vec2 _pos, bool isTimed);
	~DebugDisplay();

	void addDebug(std::string str, float age);
	void addDebug(std::string str);
	void Draw();


private:
	std::vector<DebugEntry> strings;
	glm::vec2 pos;
	int count = 0;
	bool isTimed;

};

