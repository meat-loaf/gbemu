#include "cartridge.hpp"

namespace gbemu{
	//TODO mostly temp, clean this up when testing successful
	GBCART::GBCART(char *fname){
		cart_file.open(fname, std::ifstream::binary);
		if(cart_file.is_open()){
			cart_file.seekp(0x0148);
			unsigned int size = cart_file.read(reinterpret_cast<char*>(&size), 1);
			cart_file.seekp(0x0);
			unsigned char * d = new unsigned char[size];
			cart_file.read(static_cast<char>(d), size);
			mmu = new MMU_NONE();
			mmu->setup_banks(d, size);
			//bank 0 is static
			active_banks.bank_0 = d;
			//bank 1 is always first active bank
			active_banks.active_swappable_bank = d+(16*1024);
		}
	}
	unsigned char GBCART::read(unsigned short addr){
		return addr < 0x4000 ? active_banks.bank_0[addr] 
			: active_banks.active_swappable_bank[addr-0x4000];
	}
	//
	void GBCART::write(unsigned char dest, unsigned char byte){
		active_banks.active_swappable_bank = mmu.bank(dest, byte);
	}
}
