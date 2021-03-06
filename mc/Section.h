#pragma once

#include <stdint.h>
#include "mc/nbt.h"

namespace mc_map {

	class Section {
	public:
		Section();
		bool ParseNBTSection(nbt::TagCompound* tagSection, int dataVersion);

		int8_t GetY() { return m_y; }
		int GetBlockIndex(int x, int y, int z) { return (y * 16 + z) * 16 + x; }
		int16_t GetBlockId(int x, int y, int z) { return m_block_ids[GetBlockIndex(x, y, z)]; }
		int16_t GetBlockData(int x, int y, int z) { return m_block_datas[GetBlockIndex(x, y, z)]; }
		std::string GetBlockName(int x, int y, int z);
		bool GetParaCraftBlockIdAndData(int x, int y, int z, uint16_t& block_id, uint16_t& block_data);
	private:
		void PlatteToIdAndData(nbt::TagList* tagPalettes, uint16_t platte_indexs[4096]);
	public:
		int8_t m_y;
		int16_t m_block_ids[4096];
		int8_t m_block_datas[4096];
		std::vector<std::string> m_block_names;
		std::vector<uint32_t> m_block_name_indexs;
	};
}

