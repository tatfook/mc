#include <iostream>

#include "mc/section.h"
#include "mc/nbt.h"
#include "mc/block.h"

namespace mc_map {
	

	static void post116(const uint8_t index_length, const std::vector<int64_t>* blockStates, uint16_t* buffer) {
		// NEW in 1.16, longs are padded by 0s when a block cannot fit, so no more
		// overflow to deal with !

		for (uint16_t index = 0; index < 4096; index++) {
			// Determine how many indexes each long holds
			const uint8_t blocksPerLong = 64 / index_length;

			// Calculate where in the long array is the long containing the right index.
			const uint16_t longIndex = index / blocksPerLong;

			// Once we located a long, we have to know where in the 64 bits
			// the relevant block is located.
			const uint8_t padding = (index - longIndex * blocksPerLong) * index_length;

			// Bring the data to the first bits of the long, then extract it by bitwise
			// comparison
			const uint16_t blockIndex = ((*blockStates)[longIndex] >> padding) & ((uint64_t(1) << index_length) - 1);

			buffer[index] = blockIndex;
		}
	}

	static void pre116(const uint8_t index_length, const std::vector<int64_t>* blockStates, uint16_t* buffer) {
		// The `BlockStates` array contains data on the section's blocks. You have to
		// extract it by understanfing its structure.
		//
		// Although it is a array of long values, one must see it as an array of block
		// indexes, whose element size depends on the size of the Palette. This
		// routine locates the necessary long, extracts the block with bit
		// comparisons.
		//
		// The length of a block index has to be coded on the minimal possible size,
		// which is the logarithm in base2 of the size of the palette, or 4 if the
		// logarithm is smaller.

		for (uint16_t index = 0; index < 4096; index++) {

			// We skip the `position` first blocks, of length `size`, then divide by 64
			// to get the number of longs to skip from the array
			const uint16_t skip_longs = (index * index_length) >> 6;

			// Once we located the data in a long, we have to know where in the 64 bits
			// it is located. This is the remaining of the previous operation
			const int8_t padding = (index * index_length) & 63;

			// Sometimes the data of an index does not fit entirely into a long, so we
			// check if there is overflow
			const int8_t overflow = (padding + index_length > 64 ? padding + index_length - 64 : 0);

			// This complicated expression extracts the necessary bits from the current
			// long.
			//
			// Lets say we need the following bits in a long (not to scale):
			// 10011100111001110011100
			//    ^^^^^
			// We do this by shifting (>>) the data by padding, to get the relevant bits
			// on the end of the long:
			// ???????????????10011100
			//                   ^^^^^
			// We then apply a mask to get only the relevant bits:
			// ???????????????10011100
			// 00000000000000000011111 &
			// 00000000000000000011100 <- result
			//
			// The mask is made at the size of the data, using the formula (1 << n) - 1,
			// the resulting bitset is of the following shape: 0...01...1 with n 1s.
			//
			// If there is an overflow, the mask size is reduced, as not to catch noise
			// from the padding (ie the interrogation points earlier) that appear on
			// ARM32.
			uint16_t lower_data = ((*blockStates)[skip_longs] >> padding) & ((uint64_t(1) << (index_length - overflow)) - 1);

			if (overflow > 0) {
				// The exact same process is used to catch the overflow from the next long
				const uint16_t upper_data = ((*blockStates)[skip_longs + 1]) & ((uint64_t(1) << overflow) - 1);
				// We then associate both values to create the final value
				lower_data = lower_data | (upper_data << (index_length - overflow));
			}

			// lower_data now contains the index in the palette
			buffer[index] = lower_data;
		}
	}

	Section::Section()
	{
		m_y = 0;
		for (int i = 0; i < 4096; i++) {
			m_block_ids[i] = 0;
			m_block_datas[i] = 0;
		}
	}

	void Section::PlatteToIdAndData(nbt::TagList* tagPalettes, uint16_t* platte_indexs)
	{
		size_t palette_size = tagPalettes->payload.size();
		for (int i = 0; i < 4096; i++) {
			uint16_t palette_index = platte_indexs[i];
			nbt::TagCompound* tagBlock = (nbt::TagCompound*)tagPalettes->payload.at(palette_index);
			if (tagBlock == NULL) continue;
			nbt::TagString* tagName = tagBlock->findTag<nbt::TagString>("Name", nbt::TAG_STRING);
			if (tagName) {
				std::string blockName = tagName->payload;
				m_block_ids[i] = block::GetBlockIdByName(blockName);
				m_block_datas[i] = block::GetBlockDataByName(blockName);
			}
		}
	}

	bool Section::ParseNBTSection(nbt::TagCompound* tagSection, int dataVersion)
	{
		if (tagSection == NULL) return false;
		nbt::TagByte* tagY = tagSection->findTag<nbt::TagByte>("Y", nbt::TAG_BYTE);
		if (tagY == NULL) return false;
		m_y = tagY->payload;

		if (dataVersion >= 2840) {
			nbt::TagCompound* tagBlockStates = tagSection->findTag<nbt::TagCompound>("block_states", nbt::TAG_COMPOUND);
			if (tagBlockStates == NULL) return false;
			nbt::TagLongArray* tagDatas = tagBlockStates->findTag<nbt::TagLongArray>("data", nbt::TAG_LONG_ARRAY);
			nbt::TagList* tagPalettes = tagBlockStates->findTag<nbt::TagList>("palette", nbt::TAG_LIST);
			if (tagDatas == NULL || tagPalettes == NULL) return false;
			std::vector<int64_t>* blockStates = &(tagDatas->payload);
			size_t paltte_size = tagPalettes->payload.size();
			const uint8_t blockBitLength = std::max(uint8_t(ceil(log2(paltte_size))), uint8_t(4));
			uint16_t* platte_indexs = new uint16_t[4096];
			post116(blockBitLength, &(tagDatas->payload), platte_indexs);
			PlatteToIdAndData(tagPalettes, platte_indexs);
			delete[] platte_indexs;
		}
		else if (dataVersion >= 1628) {
			nbt::TagLongArray* tagBlockStates = tagSection->findTag<nbt::TagLongArray>("BlockStates", nbt::TAG_LONG_ARRAY);
			nbt::TagList* tagPalettes = tagSection->findTag<nbt::TagList>("Palette", nbt::TAG_LIST);
			if (tagBlockStates == NULL || tagPalettes == NULL) return false;
			size_t paltte_size = tagPalettes->payload.size();
			const uint8_t blockBitLength = std::max(uint8_t(ceil(log2(paltte_size))), uint8_t(4));
			uint16_t* platte_indexs = new uint16_t[4096];
			if (dataVersion >= 2534)
				post116(blockBitLength, &(tagBlockStates->payload), platte_indexs);
			else
				pre116(blockBitLength, &(tagBlockStates->payload), platte_indexs);
			PlatteToIdAndData(tagPalettes, platte_indexs);
			delete[] platte_indexs;
		}
		else {
			nbt::TagByteArray* tagBlocks = tagSection->findTag<nbt::TagByteArray>("Blocks", nbt::TAG_BYTE_ARRAY);
			nbt::TagByteArray* tagAdd = tagSection->findTag<nbt::TagByteArray>("Add", nbt::TAG_BYTE_ARRAY);
			nbt::TagByteArray* tagData = tagSection->findTag<nbt::TagByteArray>("Data", nbt::TAG_BYTE_ARRAY);
			// make sure section is valid
			if (tagBlocks == NULL || tagData == NULL || tagBlocks->payload.size() != 4096 || tagData->payload.size() != 2048) return false;

			uint8_t* blocks = new uint8_t[16 * 16 * 16];
			uint8_t* adds = new uint8_t[16 * 16 * 8];
			uint8_t* datas = new uint8_t[16 * 16 * 8];

			std::copy(tagBlocks->payload.begin(), tagBlocks->payload.end(), blocks);
			if (tagAdd == NULL || tagAdd->payload.size() != 2048)
				std::fill(&adds[0], &adds[2048], 0);
			else
				std::copy(tagAdd->payload.begin(), tagAdd->payload.end(), adds);
			std::copy(tagData->payload.begin(), tagData->payload.end(), datas);

			for (int x = 0; x < 16; x++) {
				for (int y = 0; y < 16; y++) {
					for (int z = 0; z < 16; z++) {
						int blockindex = (y * 16 + z) * 16 + x;
						int16_t add = 0;
						if ((blockindex % 2) == 0)
							add = adds[blockindex / 2] & 0x0f;
						else
							add = (adds[blockindex / 2] >> 4) & 0x0f;
						m_block_ids[blockindex] = blocks[blockindex] + (add << 8);

						int8_t data = 0;
						if ((blockindex % 2) == 0)
							data = datas[blockindex / 2] & 0x0f;
						else
							data = (datas[blockindex / 2] >> 4) & 0x0f;
						m_block_datas[blockindex] = data;
					}
				}
			}
			delete[] blocks;
			delete[] adds;
			delete[] datas;
		}
		return true;
	}
}
