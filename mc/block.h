#pragma once

#include <exception>
#include <map>
#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>

namespace mc_map {
	namespace block {
		void InitBlockMap();

		int16_t GetBlockIdByName(std::string name);
		int8_t GetBlockDataByName(std::string name);
	}
}