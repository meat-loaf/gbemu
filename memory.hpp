#include "cartridge.hpp"
#ifndef _GBMEM_H_
#define _GBMEM_H_ 1
namespace gbemu{
class GBMEM{
public:
	//TODO implement
	unsigned char& operator[](unsigned short byte){ return _memblob[byte]; }
	GBMEM();
	~GBMEM();
	unsigned char read(unsigned char addr);
	unsigned char write(unsigned char addr, unsigned char byte);
private:
	GBCART cartridge;
	unsigned char *_memblob;
};	
}
#endif
