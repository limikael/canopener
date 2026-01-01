#include "canopener/cof.h"
#include "canopener/cof-defines.h"
#include <stdio.h>
#include <cassert>
#include <format>
#include <iostream>
#include <string.h>

//using namespace canopener;

void test_cof() {
	printf("- Cof...\n");
	cof_t cof;

	cof_init(&cof);
	cof_set(&cof, COF_FUNC,    COF_FUNC_SDO_TX);
	cof_set(&cof, COF_NODE_ID, 0x05);
	cof_set(&cof, COF_SDO_CMD, COF_SDO_CMD_ABORT);
	cof_set(&cof, COF_SDO_INDEX,    0x4001);
	cof_set(&cof, COF_SDO_SUBINDEX, 0x10);
	cof_set(&cof, COF_SDO_ABORT_CODE, 0x06020000);

	char s[32];
	cof_to_slcan(&cof,s);

	//printf("f: %s\n",s);
	assert(!strcmp(s,"t58588001401000000206"));

	cof_t cof2;
	cof_from_slcan(&cof2,s);

	assert(cof_get(&cof2, COF_FUNC)==COF_FUNC_SDO_TX);
	assert(cof_get(&cof2, COF_NODE_ID)==0x05);
	assert(cof_get(&cof2,COF_DLC)==8);
    assert(cof_get(&cof,COF_SDO_INDEX)==0x4001);
    assert(cof_get(&cof,COF_SDO_SUBINDEX)==0x10);
	assert(cof_get(&cof,COF_SDO_ABORT_CODE)==0x06020000);

	cof_t *cof3=cof_create();
	cof_set(cof3,COF_FUNC,COF_FUNC_HEARTBEAT);

	assert(cof_get(cof3,COF_FUNC)==COF_FUNC_HEARTBEAT);

	//assert(cof_get(&cof2,COF_U32_0)==0x12345678);*/
}

/*int main() {
	printf("Running tests...\n");

	test_cof();

	return 0;
}*/
