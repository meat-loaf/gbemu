#include "memory.hpp"

namespace gbemu{
	GBMEM::GBMEM() : _memblob(nullptr){ } 
	GBMEM::~GBMEM() {
		if (_memblob != nullptr){
			delete[] _memblob;
		}
	}

	// memory map is as follows:
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
	// $0000-$00FF 	interrupt vector table	
	unsigned char GBMEM::read(unsigned char addr){
		if (addr < 0x8000){
			if (addr < 0x0100){
				//interrupt vector area
			}
			//access cartridge here; it should keep track of bank etc
		}
		else if (addr < 0xA000){
			//chram, bgdata here...
		}
		else if (addr > 0xBFFF){
			//internal ram/echo ram/oam, the works
			if (addr > 0xDFFF && addr < 0xFE00){
				//load from 0xC000 - 0xDDFF
			}
		}
		else{
			//cart ram, if applicable
		}
	}
}
