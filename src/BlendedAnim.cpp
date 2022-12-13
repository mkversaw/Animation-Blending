#include "BlendedAnim.h"

using namespace std;

void BlendedAnim::genBlendedFrame(std::shared_ptr<Frame>& currFrame, int animIdx0, int animIdx1, int currBoneIdx, bool LTOR) {
	float blendFactor = getAlpha(animIdx0);

	if (currBoneIdx == 0) { // ROOT BONE (doesnt use parent's transform)
		currFrame->bones.resize(boneCount); // root bone so resiz the bones vector here
	

		glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);	
		glm::mat4 animMat2 = ((1.0f - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);

		if (LTOR) {
			glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);
			glm::mat4 animMat2 = ((1.0f - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
			currFrame->bones[currBoneIdx] = Bone(animMat1 + animMat2);
		}
		else {
			glm::mat4 animMat1 = ((1.0f - blendFactor) * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);
			glm::mat4 animMat2 = (blendFactor * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
			currFrame->bones[currBoneIdx] = Bone(animMat1 + animMat2);
		}
		
		
	}
	else {
		int parentIdx = anims[0]->joints[currBoneIdx]->parentIdx; // these SHOULD be the same for both animations
		glm::mat4 parentMat = currFrame->bones[parentIdx].mat;
	

		if (LTOR) {
			glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);
			glm::mat4 animMat2 = ((1.0f - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
			currFrame->bones[currBoneIdx] = Bone(parentMat * (animMat1 + animMat2));
		}
		else {
			glm::mat4 animMat1 = ((1.0f - blendFactor) * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);
			glm::mat4 animMat2 = (blendFactor * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
			currFrame->bones[currBoneIdx] = Bone(parentMat * (animMat1 + animMat2));
		}
		
	}

	for (int& childIdx : anims[0]->joints[currBoneIdx]->childrenIdx) { // for each child of the current bone
		genBlendedFrame(currFrame,animIdx0,animIdx1,childIdx,LTOR);
	}
}

float BlendedAnim::getAlpha(int currFrame) { // for anim0, currFrame should always be >= than framesToBlendFor
	float frame = (1.0f - ((1.0f / framesToBlendFor2) * alphaNum));
	frame = glm::clamp(frame, 0.0f, 1.0f); // min 0, max 1
	return frame; 
}



void BlendedAnim::test() {
	framesToBlendFor = min(0.5 * anims[0]->frameCount, 0.5 * anims[1]->frameCount);
	int prev = 0;
	frameCountBLENDED = 0;

	for (int i = 0; i < framesToBlendFor; i++) { // use frames from anim[0] up to the min halfway point
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[0]->frames[i]->bones); // scuffed deep copy
		blendedFrames.push_back(currFrame);
		frameCountBLENDED++;
	}
	
	prev = blendedFrames.size();

	for (int i = framesToBlendFor; i < 2 * framesToBlendFor; i++) { // use blended frames from halfpoint to 2 * halfpoint
		shared_ptr<Frame> currFrame = make_shared<Frame>();
		genBlendedFrame(currFrame, i, i - framesToBlendFor);
		blendedFrames.push_back(currFrame);
		frameCountBLENDED++;
	}

	prev = blendedFrames.size();

	for (int i = framesToBlendFor; i < anims[1]->frameCount; i++) {
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[1]->frames[i]->bones); // scuffed deep copy
		blendedFrames.push_back(currFrame);
		frameCountBLENDED++;
	}

	frameCountBLENDED--; // important but i dont know why


}

void BlendedAnim::test2(int currFrameIdx) { 
	blendedFrames2.clear();
	notBlendedFrames.clear();

	blendedFrames2.push_back(make_shared<Frame>(anims[0]->frames[0]->bones)); // DUMMY FRAME (will be skipped anyways!)
	notBlendedFrames.push_back(make_shared<Frame>(anims[0]->frames[0]->bones)); // DUMMY FRAME (will be skipped anyways!)

	framesToBlendFor2 = min(0.5 * anims[0]->frameCount, 0.5 * anims[1]->frameCount);
	alphaNum = 0;
	frameCountBLENDED2 = 0;

	int jumpAnimIdx = 0;
	for (int i = 0; i < framesToBlendFor2; i++) { // use frames from  currFrameIdx to framesTOBlendFOr
		
		int runAnimIdx = (currFrameIdx + i) % anims[0]->frameCount;
	
		if (runAnimIdx == 0) { // dont use the bind pose frame!
			runAnimIdx += 1;
		}
	
		jumpAnimIdx++;
	
	
		shared_ptr<Frame> currFrame = make_shared<Frame>();
		
		genBlendedFrame(currFrame, runAnimIdx, jumpAnimIdx);
		alphaNum++;
		blendedFrames2.push_back(currFrame);
		frameCountBLENDED2++;

		shared_ptr<Frame> currNOTBLEND = make_shared<Frame>(anims[1]->frames[runAnimIdx]->bones);
		notBlendedFrames.push_back(currNOTBLEND);
	}

	alphaNum = 0;
	int runAnimIdx222 = 0;
	for (int i = jumpAnimIdx; i < anims[1]->frameCount; i++) { // add the rest of the jump frames
		if (i >= anims[1]->frameCount - framesToBlendFor2) {
			runAnimIdx222++;
			shared_ptr<Frame> currFrame = make_shared<Frame>();
			genBlendedFrame(currFrame, runAnimIdx222, i,0, false);
			alphaNum++;
			blendedFrames2.push_back(currFrame);
			frameCountBLENDED2++;
		}
		else {
			shared_ptr<Frame> currFrame = make_shared<Frame>(anims[1]->frames[i]->bones); // scuffed deep copy
			blendedFrames2.push_back(currFrame);
			frameCountBLENDED2++;
		}
		
		
		shared_ptr<Frame> currNOTBLEND = make_shared<Frame>(anims[1]->frames[i]->bones);
		notBlendedFrames.push_back(currNOTBLEND);
	}

}

void BlendedAnim::noBlendTest2(int currFrameIdx) {
	blendedFrames2.clear();
	blendedFrames2.push_back(make_shared<Frame>(anims[0]->frames[0]->bones)); // DUMMY FRAME (will be skipped anyways!)
	framesToBlendFor2 = min(0.5 * anims[0]->frameCount, 0.5 * anims[1]->frameCount);

	frameCountBLENDED2 = 0;

	int jumpAnimIdx = 0;
	for (int i = 0; i < framesToBlendFor2; i++) { // use frames from  currFrameIdx to framesTOBlendFOr

		int runAnimIdx = (currFrameIdx + i) % anims[0]->frameCount;

		if (runAnimIdx == 0) { // dont use the bind pose frame!
			runAnimIdx += 1;
		}

		jumpAnimIdx++;
	
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[0]->frames[runAnimIdx]->bones); // scuffed deep copy
		blendedFrames2.push_back(currFrame);
		frameCountBLENDED2++;
	}


	for (int i = jumpAnimIdx; i < anims[1]->frameCount; i++) { // add the rest of the jump frames
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[1]->frames[i]->bones); // scuffed deep copy
		blendedFrames2.push_back(currFrame);
		frameCountBLENDED2++;
	}
}

std::pair<int,int> BlendedAnim::getHandIdx() {
	pair<int, int> idxs;
	idxs.first = anims[0]->hierarchy->handIdx_LEFT;
	idxs.second = anims[0]->hierarchy->handIdx_RIGHT;
	return idxs;
}