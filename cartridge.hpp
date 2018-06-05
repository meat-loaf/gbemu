#ifndef _GBCART_H_
#define _GBCART_H_ 1

namespace gbemu{
class GBCART{
public:
	change_bank(unsigned int bank);
private:
	unsigned int bank;	
};
}
#endif
