#pragma once
#ifndef SHAPESKIN_H
#define SHAPESKIN_H

#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include "Frame.h"

class MatrixStack;
class Program;
class TextureMatrix;

class ShapeSkin
{
public:
	ShapeSkin();
	virtual ~ShapeSkin();
	void setTextureMatrixType(const std::string &meshName);
	void loadMesh(const std::string &meshName);
	void loadAttachment(const std::string &filename);
	void setProgram(std::shared_ptr<Program> p) { prog = p; }
	void init();
	void update(int k, std::vector<std::shared_ptr<Frame>>& frames);
	void draw(int k) const;
	void setTextureFilename(const std::string &f) { textureFilename = f; }
	std::string getTextureFilename() const { return textureFilename; }
	std::shared_ptr<TextureMatrix> getTextureMatrix() { return T; }
	
	

private:
	std::shared_ptr<Program> prog;
	std::vector<unsigned int> elemBuf;
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;

	std::vector<float> posBufOrig;
	std::vector<float> norBufOrig;
	std::vector<float> texBufOrig;

	std::vector<glm::mat4> inverseBinds;

	int idxer(int i);
	int idxerPos(int i);

	std::vector<int> boneBuf;
	std::vector<float> weightBuf;
	int maxInfluences;
	int numVerts;
	
	GLuint elemBufID;
	GLuint posBufID;
	GLuint norBufID;
	GLuint texBufID;
	std::string textureFilename;
	std::shared_ptr<TextureMatrix> T;
};

#endif
