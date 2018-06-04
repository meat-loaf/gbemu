
#define FLAGS_ZERO (1 << 7)
#define FLAGS_OPER (1 << 6)
#define FLAGS_HALF (1 << 5)
#define FLAGS_CARR (1 << 4)


namespace gbemu{


class CPU{
public:
	CPU();
	void execute();
private:
	unsigned char _a, _b, _c, _d, _e, _h, _l; //f is flags!
	unsigned long long ticks;
	unsigned short _pc, _sp;
	unsigned int zf, nf, hf, cf;

	//following are used for opcodes
	inline void inc_16(unsigned char& high, unsigned char& low){
		low = ((low + 1) & 0xFF);
		high = high + ((low & 0xFF) >> 8) & 0xFF;
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
	
};
	
}
