#include "Framework/D3DHeaders.h"

int main()
{
#if FAT_DEBUG_BUILD
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	void *p = malloc(10);
	return 0;
}
