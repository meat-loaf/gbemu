#include "cartridge.hpp"
#include <iostream>
namespace gbemu{
	//TODO mostly temp, clean this up when testing successful
	GBCART::GBCART(char *fname) {
		cart_file.open(fname, std::ifstream::in | std::ifstream::binary);
		if(cart_file.is_open()){
			cart_file.seekg(0, std::ios::end);
			int size = cart_file.tellp();
			std::cerr << "SIZE " << size << "\n";
			std::cerr << "BANKS" << size / (16*1024) << "\n";
			cart_file.seekp(0x0);
			char *d = new char[size];
			cart_file.read(d, size);
			mmu = new MMU_NONE();
			mmu->setup_banks((unsigned char *)d, size);
			//bank 0 is static
			active_banks.bank_0 = (unsigned char *)d;
			//bank 1 is always first active bank
			active_banks.active_swappable_bank = (unsigned char *)d+(16*1024);
		}
		else std::cerr << "FILE OPEN ERROR \n";
	}
	unsigned char &GBCART::read(unsigned short addr){
		std::cerr << std::hex << "addr: " << addr << "\n";
		return addr < 0x4000 ? active_banks.bank_0[addr] 
			: active_banks.active_swappable_bank[addr-0x4000];
	}
	//
	void GBCART::write(unsigned char dest, unsigned char byte){
		active_banks.active_swappable_bank = mmu->bank(dest, byte);
	}
}
