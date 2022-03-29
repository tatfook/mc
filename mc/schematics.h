#pragma once

#include <stdint.h>
#include "mc/nbt.h"

namespace mc_map {

	class Schematics {
	public:
		bool Load(std::string filename);
		uint16_t GetWidth() { return m_width; }
		uint16_t GetHeight() { return m_height; }
		uint16_t GetLength() { return m_length; }

		int GetBlockIndex(int x, int y, int z) { return x + z * m_width + y * m_width * m_length; }
		int16_t GetBlockId(int x, int y, int z) { return m_block_ids[GetBlockIndex(x, y, z)]; }
		int16_t GetBlockData(int x, int y, int z) { return m_block_datas[GetBlockIndex(x, y, z)]; }
		std::string GetBlockName(int x, int y, int z);

		bool GetParaCraftBlockIdAndData(int x, int y, int z, uint16_t& block_id, uint16_t& block_data);
	private:
		//void PlatteToIdAndData(nbt::TagList* tagPalettes, uint16_t platte_indexs[4096]);
	public:
		std::vector<int16_t> m_block_ids;
		std::vector<int8_t> m_block_datas;
		std::vector<std::string> m_block_names;
		std::vector<uint32_t> m_block_name_indexs;
		uint16_t m_width;
		uint16_t m_height;
		uint16_t m_length;
	};
}
