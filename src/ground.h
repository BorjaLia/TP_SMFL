#pragma once

#include "irregular.h"

namespace ground
{
	struct Ground
	{
		irregular::Irregular leftPart;
		irregular::Irregular middlePart;
		irregular::Irregular rightPart;
	};

	void init();
	void update();
}
