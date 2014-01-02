#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{

	public:
		testApp() 
			: video_control_playing(false)
		{ }

		void setup();
		void exit();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	private:
		ofxCv::FlowFarneback farneback;
		ofxCv::FlowPyrLK pyrLk;
	
		ofxCv::Flow* curFlow;
		ofVideoPlayer video;

		bool video_control_playing;

		ofxPanel gui_settings;
		ofxButton gui_button_open_file;
		ofxLabel gui_label_file_name;

		ofParameterGroup parameters;
		ofParameter<std::string> parameter_video_path;

		void loadVideoFile(std::string file_path);
		void buttonOpenFilePressed();
};
