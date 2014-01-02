#include "testApp.h"
#include "ofGstVideoPlayer.h"

//--------------------------------------------------------------
void testApp::setup(){
	video.setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// set parameter names
	parameters.setName("testApp");
	parameter_video_path.setName("video_path");
	parameters.add(parameter_video_path);

	// register listeners
	gui_button_open_file.addListener(this, &testApp::buttonOpenFilePressed);

	// setup gui.
	gui_settings.setup(parameters, "settings.xml");
	gui_settings.add(gui_button_open_file.setup("Open File"));
	gui_settings.add(gui_label_file_name.setup("File Name", "<No file loaded>"));

	gui_settings.loadFromFile("settings.xml");

	// load video
	if (ofFile::doesFileExist(parameter_video_path)) {
		loadVideoFile(parameter_video_path);
	}
}

void testApp::exit() {
	gui_button_open_file.removeListener(this, &testApp::buttonOpenFilePressed);
	video.close();
}

//--------------------------------------------------------------
void testApp::update(){
	if(video.isLoaded()) {
		video.update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);

	// draw video
	if(video.isLoaded()) {
		video.draw(250, 20);
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
