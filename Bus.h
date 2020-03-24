//#ifndef BUS_H
//#define BUS_H
#pragma once
#include <cstdint>
#include <vector>
#include "mp8085.h"

class Bus
{
public:
	Bus();
	~Bus();
public:
	mp8085 cpu;

	void	write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);


	//uint8_t mem[64 * 1024];
	//uint8_t* mem = (uint8_t *) malloc(64 * 1024 * sizeof(uint8_t));
	
	// 64KB of maximum memory addressable
	std::vector<uint8_t> mem;

	void reset();
	void clock();
private:
	uint32_t nSystemClockCounter = 0;
};

//#endif
