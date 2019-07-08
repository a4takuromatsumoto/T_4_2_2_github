#ifndef MINE_READWRITE
#define MINE_READWRITE
#include <stdio.h>
#include <windows.h>
#pragma warning(disable:4267)

/*
virtual‚ðŽg‚¤‚Æ‚«
class Base{
	virtual void save(f_save &fs) = 0;
	virtual void load(f_save &fs) = 0;
	virtual int gettype() = 0;
	static Base *swload(f_load &fl){
		int type;
		Base *pbase
		fl(type);
		switch(type){
			case 0:
				pbase = new derived();
				pbase->load(fl);
				break;
		}
		return pbase;
	}
};
‚ð’è‹`‚·‚é
*/
struct f_read{
	virtual int operator()(void *, unsigned) = NULL;
};

struct f_load : public f_read{
	FILE *pf;
	f_load(char *name){
		pf = fopen(name, "rb");
		if(!pf){
			//printf("file is not found.\n");
		}
	}
	~f_load(){if(pf)fclose(pf);}
	int operator()(void *pc, unsigned length){return fread(pc, 1, length, pf);}
	int operator()(char *pc){
		int count = 0;
		int nRtn;
		while(1){
			nRtn = fread(pc + count, 1, 1, pf);
			if(nRtn == 0)
				return count;
			count++;
		};
	}
	int load_all(char *pc, int len_max){
		int count = 0;
		int nRtn;
		while(1){
			nRtn = fread(pc + count, 1, 1, pf);
			if(nRtn == 0)
				return count;
			count++;
			if(count == len_max)
				return count;
		};
	}
	template <typename T>
		int operator()(T &t){return fread(&t, 1, sizeof(t), pf);}
	bool getstring(char *pc, unsigned length){
		unsigned count = 0;
		int nRtn;
		while(1){
			if(count == length)
				return false;
			nRtn = fread(pc + count, 1, 1, pf);
			if(nRtn == 0)
				return false;
			if(*(pc + count) == 0)
				return true;
			count++;
		};
	}
	template <typename T>
		void readp(T *&pt){
			int i;
			(*this)(i);
			pt = i ? new T(*this) : NULL;
		}
		/*
	template <typename T>
		void readp_virtual(T *&pt){
			int i;
			(*this)(i);
			if(i){
				pt = T::swload(*this);
				pt->load(*this);
			}
			else
				pt = NULL;
		}
		*/
	template <typename T>
		void readp_virtual_noload(T *&pt){
			int i;
			(*this)(i);
			if(i){
				pt = T::swload(*this);
			}
			else
				pt = NULL;
		}
};

struct f_write{
	virtual void operator()(void *, unsigned) = NULL;
	void operator()(unsigned u){(*this)(&u, sizeof(unsigned));}
};

struct f_save : public f_write{
	FILE *pf;
	f_save(char *name){
		pf = fopen(name, "wb");
		if(!pf)
			throw;
	}
	~f_save(){fclose(pf);}
	void operator()(void *pc, unsigned length){fwrite(pc, 1, length, pf);}
	void operator()(unsigned u){(*this)(&u, sizeof(unsigned));}
	//void operator()(char *pc){
	//	(*this)(pc, strlen(pc));
	//}
	void write_string(char *pc){
		(*this)(pc, strlen(pc));
	}
	template <typename T>
		void operator()(T &t){fwrite(&t, 1, sizeof(t), pf);}
	template <typename T>
		void writep(T *pt){
			if(pt){
				(*this)((unsigned)1);
				pt->save(*this);
			}
			else
				(*this)((unsigned)0);
		}
	template <typename T>
		void writep_virtual(T *pt){
			if(pt){
				(*this)((unsigned)1);
				(*this)((unsigned)pt->gettype());
				pt->save(*this);
			}
			else{
				(*this)((unsigned)0);
			}
		}
};
#endif