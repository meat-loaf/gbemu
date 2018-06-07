#ifndef _GBCART_H_
#define _GBCART_H_ 1

namespace gbemu{
class GBCART{
public:
	GBCART();	
	change_bank(unsigned int bank);
private:
	typedef struct {
		unsigned char *bank_0;
		unsigned char *active_bank;
	}
	//will be an inherited class that implements the proper mmu 
	MMU_BASE mmu;
	
};
}
#endif
