#include "mmu.hpp"

namespace gbemu{
	//define inherited
	MMU_BASE::MMU_BASE(){ }
	MMU_BASE::~MMU_BASE(){ }

	void MMU_BASE::setup_banks(unsigned char* rom_dat, unsigned int size){
		bank_dat = new unsigned char*((unsigned char *)(size/(16*1024)));
		for (unsigned int i = 0, j = 0;
		     i < size; i+=16*1024,
						   j++){
			bank_dat[j] = rom_dat+i;
		}
	}
	//games with no mmu cannot swap banks.
	MMU_NONE::MMU_NONE() : curr_bank(1) { }
	unsigned char *MMU_NONE::bank(unsigned char &dest, unsigned char& byte){
		return bank_dat[1];
	}
}
