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

#include "mc/chunk.h"

#include "mc/nbt.h"

#include <iostream>
#include <cmath>
#include "MCblock.h"

namespace mapcrafter {
	namespace mc {

		Chunk::Chunk() : m_pos(0, 0), m_rotation(0), m_data_version(0) {
			clear();
		}

		Chunk::~Chunk() {
			clear();
		}

		void Chunk::setRotation(int rotation) {
			this->m_rotation = rotation;
		}

		/**
		 * Reads the chunk from (compressed) nbt data.
		 */
		bool Chunk::readNBT(const char* data, size_t len, nbt::CompressionType compression) {
			clear();

			nbt::NBTFile root;
			root.readNBT(data, len, compression);

			// find "DataVersion" tag
			nbt::TagInt* dataVersion = root.findTag<nbt::TagInt>("DataVersion", nbt::TAG_INT);
			if (dataVersion == NULL) {
				m_data_version = 0; // °æ±¾Ì«¾É ÖÃ0
			}
			else {
				m_data_version = dataVersion->payload;
			}

			// find "level" tag
			nbt::TagCompound* level = root.findTag<nbt::TagCompound>("Level", nbt::TAG_COMPOUND);
			if (m_data_version < 2844 && level == NULL) {
				std::cerr << "Warning: Corrupt chunk (No level tag)!" << std::endl;
				return false;
			}

			//// then find x/z pos of the chunk
			//nbt::TagInt* xpos = level->findTag<nbt::TagInt>("xPos", nbt::TAG_INT);
			//nbt::TagInt* zpos = level->findTag<nbt::TagInt>("zPos", nbt::TAG_INT);
			//if (xpos == NULL || zpos == NULL) {
			//	std::cerr << "Warning: Corrupt chunk (No x/z position found)!" << std::endl;
			//	return false;
			//}
			//m_pos = ChunkPos(xpos->payload, zpos->payload);
			//if (m_rotation) m_pos.rotate(m_rotation);

			//// then find Biomes of the chunk
			//nbt::TagByteArray* tagBiomes = level->findTag<nbt::TagByteArray>("Biomes", nbt::TAG_BYTE_ARRAY);
			//if (tagBiomes != NULL && tagBiomes->payload.size() == 256)
			//	std::copy(tagBiomes->payload.begin(), tagBiomes->payload.end(), m_biomes);
			//else 
			//	std::cerr << "Warning: Corrupt chunk at " << m_pos.x << ":" << m_pos.z << " (No biome data found)!" << std::endl;

			nbt::TagList* tagSections = NULL;
			// find sections list
			if (m_data_version >= 2844) {
				tagSections = root.findTag<nbt::TagList>("Sections", nbt::TAG_LIST);
				if (tagSections == NULL) tagSections = root.findTag<nbt::TagList>("sections", nbt::TAG_LIST);
				nbt::TagString* tagStatus = root.findTag<nbt::TagString>("Status", nbt::TAG_STRING);
				if (tagStatus == NULL || tagStatus->payload != "full") return false;
			}
			else if (m_data_version >= 1976) {
				tagSections = level->findTag<nbt::TagList>("Sections", nbt::TAG_LIST);
				nbt::TagString* tagStatus = level->findTag<nbt::TagString>("Status", nbt::TAG_STRING);
				if (tagStatus == NULL || tagStatus->payload != "full") return false;
			}
			else if (m_data_version >= 1628) {
				tagSections = level->findTag<nbt::TagList>("Sections", nbt::TAG_LIST);
				nbt::TagString* tagStatus = level->findTag<nbt::TagString>("Status", nbt::TAG_STRING);
				if (tagStatus == NULL || tagStatus->payload != "postprocessed") return false;
			}
			else {
				tagSections = level->findTag<nbt::TagList>("Sections", nbt::TAG_LIST);
			}

			if (tagSections == NULL || tagSections->tag_type != nbt::TAG_COMPOUND) {
				std::cerr << "Warning: Corrupt chunk at " << m_pos.x << ":" << m_pos.z << " (No valid sections list found)!" << std::endl;
				return false;
			}

			// go through all sections
			for (std::vector<nbt::NBTTag*>::const_iterator it = tagSections->payload.begin(); it != tagSections->payload.end(); ++it) {
				Section* section = new Section;

				nbt::TagCompound* tagSection = (nbt::TagCompound*)*it;

				if (section->ParseNBTSection(tagSection, m_data_version)) {
					m_sections.push_back(section);
				}
				else {
					delete section;
				}
			}

			return true;
		}

		/**
		 * Clears the whole chunk data.
		 */
		void Chunk::clear() {
			for (std::vector<Section*>::iterator it = m_sections.begin(); it != m_sections.end(); it++) {
				delete* it;
			}
			m_sections.clear();
		}

		Section* Chunk::getSection(int y)
		{
			for (std::vector<Section*>::iterator it = m_sections.begin(); it != m_sections.end(); it++) {
				if ((*it)->GetY() == y) return *it;
			}
			return NULL;
		}
		bool Chunk::hasSection(int y) {
			return getSection(y) != NULL;
		}

		void rotateBlockPos(int& x, int& z, int rotation) {
			int nx = x, nz = z;
			for (int i = 0; i < rotation; i++) {
				nx = z;
				nz = 15 - x;
				x = nx;
				z = nz;
			}
		}

		/**
		 * Returns the block id at a position.
		 */
		uint16_t Chunk::getBlockID(const LocalBlockPos& pos) {
			Section* section = getSection(pos.y / 16);
			if (section == NULL) return 0;

			int x = pos.x;
			int y = pos.y % 16;
			int z = pos.z;
			if (m_rotation) rotateBlockPos(x, z, m_rotation);

			return section->GetBlockId(x, y, z);
		}

		/**
		 * Returns the block data at a position.
		 */
		uint8_t Chunk::getBlockData(const LocalBlockPos& pos) {
			Section* section = getSection(pos.y / 16);
			if (section == NULL) return 0;

			int x = pos.x;
			int y = pos.y % 16;
			int z = pos.z;
			if (m_rotation) rotateBlockPos(x, z, m_rotation);

			return section->GetBlockData(x, y, z);
		}

		uint8_t Chunk::getBiomeAt(const LocalBlockPos& pos) {
			int x = pos.x;
			int z = pos.z;
			if (m_rotation) rotateBlockPos(x, z, m_rotation);

			return m_biomes[z * 16 + x];
		}

		//MCBlock::IsSolidBlock

		bool Chunk::hasSolidBlock(const LocalBlockPos& pos)
		{
			uint16_t block_id = getBlockID(pos);
			if (block_id > 0)
			{
				if (MCBlock::IsSolidBlock(block_id))
					return true;
			}
			return false;
		}

		bool Chunk::hasBlock(const LocalBlockPos& pos, const uint16_t block_id, const uint8_t data, const uint8_t state)
		{
			bool hasBlock = true;
			uint16_t real_block_id = getBlockID(pos);
			if (real_block_id == 0)
				return false;

			if (block_id == 0)
				return true;

			if (real_block_id != block_id)
				return false;

			if (data == 255)
				return true;

			uint16_t real_data = getBlockData(pos);
			if (real_data != data)
				return false;

			/*if (state != 255)
			{
				uint16_t real_state = getBlockState(pos);
				if (real_state != state)
					hasBlock = false;
			}*/
			return hasBlock;
		}

		bool Chunk::GetBlockInfo(LocalBlockPos pos, uint16_t& block_id, uint8_t& data, uint8_t& state)
		{
			uint16_t real_block_id = getBlockID(pos);
			if (real_block_id == 0)
				return false;
			block_id = real_block_id;
			data = getBlockData(pos);
			return true;
		}

		const ChunkPos& Chunk::getPos() const {
			return m_pos;
		}

	}
}
