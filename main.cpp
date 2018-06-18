#include "cpu.hpp"
#include <iostream>

int main(int argc, char** argv){
	gbemu::GBCPU cpu_main(argv[1]);	
	std::cout << cpu_main.gname();
	cpu_main.execute();
	cpu_main.execute();
	//cpu_main.execute();
	cpu_main.dbg_dump();
	return 0;
}
