#include "memory.hpp"

#ifndef _GBCPU_H_
#define _GBCPU_H_ 1

namespace gbemu{
#define _bc ((_b << 8) + _c)
#define _de ((_d << 8) + _e)
#define _hl ((_h << 8) + _l)
class GBCPU{
public:
	GBCPU();
	void execute();
private:
	unsigned char _a, _b, _c, _d, _e, _h, _l; //f is flags!
	unsigned long long ticks;
	unsigned short _pc, _sp;
	unsigned int zf, nf, hf, cf;
	GBMEM mem;
	//following are used for opcodes

	//flags aren't effected on 16-bit opcodes...
	//stack var used incase low reg overflows
	//TODO check validity
	inline void inc_u16(unsigned char& high, unsigned char& low){
		unsigned short li = low;
		low = ((li + 1) & 0xFF);
		high = high + (li >> 8) & 0xFF;
	}
	inline void dec_u16(unsigned char& high, unsigned char& low){
		unsigned short li = low;
		low = (li - 1) & 0xFF;
		high = high + ((li >> 8) & 1) & 0xFF;	
	}
	inline void inc_u8(unsigned char &reg){
		hf = ((reg & 0x10) == 0x10);
		reg = zf = reg+1;
		nf = 0;
	}
	inline void dec_u8(unsigned char &reg){
		hf = ((reg & 0x10) == 0x10);
		reg = zf = reg-1;
		nf = 1;
	}
	inline void reg_a_add_u8(unsigned char& reg, bool carry){
		unsigned short res;
		res = carry? _a + reg + (!!cf) : _a + reg;
		hf = (reg & 0x10) == 0x10;
		cf = (reg & 0x100) == 0x100;
		_a = zf = res & 0xFF;
		nf = 0;
	}
	inline void reg_write_a(unsigned char& high, unsigned char& low){
		mem[(high << 8) + low] = _a;
	}
	inline void reg_and(unsigned char& reg){
	}
	inline void reg_load_u16(unsigned char& high, unsigned char &low, 
			unsigned char &mhigh, unsigned char &mlow){
		high = mhigh;
		low = mlow;
	}	
	inline void reg_load(unsigned char& dest, unsigned char &val){
		dest = val;
	}
	//CB Opcodes (extended instructions)
	//swap register (8 cycles)
	//swaps upper/lower nibble, resets flags, then sets zero if
	//result is 0
	inline void swap(unsigned char& reg){
		reg = (((reg & 0x0F) << 4) | ((reg & 0xF0) >> 4));
		nf = hf = cf = 0x0;
		zf = reg;
	}
	inline void set(unsigned char& mask, unsigned char &reg){
		reg |= mask;
	}
	inline void reset(unsigned char& mask, unsigned char &reg){
		reg &= mask;
	}
};
	
}
#endif
