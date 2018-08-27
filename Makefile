#TODO make a not dumb makefile
TARGET_EXEC ?= gbemu
CPPFLAGS := --std=c++11 -g -O2

TARGET_EXEC: cartridge.o cpu.o memory.o mmu.o lcd.o
	g++ main.cpp -o $(TARGET_EXEC) cartridge.o cpu.o memory.o mmu.o lcd.o $(CPPFLAGS) -lSDL2

cartridge.o: cartridge.cpp cartridge.hpp
	g++ -c cartridge.cpp -o cartridge.o $(CPPFLAGS)
cpu.o: cpu.cpp cpu.hpp
	g++ -c cpu.cpp -o cpu.o $(CPPFLAGS)
memory.o:
	g++ -c memory.cpp -o memory.o $(CPPFLAGS)
mmu.o:
	g++ -c mmu.cpp -o mmu.o $(CPPFLAGS)
lcd.o:
	g++ -c lcd.cpp -o lcd.o $(CPPFLAGS) 
default: TARGET_EXEC
clean:
	rm *.o
