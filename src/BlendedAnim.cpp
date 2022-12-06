#include "BlendedAnim.h"
using namespace std;

////parentSpaceFrames
//void BlendedAnim::genBlended(int idx) { // REDO THIS
//	if (idx == 0) { // root!
//		assert(frameCount != NULL && frameCount > 0);
//		boneCount = anims[0]->boneCount;
//
//		cout << "FC: " << frameCount << "\n";
//		blendedFrames.resize(frameCount); // enables indxing into blendedFrames
//
//		blendedFrames[idx] = make_shared<Frame>(); // initialize new frame
//		for (int j = 0; j < boneCount; j++) { // for each bone in the frame
//			glm::mat4 blendedMat = (blendFactor * anims[0]->parentSpaceFrames[idx]->bones[j].mat) +
//				((1 - blendFactor) * anims[1]->parentSpaceFrames[idx]->bones[j].mat);
//		
//			blendedFrames[idx]->bones.push_back({blendedMat});
//		}
//
//	}
//	else {
//		int parentIdx = anims[0]->joints[idx]->parentIdx;
//		for (int j = 0; j < boneCount; j++) { // for each bone in the frame
//			
//		
//			glm::mat4 parent_factor = blendedFrames[parentIdx]->bones[j].mat;
//		
//			glm::mat4 child_factor = (blendFactor * anims[0]->parentSpaceFrames[idx]->bones[j].mat) +
//				((1 - blendFactor) * anims[1]->parentSpaceFrames[idx]->bones[j].mat);
//		
//			glm::mat4 blendedMat = parent_factor * child_factor;
//		
//			blendedFrames[idx]->bones.push_back(blendedMat);
//		}
//		
//		for (int& childIdx : anims[0]->joints[idx]->childrenIdx) { // for each child of this bone
//			genBlended(childIdx);
//		}
//	}
//}


void BlendedAnim::genBlendedFrame(std::shared_ptr<Frame>& currFrame, int animIdx0, int animIdx1, int currBoneIdx) {
	float blendFactor = getAlpha(animIdx0);
	cout << "idx0: " << animIdx0 << "\n";
	cout << "idx1: " << animIdx1 << "\n";
	if (currBoneIdx == 0) { // ROOT BONE (doesnt use parent's transform)
		currFrame->bones.resize(boneCount); // root bone so resiz the bones vector here
		
		cout << "BC: " << anims[0]->parentSpaceFrames[animIdx0]->bones.size() << "\n";

		glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);	
		glm::mat4 animMat2 = ((1 - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);

		currFrame->bones[currBoneIdx] = Bone(animMat1 + animMat2);
	}
	//else {
	//	int parentIdx = anims[0]->joints[currBoneIdx]->parentIdx; // these SHOULD be the same for both animations
	//	glm::mat4 parentMat = currFrame->bones[parentIdx].mat;
	//
	//	glm::mat4 animMat1 = (blendFactor * anims[0]->parentSpaceFrames[animIdx0]->bones[currBoneIdx].mat);
	//	glm::mat4 animMat2 = ((1 - blendFactor) * anims[1]->parentSpaceFrames[animIdx1]->bones[currBoneIdx].mat);
	//
	//	currFrame->bones[currBoneIdx] = Bone(parentMat * (animMat1 + animMat2));
	//}

	//for (int& childIdx : anims[0]->joints[currBoneIdx]->childrenIdx) { // for each child of the current bone
	//	genBlendedFrame(currFrame,animIdx0,animIdx1,childIdx);
	//}
}

float BlendedAnim::getAlpha(int currFrame) { // for anim0, currFrame should always be >= than framesToBlendFor
	float frame = (1.0f - (1.0f / framesToBlendFor * (currFrame - framesToBlendFor)) );
	return glm::clamp(frame, 0.0f, 1.0f); // min 0, max 1
}

void BlendedAnim::test() {
	int halfWayOfFirst = 0.5 * anims[0]->frameCount;
	int framesToBlendFor = min(0.5 * anims[0]->frameCount, 0.5 * anims[1]->frameCount);
	
	for (int i = 0; i < framesToBlendFor; i++) { // use frames from anim[0] up to the min halfway point
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[0]->frames[i]->bones); // scuffed deep copy
		blendedFrames.push_back(currFrame);
	}

	for (int i = framesToBlendFor; i < 2 * framesToBlendFor; i++) { // use blended frames from halfpoint to 2 * halfpoint
		shared_ptr<Frame> currFrame = make_shared<Frame>();
		genBlendedFrame(currFrame, i, i - framesToBlendFor);
	}

	for (int i = framesToBlendFor; i < anims[1]->frameCount; i++) {
		shared_ptr<Frame> currFrame = make_shared<Frame>(anims[1]->frames[i]->bones); // scuffed deep copy
		blendedFrames.push_back(currFrame);
	}
}