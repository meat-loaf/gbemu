#TODO make a not dumb makefile
TARGET_EXEC ?= gbemu
CPPFLAGS := --std=c++11 -g -O2
LIBFLAGS := -lSDL2
TARGET_EXEC: cartridge.o cpu.o memory.o mmu.o lcd.o
	g++ main.cpp -o $(TARGET_EXEC) cartridge.o cpu.o memory.o mmu.o lcd.o $(CPPFLAGS) $(LIBFLAGS)

cartridge.o: cartridge.cpp cartridge.hpp
	g++ -c cartridge.cpp -o cartridge.o $(CPPFLAGS)
cpu.o: cpu.cpp cpu.hpp lcd.o
	g++ -c cpu.cpp -o cpu.o $(CPPFLAGS)
memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp -o memory.o $(CPPFLAGS)
mmu.o:
	g++ -c mmu.cpp -o mmu.o $(CPPFLAGS)
lcd.o: lcd.hpp lcd.cpp
	g++ -c lcd.cpp -o lcd.o $(CPPFLAGS) 
default: TARGET_EXEC
clean:
	rm *.o gbemu
