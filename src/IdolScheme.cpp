#include "../include/IdolScheme.h"

////////////////////////////////////////////////////////////////
   /////////////////      Initialization      ///////////////
////////////////////////////////////////////////////////////////

void IdolScheme::setup() {
	ofBackground(45, 45, 190, 255);
	IdolScheme_State = GAME_ACTIVE;

	ofTrueTypeFont::setGlobalDpi(60);
	textOut.load("verdana.ttf", 20, true, false);
	textOut.setLetterSpacing(1.037);

	/// debug beatmap picker
	/// --------------------
	beatMap currBeatMap = bmh.beatMapMenu();

	mainConductor._bpm = currBeatMap.noteParams.at(0).bpm;
	mainConductor._offsetInMs = currBeatMap.noteParams.at(0).offset;
	mainConductor._lengthInS = currBeatMap.noteParams.at(0).length;
	//note.setup(ofPoint(200, -100), ofPoint(400, 300), BUTTON, BUTTON_A);
	
	cout << "type the degree of the note as a decimal\n\n> ";
	float choice;
	cin >> choice;

	note.setup(
		ofPoint(
			note.calcPointsFromAngle(choice).x,
			note.calcPointsFromAngle(choice).y
		),
		ofPoint(ofGetWidth() / 2, ofGetHeight() / 2)
	);

	mainConductor.startTimer();
	
	//note.setup(ofPoint(0, (ofGetHeight())),ofPoint(0, 0),BUTTON,BUTTON_A);
	note.calcNoteParams();
	// something is going on with the formula and it doesn't work properly
	// unless the note is in its sepcial position.
}


///////////////////////////////////////////////////////////////
   //////////////////      Game Loop      //////////////////
///////////////////////////////////////////////////////////////

void IdolScheme::update() {
	mainConductor.refreshMembers();
	mainConductor.beatSinceRefresh = mainConductor.currBeat;
	
	//note.moveByBeats(mainConductor.numBeatsSinceRefresh);

#ifdef TARGET_LINUX
	int w = ofGetWidth();
	int h = ofGetHeight();
	if(w != optionMenu.getWinHeight() || h != optionMenu.getWinWidth())
		ofSetWindowShape(optionMenu.getWinWidth(), optionMenu.getWinHeight());
#endif
}

//--------------------------------------------------------------
void IdolScheme::draw() {
	string currBeatString = "Current beat: " + ofToString(mainConductor.currBeat) + "/" + ofToString(mainConductor.totalBeats);
	textOut.drawString(currBeatString, 10, 20);
	string lengthString = "Current time: " + ofToString(((float) mainConductor.timeDiff.count()) / 1000) + "/" + ofToString(mainConductor._lengthInS);
	textOut.drawString(lengthString, 10, 40);
	string beatDiffString = "Beats since last refresh: " + ofToString(mainConductor.numBeatsSinceRefresh);
	textOut.drawString(beatDiffString, 10, 60);
	textOut.drawString(ofToString(ofGetFrameRate()), 10, 80);


//	note.draw(-150, note.notey, -150, 0);
	note.moveByBeats(mainConductor.numBeatsSinceRefresh);
	//note.draw(note.notex, note.notey, (ofGetWidth() / 2), (ofGetHeight() / 2));
	//note.draw(0.5 * ofGetWidth(), note.notey, (ofGetWidth() / 2), (ofGetHeight() / 2));

	if(optionMenuShow)
		optionMenu.draw();
}

///////////////////////////////////////////////////////////////
   ///////////////      Misc. Functions      ///////////////
///////////////////////////////////////////////////////////////

float IdolScheme::qSetHCoord(float coordinate) {
	return coordinate * ofGetHeight();
}

//--------------------------------------------------------------
float IdolScheme::qSetWCoord(float coordinate) {
	return coordinate * ofGetWidth();
}

//--------------------------------------------------------------
ofPoint IdolScheme::qSetCoords(ofPoint coordinates) {
	return ofPoint(coordinates.x * ofGetWidth(), coordinates.y * ofGetHeight());
}

///////////////////////////////////////////////////////////////
   //////////////////      Callbacks      //////////////////
///////////////////////////////////////////////////////////////

void IdolScheme::keyPressed(int key) {
	switch (key) {
		case 'o':
			optionMenuShow = !optionMenuShow;
			break;
	}
}

//--------------------------------------------------------------
void IdolScheme::keyReleased(int key) {

}

//--------------------------------------------------------------
void IdolScheme::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void IdolScheme::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void IdolScheme::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void IdolScheme::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void IdolScheme::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void IdolScheme::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void IdolScheme::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void IdolScheme::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void IdolScheme::dragEvent(ofDragInfo dragInfo) {

}
