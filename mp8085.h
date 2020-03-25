//#ifndef MP8085_H
//#define MP8085_H

#pragma once
#include <cstdint>
//#include "Bus.h"
#include <string>
#include <vector>

#define LOGMODE 

#ifdef LOGMODE
#include <stdio.h>
#include <iostream>
#include <iomanip>
#endif

class Bus;

class mp8085
{
public:
	mp8085();
	~mp8085();

	enum class REGISTER
	{
		// Numbering the registers the same way op codes use them
		B = 0b0000,
		C = 0b0001,
		D = 0b0010,
		E = 0b0011,
		H = 0b0100,
		L = 0b0101,
		M = 0b0110, // and sometimes SP
		SP = 0b110, // Apparently this is allowed, see:
		// https://stackoverflow.com/questions/11412516/why-can-two-different-enum-enumeration-constants-have-the-same-integer-value
		A = 0b0111,
		F = 0b1000,
		Y = 0b1010 // Unused 
	};

	// General-purpose registers
	uint8_t reg[10];

	// We won't actually use reg[M|SP], but storing its value anyway
	// Can consider storing status there if needed

	// ~Define aliases to avoiding having to write reg[B] every time~
	// Actually I shouldn't be writing reg[B] everytime, use GetRegister(B) for safetly reasons(?)
	// ...
	// I have no clue what I'm talking about tbh

	uint8_t& b = reg[(uint8_t)REGISTER::B];
	uint8_t& c = reg[(uint8_t)REGISTER::C];
	uint8_t& d = reg[(uint8_t)REGISTER::D];
	uint8_t& e = reg[(uint8_t)REGISTER::E];
	uint8_t& h = reg[(uint8_t)REGISTER::H];
	uint8_t& l = reg[(uint8_t)REGISTER::L];
	uint8_t& a = reg[(uint8_t)REGISTER::A];
	uint8_t& status = reg[(uint8_t)REGISTER::F]; // Status/Flag register
	uint16_t sp, pc;

	// Stack pointer works bottom up i.e 0xFFFF is base of the stack
	// Program counter defaults to 0x2000, program data is stored there
	// Remember that 8085 has Von Neumann architecture

	void reset();
	void start(); // signal to start the cpu
	void clock(); // Can use external clock directly
	void irq(uint8_t n);   // When you get an interrupt request

	void ConnectBus(Bus* n) { bus = n; }

	enum FLAGS
	{
		CY = (1 << 0),// Carry bit
		P = (1 << 2), // Parity
		AC = (1 << 4),// Aux. carry
		Z = (1 << 6), // Zero
		S = (1 << 7), // Sign
	};

private:
	uint8_t  GetFlag(FLAGS f);
	void	 SetFlag(FLAGS f, bool v);
	uint16_t GetRegisterPair(REGISTER r);
	uint8_t  GetRegister(REGISTER r);
	void	 SetRegisterPair(REGISTER rp, uint16_t d);
	void	 SetRegister(REGISTER rp, uint8_t d);
	void	 WriteRegisterPair(REGISTER r, uint16_t a);
	void	 SetStatus(uint8_t FLAGSTOSET, REGISTER r); // Sets status flags to reflect the value in the accumulator
	void	 PrintFlag(); // helper function
	bool	 GetParity(uint16_t v); // Helper function since MSVC doens't have __builtin_popcount
								// Can consider making this O(1) lookup later
	
	uint16_t addr_abs	= 0x0000;
	uint16_t addr_rel	= 0x0000;
	uint16_t tmp		= 0x00;

	// 8-bit fetch
	uint8_t	 fetched_8	= 0x00;

	// 16-bit fetch
	// used in case of immediate addressing
	// or when data is pointed to by the address
	uint16_t fetched_16 = 0x0000;

	//opcode of instruction
	uint8_t	 opcode = 0x00;
	uint8_t	 cycles = 0;
	Bus* bus = nullptr;
	uint8_t mask = 0b00000000;
	bool interrupt_enable = true;
	bool is_running = false;

	// The two main things that a microproccesor does
	void	write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);
	uint8_t fetch();
	uint16_t fetch_16();
	struct INSTRUCTION 
	{
		std::string name;
		void (mp8085::* operate )(void)  = nullptr;
		void (mp8085::* addr_mode)(void) = nullptr;
		// By default all operations happen with respect to the accumulator
		uint8_t bytes = 0;
		uint8_t	cycles = 0;
	};

	std::vector<INSTRUCTION> lookup;

	/*
		Addressing modes
		Only five of them, thankfully.
		1. IMMediate
		2. ABSolute/Direct
		3. REgister Direct
		4. REgister Indirect
		5. IMPlied

		Maybe remove addressing mode thing altogether?
		Don't see the point since the addressing mode doesn't change for the same instruction
	*/

	void IMM();
	void DIR();
	void RED();
	void REI();
	void IMP();
	
	/* Operations */
	void NOP() ; void RST() ; void LXI() ; void DAD() ;
	void SHLD(); void LHLD(); void STA() ; void LDA() ;
	void INX() ; void DCX() ; void INR() ; void DCR() ;
	void STAX(); void LDAX(); void CMA() ; void CMC() ;
	void RLC() ; void RAL() ; void RRC() ; void RAR() ;
	void DAA() ; void STC() ; void RIM() ; void SIM() ;
	void MOV() ; void MVI() ; void ADD() ; void ADC() ;
	void SUB() ; void SBB() ; void ANA() ; void XRA() ;
	void CMP() ; void ORA() ; void POP() ; void PUSH();
	void JNZ() ; void JNC() ; void JPO() ; void JP()  ;
	void JZ()  ; void JC()  ; void JPE() ; void JM()  ;
	void RNZ() ; void RNC() ; void RPO() ; void RP()  ;
	void RZ()  ; void RC()  ; void RPE() ; void RM()  ;
	void CNZ() ; void CNC() ; void CPO() ; void CP()  ;
	void CZ()  ; void CC()  ; void CPE() ; void CM()  ;
	void JMP() ; void RET() ; void CALL(); void HLT() ;
	void ADI() ; void SUI() ; void ANI() ; void ORI() ;
	void ACI() ; void SBI() ; void XRI() ; void CPI() ;
	void XTHL(); void PCHL(); void XCHG(); void SPHL();
	void DI()  ; void EI()  ; void IN()  ; void OUT() ;
	
	void XXX() ;
};

//#endif