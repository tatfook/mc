/*
 * Copyright 2012, 2013 Moritz Hilscher
 *
 * This file is part of mapcrafter.
 *
 * mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include "mc/nbt.h"
#include "mc/pos.h"
#include "mc/section.h"

#include <stdint.h>

namespace mc_map {

	/**
	 * This is a Minecraft chunk from the Anvil world format. The class stores only the
	 * sections, which exist in the nbt data, to save memory.
	 */
	class Chunk {
	private:
		ChunkPos m_pos;

		int m_data_version;
		int m_rotation;
		std::vector<Section*> m_sections;
		uint8_t m_biomes[256];
	public:
		Chunk();
		virtual ~Chunk();

		void setRotation(int rotation);

		bool readNBT(const char* data, size_t len, nbt::CompressionType compression = nbt::ZLIB);
		void clear();

		bool hasSection(int y);
		Section* Chunk::getSection(int y);
		int getMinSectionIndex();
		uint16_t getBlockID(const LocalBlockPos& pos);
		uint8_t getBlockData(const LocalBlockPos& pos);


		bool Chunk::hasSolidBlock(const LocalBlockPos& pos);
		// if data or state value is 255,don't need check it;
		bool hasBlock(const LocalBlockPos& pos, const uint16_t block_id = 0, const uint8_t data = 255, const uint8_t state = 255);

		bool GetBlockInfo(LocalBlockPos pos, uint16_t& block_id, uint8_t& data, uint8_t& state);

		/*uint8_t getBlockState(const LocalBlockPos& pos) const;
		uint8_t getStairsBlockState(const LocalBlockPos& pos,uint16_t block_id) const;*/

		uint8_t getBiomeAt(const LocalBlockPos& pos);

		const ChunkPos& getPos() const;
	};
}

#endif /* CHUNK_H_ */
