#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofNoFill();

	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", 20);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	this->word_list = {

		u8"!", u8"#", u8"$", u8"%", u8"&",
		u8"'", u8"(", u8")", u8"*", u8"+",
		u8",", u8"-", u8".", u8"/", u8"0",
		u8"1", u8"2", u8"3", u8"4", u8"5",
		u8"6", u8"7", u8"8", u8"9", u8"0",
		u8":", u8";", u8"<", u8"=", u8">",
		u8"?", u8"@",

		u8"A", u8"B", u8"C", u8"D", u8"E",
		u8"F", u8"G", u8"H", u8"I", u8"J",
		u8"K", u8"L", u8"M", u8"N", u8"O",
		u8"P", u8"Q", u8"R", u8"S", u8"T",
		u8"U", u8"V", u8"W", u8"X", u8"Y", u8"Z",

		u8"a", u8"b", u8"c", u8"d", u8"e",
		u8"f", u8"g", u8"h", u8"i", u8"j",
		u8"k", u8"l", u8"m", u8"n", u8"o",
		u8"p", u8"q", u8"r", u8"s", u8"t",
		u8"u", u8"v", u8"w", u8"x", u8"y", u8"z",

		u8"ｱ", u8"ｲ", u8"ｳ", u8"ｴ", u8"ｵ",
		u8"ｶ", u8"ｷ", u8"ｸ", u8"ｹ", u8"ｺ",
		u8"ｻ", u8"ｼ", u8"ｽ", u8"ｾ", u8"ｿ",
		u8"ﾀ", u8"ﾁ", u8"ﾂ", u8"ﾃ", u8"ﾄ",
		u8"ﾅ", u8"ﾆ", u8"ﾇ", u8"ﾈ", u8"ﾉ",
		u8"ﾊ", u8"ﾋ", u8"ﾌ", u8"ﾍ", u8"ﾎ",
		u8"ﾏ", u8"ﾐ", u8"ﾑ", u8"ﾒ", u8"ﾓ",
		u8"ﾔ", u8"ﾕ", u8"ﾖ",
		u8"ﾗ", u8"ﾘ", u8"ﾙ", u8"ﾚ", u8"ﾛ",
		u8"ﾜ", u8"ｦ", u8"ﾝ",
	};

	this->fbo.allocate(ofGetWidth() + 25, ofGetHeight() + 25);
	this->fbo.readToPixels(this->pixels);
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 10 == 0) {

		glm::vec3 location(ofRandom(this->fbo.getWidth()), ofRandom(this->fbo.getHeight()), ofRandom(800, 1500));
		unique_ptr<Ripple> ripple(new Ripple(location));
		this->ripple_list.push_back(move(ripple));
	}

	this->fbo.begin();
	ofClear(0);

	for (int i = this->ripple_list.size() - 1; i >= 0; i--) {

		if (this->ripple_list[i]->isDead()) {

			this->ripple_list.erase(this->ripple_list.begin() + i);
		}
		else {

			this->ripple_list[i]->update();
			this->ripple_list[i]->draw(this->font, this->word_list);
		}
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	cv::Mat gray_mat;
	cv::cvtColor(this->pixels_mat, gray_mat, cv::COLOR_RGBA2GRAY);
	for (int y = 0; y < this->pixels_mat.cols; y++) {

		for (int x = 0; x < this->pixels_mat.rows; x++) {

			if (gray_mat.at<unsigned char>(y, x) > 0) {

				this->pixels_mat.at<cv::Vec4b>(y, x) = cv::Vec4b(gray_mat.at<unsigned char>(y, x), gray_mat.at<unsigned char>(y, x), gray_mat.at<unsigned char>(y, x), this->pixels_mat.at<cv::Vec4b>(y, x)[3]);
			}
		}
	}

	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(19, 19), 10, 10);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);

	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(-5, -5);

	this->fbo.draw(-25, -25);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}