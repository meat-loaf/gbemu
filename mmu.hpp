#ifndef _MMU_H_
#define _MMU_H_ 1
namespace gbemu{
class MMU_BASE{
public:
	MMU_BASE();
	~MMU_BASE();
	void setup_banks(unsigned char *rom_dat, unsigned int size);
	unsigned char *bank(unsigned char &dest, unsigned char &byte);
private:
	//banks are 16 KiB big; number of banks depends
	//on mmu used
	unsigned char **bank_dat;
};

class MMU_NONE : public MMU_BASE {
public:
	MMU_NONE();
	unsigned char *bank(unsigned char &dest, unsigned char&byte);
private:
	unsigned int type;
	unsigned short curr_bank;
};
}
#endif
