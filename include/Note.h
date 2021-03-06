#pragma once

#ifndef NOTE_H
#define NOTE_H

#ifndef OFMAIN_H
	#define OFMAIN_H
	#include "ofMain.h"
#endif //!OFMAIN_H

#ifndef SCOREKEEPER_H
	#include "ScoreKeeper.h"
#endif

#ifndef ERRORCODES_H
	#include "ErrorCodes.h"
#endif //!ERRORCODES_H

#ifndef CONDUCTOR_H
	#define CONDUCTOR_H
    #include "Conductor.h"
#endif //!CONDUCTOR_H

#ifndef NOTEINFO_H
	#include "NoteInfo.h"
#endif //!NOTEINFO_H

#ifndef SHADOW_H
	#include "Shadow.h"
#endif //!SHADOW_H


/* @brief - Note
 * The Note class is what stores
 * all the characteristics that
 * makes a note a note. These
 * include the note size, the
 * note color, and the note image.
 */
class Note {
protected:
	int lengthInBeats = BAD_LENGTH;
	GLint noteSize = (ofGetWindowWidth() + ofGetWindowHeight()) * 0.06; // for width and the height.
	ofColor noteColor = ofColor(255, 255, 255, 255);
	string spriteDir = ofToDataPath("images/note.png"); // directory to note image.
	ofImage noteSprite; // object for sprite.

	ofVec2f startPos;
	ofVec2f distToShadow;
	GLfloat notex{}, notey{}, shadowX{}, shadowY{}; // current positions
	Shadow *shadow{}; // The note shadow. (This object shouldn't move)
public:
	/// Note settings
	/// -------------
	NoteInfo noteSettings;

	/// Pointers
	/// --------
	ScoreKeeper *scoreKeeper{};
	Conductor *conductor{};
	
	/// Constructors/Desconstructor
	/// ---------------------------
	Note();
	Note(ofPoint initCoords, ofPoint shadowCoords, NoteType type, NoteButton button);
	Note(ofPoint initCoords, ofPoint shadowCoords, NoteType type);
	~Note();

	/// Functions
	/// ---------
	void setup(ofPoint initCoords = ofPoint(0,0), ofPoint shadowCoords = ofPoint(0,0),
		NoteType type = BUTTON, NoteButton button = BUTTON_EMPTY);
	void calcNoteParams();
	void setBeatRest(const NoteInfo& settings);
	void setBeatNote(const NoteInfo& settings);
	void setSize(GLint newSize);
	void setPosition(GLfloat x = 0, GLfloat y = 0, GLfloat xS = 0, GLfloat yS = 0);
	void init();
	void draw(GLfloat nX = 0, GLfloat nY = 0, GLfloat sX = 0, GLfloat sY = 0) const; // draws the note and shadow objects.
	void moveByBeats(GLfloat currBeat);
	void hit(NoteButton buttonPressed) const; // For catching key presses.
	void updateScoreKeeper() const;
	void setColor(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0, GLfloat a = 255);
	bool destroy();
	ofColor getColor() const;
	ofPoint calcPolarPoint(float angle);
	Shadow *getShadow() const;
};

#endif //!NOTE_H
