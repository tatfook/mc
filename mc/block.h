#pragma once

#include <exception>
#include <map>
#include <set>
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>

namespace mc_map {
	namespace block {
		void StaticInit();
		void AddParaCraftBlock(int id, int data, std::string name);
		int16_t GetParaCraftBlockIdByMineCraftName(std::string name);
		uint8_t GetParaCraftBlockDataByMineCraftName(std::string name);
		bool MineCraftToParaCraft(uint16_t& block_id, uint16_t& block_data);

		void AddMineCraftBlock(int id, int data, std::string name);
		int16_t GetBlockIdByName(std::string name);
		uint8_t GetBlockDataByName(std::string name);
		std::set<std::string>& GetNotExistBlockNameSet();
		bool IsStairBlock(uint16_t blockId);
		bool IsSolidBlock(int block_id);
	}
}