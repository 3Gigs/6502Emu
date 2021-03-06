#pragma once
#include <cstdint>
#include <array>

const int MEM_SIZE = 0x10000;

namespace emu6502
{
	class RAM
	{
	public:
		RAM();
		~RAM();
		void init();
		uint8_t mem_read(uint16_t addr);
		void mem_write(uint16_t addr, uint8_t data);
	private:
		std::array<uint8_t, MEM_SIZE> ram;
	};
}


