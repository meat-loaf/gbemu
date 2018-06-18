#ifndef _GBCART_H_
#define _GBCART_H_ 1
#include <fstream>
#include "mmu.hpp"

namespace gbemu{
class GBCART{
public:
	GBCART(char *fname);
	unsigned char &read(unsigned short addr);
	//`write' is actually a bank change...
	void write(unsigned char dest, unsigned char byte);
	
private:
	struct banks {
		unsigned char *bank_0;
		unsigned char *active_swappable_bank;
	} active_banks;
	//will be an inherited class that implements the proper mmu
	//TODO probably should be pointer to control when constructor is called 
	MMU_BASE *mmu;
	std::fstream cart_file;
};
}
#endif
