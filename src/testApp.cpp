#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	plotHeight = 128;
	bufferSize = 512;
	
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT);
	// To use with FFTW, try:
	// fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT, OF_FFT_FFTW);
	
	audioInput = new float[bufferSize];
	fftOutput = new float[fft->getBinSize()];
	eqFunction = new float[fft->getBinSize()];
	eqOutput = new float[fft->getBinSize()];
	ifftOutput = new float[bufferSize];
	
	// 0 output channels,
	// 1 input channel
	// 44100 samples per second
	// [bins] samples per buffer
	// 4 num buffers (latency)
	
	// this describes a linear low pass filter
	for(int i = 0; i < fft->getBinSize(); i++)
		eqFunction[i] = (float) (fft->getBinSize() - i) / (float) fft->getBinSize();
	
	mode = SINE;
	appWidth = ofGetWidth();
	appHeight = ofGetHeight();
	
	ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);
	
	// OSC setup
	// open an outgoing connection to HOST:PORT
	sender.setup( HOST, PORT );
	
	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	ofPushMatrix();
	
	glTranslatef(16, 16, 0);
	ofDrawBitmapString("Audio Input", 0, 0);
	plot(audioInput, bufferSize, plotHeight / 2, 0, false);
	
	glTranslatef(0, plotHeight + 16, 0);
	ofDrawBitmapString("FFT Output", 0, 0);
	plot(fftOutput, fft->getBinSize(), -plotHeight, plotHeight / 2, false);
	
	ofPushMatrix();
	glTranslatef(fft->getBinSize(), 0, 0);
	ofDrawBitmapString("EQd FFT Output", 0, 0);
	plot(eqOutput, fft->getBinSize(), -plotHeight, plotHeight / 2, true);
	ofPopMatrix();
	
	glTranslatef(0, plotHeight + 16, 0);
	ofDrawBitmapString("IFFT Output", 0, 0);
	plot(ifftOutput, fft->getSignalSize(), plotHeight / 2, 0, false);
	
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, appWidth - 80, appHeight - 20);
}

void testApp::plot(float* array, int length, float scale, float offset, bool makeCircle) {
	ofNoFill();
	ofRect(0, 0, length, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	
	
	for (int i = 0; i < length; i++) {
		ofVertex(i, array[i] * scale);
		//cout<<"height: "<<array[i]<<endl;
		if (makeCircle == TRUE) {
			if (array[i] > 0.5) {
				ofCircle(500, -50, array[i]*scale);
				sendTheBeat();
			}
		}
	}
	ofEndShape();
	glPopMatrix();
}

void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
	if (mode == MIC) {
		// store input in audioInput buffer
		memcpy(audioInput, input, sizeof(float) * bufferSize);
	} else if (mode == NOISE) {
		for (int i = 0; i < bufferSize; i++)
			audioInput[i] = ofRandom(-1, 1);
	} else if (mode == SINE) {
		for (int i = 0; i < bufferSize; i++)
			audioInput[i] = sinf(PI * i * mouseX / appWidth);
	}
	
	fft->setSignal(audioInput);
	memcpy(fftOutput, fft->getAmplitude(), sizeof(float) * fft->getBinSize());
	
	for(int i = 0; i < fft->getBinSize(); i++)
		eqOutput[i] = fftOutput[i] * eqFunction[i];
	
	fft->setPolar(eqOutput, fft->getPhase());
	
	fft->clampSignal();
	memcpy(ifftOutput, fft->getSignal(), sizeof(float) * fft->getSignalSize());
}

void testApp::sendTheBeat() {
	
	ofxOscMessage m;
	
	m.clear();
	m.setAddress( "/theBeat" );
	m.addFloatArg(ofRandom(0,1));
	sender.sendMessage( m );	
	
}

void testApp::sendMessage(string _theMessage) {
	ofxOscMessage m;
	
	m.clear();
	m.setAddress( "/message" );
	m.addStringArg(_theMessage);
	// b.addMessage(m);		
	// send
	sender.sendMessage( m );
	cout<<"SENT WII MESSAGE"<<endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		case 'm':
			mode = MIC;
			break;
		case 'n':
			mode = NOISE;
			break;
		case 's':
			mode = SINE;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

