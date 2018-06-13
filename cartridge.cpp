#include "cartridge.hpp"

namespace gbemu{
	//TODO mostly temp, clean this up when testing successful
	GBCART::GBCART(char *fname){
		cart_file.open(fname, std::ifstream::binary);
		if(cart_file.is_open()){
			cart_file.seekp(0x0148);
			char size;
			cart_file.read(&size, 1);
			cart_file.seekp(0x0);
			char * d = new char[size];
			cart_file.read(d, size);
			mmu = new MMU_NONE();
			mmu->setup_banks((unsigned char *)d, size);
			//bank 0 is static
			active_banks.bank_0 = (unsigned char *)d;
			//bank 1 is always first active bank
			active_banks.active_swappable_bank = (unsigned char *)d+(16*1024);
		}
	}
	unsigned char GBCART::read(unsigned short addr){
		return addr < 0x4000 ? active_banks.bank_0[addr] 
			: active_banks.active_swappable_bank[addr-0x4000];
	}
	//
	void GBCART::write(unsigned char dest, unsigned char byte){
		active_banks.active_swappable_bank = mmu->bank(dest, byte);
	}
}
