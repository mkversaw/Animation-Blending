#pragma once

#include "Anim.h"

struct BlendedAnim {
	std::vector<std::shared_ptr<Anim> > anims;

	int frameCountBLENDED;
	int boneCount;
	std::vector<std::shared_ptr<Frame>> blendedFrames;

	int startBlendFrame;
	int framesToBlendFor;

	BlendedAnim() {}

	void genBlendedFrame(std::shared_ptr<Frame>& currFrame, int animIdx0, int animIdx1, int currBoneIdx = 0);

	float getAlpha(int currFrame);
	void test();
	std::pair<int,int> getHandIdx();
};