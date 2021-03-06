#include "../include/Note.h"
#include <complex>
////////////////////////////////////////////////////////////////////////////////////////
       //////////////////      Constructors/Desconstructors      //////////////////
////////////////////////////////////////////////////////////////////////////////////////

Note::Note() = default;

Note::Note(ofPoint initCoords, ofPoint shadowCoords, const NoteType type, const NoteButton button) 
	: notex(initCoords[0]), notey(initCoords[1]), shadowX(shadowCoords[0]), shadowY(shadowCoords[1]) {
	noteSettings.type = type;
	noteSettings.button = button;
	noteSprite.load(spriteDir);
	init();
}

//----------------------------------------------------------------------------------
Note::Note(ofPoint initCoords, ofPoint shadowCoords, const NoteType type) 
	: notex(initCoords[0]), notey(initCoords[1]), shadowX(shadowCoords[0]), shadowY(shadowCoords[1]) {
	noteSettings.type = type;
	noteSprite.load(spriteDir);
	init();
}

//----------------------------------------------------------------------------------
void Note::setBeatRest(const NoteInfo& settings) {
	noteSettings = settings;
	init();
}

//----------------------------------------------------------------------------------
void Note::setBeatNote(const NoteInfo& settings) {
	noteSettings = settings;
	const ofPoint setupParam = calcPolarPoint(noteSettings.angle);
	setup(
		ofPoint(setupParam.x, setupParam.y),
		ofPoint(noteSettings.xS * ofGetWidth(), noteSettings.yS * ofGetHeight()),
		noteSettings.type, noteSettings.button
	);
}

//----------------------------------------------------------------------------------
void Note::setup(ofPoint initCoords, ofPoint shadowCoords, const NoteType type, const NoteButton button) {
	setPosition(initCoords[0], initCoords[1], shadowCoords[0], shadowCoords[1]);

	noteSprite.load(spriteDir);
	
	noteSettings.type = type;
	noteSettings.button = button;
	init();
}

//----------------------------------------------------------------------------------
Note::~Note() {
	destroy();
}

//----------------------------------------------------------------------------------
bool Note::destroy() {
	shadow = nullptr;
	delete shadow;

	return false;
}

//----------------------------------------------------------------------------------
void Note::init() {
	shadow = new Shadow(); // creates the shadow.
	shadow->load(); // loads the shadow image.

	/// Setting the anchor point to the center of the note
	/// --------------------------------------------------
	noteSprite.setAnchorPercent(.5, .5);

	noteSprite.resize(noteSize, noteSize); // resizing the note.
	shadow->resize(noteSize, noteSize); // resizing the shadow.
}



///////////////////////////////////////////////////////////////
   //////////////////      Functions      //////////////////
///////////////////////////////////////////////////////////////

void Note::hit(const NoteButton buttonPressed) const {
	/// This is when the user pressed the assigned note button.
	/// -------------------------------------------------------
    if (buttonPressed != noteSettings.button) {
        scoreKeeper->wrongCount += 1;
        return;
    }

    updateScoreKeeper();
}

void Note::updateScoreKeeper() const {
	const auto accuracy = assessAccuracy(conductor->currBeat, noteSettings.noteNum); 
	switch(accuracy) {
		case WONDERFUL:
		    scoreKeeper->wonderfulCount += 1;
		    scoreKeeper->score += 1000;
		    break;
		case GOOD:
		    scoreKeeper->goodCount += 1;
		    scoreKeeper->score += 500;
		    break;
		case MEDIOCRE:
		    scoreKeeper->mediocreCount += 1;
		    scoreKeeper->score += 100;
		    break;
		case BAD:
		    scoreKeeper->badCount += 1;
		    scoreKeeper->score += 50;
		    break;
		case GARBAGE:
		    scoreKeeper->garbageCount += 1;
		    break;
		default:
		    break;
	}
}

//----------------------------------------------------------------------------------
void Note::draw(const GLfloat nX, const GLfloat nY, const GLfloat sX, const GLfloat sY) const {
	/// checking if the x and y parameters are present.
	/// -----------------------------------------------
	if (nX != BAD_COORDINATE && nY != BAD_COORDINATE && sX != BAD_COORDINATE && sY != BAD_COORDINATE) {
		getShadow()->draw(sX, sY);
		noteSprite.draw(nX, nY);
	} else {
		getShadow()->draw(shadowX, shadowY);
		noteSprite.draw(notex, notey);
	}
}

//----------------------------------------------------------------------------------
void Note::moveByBeats(const GLfloat currBeat) {
	const auto time = currBeat - noteSettings.noteNum;
	
	notex = startPos.x + time * distToShadow.x;
	notey = startPos.y + time * distToShadow.y;

	/// draw the notes after calculating the positions
	ofSetColor(noteColor);
	draw(notex, notey, shadowX, shadowY);
	ofSetColor(255, 255, 255, 255); // sets color to normal
}

//----------------------------------------------------------------------------------
void Note::calcNoteParams() {
	startPos.x = notex;
	startPos.y = notey;
	distToShadow.x = shadowX - notex;
	distToShadow.y = shadowY - notey;
}

//----------------------------------------------------------------------------------
void Note::setColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) {
	noteColor.r = r;
	noteColor.g = g;
	noteColor.b = b;
	noteColor.a = a;
}

//----------------------------------------------------------------------------------
ofColor Note::getColor() const {
	return noteColor;
}

//----------------------------------------------------------------------------------
void Note::setPosition(const GLfloat x, const GLfloat y, const GLfloat xS, const GLfloat yS) {
	notex   = x;
	notey   = y;
	shadowX = xS;
	shadowY = yS;
}

//----------------------------------------------------------------------------------
void Note::setSize(const GLint newSize) {
	noteSize = newSize;

	/// note resizing
	/// -------------
	noteSprite.load(spriteDir); // reloads image
	noteSprite.resize(noteSize, noteSize);
	
	/// shadow resizing
	/// ---------------
	getShadow()->shadowSprite.load(getShadow()->shadowDir); // reloads image
	getShadow()->shadowSprite.resize(noteSize, noteSize);
}

//----------------------------------------------------------------------------------
Shadow *Note::getShadow() const {
	return shadow; // returns the shadow object attached to the note.
}


//----------------------------------------------------------------------------------
ofPoint Note::calcPolarPoint(float angle) {
	ofPoint result;
	auto halfC = 3.14169f / 180.f; // modifier to turn degrees to radians
	float zMod, off = 0;

	using namespace placeholders;
	
	// lambdas for setting the coordinates of the result ofPoint
	auto setResult = [&](float offset, float offset2, int val, int val2) {
		result = ofPoint(offset + float(val), offset2 + float(val2));
	};
	
	auto sideCheck = [](float a) {
		return ((a >= 60 && a < 120) ? 1 : (a >= 240 && a < 300) ? 2 : (a >= 120 && a < 240) ? 3 : 4);
	};
	
	const auto mag = [&]() {
		if (sideCheck(angle) <= 2) { zMod = static_cast<float>(ofGetWidth()) / 2; off = 90 * halfC * (sideCheck(angle) == 2 ? 3 : 1); }
		else { zMod = static_cast<float>(ofGetHeight()) / 2; off = 0; }
		return tan((angle * halfC) - off) * (zMod + noteSize);
	};
	
	auto sideSet = bind(setResult, _1, static_cast<float>(ofGetHeight())/2.f, noteSize, _2);
	auto normSet = bind(setResult, static_cast<float>(ofGetWidth())/2.f, _1, _2, noteSize);
	
	if(sideCheck(angle) == 2)	   sideSet(static_cast<float>((-1*(noteSize * 2))), static_cast<int>((-1*(mag()))));
	else if(sideCheck(angle) == 1) sideSet(static_cast<float>(ofGetWidth()),		static_cast<int>(mag()));
	else if(sideCheck(angle) == 3) normSet(static_cast<float>(ofGetHeight()),	    static_cast<int>((-1*(mag()))));
	else /*wowowowowowowowowowow*/ normSet(static_cast<float>((-1*(noteSize * 2))), static_cast<int>(mag()));
	
	return result;
}