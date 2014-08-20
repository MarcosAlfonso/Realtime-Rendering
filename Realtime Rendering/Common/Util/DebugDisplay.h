#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "text2D.h"
#include <string>

class DebugDisplay
{
	
public:
	DebugDisplay();
	DebugDisplay(glm::vec2 _pos, bool isTimed);
	~DebugDisplay();

	void addDebug(std::string str, float lifespan);
	void addDebug(std::string str);
	void Draw();


private:
	std::vector<std::string> strings;
	std::vector<float> lifespans;
	std::vector<double> birthtimes;

	glm::vec2 pos;
	int count = 0;
	bool isTimed;

};

