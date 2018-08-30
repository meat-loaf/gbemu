#include "memory.hpp"
#include <iostream>
namespace gbemu{
	GBMEM::GBMEM(char *f) : _memblob{
				new unsigned char[0x1800],
				new unsigned char[0x500],
				new unsigned char[0x500],
//				new unsigned char[0x1800],
				new unsigned char[0x2000],
				new unsigned char[0x1FF],
				}{ std::cerr << "memconst\n"; cartridge = new GBCART(f); } 
	GBMEM::~GBMEM() {
		delete[] _memblob.chram;
		delete[] _memblob.bg1_ram;
		delete[] _memblob.bg2_ram;
		delete[] _memblob.internal_ram;
		delete[] _memblob.oam_hwio_zeroief;
	}

	// memory map is as follows, top down:
	// $FFFF	interrupt enable flag 
	// $FF80-$FFFE	zero
	// $FF00-$FF7F	hardware registers	
	// $FEA0-$FEFF 	unusable (behavior?)
	// $FE00-$FE9F	oam	
	// $E000-$FDFF 	echo ram (echos $C000-$CFFF)
	// $D000-$DFFF	internal ram, bankable if gbc	
	// $C000-$CFFF 	internal ram, bank 0
	// $A000-$BFFF	cart ram (if supported by game)	
	// $9C00-$9FFF 	bg map data 2
	// $9800-$9BFF 	bg map data 1
	// $8000-$97FF 	chram	
	// $4000-$7FFF 	cart rom, active bank
	// $0150-$3FFF 	cart rom, bank 0
	// $0100-$014F	cart header 
	// $0000-$00FF 	interrupt vector table (first 255 bytes of cart)
	unsigned char &GBMEM::read(unsigned short addr){
		//read from cartridge
		if (addr < 0x8000){
			return cartridge->read(addr);
		}
		//chram, bgdata here...
		//TODO double check timing? i think bgdata can only be
		//written/read during h/vblank?
		else if (addr < 0xA000){
			return addr > 0x97FF ?
				(addr > 0x9BFF ?
				 _memblob.bg2_ram[addr-0x9800]
				: _memblob.bg1_ram[addr-0x9C00]
				)
				: _memblob.chram[addr-0x9800]; 
		}
		else if (addr > 0xBFFF && addr < 0xFE00){
			//if we're accessing from echo ram, access same area
			//in normal ram instead
			//TODO can we do adjustments by operator override in struct?
			//TODO ram from 0xD000-0xDFFF is bankable in cgb, support this eventually
			return addr > 0xDFFF ?
				//adjust addresses by 0xC000 as internal ram arr is 0-base
				//if we're in echo ram adjust by a further 0x2000 
				_memblob.internal_ram[addr - (0xC000+0x2000)]
				: _memblob.internal_ram[addr - 0xC000];
		}
		//OAM/internal registers
		//need to check OAM writes; can only be written during v/hblank i think
		else if (addr > 0xFDFF){
			return _memblob.oam_hwio_zeroief[addr-0xFDFF];
		}
		//cart ram, if applicable	
		else{
		}
	}
	void GBMEM::write(unsigned short addr, unsigned char byte){
		//TODO clean this up
		std::cout << "writing to " << std::hex << (unsigned int)addr << "\n";
		//write == perform a bank switch
		if (addr < 0x8000){
			cartridge->write(addr, byte);
		}
		//write chdata
		else if (addr < 0xA000){
			    (addr > 0x97FF ?
				(addr > 0x9BFF ?
				 _memblob.bg2_ram[addr-0x9800]
				: _memblob.bg1_ram[addr-0x9C00]
				)
				: _memblob.chram[addr-0x9800]) = byte;
		}
		//write to internal ram; writing to echo ram is
		//the same as writing to normal ram in practice, so we redirect
		else if (addr > 0xBFFF && addr < 0xFE00){
			(addr > 0xDFFF ?
				//adjust addresses by 0xC000 as internal ram arr is 0-base
				//if we're in echo ram adjust by a further 0x2000 
				_memblob.internal_ram[addr - (0xC000+0x2000)]
				: _memblob.internal_ram[addr - 0xC000]) = byte;
		}
		//OAM/internal registers
		//need to check OAM writes; can only be written during v/hblank i think
		else if (addr > 0xFDFF){
			if (addr > 0xFF00 & addr < 0xFF7F){
				//lots of hw regs have different behavior on writes
				write_hw_reg(addr, byte);
			}
			else _memblob.oam_hwio_zeroief[addr-0xFDFF] = byte;
		}
		//cart ram, if applicable
		//TODO implement in cartridge class
		else{
			;
		}
	
	}
}
