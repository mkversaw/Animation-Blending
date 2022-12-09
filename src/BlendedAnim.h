#pragma once

#include "Anim.h"

struct BlendedAnim {
	std::vector<std::shared_ptr<Anim> > anims;

	int frameCountBLENDED;
	int frameCountBLENDED2;
	int boneCount;
	std::vector<std::shared_ptr<Frame>> blendedFrames;
	std::vector<std::shared_ptr<Frame>> blendedFrames2;

	int startBlendFrame;
	int framesToBlendFor;

	BlendedAnim() {}

	void genBlendedFrame(std::shared_ptr<Frame>& currFrame, int animIdx0, int animIdx1, int currBoneIdx = 0);

	float getAlpha(int currFrame);
	void test();
	void test2(int currFrameIdx);
	std::pair<int,int> getHandIdx();
};