#include "mp8085.h"
#include "Bus.h"



//---------------------------------PROCESSOR FUNCTIONS--------------------------------
mp8085::mp8085()
{
	// Define opcode lookup table here
	using a = mp8085;
	lookup =
	{
		//						0									1									  2									3										   4								5										6						     7											 8							9									 A									   B								C								   D									E									  F
		/*	0	*/{ "NOP", &a::NOP, &a::IMP, 1, 1 }, { "LXI", &a::LXI, &a::IMM, 3, 3 },	{ "STAX", &a::STAX, &a::REI, 3, 2 }, { "INX" , &a::INX , &a::RED, 1, 1 }, {	"INR",  &a::INR, &a::RED, 1, 1 }, {	"DCR ", &a::DCR , &a::RED, 1, 1 }, { "MVI", &a::MVI, &a::IMM, 2, 3 }, {"RLC", &a::RLC, &a::IMP, 1, 1 }, {"XXX", &a::XXX, &a::IMP, 1, 1 }, { "DAD" , &a::DAD , &a::RED, 1, 3 }, { "LDAX", &a::LDAX, &a::REI, 1, 2 }, { "DCX ", &a::DCX , &a::RED, 1, 1 }, { "INR", &a::INR, &a::RED, 1, 1 }, { "DCR ", &a::DCR , &a::RED, 1, 1 }, {	"MVI", &a::MVI, &a::IMM, 2, 3 }, { "RRC", &a::RRC, &a::IMP, 1, 1 },	// 1
		/*	1	*/{ "XXX", &a::XXX, &a::IMP, 1, 1 }, { "LXI", &a::LXI, &a::IMM, 3, 3 },	{ "STAX", &a::STAX, &a::REI, 3, 2 }, { "INX" , &a::INX , &a::RED, 1, 1 }, {	"INR",  &a::INR, &a::RED, 1, 1 }, {	"DCR ", &a::DCR , &a::RED, 1, 1 }, { "MVI", &a::MVI, &a::IMM, 2, 3 }, {"RAL", &a::RAL, &a::IMP, 1, 1 }, {"XXX", &a::XXX, &a::IMP, 1, 1 }, { "DAD" , &a::DAD , &a::RED, 1, 3 }, { "LDAX", &a::LDAX, &a::REI, 1, 2 }, { "DCX ", &a::DCX , &a::RED, 1, 1 }, { "INR", &a::INR, &a::RED, 1, 1 }, { "DCR ", &a::DCR , &a::RED, 1, 1 }, {	"MVI", &a::MVI, &a::IMM, 2, 3 }, { "RAR", &a::RAR, &a::IMP, 1, 1 },	// 1 
		/*	2	*/{ "RIM", &a::RIM, &a::IMP, 1, 1 }, { "LXI", &a::LXI, &a::IMM, 3, 3 },	{ "SHLD", &a::SHLD, &a::DIR, 1, 5 }, { "INX" , &a::INX , &a::RED, 1, 1 }, {	"INR",  &a::INR, &a::RED, 1, 1 }, {	"DCR ", &a::DCR , &a::RED, 1, 1 }, { "MVI", &a::MVI, &a::IMM, 2, 3 }, {"DAA", &a::DAA, &a::IMP, 1, 1 }, {"XXX", &a::XXX, &a::IMP, 1, 1 }, { "DAD" , &a::DAD , &a::RED, 1, 3 }, { "LHLD", &a::LHLD, &a::DIR, 1, 5 }, { "DCX ", &a::DCX , &a::RED, 1, 1 }, { "INR", &a::INR, &a::RED, 1, 1 }, { "DCR ", &a::DCR , &a::RED, 1, 1 }, {	"MVI", &a::MVI, &a::IMM, 2, 3 }, { "CMA", &a::CMA, &a::IMP, 1, 1 },	// 2
		/*	3	*/{ "SIM", &a::SIM, &a::IMP, 1, 1 }, { "LXI", &a::LXI, &a::IMM, 3, 3 },	{ "STA"	, &a::STA , &a::DIR, 3, 4 }, { "INX" , &a::INX , &a::RED, 1, 1 }, {	"INR",  &a::INR, &a::RED, 1, 1 }, {	"DCR ", &a::DCR , &a::RED, 1, 1 }, { "MVI", &a::MVI, &a::IMM, 2, 3 }, {"STC", &a::STC, &a::IMP, 1, 1 }, {"XXX", &a::XXX, &a::IMP, 1, 1 }, { "DAD" , &a::DAD , &a::RED, 1, 3 }, { "LDA" , &a::LDA , &a::DIR, 3, 4 }, { "DCX ", &a::DCX , &a::RED, 1, 1 }, { "INR", &a::INR, &a::RED, 1, 1 }, { "DCR ", &a::DCR , &a::RED, 1, 1 }, {	"MVI", &a::MVI, &a::IMM, 2, 3 }, { "CMC", &a::CMC, &a::IMP, 1, 1 },	// 3
		/*	4	*/{ "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 },	{ "MOV"	, &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, {	"MOV",  &a::MOV, &a::RED, 1, 1 }, {	"MOV ", &a::MOV , &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 2, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::IMP, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::RED, 1, 1 }, {	"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, // 4
		/*	5	*/{ "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 },	{ "MOV"	, &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, {	"MOV",  &a::MOV, &a::RED, 1, 1 }, {	"MOV ", &a::MOV , &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::IMP, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::RED, 1, 1 }, {	"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, // 5
		/*	6	*/{ "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 },	{ "MOV"	, &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, {	"MOV",  &a::MOV, &a::RED, 1, 1 }, {	"MOV ", &a::MOV , &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::IMP, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::RED, 1, 1 }, {	"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, // 6
		/*	7	*/{ "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 },	{ "MOV"	, &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, {	"MOV",  &a::MOV, &a::RED, 1, 1 }, {	"MOV ", &a::MOV , &a::RED, 1, 1 }, { "HLT", &a::HLT, &a::IMP, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, {"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV" , &a::MOV , &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::IMP, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV ", &a::MOV , &a::RED, 1, 1 }, {	"MOV", &a::MOV, &a::RED, 1, 1 }, { "MOV", &a::MOV, &a::RED, 1, 1 }, // 7
		/*	8	*/{ "ADD", &a::ADD, &a::RED, 1, 1 }, { "ADD", &a::ADD, &a::RED, 1, 1 },	{ "ADD"	, &a::ADD , &a::RED, 1, 1 }, { "ADD" , &a::ADD , &a::RED, 1, 1 }, {	"ADD",  &a::ADD, &a::RED, 1, 1 }, {	"ADD ", &a::ADD , &a::RED, 1, 1 }, { "ADD", &a::ADD, &a::RED, 1, 1 }, {"ADD", &a::ADD, &a::REI, 1, 1 }, {"ADC", &a::ADC, &a::REI, 2, 2 }, { "ADC" , &a::ADC , &a::REI, 2, 2 }, { "ADC" , &a::ADC , &a::REI, 2, 2 }, { "ADC ", &a::ADC , &a::IMP, 2, 2 }, { "ADC", &a::ADC, &a::REI, 2, 2 }, { "ADC ", &a::ADC , &a::REI, 2, 2 }, {	"ADC", &a::ADC, &a::REI, 2, 2 }, { "ADC", &a::ADC, &a::REI, 2, 2 },	// 8
		/*	9	*/{ "SUB", &a::SUB, &a::RED, 1, 1 }, { "SUB", &a::SUB, &a::RED, 1, 1 },	{ "SUB"	, &a::SUB , &a::RED, 1, 1 }, { "SUB" , &a::SUB , &a::RED, 1, 1 }, {	"SUB",  &a::SUB, &a::RED, 1, 1 }, {	"SUB ", &a::SUB , &a::RED, 1, 1 }, { "SUB", &a::SUB, &a::RED, 1, 1 }, {"SUB", &a::SUB, &a::REI, 1, 1 }, {"SBB", &a::SBB, &a::REI, 2, 2 }, { "SBB" , &a::SBB , &a::REI, 2, 2 }, { "SBB" , &a::SBB , &a::REI, 2, 2 }, { "SBB ", &a::SBB , &a::IMP, 2, 2 }, { "SBB", &a::SBB, &a::REI, 2, 2 }, { "SBB ", &a::SBB , &a::REI, 2, 2 }, {	"SBB", &a::SBB, &a::REI, 2, 2 }, { "SBB", &a::SBB, &a::REI, 2, 2 },	// 9
		/*	A	*/{ "ANA", &a::ANA, &a::RED, 1, 1 }, { "ANA", &a::ANA, &a::RED, 1, 1 },	{ "ANA"	, &a::ANA , &a::RED, 1, 1 }, { "ANA" , &a::ANA , &a::RED, 1, 1 }, {	"ANA",  &a::ANA, &a::RED, 1, 1 }, {	"ANA ", &a::ANA , &a::RED, 1, 1 }, { "ANA", &a::ANA, &a::RED, 1, 1 }, {"ANA", &a::ANA, &a::RED, 1, 1 }, {"XRA", &a::XRA, &a::RED, 1, 1 }, { "XRA" , &a::XRA , &a::RED, 1, 1 }, { "XRA" , &a::XRA , &a::RED, 1, 1 }, { "XRA ", &a::XRA , &a::IMP, 1, 1 }, { "XRA", &a::XRA, &a::RED, 1, 1 }, { "XRA ", &a::XRA , &a::RED, 1, 1 }, {	"XRA", &a::XRA, &a::RED, 1, 1 }, { "XRA", &a::XRA, &a::RED, 1, 1 },	// A
		/*	B	*/{ "ORA", &a::ORA, &a::RED, 1, 1 }, { "ORA", &a::ORA, &a::RED, 1, 1 },	{ "ORA"	, &a::ORA , &a::RED, 1, 1 }, { "ORA" , &a::ORA , &a::RED, 1, 1 }, {	"ORA",  &a::ORA, &a::RED, 1, 1 }, {	"ORA ", &a::ORA , &a::RED, 1, 1 }, { "ORA", &a::ORA, &a::RED, 1, 1 }, {"ORA", &a::ORA, &a::RED, 1, 1 }, {"CMP", &a::CMP, &a::RED, 1, 1 }, { "CMP" , &a::CMP , &a::RED, 1, 1 }, { "CMP" , &a::CMP , &a::RED, 1, 1 }, { "CMP ", &a::CMP , &a::IMP, 1, 1 }, { "CMP", &a::CMP, &a::IMM, 3, 1 }, { "CMP ", &a::CMP , &a::RED, 1, 1 }, {	"CMP", &a::CMP, &a::RED, 1, 1 }, { "CMP", &a::CMP, &a::RED, 1, 1 },	// B
		/*	C	*/{ "RNZ", &a::RNZ, &a::REI, 1, 1 }, { "POP", &a::POP, &a::REI, 1, 3 },	{ "JNZ"	, &a::JNZ , &a::IMM, 3, 2 }, { "JMP" , &a::JMP , &a::IMM, 3, 3 }, {	"CNZ",  &a::CNZ, &a::IMM, 3, 2 }, {	"PUSH", &a::PUSH, &a::REI, 1, 3 }, { "ADI", &a::ADI, &a::IMM, 2, 2 }, {"RST", &a::RST, &a::REI, 1, 3 }, {"RZ" , &a::RZ , &a::REI, 1, 1 }, { "RET" , &a::RET , &a::IMP, 1, 3 }, { "JZ"  , &a::JZ  , &a::IMM, 3, 2 }, { "XXX ", &a::XXX , &a::IMP, 1, 1 }, { "CZ ", &a::CZ , &a::IMM, 3, 2 }, { "CALL", &a::CALL, &a::IMM, 3, 5 }, {	"ACI", &a::ACI, &a::IMM, 2, 2 }, { "RST", &a::RST, &a::REI, 1, 3 },	// C
		/*	D	*/{ "RNX", &a::RNC, &a::REI, 1, 1 }, { "POP", &a::POP, &a::REI, 1, 3 },	{ "JNC"	, &a::JNC , &a::IMM, 3, 2 }, { "OUT" , &a::OUT , &a::DIR, 3, 3 }, {	"CNC",  &a::CNC, &a::IMM, 3, 2 }, {	"PUSH", &a::PUSH, &a::REI, 1, 3 }, { "SUI", &a::SUI, &a::IMM, 2, 2 }, {"RST", &a::RST, &a::REI, 1, 3 }, {"RC" , &a::RC , &a::REI, 1, 1 }, { "XXX" , &a::XXX , &a::IMP, 1, 1 }, { "JC"  , &a::JC  , &a::IMM, 3, 2 }, { "IN"  , &a::IN  , &a::IMP, 1, 1 }, { "CC ", &a::CC , &a::IMM, 3, 2 }, { "XXX ", &a::XXX , &a::IMP, 1, 1 }, {	"SBI", &a::SBI, &a::IMM, 2, 2 }, { "RST", &a::RST, &a::REI, 1, 3 },	// D
		/*	E	*/{ "RPO", &a::RPO, &a::REI, 1, 1 }, { "POP", &a::POP, &a::REI, 1, 3 },	{ "JPO"	, &a::JPO , &a::IMM, 3, 2 }, { "XTHL", &a::XTHL, &a::REI, 1, 5 }, {	"CPO",  &a::CPO, &a::IMM, 3, 2 }, {	"PUSH", &a::PUSH, &a::REI, 1, 3 }, { "ANI", &a::ANI, &a::IMM, 2, 2 }, {"RST", &a::RST, &a::REI, 1, 3 }, {"RPE", &a::RPE, &a::REI, 1, 1 }, { "PCHL", &a::PCHL, &a::RED, 1, 1 }, { "JPE" , &a::JPE , &a::IMM, 3, 2 }, { "XCHG", &a::XCHG, &a::RED, 1, 1 }, { "CPE", &a::CPE, &a::IMM, 3, 2 }, { "XXX ", &a::XXX , &a::IMP, 1, 1 }, {	"XRI", &a::XRI, &a::IMM, 2, 2 }, { "RST", &a::RST, &a::REI, 1, 3 },	// E
		/*	F	*/{ "RP" , &a::RP , &a::REI, 1, 1 }, { "POP", &a::POP, &a::REI, 1, 3 },	{ "JP"	, &a::JP  , &a::IMM, 3, 2 }, { "DI"  , &a::DI  , &a::IMP, 1, 1 }, {	"CP	",  &a::CP , &a::IMM, 3, 2 }, {	"PUSH", &a::PUSH, &a::REI, 1, 3 }, { "ORI", &a::ORI, &a::IMM, 2, 2 }, {"RST", &a::RST, &a::REI, 1, 3 }, {"RM" , &a::RM , &a::REI, 1, 1 }, { "SPHL", &a::SPHL, &a::RED, 1, 1 }, { "JM"  , &a::JM  , &a::IMM, 3, 2 }, { "EI  ", &a::EI  , &a::IMP, 1, 1 }, { "CM ", &a::CM , &a::IMM, 3, 2 }, { "XXX ", &a::XXX , &a::IMP, 1, 1 }, {	"CPI", &a::CPI, &a::IMM, 2, 2 }, { "RST", &a::RST, &a::REI, 1, 3 },	// F
			//{{"NOP", &a::NOP, &a::IMP, 1, 4}}
	};
}

mp8085::~mp8085()
{
}

void mp8085::write(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

uint8_t mp8085::read(uint16_t addr)
{
	return bus->read(addr);
}

// Reset microprocessor
void mp8085::reset()
{
	// set all registers to zero
	memset(reg, 0x00, sizeof(reg));
	status = 0x00;
	sp = 0xFFFF; // empty stack

	// get pc value from hard coded address at 0x0100
	addr_abs = 0x0100;
	uint8_t lo = read(addr_abs++);
	uint8_t hi = read(addr_abs);
	pc = (hi << 8) | lo; // going back to start of program

	// Reset private variables too
	addr_rel = 0x0000;
	tmp = 0x00;
	fetched_8 = 0x00;
	fetched_16 = 0x0000;

	// Reset takes time...
	cycles = 8;
}

void mp8085::clock()
{
	// Do stuff:
	if (cycles == 0) {
		opcode = read(pc);
#ifdef LOGMODE
		std::cout << std::showbase << std::internal << std::setfill('0') << std::hex;
		std::cout << "PC:\t" << std::setw(4) << pc << "\n";
		std::cout << "INSTR:\t" << lookup[opcode].name << "\n";
#endif
		pc++;
		cycles = lookup[opcode].cycles;

		(this->*lookup[opcode].addr_mode)();
		(this->*lookup[opcode].operate)();

#ifdef LOGMODE
		std::cout << "A\t" << std::setw(6) << +GetRegister(REGISTER::A) << "\n";
		std::cout << "B\t" << std::setw(6) << +GetRegister(REGISTER::B) << "\n";
		std::cout << "C\t" << std::setw(6) << +GetRegister(REGISTER::C) << "\n";
		std::cout << "D\t" << std::setw(6) << +GetRegister(REGISTER::D) << "\n";
		std::cout << "E\t" << std::setw(6) << +GetRegister(REGISTER::E) << "\n";
		std::cout << "H\t" << std::setw(6) << +GetRegister(REGISTER::H) << "\n";
		std::cout << "L\t" << std::setw(6) << +GetRegister(REGISTER::L) << "\n";
		std::cout << "M\t" << std::setw(6) << +GetRegister(REGISTER::M) << "\n";
		std::cout << "F\t" << std::setw(6) << +GetRegister(REGISTER::F) << "\n\n";
		PrintFlag();
#endif
	}
	// Or don't:
	cycles--;
}

//------------------------------------HELPER FUNCTIONS--------------------------------
uint8_t mp8085::GetFlag(FLAGS f)
{
	return uint8_t((status & f) > 0 ? 1 : 0);
}

void mp8085::SetFlag(FLAGS f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}

uint16_t mp8085::GetRegisterPair(REGISTER rp)
{

	if (rp == REGISTER::C || rp == REGISTER::E || rp == REGISTER::L || rp == REGISTER::A)
		rp = REGISTER((uint8_t)rp - 0x01);

	if (rp == REGISTER::SP) {
		return sp;
	}
	else {
		uint16_t tmp = ((uint16_t(reg[(uint8_t)rp]) << 8) | reg[(uint8_t)rp + 1]);
		return tmp;
	}
}

void mp8085::SetRegisterPair(REGISTER rp, uint16_t d) {
	// Treat M as SP whenever called by SetRegisterPair since that's what the opcodes do anyway

	if (rp == REGISTER::C || rp == REGISTER::E || rp == REGISTER::L)
		rp = REGISTER((uint8_t)rp - 1);

	if (rp == REGISTER::SP) {
		sp = d;
	}
	else {
		uint16_t xyz = ((d & 0xFF00));
		reg[(uint8_t)rp] = (uint8_t)((d & 0xFF00) >> 8);
		reg[(uint8_t)rp + 1] = (d & 0x00FF);
	}
}

void mp8085::WriteRegisterPair(REGISTER rp, uint16_t addr)
{
	uint16_t data = GetRegisterPair(rp);
	write(addr++, data & 0x00FF);
	write(addr++, (data & 0xFF00) >> 8);
}

uint8_t mp8085::GetRegister(REGISTER r) {
	if (r == REGISTER::M) {
		uint16_t addr = (uint16_t(h) << 8 | l);
		uint8_t tmp = read(addr);
		return tmp;
	}
	else if (r == REGISTER::F) {
		return status;
	}
	else {
		return reg[(uint8_t)r];
	}
}

void mp8085::SetRegister(REGISTER r, uint8_t d)
{
	if (r == REGISTER::M)
		write(((h << 8) | l), d);
	else
		reg[(uint8_t)r] = d;
}

void mp8085::SetStatus(uint8_t FLAGSTOSET, REGISTER r = REGISTER::A)
{
	// kindly do not pass AC & CY flags
	// won't do anything anyway
	uint8_t v = GetRegister(r);
	if((FLAGS)FLAGSTOSET & FLAGS::S)
		SetFlag(FLAGS::S, (v >> 7) & 1);
	if((FLAGS)FLAGSTOSET & FLAGS::Z)
		SetFlag(FLAGS::Z, v == 0x00);
	if((FLAGS)FLAGSTOSET & FLAGS::P)
		SetFlag(FLAGS::P, GetParity(v));
}

void mp8085::PrintFlag()
{
	std::cout << "S\t" << +GetFlag(FLAGS::S) << "\n";
	std::cout << "Z\t" << +GetFlag(FLAGS::Z) << "\n";
	std::cout << "P\t" << +GetFlag(FLAGS::P) << "\n";
	std::cout << "AC\t" << +GetFlag(FLAGS::AC) << "\n";
	std::cout << "CY\t" << +GetFlag(FLAGS::CY) << "\n";
}

// Go and get the data using the addressing mode defined (read:only DIR and IMM)

bool mp8085::GetParity(uint16_t v) {
	uint8_t ct = 0;
	for (ct = 0; v; ct++)
		v = v & (v - 1);
	return !(ct & 1);
}

uint8_t mp8085::fetch()
{
	auto curr = lookup[opcode].addr_mode;
	if (curr == &mp8085::DIR)
	{
		fetched_8 = read(addr_abs);
	}
	else if (curr == &mp8085::IMM)
	{
		// else 8-bit fetch
		fetched_8 = read(addr_abs);
	}

	return fetched_8;

	// // Since I don't actually need this
	// else if (curr.addr_mode == &mp8085::REI) {
	// 	fetched_8 = read(addr_abs);
	// }
	// else if (curr.addr_mode == &mp8085::RED) {
	// 	fetched_8 = GetRegister(curr.src);
	// }
}

uint16_t mp8085::fetch_16()
{
	auto curr = lookup[opcode].addr_mode;
	if (curr == &mp8085::DIR)
	{
		uint16_t lo = read(addr_abs++);
		uint16_t hi = read(addr_abs);
		//uint16_t addr_mem = (hi << 8) | lo;
		// Can reuse lo & hi
		//lo = read(addr_mem++);
		//hi = read(addr_mem);
		fetched_16 = (hi << 8) | lo;
	}
	else if (curr == &mp8085::IMM)
	{
		uint16_t lo = read(addr_abs++);
		uint16_t hi = read(addr_abs);
		fetched_16 = (hi << 8) | lo;
	}
	return fetched_16;
}
//-----------------------------------ADDRESSING MODES---------------------------------

// IMM addressing mode means that the data is in the program instruction itself
void mp8085::IMM()
{
	addr_abs = pc;
	pc += (lookup[opcode].bytes - 1);
}

// DIR means that the data is at the address provided
void mp8085::DIR()
{
	uint16_t lo = read(pc++);
	uint16_t hi = read(pc++);
	addr_abs = (hi << 8) | lo;
}

//RED means that the data is in the register provided
void mp8085::RED()
{
	// I need to access the register to get / put data from using fetch
}

// REI means that the data is in the address that is present in the register
void mp8085::REI()
{
	// Register pair will contain the address of the data
	// addr_abs = GetRegisterPair(lookup[opcode].src);
}

// IMP means that the data/registers to work with is implied in the instruction itself
void mp8085::IMP()
{
	// Instruction will handle getting hold of the data to operate on, usually A
	fetched_8 = GetRegister(REGISTER::A);
}

//------------------------------------INSTRUCTIONS------------------------------------

// Don't do anything, but eat clock cycles anyway
void mp8085::NOP()
{

}

// Undefined instruction, in case of typos :P
void mp8085::XXX()
{
	// Unknown instructions caught here
	// Basically NOP	
}

// Load Rp immediately with data provided
void mp8085::LXI()
{
	// 0x01 | 0x11 | 0x21 | 0x31

	// Figure out which Rp we are loading into
	// 0b00 000 001 B
	// 0b00 010 001 D
	// 0b00 100 001 H
	// 0b00 110 001 SP
	mask = (0b111 << 3);

	// Have to treat SP differently
	REGISTER rp = REGISTER((opcode & mask) >> 3);
	SetRegisterPair(rp, fetch_16());
}

void mp8085::RST()
{
	// is irq, will deal with this later
	// For now just do nothing
	// 0b 11 ddd 111
	mask = 0b111;
	uint8_t n = ((mask << 3) & opcode);
	uint16_t addr = (uint16_t)n * 0x01;
	uint8_t lo = pc & 0x00FF;
	uint8_t hi = (pc & 0xFF00) >> 8;

	write(sp++, lo);
	write(sp++, hi);
	pc = addr;
}

void mp8085::DI()
{
	interrupt_enable = false;
}

void mp8085::EI()
{
	interrupt_enable = true;
}

void mp8085::DAD()
{
	// 0b 00 ddd 001
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((opcode & mask) >> 3);
	uint16_t v = GetRegisterPair(REGISTER::H) + GetRegisterPair(REGISTER::L);
	SetRegisterPair(REGISTER::H, v);
	// only affects CY flag
}

void mp8085::IN()
{
	// No hardware to get data from
}

void mp8085::OUT()
{
	// No hardware to give data to
}

void mp8085::SHLD()
{
	// 0x22
	// here addr_abs will have the address we have to write to
	WriteRegisterPair(REGISTER::H, addr_abs);
}

void mp8085::LHLD()
{
	// 0x2A
	// Get data directly from the address specified
	SetRegisterPair(REGISTER::H, fetch_16());
}

void mp8085::STA()
{
	// 32
	write(addr_abs, a);
}

void mp8085::LDA()
{
	// 3A
	a = fetch();
}

void mp8085::INX()
{
	// 0b 00  ddd 011
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((mask & opcode) >> 3);
	uint16_t addr = GetRegisterPair(rp);
	uint8_t val = read(addr);

	write(addr, val + 1);
}

void mp8085::DCX()
{
	// 0b 00  ddd 011
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((mask & opcode) >> 3);
	uint16_t addr = GetRegisterPair(rp);
	uint8_t val = read(addr);

	write(addr, val - 1);
}

void mp8085::INR()
{
	// 00 ddd 100
	mask = 0b111 << 3;
	REGISTER r = REGISTER((mask & opcode) >> 3);
	
	if (r == REGISTER::M)
		cycles += 2;

	SetRegister(r, GetRegister(r) + 1);

	// Deal with flag stuff (Why no CY??)
	//S/Z/AC/P/CY
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P, r);
	//SetFlag(FLAGS::AC, 0);
	//SetFlag(FLAGS::CY, a == 0x00);
}

void mp8085::DCR()
{
	// 00 ddd 101
	mask = 0b111 << 3;
	REGISTER r = REGISTER((mask & opcode) >> 3);
	
	if (r == REGISTER::M)
		cycles += 2;

	SetRegister(r, GetRegister(r) - 1);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P, r);
}

void mp8085::STAX()
{
	//02 || 12	
	// 0b00 ddd 010
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((opcode & mask) >> 3);
	write(GetRegisterPair(rp), a);
}

void mp8085::LDAX()
{
	//0A || 1A
	// 0b00 dd 010
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((opcode & mask) >> 3);
	// GetRegisterPair implementation takes care of the fact that we get 'C' and 'E'
	a = read(GetRegisterPair(rp));
}

void mp8085::CMA()
{
	a = ~a;
}

void mp8085::CMC()
{
	uint8_t v = GetFlag(FLAGS::CY);
	SetFlag(FLAGS::CY, ~v);
}

void mp8085::RLC()
{
	SetFlag(FLAGS::CY, (a >> 7));
	a <<= 1;
	a |= GetFlag(FLAGS::CY);
}

void mp8085::RAL()
{
	FLAGS old_CY = FLAGS(GetFlag(FLAGS::CY));
	SetFlag(FLAGS::CY, (a >> 7));
	a <<= 1;
	a |= old_CY;
}

void mp8085::RRC()
{
	SetFlag(FLAGS::CY, (a & 1));
	a >>= 1;
	a |= (GetFlag(FLAGS::CY) << 7);
}

void mp8085::RAR()
{
	FLAGS old_CY = FLAGS(GetFlag(FLAGS::CY));
	SetFlag(FLAGS::CY, (a & 1));
	a >>= 1;
	a |= (old_CY << 7);
}

void mp8085::DAA()
{
	// Tl;dr: Add 0x00, 0x06, 0x60, 0x66
	// thanks https://www.felixcloutier.com/x86/daa
	uint8_t old_a = a;
	uint8_t old_cy = GetFlag(FLAGS::CY);
	SetFlag(FLAGS::CY, 0);

	if (((a & 0x0f) > 9) || GetFlag(FLAGS::AC)) {
		a += 0x06;
		// if this results in carry, it will overflow
		SetFlag(FLAGS::CY, (old_cy | (a < old_a)));
		SetFlag(FLAGS::AC, 1);
	}
	else {
		SetFlag(FLAGS::AC, 0);
	}

	if ((old_a > 0x99) || old_cy) {
		a += 0x60;
		SetFlag(FLAGS::CY, 1);
	}
	else {
		SetFlag(FLAGS::CY, 0);
	}
}

void mp8085::STC()
{
	SetFlag(FLAGS::CY, 1);
}

void mp8085::RIM()
{
	// totally different from SIM, btw

	// SID I7.5 I6.5 I5.5 IE M7.5 M6.5 M5.5

	// I'm really gonna just ignore everything except IE
	// All h/w stuff I think

	interrupt_enable = ((a >> 3) & 0x1);
}

void mp8085::SIM()
{
	// SOD SDE X R7.5 MSE M7.5 M6.5 M5.5
	// ...yeah ignoring all of them
}

void mp8085::MOV()
{
	// 01 ddd sss
	mask = 0b111 << 3;
	REGISTER dst = REGISTER((mask & opcode) >> 3);

	mask = 0b111;
	REGISTER src = REGISTER(mask & opcode);

	if (dst == REGISTER::M || src == REGISTER::M)
		cycles++;

	SetRegister(dst, GetRegister(src));
}

void mp8085::MVI()
{
	// 00 ddd 110
	// 000	/  001	/  010	/  011	/  100	/  101	/  110
	// B	/  C	/  D	/	E	/	H	/	L	/	M
	mask = 0b111 << 3;
	REGISTER r = REGISTER((opcode & mask) >> 3);
	SetRegister(r, fetch());
}

void mp8085::ADD()
{
	// 0b 10 000 ddd
	mask = 0b111;
	REGISTER r = REGISTER(mask & opcode);

	if (r == REGISTER::M)
		cycles++;

	uint8_t t = a;
	a += GetRegister(r);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, t < a);
}

void mp8085::ADC()
{
	// 0b 100 01 ddd
	mask = 0b111;
	REGISTER r = REGISTER(mask & opcode);

	if (r == REGISTER::M)
		cycles++;

	uint8_t t = a;
	uint8_t c = GetFlag(FLAGS::CY);
	a += (c + GetRegister(r));

	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, t < a);
}

void mp8085::SUB()
{
	//0b 100 10 ddd
	mask = 0b111;
	REGISTER r = REGISTER(mask & opcode);

	if (r == REGISTER::M)
		cycles++;

	a -= GetRegister(r);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, GetFlag(FLAGS::S));
}

void mp8085::SBB()
{
	//0b 100 11 ddd
	mask = 0b111;
	REGISTER r = REGISTER(mask & opcode);

	if (r == REGISTER::M)
		cycles++;

	uint8_t c = GetFlag(FLAGS::CY);
	a -= (GetRegister(r) + c);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	
	// See if we needed borrow or not -> i.e. if answer is -ve
	SetFlag(FLAGS::CY, GetFlag(FLAGS::S));
}

void mp8085::ANA()
{
	// 101 00 sss
	mask = 0b111;
	REGISTER r = REGISTER(opcode & mask);

	if (r == REGISTER::M)
		cycles++;

	a &= GetRegister(r);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
}

void mp8085::XRA()
{
	// 101 01 sss
	mask = 0b111;
	REGISTER r = REGISTER(opcode & mask);

	if (r == REGISTER::M)
		cycles++;

	a ^= GetRegister(r);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
}

void mp8085::CMP()
{
	// 101 11 sss
	mask = 0b111;
	REGISTER r = REGISTER(opcode & mask);

	if (r == REGISTER::M)
		cycles++;

	// Only change flags
	// Now ideally you should re write all the code in SetStatus()
	// But I'm lazy
	// So here's a less-than-ideal, do-not-try-at-home-kids solution:

	uint8_t t = a;
	a -= GetRegister(r);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	a = t;
}
	 
void mp8085::ORA()
{
	// 101 10 sss
	mask = 0b111;
	REGISTER r = REGISTER(opcode & mask);

	if (r == REGISTER::M)
		cycles++;

	a |= GetRegister(r);
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
}

void mp8085::POP()
{
	//0b 11 ddd 001
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((mask & opcode) >> 3);
	uint8_t lo = read(sp++);
	uint8_t hi = read(sp++);

	if (rp == REGISTER::SP)
	{
		a = lo;
		SetRegister(REGISTER::F, hi);
	}
	else 
	{
		uint16_t val = (hi << 8) | lo;
		SetRegisterPair(rp, val);
	}

}

void mp8085::PUSH()
{
	//0b 11 ddd 101
	mask = 0b111 << 3;
	REGISTER rp = REGISTER((mask & opcode) >> 3);
	uint16_t v;
	if (rp == REGISTER::SP)
		v = (a << 8) | GetRegister(REGISTER::F);
	else
		v = GetRegisterPair(rp);
	uint8_t lo = v & 0x00FF;
	uint8_t hi = (v & 0xFF00) >> 8;
	write(--sp, hi);
	write(--sp, lo);
}

void mp8085::JNZ()
{
	if (GetFlag(FLAGS::Z) == 0)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JNC()
{
	if (GetFlag(FLAGS::CY) == 0)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JPO()
{
	if (GetFlag(FLAGS::P) == 0)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JP()
{
	if (GetFlag(FLAGS::S) == 0)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JZ()
{
	if (GetFlag(FLAGS::Z) == 1)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JC()
{
	if (GetFlag(FLAGS::CY) == 1)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JPE()
{
	if (GetFlag(FLAGS::P) == 1)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::JM()
{
	if (GetFlag(FLAGS::S) == 1)
	{
		cycles++;
		pc = fetch_16();
	}
}

void mp8085::RNZ()
{
	if (GetFlag(FLAGS::Z) == 0) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}
}

void mp8085::RNC()
{
	if (GetFlag(FLAGS::CY) == 0) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}

}

void mp8085::RPO()
{
	if (GetFlag(FLAGS::P) == 0) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}

}

void mp8085::RP()
{
	if (GetFlag(FLAGS::S) == 0) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}

}

void mp8085::RZ()
{
	if (GetFlag(FLAGS::Z) == 1) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}

}

void mp8085::RC()
{
	if (GetFlag(FLAGS::CY) == 1) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}

}

void mp8085::RPE()
{
	if (GetFlag(FLAGS::P) == 1) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}
}

void mp8085::RM()
{
	if (GetFlag(FLAGS::S) == 1) {
		cycles += 2;

		uint8_t hi = read(++sp);
		uint8_t lo = read(++sp);

		pc = (hi << 8) | lo;
	}
}

void mp8085::CNZ()
{
	if (GetFlag(FLAGS::Z) == 0) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}
}

void mp8085::CNC()
{
	if (GetFlag(FLAGS::CY) == 0) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}


}

void mp8085::CPO()
{
	if (GetFlag(FLAGS::P) == 0) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}


}

void mp8085::CP()
{
	if (GetFlag(FLAGS::S) == 0) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}


}

void mp8085::CZ()
{
	if (GetFlag(FLAGS::Z) == 1) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}


}

void mp8085::CC()
{
	if (GetFlag(FLAGS::CY) == 1) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}



}

void mp8085::CPE()
{
	if (GetFlag(FLAGS::P) == 1) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}



}

void mp8085::CM()
{
	if (GetFlag(FLAGS::S) == 1) {
		cycles += 3;

		uint8_t lo = pc & 0x00FF;
		uint8_t hi = (pc & 0xFF00) >> 8;
		// Pushing to stack is basically writing to mem @ sp
		write(sp--, lo);
		write(sp--, hi);

		pc = fetch_16();
	}



}

void mp8085::JMP()
{
	pc = fetch_16();
}

void mp8085::RET()
{
	uint8_t hi = read(++sp);
	uint8_t lo = read(++sp);

	pc = (hi << 8) | lo;
}

void mp8085::CALL()
{
	uint8_t lo = pc & 0x00FF;
	uint8_t hi = (pc & 0xFF00) >> 8;
	// Pushing to stack is basically writing to mem @ sp
	write(sp--, lo);
	write(sp--, hi);

	pc = fetch_16();
}

void mp8085::HLT()
{

}

void mp8085::ADI()
{
	uint8_t t = a;
	a += fetch();
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, t < a);
}

void mp8085::SUI()
{
	a -= fetch();
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, GetFlag(FLAGS::S));
}

void mp8085::ANI()
{
	a &= fetch();
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
}

void mp8085::ORI()
{
	a |= fetch();
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
}

void mp8085::ACI()
{
	uint8_t t = a;
	a += (fetch() + GetFlag(FLAGS::CY));
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, t < a);
}

void mp8085::SBI()
{
	a -= (fetch() + GetFlag(FLAGS::CY));
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	SetFlag(FLAGS::CY, GetFlag(FLAGS::S));
}

void mp8085::XRI()
{
	a ^= fetch();
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
}

void mp8085::CPI()
{
	uint8_t t = a;
	a -= fetch();
	SetStatus(FLAGS::S | FLAGS::Z | FLAGS::P);
	a = t;
}

void mp8085::XTHL()
{
	// E3
	uint16_t d = GetRegisterPair(REGISTER::H);
	SetRegisterPair(REGISTER::H, sp);
	sp = d;

	//uint8_t t = h;
	//h = read(sp);
	//write(sp, t);

	//t = l;
	//l = read(sp + 1);
	//write(sp + 1, t);
}

void mp8085::PCHL()
{
	//E9
	pc = GetRegisterPair(REGISTER::H);
}

void mp8085::XCHG()
{
	uint16_t d = GetRegisterPair(REGISTER::H);
	SetRegisterPair(REGISTER::H, GetRegisterPair(REGISTER::D));
	SetRegisterPair(REGISTER::D, d);
	/*
	uint8_t t = h;
	h = d;
	d = t;

	t = l;
	l = e;
	e = t;
	*/
}

void mp8085::SPHL()
{
	//F9
	sp = GetRegisterPair(REGISTER::H);
}
