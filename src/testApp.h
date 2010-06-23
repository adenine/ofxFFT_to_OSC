#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFft.h"
#include "ofxOsc.h"

#define MIC 0
#define NOISE 1
#define SINE 2

#define HOST "localhost"
#define PORT 1200

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		// ofxFft related
		void plot(float* array, int length, float scale, float offset, bool makeCircle);
		void audioReceived(float* input, int bufferSize, int nChannels);
		
	
		int plotHeight, bufferSize;
		ofxFft* fft;
		float* audioInput;
		float* fftOutput;
		float* eqFunction;
		float* eqOutput;
		float* ifftOutput;
		
		// ofxFft related app values
		float appWidth;
		float appHeight;
		ofImage spectrogram;
		int spectrogramOffset;
		int mode;
		
		// OSC related
		ofxOscSender sender;
		void sendTheBeat();
		void sendMessage(string _theMessage);
};

#endif
