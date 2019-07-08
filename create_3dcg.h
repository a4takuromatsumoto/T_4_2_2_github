#include "INCLUDE/list2.h"
#include "INCLUDE/avi.h"

class create_3dcg{
public:
	int x, y;
	int time;
	int rate;
	byte **ppb;
	create_3dcg(int x, int y, int time, int rate){
		this->x = x;
		this->y = y;
		this->time = time;
		this->rate = rate;
		ppb = new byte *[time * rate];
		ZeroMemory(ppb, sizeof(byte *) * time * rate);
	}
	void set_white(){
		for(int i = 0; i < time * rate; i++){
			if(ppb[i] == NULL){
				ppb[i] = new byte[x * y * 3];
			}
			for(int i2 = 0; i2 < x; i2++){
				for(int j2 = 0; j2 < y; j2++){
					ppb[i][0 + i2 * 3 + j2 * x * 3] = 255;
					ppb[i][1 + i2 * 3 + j2 * x * 3] = 255;
					ppb[i][2 + i2 * 3 + j2 * x * 3] = 255;
				}
			}
		}
	}
	void set_circle(){
		for(int i = 0; i < time * rate; i++){
			if(ppb[i] == NULL){
				ppb[i] = new byte[x * y * 3];
			}
			for(int i2 = 0; i2 < x; i2++){
				for(int j2 = 0; j2 < y; j2++){
					if((i2 - x / 2) * (i2 - x / 2) + (j2 - y / 2) * (j2 - y / 2) <= 100 * i / rate){
						ppb[i][0 + i2 * 3 + j2 * x * 3] = 0;
						ppb[i][1 + i2 * 3 + j2 * x * 3] = 0;
						ppb[i][2 + i2 * 3 + j2 * x * 3] = 128;
					}
					else{
						ppb[i][0 + i2 * 3 + j2 * x * 3] = 128;
						ppb[i][1 + i2 * 3 + j2 * x * 3] = 128;
						ppb[i][2 + i2 * 3 + j2 * x * 3] = 255;
					}
				}
			}
		}
	}
	void save(){
		avi av(L"test.avi", x, y);
		for(int i = 0; i < time * rate; i++){
			av.write(ppb[i]);
		}
		av.save();
	}
};