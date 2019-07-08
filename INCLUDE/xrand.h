#pragma once
#include <time.h>
#include "INCLUDE/readwrite.h"

unsigned long xrand_x=123456789,xrand_y=362436069,xrand_z=521288629,xrand_w=88675123;

unsigned long xrand(){
	unsigned long t;
	t=(xrand_x^(xrand_x<<11));xrand_x=xrand_y;xrand_y=xrand_z;xrand_z=xrand_w; return( xrand_w=(xrand_w^(xrand_w>>19))^(t^(t>>8)) );
}
void xsrand(int i){
	xrand_x ^= i;
}
void xsrand_time(){
	xsrand((unsigned int)time(NULL));
}

void xrand_save(f_save &fs){
	fs(xrand_x);
	fs(xrand_y);
	fs(xrand_z);
	fs(xrand_w);
}
void xrand_load(f_load &fl){
	fl(xrand_x);
	fl(xrand_y);
	fl(xrand_z);
	fl(xrand_w);
}
