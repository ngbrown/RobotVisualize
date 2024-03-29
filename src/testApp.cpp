#include "testApp.h"
#include "ofGstVideoPlayer.h"

//--------------------------------------------------------------
void testApp::setup(){
	video.setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// use ofParameterGroup so we can set parameters not directly exposed on GUI
	parameters.setName("testApp");
	parameters.add(parameter_video_path.set("video_path", ""));

	// register listeners
	gui_button_open_file.addListener(this, &testApp::buttonOpenFilePressed);
	gui_button_reset_features.addListener(this, &testApp::buttonResetFeaturesPressed);
	parameter_use_farneback.addListener(this, &testApp::toggleButtonFarneback);
	parameter_use_pyrlk.addListener(this, &testApp::toggleButtonPyrlk);

		// setup gui.
	gui_settings.setup(parameters, "settings.xml");
	gui_settings.add(gui_button_open_file.setup("Open File"));
	gui_settings.add(gui_label_file_name.setup("File Name", "<No file loaded>"));

	gui_settings.add(gui_settings_farneback.setup("Farneback"));
	gui_settings_farneback.add(parameter_farneback_pyramid_scale.set("pyramid_scale", 0.5, 0, 1));
	gui_settings_farneback.add(parameter_farneback_num_levels.set("num_levels", 4, 1, 8));
	gui_settings.add(parameter_use_farneback.set("use_farneback", true));

	gui_settings.add(gui_settings_pyrlk.setup("Pyramid Lucas-Kanade"));
	gui_settings_pyrlk.add(gui_button_reset_features.setup("Reset Features"));
	gui_settings.add(parameter_use_pyrlk.set("use_pyrlk", false));


	gui_settings.loadFromFile("settings.xml");

	// load video
	if (ofFile::doesFileExist(parameter_video_path)) {
		loadVideoFile(parameter_video_path);
	}

	curFlow = &farneback;
}

void testApp::exit() {
	gui_button_open_file.removeListener(this, &testApp::buttonOpenFilePressed);
	video.close();
}

//--------------------------------------------------------------
void testApp::update(){
	if(video.isLoaded()) {
		video.update();

		if (video.isFrameNew()) {
			if (parameter_use_farneback) {
				curFlow = &farneback;
				farneback.setPyramidScale(parameter_farneback_pyramid_scale);
				farneback.setNumLevels(parameter_farneback_num_levels);
			} else if (parameter_use_pyrlk) {
				curFlow = &pyrLk;
			}

			curFlow->calcOpticalFlow(video);
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);

	// draw video
	if(video.isLoaded()) {
		ofRectangle video_rectangle(250, 20, 320, 240);
		video.draw(video_rectangle);
		curFlow->draw(video_rectangle);
	}

	// draw gui
	gui_settings.draw();

	// draw frame rate
    ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString("framerate: " + ofToString(ofGetFrameRate()), ofGetWindowWidth() - 150, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		case ' ':
			video_control_playing = !video_control_playing;
			video.setPaused(!video_control_playing);
			break;
        case OF_KEY_RIGHT:
            video.nextFrame();
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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::buttonOpenFilePressed() {
	ofLogNotice("testApp") << "Open File started";
	ofFileDialogResult result = ofSystemLoadDialog("Select Movie File", false, parameter_video_path);

	if (result.bSuccess == false) {
		ofLogWarning("testApp") << "Open File failed";
		gui_label_file_name = "<No file loaded>";
	} else {
		parameter_video_path = result.getPath();

		loadVideoFile(result.getPath());
	}
}

void testApp::buttonResetFeaturesPressed() {
	pyrLk.resetFeaturesToTrack();
}

void testApp::toggleButtonFarneback(bool& value) {
	if (value == true) {
		parameter_use_pyrlk = !value;
		fixupMethodGroups();
	}
}

void testApp::toggleButtonPyrlk(bool& value) {
	if (value == true) {
		parameter_use_farneback = !value;
		fixupMethodGroups();
	}
}

void testApp::fixupMethodGroups() {
	if (parameter_use_farneback) {
		gui_settings_farneback.maximize();
		gui_settings_pyrlk.minimize();
	} else if (parameter_use_pyrlk) {
		gui_settings_pyrlk.maximize();
		gui_settings_farneback.minimize();
	}
}

void testApp::loadVideoFile(std::string file_path) {
	ofLogNotice("testApp") << "Loading video \"" << file_path << "\"";

	gui_label_file_name = ofFilePath::getFileName(file_path);

	video.loadMovie(file_path);

	if(video.isLoaded()) {
		ofLogNotice("testApp") << "Video loaded";
	} else {
		ofLogWarning("testApp") << "Video loading failed";
		gui_label_file_name = "<No file loaded>";
	}
}
