namespace gbemu{
class MMU_BASE{
public:
	MMU_BASE();
	~MMU_BASE();
private:
	//banks are 16 KiB big; number of banks depends
	//on mmu used
	unsigned char **bank_dat;
};
}

class MMU_NONE : public MMU_BASE {
public:
	MMU_NONE();
	~MMU_NONE();
private:
};
