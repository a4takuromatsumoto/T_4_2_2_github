#ifndef MINE_QUEUE2
#define MINE_QUEUE2
#include "INCLUDE/list2.h"

template <typename T>
class queue2 : public list2<T>{
public:
	queue2(){}
	T *push(T &t){return ins2(t);}
	T &peek(){return *end();}
	bool iszero(){return (number == 0);}
	void pop(){del(end());}
};
#endif