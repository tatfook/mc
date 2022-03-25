#pragma once

#include <exception>
#include <map>
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>

namespace mc_map {
	namespace block {
		void StaticInit();

		void AddMineCraftBlock(int id, int data, std::string name);
		int16_t GetBlockIdByName(std::string name);
		int8_t GetBlockDataByName(std::string name);

		bool IsStairBlock(uint16_t blockId);
		bool IsSolidBlock(int block_id);

	}
}