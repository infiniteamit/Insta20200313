#include "Ripple.h"

//--------------------------------------------------------------
Ripple::Ripple() : Ripple(glm::vec3()) {

}

//--------------------------------------------------------------
Ripple::Ripple(glm::vec3 location) {

	this->location = location;
	this->radius = 1;
	this->radius_max = location.z / 10;
}

//--------------------------------------------------------------
void Ripple::update() {

	if (this->location.z > 0) {

		this->location.z = this->location.z > 10 ? this->location.z - 10 : 0;
		return;
	}

	if (this->radius < this->radius_max) {  this->radius += 1; }
}

//--------------------------------------------------------------
void Ripple::draw(ofTrueTypeFont& font, vector<string>& word_list) {

	ofPushMatrix();
	ofTranslate(this->location);

	if (this->location.z > 0) {

		ofSetColor(39);
		ofDrawLine(glm::vec3(), glm::vec3(0, 0, 30));
	}
	else {

		auto alpha = this->radius < this->radius_max * 0.5 ? 255 : ofMap(this->radius, this->radius_max * 0.5, this->radius_max, 255, 10);
		ofSetColor(39, alpha);
		for (int deg = 0; deg < 360; deg += 20) {

			glm::vec2 word_location(this->radius * cos(deg * DEG_TO_RAD), this->radius * sin(deg * DEG_TO_RAD));
			auto noise_seed_location = glm::vec2(this->location) + word_location;
			int word_index = ofMap(ofNoise(glm::vec3(noise_seed_location * 0.003, ofGetFrameNum() * 0.001)), 0, 1, 0, word_list.size());

			ofPushMatrix();
			ofTranslate(word_location);
			ofRotateZ(deg + 90);

			font.drawString(word_list[word_index], -10.5, 10);

			ofPopMatrix();
		}
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
bool Ripple::isDead() {

	return this->radius >= this->radius_max;
}