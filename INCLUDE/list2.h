#ifndef MYLIST2

#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "INCLUDE/readwrite.h"
template <class T> class list2{
	struct _data{
		T d;
		_data* f, *n;	//�O
	};
public:
	unsigned number;
	typedef _data* pdata;
	pdata data, data_first;
	pdata start_n;			//�m�[�h�̎n��
	pdata start_d, start_g;	//�f�[�^�Ƃ��݂̎n�߂̃|�C���^
	pdata end_d;
	pdata next_notouch;
	class iterator;
	unsigned size;
	unsigned block_number;
	T **ppt_sort;
	list2(){sub_list(31);}
	list2(unsigned s){sub_list(s);}
	void sub_list(unsigned s){
		size = s + 1;
		number = 0;
		block_number = 1;
		data = (pdata)malloc(size * sizeof(_data));
		data_first = data;
		start_n = data;
		start_d = 0;
		end_d = 0;
		start_g = NULL;	//����
		next_notouch = data + 1;
		data->f = NULL;
		data->n = next_notouch;	//�킩��₷���悤�Ɂi���������Ă����j
	}
	~list2(){
		pdata pd = data;
		pdata pd2 = data->f;
		while(1){
			free(pd);
			if(!pd2)
				break;
			pd = pd2;
			pd2 = pd->f;
		}
	}
	void clear(){
		//�폜
		pdata pd = data_first;
		pdata pd2 = data_first->f;
		while(1){
			free(pd);
			if(!pd2)
				break;
			pd = pd2;
			pd2 = pd->f;
		}
		//�V�K�쐬
		size = 32;
		number = 0;
		block_number = 1;
		data = (pdata)malloc(size * sizeof(_data));
		start_n = data;
		start_d = 0;
		end_d = 0;
		start_g = NULL;	//����
		next_notouch = data + 1;
		data->f = NULL;
		data->n = next_notouch;	//�킩��₷���悤�Ɂi���������Ă����j
	}
	T *ins(T &t){
		//���݃��X�g���L�邩������
		if(start_g){
			pdata pd = start_g->n;
			//�O����q��
			if(end_d)
				end_d->n = start_g;
			//�f�[�^��������
			//memcpy(&(start_g->d), &t, sizeof(T));
			start_g->d = t;
			start_g->f = end_d;
			start_g->n = 0;
			//�擪��������
			if(!start_d)
				start_d = start_g;
			//�I�[��������
			end_d = start_g;
			//���݃��X�g�ό`
			start_g = pd;
			//�v�f���C���N�������g
			number++;
		}
		else{
			//�O����q��
			if(end_d)
				end_d->n = next_notouch;
			//�f�[�^��������
			//memcpy(&(next_notouch->d), &t, sizeof(T));
			next_notouch->d = t;
			next_notouch->f = end_d;
			next_notouch->n = NULL;
			//�擪��������
			if(!start_d)
				start_d = next_notouch;
			//�I�[��������
			end_d = next_notouch;
			//�v�f���C���N�������g
			number++;
			//���X�g����t�ɂȂ����Ƃ�
			if(size == number + block_number)
			{
				//�V�����̈���m��
				pdata data2 = (pdata)malloc(size * sizeof(_data));
				size *= 2;
				block_number++;
				next_notouch = data2 + 1;
				data->f = data2;
				data2->f = NULL;
				data2->n = next_notouch;	//�킩��₷���悤�Ɂi���������Ă����j

				//�ǉ�
				data = data2;
			}
			else
			{
				//�|�C���^�̃C���N�������g
				next_notouch++;
			}
		}
		return &(end_d->d);
	}

	T *ins2(T &t){
		if(start_d)
			return ins_front((T *)start_d, t);
		return ins(t);
	}
	T *ins_front(T *pnow_t, T &t){
		pdata pnow = (pdata)pnow_t;
		pdata pfront = pnow->f;
		pdata preturn;
		//���݃��X�g���L�邩������
		if(start_g){
			//pfront -> start_g -> pnow
			preturn = start_g;
			pdata pd2 = start_g->n;
			//�O����q��
			if(pfront)
				pfront->n = start_g;
			//��납��q��
			pnow->f = start_g;
			//�f�[�^��������
			start_g->d = t;
			start_g->f = pfront;
			start_g->n = pnow;
			//�擪��������
			if(!pfront)
				start_d = start_g;
			//���݃��X�g�ό`
			start_g = pd2;
			//�v�f���C���N�������g
			number++;
		}
		else{
			//pfront -> next_notouch -> pnow
			preturn = next_notouch;
			//�O����q��
			if(pfront)
				pfront->n = next_notouch;
			//��납��q��
			pnow->f = next_notouch;
			//�f�[�^��������
			next_notouch->d = t;
			next_notouch->f = pfront;
			next_notouch->n = pnow;
			//�擪��������
			if(!pfront)
				start_d = next_notouch;
			//�v�f���C���N�������g
			number++;
			//���X�g����t�ɂȂ����Ƃ�
			if(size == number + block_number)
			{
				//�V�����̈���m��
				pdata data2 = (pdata)malloc(size * sizeof(_data));
				size *= 2;
				block_number++;
				next_notouch = data2 + 1;
				data->f = data2;
				data2->f = NULL;
				data2->n = next_notouch;	//�킩��₷���悤�Ɂi���������Ă����j
			}
			else
			{
				//�|�C���^�̃C���N�������g
				next_notouch++;
			}
		}
		return &(preturn->d);
	}
	T *ins_next(T *pnow_t, T &t){
		pdata pnow = (pdata)pnow_t;
		pdata pnext = pnow->n;
		pdata preturn;
		//���݃��X�g���L�邩������
		if(start_g){
			//pnow -> start_g -> pnext
			preturn = start_g;
			pdata pd2 = start_g->n;
			//��납��q��
			if(pnext)
				pnext->f = start_g;
			//�O����q��
			pnow->n = start_g;
			//�f�[�^��������
			start_g->d = t;
			start_g->n = pnext;
			start_g->f = pnow;
			//�I�[��������
			if(!pnext)
				end_d = start_g;
			//���݃��X�g�ό`
			start_g = pd2;
			//�v�f���C���N�������g
			number++;
		}
		else{
			//pnow -> next_notouch -> pnext
			preturn = next_notouch;
			//��납��q��
			if(pnext)
				pnext->f = next_notouch;
			//�O����q��
			pnow->n = next_notouch;
			//�f�[�^��������
			next_notouch->d = t;
			next_notouch->n = pnext;
			next_notouch->f = pnow;
			//�I�[��������
			if(!pnext)
				end_d = next_notouch;
			//�v�f���C���N�������g
			number++;
			//���X�g����t�ɂȂ����Ƃ�
			if(size == number + block_number)
			{
				//�V�����̈���m��
				pdata data2 = (pdata)malloc(size * sizeof(_data));
				size *= 2;
				block_number++;
				next_notouch = data2 + 1;
				data->f = data2;
				data2->f = NULL;
				data2->n = next_notouch;	//�킩��₷���悤�Ɂi���������Ă����j
			}
			else
			{
				//�|�C���^�̃C���N�������g
				next_notouch++;
			}
		}
		return &(preturn->d);
	}
	static inline T *next(T *pt){return (T *)((list2<T>::pdata)(pt))->n;}
	static inline T *front(T *pt){return (T *)((list2<T>::pdata)(pt))->f;}
	inline T *start(){return &start_d->d;}
	inline T *end(){return &end_d->d;}

	void del(T *pnow_t){
		pdata pd = (pdata)pnow_t;
		//�擪�|�C���^��������
		if(pd == start_d)
			start_d = start_d->n;
		//�I�[�|�C���^��������
		if(pd == end_d)
			end_d = end_d->f;
		//�O��m�[�h�̏�������
		if(pd->f)
			pd->f->n = pd->n;
		if(pd->n)
			pd->n->f = pd->f;
		//���g�̏�������
		pd->f = 0;	//�����Ă������Ǝv��
		pd->n = start_g;
		//���݃|�C���^�̏�������
		start_g = pd;
		number--;
	}
	void del(iterator iter){del((T *)iter.d);}
	void foreach(void (*func)(T &)){
		for(iterator iter = begin(); iter.d != 0; iter++)
			func(*iter);
	}

	struct func{virtual double operator()(T *pt) = NULL;};
	void sub_sort2(func &f, int left, int right){
		int left_index = left;
		int right_index = right;
		double pivot = f(ppt_sort[(left + right) / 2]);

		while(left_index <= right_index){
			for(; f(ppt_sort[left_index]) > pivot; left_index++)
				;
			for(; f(ppt_sort[right_index]) < pivot; right_index--)
				;
			if(left_index <= right_index){
				T *pt = ppt_sort[left_index];
				ppt_sort[left_index] = ppt_sort[right_index];
				ppt_sort[right_index] = pt;
				left_index++;
				right_index--;
			}
		}
		if(right_index > left){
			sub_sort2(f, left, right_index);
		}
		if(left_index < right){
			sub_sort2(f, left_index, right);
		}
	}
	void sub_sort(func &f, int left, int right){
		int left_index = left;
		int right_index = right;
		double pivot = f(ppt_sort[(left + right) / 2]);

		while(left_index <= right_index){
			for(; f(ppt_sort[left_index]) < pivot; left_index++)
				;
			for(; f(ppt_sort[right_index]) > pivot; right_index--)
				;
			if(left_index <= right_index){
				T *pt = ppt_sort[left_index];
				ppt_sort[left_index] = ppt_sort[right_index];
				ppt_sort[right_index] = pt;
				left_index++;
				right_index--;
			}
		}
		if(right_index > left){
			sub_sort(f, left, right_index);
		}
		if(left_index < right){
			sub_sort(f, left_index, right);
		}
	}
	void sort(func &f){
		if(number > 1){
			//�����ݒ�
			unsigned p_ppt = 0;
			ppt_sort = new T *[number];
			for(T *pt = start(); pt != 0; pt = next(pt)){
				ppt_sort[p_ppt] = pt;
				p_ppt++;
			}
			sub_sort(f, 0, number - 1);
			start_d = (_data *)ppt_sort[0];
			end_d = (_data *)ppt_sort[number - 1];
			((_data *)ppt_sort[0])->f = NULL;
			((_data *)ppt_sort[0])->n = (_data *)ppt_sort[1];
			((_data *)ppt_sort[number - 1])->n = NULL;
			((_data *)ppt_sort[number - 1])->f = (_data *)ppt_sort[number - 2];
			for(unsigned ai = 1; ai < number - 1; ai++){
				((_data *)ppt_sort[ai])->f = (_data *)ppt_sort[ai - 1];
				((_data *)ppt_sort[ai])->n = (_data *)ppt_sort[ai + 1];
			}
			delete ppt_sort;
		}
	}
	void sort2(func &f){
		if(number > 1){
			//�����ݒ�
			unsigned p_ppt = 0;
			ppt_sort = new T *[number];
			for(T *pt = start(); pt != 0; pt = next(pt)){
				ppt_sort[p_ppt] = pt;
				p_ppt++;
			}
			sub_sort2(f, 0, number - 1);
			start_d = (_data *)ppt_sort[0];
			end_d = (_data *)ppt_sort[number - 1];
			((_data *)ppt_sort[0])->f = NULL;
			((_data *)ppt_sort[0])->n = (_data *)ppt_sort[1];
			((_data *)ppt_sort[number - 1])->n = NULL;
			((_data *)ppt_sort[number - 1])->f = (_data *)ppt_sort[number - 2];
			for(unsigned ai = 1; ai < number - 1; ai++){
				((_data *)ppt_sort[ai])->f = (_data *)ppt_sort[ai - 1];
				((_data *)ppt_sort[ai])->n = (_data *)ppt_sort[ai + 1];
			}
			delete ppt_sort;
		}
	}
	void save(f_save &fs){
		fs(number);
		for(T *pt = start(); pt != NULL; pt = next(pt))
			pt->save(fs);
	}
	void load(f_load &fl){
		int i;
		fl(i);
		for(int ai = 0; ai < i; ai++){
			T t;
			t.load(fl);
			ins(t);
		}
	}
	void save2(f_save &fs){
		fs(number);
		for(T *pt = start(); pt != NULL; pt = next(pt))
			fs(*pt);
	}
	void load2(f_load &fl){
		int i;
		fl(i);
		for(int ai = 0; ai < i; ai++){
			T t;
			fl(t);
			ins(t);
		}
	}
	T **carray(){
		T **ppt = new T *[number];
		int count = 0;
		for(T *pt = start(); pt != NULL; pt = next(pt)){
			ppt[count] = pt;
			count++;
		}
		return ppt;
	}
	T *operator[](int i){
		T *pt = start();
		for(int ai = 0; ai < i; ai++)
			pt = next(pt);
		return pt;
	}
	int getnumber(){return number;}
	int getnumber2(){
		int i_number = 0;
		for(T *pt = start(); pt; pt = next(pt))
			i_number++;
		return i_number;
	}
};

#define MYLIST2
#endif