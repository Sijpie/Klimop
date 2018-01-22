#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
//arduino
	ofAddListener(arduino.EInitialized, this,
		&ofApp::setupArduino);
	
	arduino.connect("COM4");
	arduino.sendFirmwareVersionRequest();
	//super belangrijk om te connecten	

	Servo = 9;

	ofSetLogLevel(OF_LOG_NOTICE);

//blobtracker
	grabber.listDevices();
	grabber.setDeviceID(0);
	//webcam selecteren

	grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);

	rgbImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
	hsvImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
	hue.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
	saturation.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
	value.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
	filtered.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
	filtered2.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);


}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version)
{
	ofLog() << "Arduino firmware found" << arduino.getFirmwareName()
		<< arduino.getMajorFirmwareVersion()
		<< arduino.getMinorFirmwareVersion()
		<< endl;
	//checken of arduino goed opstart enz

	//arduino.sendDigitalPinMode(9, ARD_OUTPUT); //digitaal output servo
	arduino.sendServoAttach(9);
}

//--------------------------------------------------------------
void ofApp::update() {
	arduino.update();
	//communiceren met arduino - checken met arduino

	grabber.update();
	if (grabber.isFrameNew()) {
		rgbImage.setFromPixels(grabber.getPixels().getData(), GRABBER_WIDTH, GRABBER_HEIGHT);
		//geef me de pixels binnen de grab. If == doet dit alleen als er een nieuwe frame binnenkomt om te veel werk te voorkomen

		rgbImage.mirror(false, true);

		hsvImage = rgbImage;
		hsvImage.convertRgbToHsv();

		hsvImage.convertToGrayscalePlanarImages(hue, saturation, value);

		if (testBool == true) {
			for (int i = 0; i < GRABBER_WIDTH*GRABBER_HEIGHT; i++) {
				if (ofInRange(hue.getPixels()[i], findHue - HUE_MARGIN, findHue + HUE_MARGIN)) {
					filtered.getPixels()[i] = 255;
				}
				else {
					filtered.getPixels()[i] = 0;
				}
			}
			filtered.flagImageChanged();

			contours.findContours(filtered, MIN_SIZE, GRABBER_WIDTH * GRABBER_HEIGHT, 1, false);
		}
		if (testBool == false) {
			for (int i = 0; i < GRABBER_WIDTH*GRABBER_HEIGHT; i++) {
				if (ofInRange(hue.getPixels()[i], findHue2 - HUE_MARGIN, findHue2 + HUE_MARGIN)) {
					filtered2.getPixels()[i] = 255;
				}
				else {
					filtered2.getPixels()[i] = 0;
				}

			}
			filtered2.flagImageChanged();

			contours2.findContours(filtered2, MIN_SIZE, GRABBER_WIDTH * GRABBER_HEIGHT, 1, false);
			//blob minimaal 50 groot, en maximaal helft van het scherm			

		}

	}



}


//--------------------------------------------------------------
void ofApp::draw() {
	if (showVideo) {
		rgbImage.draw(0, 0, ofGetWidth(), ofGetHeight());
	}

	if (showHSVComponents) {
		hsvImage.draw(0, 0);
		hue.draw(0, 240);
		saturation.draw(320, 240);
		value.draw(640, 240);
	}

	if (showFiltered) {
		filtered.draw(0, 480);
		if (showContours) {
			contours.draw(0, 480);
		}
	}

	ofSetColor(ofColor::blue, 100);
	ofFill();
	for (int i = 0; i < contours.blobs.size(); i++) {
		ofDrawCircle(contours.blobs[i].centroid.x * ofGetWidth() / GRABBER_WIDTH,
			contours.blobs[i].centroid.y * ofGetHeight() / GRABBER_HEIGHT, 20);


		//smoothing

//		blobx = blobx * 0.95 + contours.blobs[i].centroid.x * 0.05;
	

		//int blobpos = contours.blobs[i].centroid.x;
		//
		//newblobpos.resize(5);

		//for (int n = 0; n < newblobpos.size(); n++) {
		//	blobpos[n] = 0;
		//}

		//total -= blobpos;
		//blobpos = contours.blobs[i].centroid.x;
		//total += blobpos;
		//newblobpos = newblobpos + 1;
		//
		//
		//
	
		//// subtract the last reading:
		//total = total - readings[readIndex];
		//// read from the sensor:
		//readings[readIndex] = analogRead(inputPin);
		//// add the reading to the total:
		//total = total + readings[readIndex];
		//// advance to the next position in the array:
		//readIndex = readIndex + 1;

		//// if we're at the end of the array...
		//if (readIndex >= numReadings) {
		//	// ...wrap around to the beginning:
		//	readIndex = 0;
		//}

		//// calculate the average:
		//average = total / numReadings;


	

//arduino
		newx = ofMap(contours.blobs[i].centroid.x, 0, GRABBER_WIDTH, 0, 180);
		arduino.sendServo(9, newx);
		/*newx = ofMap(blobx, 0, GRABBER_WIDTH, 0, 180);
		arduino.sendServo(9, newx);*/

		ofLog() << newx << endl;

	}
	
	
	ofSetColor(ofColor::white);


}

//void ofApp::newHue()
//{
//	hue1 = findHue;
//	hue2 = findHue2;
//
//}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'h') {
		showHSVComponents = !showHSVComponents;
	}
	else if (key == 'v') {
		showVideo = !showVideo;
	}
	else if (key == 'c') {
		showContours = !showContours;
	}
	else if (key == 'f') {
		showFiltered = !showFiltered;
	}
	else if (key == 'i') {
		testBool = !testBool;
		ofLog() << "testBool =" << testBool;
	}
}


void ofApp::mousePressed(int x, int y, int button) {
	if (testBool == true) {
		findHue = hue.getPixels()[y * GRABBER_WIDTH + x];
		ofLog() << "true hue 1 =" << findHue;
	}

	if (testBool == false) {
		findHue2 = hue.getPixels()[y * GRABBER_WIDTH + x];
		ofLog() << "false hue 2 =" << findHue2;
	}



}



