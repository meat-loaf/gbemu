#include <iostream>
#include "memory.hpp"
//TODO clean this up a bit, theres a small amount of duplication that could be cleaned up a little nicer
//TODO determine best place to put clock tick incrementation
#ifndef _GBCPU_H_
#define _GBCPU_H_ 1

namespace gbemu{
#define _bc ((_b << 8) + _c)
#define _de ((_d << 8) + _e)
#define _hl ((_h << 8) + _l)
#define IMM_16_PC ((mem->read(_pc) << 8) | (mem->read(_pc+1)))
class GBCPU{
public:
	GBCPU(char *f): _pc(0x100) { mem = new GBMEM(f); }
	void execute();
	unsigned char gname(){
		return mem->read(0x134);
	}
	GBMEM *mem;
	void dbg_dump();
	unsigned short pc(){ return _pc; }
private:
	unsigned char _a, _b, _c, _d, _e, _h, _l; //f is flags!
	unsigned int ticks;
	unsigned short _pc, _sp;
	//zero flag stored as complement by operations,
	//so we check for !zf to determine if set
	//cf generally gets some non-zero value if set,
	//so in many operations !!cf is used
	//notably the extra bit width is useful
	//for scratch space during calculations
	unsigned int zf, nf, hf, cf;
	//following are used for opcodes
	bool runnable = true;
	bool interrupts = true;
	bool halted = false;
	inline void check_interrupts(){
		;
	}
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
		hf = reg & 0x0F;
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
	inline void dec_u8_mem(unsigned short dest){
		unsigned char b = mem->read(dest);
		hf = ((b & 0x10) == 0x10);
		zf = b-1;
		nf = 1;
		mem->write(dest, zf & 0xFF);
	}
	inline void reg_a_add_u8(unsigned char& reg, bool carry){
		//TODO check flag setting logic...
		unsigned short res;
		res = carry? _a + reg + (!!cf) : _a + reg;
		hf = (reg & 0x10) == 0x10;
		cf = (reg & 0x100) == 0x100;
		_a = zf = res & 0xFF;
		nf = 0;
	}
	inline void add_a_imm_mem(unsigned short addr, bool carry){
		unsigned char reg = mem->read(addr);
		reg_a_add_u8(reg, carry);
		mem->write(addr, reg);
		_pc++;
		ticks += 4;
	}
	inline void reg_a_sub_u8(unsigned char& reg, bool carry){
		//TODO check flag setting logic...
		unsigned short res;
		res = carry? _a - reg - (!!cf) : _a - reg;
		//set if no borrow
		hf = !((reg & 0x10) == 0x10);
		cf = !((reg & 0x100) == 0x100);
		_a = zf = res & 0xFF;
		nf = 1;
	}
	inline void sub_a_imm_mem(unsigned short addr, bool carry){
		unsigned char reg = mem->read(addr);
		reg_a_sub_u8(reg, carry);
		mem->write(addr, reg);
		_pc++;
		ticks+= 4;
	}
	inline void reg_a_and_u8(unsigned char& reg){
		_a &= reg;
		zf = _a;
		nf = 0;
		cf = 0;
		hf = 1;
	}
	inline void reg_a_xor_u8(unsigned char& reg){
		_a ^= reg;
		zf = _a;
		nf = 0;
		cf = 0;
		hf = 0;
	}
	inline void reg_a_or_u8(unsigned char& reg){
		_a |= reg;
		zf = _a;
		nf = 0;
		cf = 0;
		hf = 0;
	}
	inline void ret(bool ifjmp){
		if(ifjmp){
			_pc = (mem->read(_sp) << 8 & mem->read(_sp+1));
			_sp += 2;
			ticks+=16;
		} else {
			_pc += 1;
			ticks+=4;
		}
	}
	inline void pop(unsigned char& regl, unsigned char &regh){
		regl = mem->read(_sp);
		regh = mem->read(_sp+1);
		_sp += 2;
		ticks += 8;
	}
	inline void pop_af(){
		unsigned char f;
		_a = mem->read(_sp);
		f = mem->read(_sp+1);
		zf = !(f & 0x80);
		nf = f & 0x40;
		hf = f & 0x20;
		cf = f & 0x10;
		_sp += 2;
		ticks += 8;
	}
	inline void push(unsigned char& regl, unsigned char &regh){
		mem->write(_sp-1, regh);
		mem->write(_sp-2, regl);
		_sp -= 2;
		ticks += 12;
	}
	inline void push_af(){
		unsigned char f =((!zf << 8) | 
				(!!nf << 7)  |
				(!!hf << 6)  |
				(!!cf << 5)  )& 0xF0;
		mem->write(_sp-1, _a);
		mem->write(_sp-2, f);
		_sp -= 2;
		ticks += 12;
	}
/*	inline void ldh_into_mem(){
		mem->write(0xFF00 + mem->read(_pc), _a);
		_pc+=1;
		ticks+=8;
	}
	inline void ldh_into_a(){
		_a = (mem->read(_pc) & 0xFF) + 0xFF00;
		_pc+=1;
		ticks+=8;
	}
*/
	inline void ldh_into_a(unsigned char& offset){
		_a = mem->read(static_cast<unsigned short>(0xFF00 + offset));
	}
	inline void ldh_into_mem(unsigned char& offset){
		mem->write(static_cast<unsigned short>(0xFF00 + offset), _a);
	}
	//really just a subtraction that sets flags...
	//TODO maybe make sub and this the same?
	inline void reg_a_cmp_u8(unsigned char& reg){
		//TODO check flag setting logic...
		unsigned short res;
		res = _a - reg;
		//set if no borrow
		hf = !((reg & 0x10) == 0x10);
		cf = !((reg & 0x100) == 0x100);
		zf = res & 0xFF;
		nf = 1;
	}
	inline void cmp_imm(unsigned char val){
		reg_a_cmp_u8(val);
	}
	inline void reg_add_u16(unsigned char& high_s, unsigned char &low_s, unsigned int v){
		hf = (low_s & 0x0F + (v & 0xF)) & 0x10;
		cf = ((high_s << 8) + low_s) + v;
		high_s = (cf & 0xFF00) >> 8;
		low_s = (cf & 0x00FF);
		nf = 0;
	}
	inline void add_signed_sp(bool which){
		//TODO check flag setting (invert on subtraction?)
		signed char val = static_cast<signed char>(mem->read(_pc));
		cf =  ((_sp & 0x100) == 0x100);
		hf = (_sp & 0x10) == 0x10;
		if(which)
			_sp += val;
		else {
			unsigned short v = _sp + val;
			_h = _sp & (0xFF00 >> 8);
			_l = _sp & 0x00FF;
		}
		hf = nf = 0;
	}
	inline void write_a_to_mem(unsigned char &high, unsigned char &low){
		mem->write((high & 0xFF) << 8 | (low & 0xFF), _a);
	}
	inline void write_pc(unsigned char &high, unsigned char &low){
		_pc = low + (high << 8);
	}
	inline void cond_jmp_rel(unsigned int flag, signed char off){
		if (flag){
			_pc += off;
			ticks += 8;
		} else {
			_pc += 1;
			ticks += 4;
		}
	}
	inline void cond_jmp_abs(unsigned int flag){
		if (flag){
			_pc = (mem->read(_pc) + (mem->read(_pc+1) >> 8));
			ticks += 12;
		} else {
			_pc+= 2;
			ticks += 4;
		}
	}
	inline void call(unsigned int flag, unsigned short addr){
		if(flag){
			mem->write(_sp-1, (_pc & 0xFF00) >> 8);
			mem->write(_sp-2, (_pc & 0x00FF));
			_sp -= 2;
			_pc = addr;
			ticks += 20;
		} else { _pc += 2;
			ticks += 8; }
	}
	inline void rst(unsigned char val){
		mem->write(_sp-1, (_pc & 0xFF00) >> 8);
		mem->write(_sp-2, (_pc & 0xFF));
		_pc = 0x0000 | val;
		ticks += 12;
	};
	//satan's opcode
	inline void daa(){
		//TODO check this; how to set carry flag (?)
		if (((_a & 0x0F) > 0x09) || hf){
			_a += (nf ? -0x06 : 0x06);
		}
		if (((_a & 0xF0) > 0x90) || cf){
			_a += (nf ? -0x60 : 0x60);
		} else { cf = 0; }
		zf = _a;
		hf = 0;
	}
	//instructions that perform accumulator shifts always clear
	//zero flag; CB-prefixed shifts set as you might expect.
	inline void rlca(){
		cf = _a << 1;
		_a = (cf | cf >> 8) & 0xFF;
		zf = hf = nf = 0;
	}
	inline void rla(){
		cf = _a << 8 | !!cf;
		zf = hf = nf = 0;
		return;	
	}
	inline void rra(){
		cf = (_a << 1) + (!!cf);
		_a = cf & 0xFF;
		cf &= 0x100;
		zf = hf = nf = 0;
	}
	inline void rrca(){
		cf = _a & 0x01;
		_a = (_a >> 1) | (cf << 8);
		zf = hf = nf = 0;
	}
	//sp never referenced as two 8-bit regs, so it gets its own funcs
	inline void write_sp(unsigned short sp_n){
		_sp = sp_n;
	}
	inline void reg_load_u16(unsigned char& high, unsigned char &low, 
			unsigned char &mhigh, unsigned char &mlow){
		high = mhigh;
		low = mlow;
	}	
	inline void reg_load(unsigned char& dest, unsigned char &val){
		dest = val;
	}
	inline void reg_load_a(unsigned short addr){
		_a = mem->read(addr);
	}
	inline void reg_store_mem(unsigned short addr){
		mem->write(addr, _a);
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
	inline void swap(unsigned short addr){
		unsigned short reg = mem->read(addr);
		swap(reg);
		mem->write(addr, reg);
	}
	inline void set(unsigned char mask, unsigned char &reg){
		reg |= mask;
	}
	inline void set(unsigned char mask, unsigned short addr){
		unsigned char reg = mem->read(addr);
		set(mask, reg);
	}
	inline void reset(unsigned char mask, unsigned char &reg){
		reg &= mask;
	}
	inline void reset(unsigned char mask, unsigned short addr){
		unsigned char reg = mem->read(addr);
		reset(mask, reg);
	}
	inline void bit(unsigned char& reg, unsigned char num){
		zf = reg & (0x1 << num);
		hf = nf = 0;	
	}
	inline void bit(unsigned short addr, unsigned char num){
		unsigned char reg = mem->read(addr);
		bit(reg, num);
		_pc++;
		ticks += 8; 
	}
	inline void rlc(unsigned char& reg){
		cf = reg << 1;
		reg = cf & 0xFF;
		cf = cf &= 0x100;
		zf = reg;
		hf = nf = 0;
	}
	inline void rlc_mem(unsigned short addr){
		unsigned char reg = mem->read(addr);
		cf = reg << 1;
		reg = cf & 0xFF;
		cf = cf &= 0x100;
		zf = reg;
		hf = nf = 0;
		mem->write(addr, reg);
		ticks+=8;
	}
	inline void rl(unsigned char& reg){
		cf = (reg << 1) | !!cf;
		zf = reg = cf & 0xFF;
		cf = cf & 0x100;
		hf = nf = 0;
	}
	inline void rl_mem(unsigned short addr){
		unsigned char reg = mem->read(addr);
		cf = (reg << 1) | !!cf;
		zf = reg = cf & 0xFF;
		cf = cf & 0x100;
		hf = nf = 0;
		mem->write(addr, reg);
		ticks+=8;
	}
	inline void rrc(unsigned char& reg){
		cf = reg & 0x1;
		reg = reg >> 1;
		zf = reg;
		nf = hf = 0;
	}
	inline void rrc_mem(unsigned short addr){
		unsigned char reg = mem->read(addr);
		cf = reg & 0x1;
		reg = reg >> 1;
		zf = reg;
		hf = nf = 0;
		mem->write(addr, reg);
		ticks += 8;
	}
	inline void rr(unsigned char& reg){
		unsigned char ocf = !!cf;
		cf = reg & 0x01;
		zf = reg = (reg >> 1) | ((ocf << 8) & 0xFF);
		hf = nf = 0;
	}
	inline void rr_mem(unsigned short addr){
		unsigned char reg = mem->read(addr);
		unsigned char ocf = !!cf;
		cf = reg & 0x01;
		zf = reg = (reg >> 1) | ((ocf << 8) & 0xFF);
		hf = nf = 0;
		mem->write(addr, reg);
		ticks+=8;
	}
	inline void sla(unsigned char &reg){
		cf = reg << 1;
		zf = reg = cf & 0xFE;
		cf &= 0x100;
		hf = nf = 0;
	}
	inline void sla(unsigned short addr){
		unsigned char reg = mem->read(addr);
		sla(reg);
		mem->write(addr, reg);
	}
	inline void sra(unsigned char& reg){
		cf = reg & 0x01;
		reg = reg >> 1;
		zf = reg = reg | ((reg & 0x40) << 1);
		hf = nf = 0;
	}
	inline void sra(unsigned short addr){
		unsigned char reg = mem->read(addr);
		sra(reg);
		mem->write(addr, reg);
		ticks+=8;
	}
	inline void srl(unsigned char& reg){
		cf = reg & 0x01;
		zf = reg = (reg >> 1) & 0x7F;
		hf = nf = 0;
	}
	inline void srl(unsigned short addr){
		unsigned char reg = mem->read(addr);
		srl(reg);
		mem->write(addr, reg);
		ticks += 8;
	}
};
	
}
#endif
