#include "Bus.h"

Bus::Bus()
{
	cpu.ConnectBus(this);
	mem = std::vector<uint8_t>(64 * 1024, 0x00);
	//memset(mem, 0x00, sizeof mem);
	//for (auto& i : mem) i = 0x00;
}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
	mem[addr] = data;
}

uint8_t Bus::read(uint16_t addr)
{
	return mem[addr];
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	cpu.clock();
	nSystemClockCounter++;
}
