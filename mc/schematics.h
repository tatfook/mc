#pragma once

#include <stdint.h>
#include "mc/nbt.h"

namespace mc_map {

	class Schematics {
	public:
		bool Load(std::string filename);

		/*int GetBlockIndex(int x, int y, int z) { return (y * 16 + z) * 16 + x; }
		int16_t GetBlockId(int x, int y, int z) { return m_block_ids[GetBlockIndex(x, y, z)]; }
		int16_t GetBlockData(int x, int y, int z) { return m_block_datas[GetBlockIndex(x, y, z)]; }*/
	private:
		//void PlatteToIdAndData(nbt::TagList* tagPalettes, uint16_t platte_indexs[4096]);
	public:
		std::vector<int16_t> m_block_ids;
		std::vector<int8_t> m_block_datas;
		uint16_t m_width;
		uint16_t m_height;
		uint16_t m_length;
	};
}
