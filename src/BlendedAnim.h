#pragma once

#include "Anim.h"

struct BlendedAnim {
	std::vector<std::shared_ptr<Anim> > anims;

	int frameCountBLENDED = 0;
	int frameCountBLENDED2 = 0;
	int boneCount;
	std::vector<std::shared_ptr<Frame>> blendedFrames;
	std::vector<std::shared_ptr<Frame>> blendedFrames2;

	int startBlendFrame;
	int framesToBlendFor;
	int framesToBlendFor2;

	int alphaNum = 0;

	BlendedAnim() {}

	void genBlendedFrame(std::shared_ptr<Frame>& currFrame, int animIdx0, int animIdx1, int currBoneIdx = 0, bool LTOR = true);

	float getAlpha(int currFrame);
	
	void test();
	void test2(int currFrameIdx);
	void noBlendTest2(int currFrameIdx);
	std::pair<int,int> getHandIdx();
};