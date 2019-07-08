#include "ai3.h"
#include "create_3dcg.h"

class cai3{
public:
	ai3 ai;
	create_3dcg *pc3;
	cai3(){
		pc3 = new create_3dcg(60, 45, 5, 30);
	}
};