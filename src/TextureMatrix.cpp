#include <iostream>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "TextureMatrix.h"

using namespace std;
using namespace glm;

TextureMatrix::TextureMatrix()
{
	type = Type::NONE;
	T = mat3(1.0f);
}

TextureMatrix::~TextureMatrix()
{
	
}

void TextureMatrix::setType(const string &str)
{
	if(str.find("Body") != string::npos) {
		type = Type::BODY;
	} else if(str.find("Mouth") != string::npos) {
		type = Type::MOUTH;
	} else if(str.find("Eyes") != string::npos) {
		type = Type::EYES;
	} else if(str.find("Brows") != string::npos) {
		type = Type::BROWS;
	} else {
		type = Type::NONE;
	}
}

void TextureMatrix::update(unsigned int key)
{
	// Update T here
	if(type == Type::BODY) {
		// Do nothing
	} else if(type == Type::EYES) {
		if ((unsigned)'e' == key) { // horizontal : 3
			eyeRL++;
			if (eyeRL == 3) {
				eyeRL = 0;
			}
			T[2].x = (0.2f * eyeRL);
		}
		else if ((unsigned)'E' == key) { // vertical : 10
			eyeUD--;
			if (eyeUD == 0) {
				eyeUD = 10;
			}
			T[2].y = (0.1f * eyeUD);
		}
	} else if(type == Type::MOUTH) {
		if ((unsigned)'m' == key) {
			mouthRL++;
			if (mouthRL == 3) {
				mouthRL = 0;
			}
			T[2].x = (0.1f * mouthRL);
		}
		else if ((unsigned)'M' == key) {
			mouthUD--;
			if (mouthUD == 0) {
				mouthUD = 10;
			}
			T[2].y = (0.1f * mouthUD);
		}
	} else if(type == Type::BROWS) {
		if ((unsigned)'b' == key) {
			browUD--;
			if (browUD == 0) {
				browUD = 10;
			}
			T[2].y = (0.1f * browUD);
		}
	}
}
