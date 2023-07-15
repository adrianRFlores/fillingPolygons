#pragma once
#include <iostream>
#include <cmath>

struct Vertex2 {
	unsigned int x;
	unsigned int y;

	Vertex2(unsigned int cordX, unsigned int cordY): x(round(cordX)), y(round(cordY)) {}

};