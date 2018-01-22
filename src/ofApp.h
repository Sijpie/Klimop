#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include <vector>

#define GRABBER_WIDTH 320
#define GRABBER_HEIGHT 240
#define HUE_MARGIN 5
#define MIN_SIZE 50


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	/*void newHue();*/

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);

//arduino
	ofArduino arduino;
	//openframeworks arduino class

	int Servo;
	//float x = centroid.x;
	int newx = 0;
	// 0 <= x < 10

	void setupArduino(const int& version);
	

//blobtracker
	ofVideoGrabber grabber;

	ofxCvColorImage rgbImage;
	ofxCvColorImage hsvImage;
	//hue saturation value (brightness)
	ofxCvGrayscaleImage hue;
	ofxCvGrayscaleImage saturation;
	ofxCvGrayscaleImage value;

	ofxCvGrayscaleImage filtered;
	ofxCvGrayscaleImage filtered2;

	ofxCvContourFinder contours;
	ofxCvContourFinder contours2;

	int findHue;
	int findHue2;

	bool showHSVComponents = false;
	bool showVideo = true;
	bool showFiltered = true;
	bool showContours = true;

	bool testBool = true;

//smoothing
//	int blobx = 0;
	
	//int readings = 0;  // the readings from the analog input
	//int readIndex = 0; // positie in de lijst
	//int total = 0;     // the running total
	//int average = 0;   // the average

	////int arraysize = 5;

	//vector<int> newblobpos;




};
