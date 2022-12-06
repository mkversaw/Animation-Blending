#include "BlendedAnim.h"

using namespace std;

void BlendedAnim::genBlendedFrame(std::shared_ptr<Frame>& currFrame, int animIdx0, int animIdx1, int currBoneIdx) {
	float blendFactor = getAlpha(animIdx0);
	//cout << blendFactor << "\n";
	//cout << "idx0: " << animIdx0 << "\n";
	//cout << "idx1: " << animIdx1 << "\n";
	if (currBoneIdx == 0) { // ROOT BONE (doesnt use parent's transform)
		currFrame->bones.resize(boneCount); // root bone so resiz the bones vector here
		
		//cout << "BC: " << anims[0]->parentSpaceFrames[animIdx0]->bones.size() << "\n";

		glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);	
		glm::mat4 animMat2 = ((1.0f - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
		
		currFrame->bones[currBoneIdx] = Bone(animMat1 + animMat2);
	}
	else {
		int parentIdx = anims[0]->joints[currBoneIdx]->parentIdx; // these SHOULD be the same for both animations
		glm::mat4 parentMat = currFrame->bones[parentIdx].mat;
	
		glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);
		glm::mat4 animMat2 = ((1.0f - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
		//cout << "mat2: " << to_string(animMat2) << "\n";
		//cout << "mat2: " << to_string(animMat2) << "\n";
		//cout << blendFactor << "\n";
		//cout << (1.0f - blendFactor) << "\n";
		currFrame->bones[currBoneIdx] = Bone(parentMat * (animMat1 + animMat2));
	}

	for (int& childIdx : anims[0]->joints[currBoneIdx]->childrenIdx) { // for each child of the current bone
		genBlendedFrame(currFrame,animIdx0,animIdx1,childIdx);
	}
}

float BlendedAnim::getAlpha(int currFrame) { // for anim0, currFrame should always be >= than framesToBlendFor
	//cout << "I_frame: " << currFrame << "\n";
	float frame = (1.0f - (1.0f / framesToBlendFor * (currFrame - framesToBlendFor)) );
	frame = glm::clamp(frame, 0.0f, 1.0f); // min 0, max 1
	//cout << "factor: " << frame << "\n";
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
	
	cout << "anim0 frames: " << blendedFrames.size() << "\n";
	prev = blendedFrames.size();
	//blendedFrames = anims[1]->frames;
	//frameCountBLENDED = anims[1]->frameCount;


	for (int i = framesToBlendFor; i < 2 * framesToBlendFor; i++) { // use blended frames from halfpoint to 2 * halfpoint
		shared_ptr<Frame> currFrame = make_shared<Frame>();
		genBlendedFrame(currFrame, i, i - framesToBlendFor);
		blendedFrames.push_back(currFrame);
		frameCountBLENDED++;
	}

	cout << "blended frames: " << blendedFrames.size() - prev << "\n";
	prev = blendedFrames.size();

	for (int i = framesToBlendFor; i < anims[1]->frameCount; i++) {
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[1]->frames[i]->bones); // scuffed deep copy
		blendedFrames.push_back(currFrame);
		frameCountBLENDED++;
	}
	cout << "anim1 frames: " << blendedFrames.size() - prev << "\n";

	frameCountBLENDED--; // important but i dont know why


}