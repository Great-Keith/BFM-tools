#include "bfm.h"

int main()
{
	if (!init_bfm(TEXT_DATA))
		return -1;
	data_check();
	generate_random_face();
	system("pause");
    return 0;
}
