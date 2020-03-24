#include "mp8085.h"
#include "Bus.h"

//#include <iostream>
#include <sstream>

#define LOGMODE

void Launch()
{
	Bus mp;
	std::stringstream ss;
	// LXI H, 3005H
	// MVI B, 06H
	// MVI A, 25H
	// LXI H, 300
	ss << "21 00 30 2C 2C 2D 2D 2D"; // Program instructions in hex
	mp.mem[0x3005] = 0x25;
	mp.mem[0x3006] = 0x19;
	std::stringstream srss;/*
	srss << "21 07 30 C9";*/
	// Copy this to the program memory

	int nOffset = 0x8000;
	while (!ss.eof())
	{
		std::string instr;
		ss >> instr;
		mp.mem[nOffset++] = (uint8_t)std::stoul(instr, nullptr, 16);
	}
	/*
	nOffset = 0x8032;

	while (!srss.eof())
	{
		std::string instr;
		srss >> instr;
		mp.mem[nOffset++] = (uint8_t)std::stoul(instr, nullptr, 16);
	}*/

	// Have to hard code program location
	mp.mem[0x0100] = 0x00;
	mp.mem[0x0101] = 0x80;

	mp.cpu.reset();
	int maxCycles = 20;

	while (maxCycles --) {
		mp.cpu.clock();
	}
	std::cout << "3005:\t" << std::setw(6) << +mp.mem[0x3005] << "\n";
	std::cout << "3006:\t" << std::setw(6) << +mp.mem[0x3006] << "\n";
	
}

int main()
{
	Launch();
	return 0;
}