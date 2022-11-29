#include "Frame.h"

Frame::Frame() {}

Frame::Frame(std::vector<Bone>& bones) : bones(bones) {}

void Frame::print(int i) {
	bones[i].print();
}