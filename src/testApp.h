#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{

	public:
		testApp() : 
			video_control_playing(false)
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

		ofParameter<bool> parameter_use_pyrlk;
		ofParameter<bool> parameter_use_farneback;

		ofxPanel gui_settings_farneback;
		ofParameter<float> parameter_farneback_pyramid_scale;
		ofParameter<int> parameter_farneback_num_levels;

		ofxPanel gui_settings_pyrlk;
		ofxButton gui_button_reset_features;

		void loadVideoFile(std::string file_path);

		void buttonOpenFilePressed();
		void buttonResetFeaturesPressed();

		void toggleButtonFarneback(bool& value);
		void toggleButtonPyrlk(bool& value);

		void fixupMethodGroups();
};
