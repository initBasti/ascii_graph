#include <stdlib.h>

#include "test.h"

Test::Test()
{
}

Test::~Test()
{
}

int Test::execute()
{
	int ret;

	ret = init();
	if (ret)
		return ret;

	ret = run();

	cleanup();

	return ret;
}
