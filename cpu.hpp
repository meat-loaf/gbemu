#include "memory.hpp"

#ifndef _GBCPU_H_
#define _GBCPU_H_ 1

namespace gbemu{
#define _bc ((_b << 8) + _c)
#define _de ((_d << 8) + _e)
#define _hl ((_h << 8) + _l)
class GBCPU{
public:
	GBCPU(char *f): _pc(0x100) { mem = new GBMEM(f); }
	void execute();
	unsigned char &gname(){
		return mem->read(0x134);
	}
	GBMEM *mem;
	void dbg_dump();
private:
	unsigned char _a, _b, _c, _d, _e, _h, _l; //f is flags!
	unsigned long long ticks;
	unsigned short _pc, _sp;
	unsigned int zf, nf, hf, cf;
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
	inline void inc_u8_mem(unsigned short dest){
		unsigned char b = mem->read(dest);
		hf = ((b & 0x10) == 0x10);
		zf = b+1;
		nf = 0;
		mem->write(dest, zf & 0xFF);
	}
	inline void dec_u8(unsigned char &reg){
		hf = ((reg & 0x10) == 0x10);
		reg = zf = reg-1;
		nf = 1;
	}
	inline void dec_u8_mem(unsigned char &dest){
		unsigned char b = mem->read(dest);
		hf = ((b & 0x10) == 0x10);
		zf = b-1;
		nf = 1;
		mem->write(dest, zf & 0xFF);
	}
	inline void reg_a_add_u8(unsigned char& reg, bool carry){
		unsigned short res;
		res = carry? _a + reg + (!!cf) : _a + reg;
		hf = (reg & 0x10) == 0x10;
		cf = (reg & 0x100) == 0x100;
		_a = zf = res & 0xFF;
		nf = 0;
	}
	inline void write_a_to_mem(unsigned char &high, unsigned char &low){
		mem->write((high & 0xFF) << 8 | (low & 0xFF), _a);
	}
	inline void write_pc(unsigned char &high, unsigned char &low){
		_pc = low + (high << 8);
	}
	//these operate slightly differently than their CB-prefixed bretheren
	inline void rlca(){
		cf = _a << 1;
		_a = (cf | cf >> 8) & 0xFF;
		zf = hf = nf = 0;
	}
	inline void rrca(){
		cf = _a & 0x1;
		_a = (_a >> 1) | (cf << 8);
		zf = hf = nf = 0;
	}
	inline void write_sp(unsigned short sp_n){
		_sp = sp_n;
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
	inline void set(unsigned char mask, unsigned char &reg){
		reg |= mask;
	}
	inline void reset(unsigned char mask, unsigned char &reg){
		reg &= mask;
	}
	inline void rlc(unsigned char& reg){
		cf = reg << 1;
		reg = cf & 0xFF;
		cf = cf &= 0x100;
		zf = reg;
		hf = nf = 0;
	}
	inline void rl(unsigned char& reg){
		cf = (reg << 1) | !!cf;
		zf = reg = cf & 0xFF;
		cf = cf & 0x100;
		hf = nf = 0;
	}

};
	
}
#endif
