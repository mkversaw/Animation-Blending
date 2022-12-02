#include "Anim.h"

struct BlendedAnim {
	std::vector<std::shared_ptr<Anim> > anims;

	BlendedAnim() {}

};