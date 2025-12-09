#include "canopener.h"
#include <stdio.h>

using namespace canopener;

SlcanStdioBus::SlcanStdioBus(int readfd, int writefd) {
	readf=fdopen(readfd,"r");
	writef=fdopen(writefd,"w");
}

bool SlcanStdioBus::available() {
	return rxBuf.size();
}

bool SlcanStdioBus::read(cof_t *frame) {
	//printf("reading...\n");

	cof_t c=rxBuf.front();
	cof_cpy(frame,&c);
	rxBuf.pop();
	return true;
}

void SlcanStdioBus::write(cof_t *frame) {
	char s[256];
	if (cof_to_slcan(frame,s)) {
		fprintf(writef,"%s\r\n",s);
		fflush(writef);
	}
}

void SlcanStdioBus::loop() {
	char s[256];
	cof_t cof;

	if (!fgets(s,256,readf))
		exit(-1);

	if (cof_from_slcan(&cof,s)) {
		//printf("pushing...\n");
		rxBuf.push(cof);
	}
}