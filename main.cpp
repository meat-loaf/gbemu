#include "cpu.hpp"
#include <fstream>
#include <limits>
#include <signal.h>


bool nointerrupt = true;

void ctrl_c_handler(int s){
	nointerrupt = false;	
}	
int main(int argc, char** argv){
	signal(SIGINT, ctrl_c_handler);
	unsigned short bp;
	bool breakpoint = false;
	gbemu::GBCPU cpu_main(argv[1]);	
	//std::cout << cpu_main.gname();
	std::string input;
	std::cin >> input;
	while (input != "end"){
		if (input == "dmp")
			cpu_main.dbg_dump();
		else if (input == "exec")
			cpu_main.execute();
		else if (input == "cont"){
			std::cout << "cont : breakpoint, nointerrupt (" << breakpoint << nointerrupt << ")\n"; 
			if (breakpoint && nointerrupt){
				std::cout << std::hex << "bp, cpu_main.pc() " << (unsigned int)bp << " " << (unsigned int)cpu_main.pc() << "\n";
				while(bp >= cpu_main.pc()){
					std::cout << "endless exec\n";
					cpu_main.execute();
				}
			nointerrupt = true;
			}
			else cpu_main.execute();	
		}
		else if (input == "bp"){
			breakpoint = true;
			std::cin.clear();
			std::cout << "breakpoint addr: ";
			while(!(std::cin >> std::hex >> bp)){
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        			std::cout << "Breakpoint must be 16-bit address. Try again: ";
			}
		}
		else if (input == "lbp"){ 
			if (breakpoint){
				std::cout << std::hex << (unsigned int)bp << "\n"; 
			} else { std::cout << "no breakpoint set.\n"; } 
		}
		else if (input == "q" || input == "quit"){
			exit(0);
		}
		else std::cout << "bad command " << input << ": dmp, exec are valid\n";
	std::cin.clear();
	std::cin >> input;
	}
	return 0;
}
