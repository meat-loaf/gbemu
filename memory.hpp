#include "cartridge.hpp"
#ifndef _GBMEM_H_
#define _GBMEM_H_ 1
namespace gbemu{
class GBMEM{
public:

	unsigned char &read(unsigned short addr);
	void write(unsigned short addr, unsigned char byte);
	GBMEM();
	~GBMEM();
	//TODO check if this is needed

private:
	struct memblob {
		unsigned char *chram,
		unsigned char *bg1_ram,
		unsigned char *bg2_ram,
		unsigned char *internal_ram,
		unsigned char *oam,
		unsigned char *hw_io,
		unsigned char *zero_ief
	} memblob;
	GBCART cartridge;
	struct memblob _memblob;
};

class memabst_internal : public memabst{
};
}
#endif
