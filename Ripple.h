#pragma once
#include "ofMain.h"

class Ripple
{
public:

	Ripple();
	Ripple(glm::vec3 location);
	void update();
	void draw(ofTrueTypeFont& font, vector<string>& word_list);
	bool isDead();
private:

	glm::vec3 location;
	float radius;
	float radius_max;
	float radius_span;
};