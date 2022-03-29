
#include "mc/schematics.h"
#include "mc/block.h"

#include <iostream>

namespace mc_map {
	bool Schematics::Load(std::string filename)
	{
		nbt::NBTFile root;
		try {
			root.readNBT(filename.c_str());
		}
		catch (std::exception& e) {
			std::cout << "parse nbt file failed:" << filename  << e.what() << std::endl;
			return false;
		}

		nbt::TagShort* tagWidth = root.findTag<nbt::TagShort>("Width", nbt::TAG_SHORT);
		nbt::TagShort* tagHeight = root.findTag<nbt::TagShort>("Height", nbt::TAG_SHORT);
		nbt::TagShort* tagLength = root.findTag<nbt::TagShort>("Length", nbt::TAG_SHORT);
		if (tagWidth == NULL || tagHeight == NULL || tagLength == NULL) {
			std::cerr << "Invalid Width Height Length" << std::endl;
			return false;
		}
		m_width = tagWidth->payload;
		m_height = tagHeight->payload;
		m_length = tagLength->payload;
		uint32_t size = m_width * m_height * m_length;
		m_block_ids.resize(size);
		m_block_datas.resize(size);
		m_block_name_indexs.resize(size);

		nbt::TagInt* tagDataVersion = root.findTag<nbt::TagInt>("DataVersion", nbt::TAG_INT);
		int dataVersion = 0;
		if (tagDataVersion != NULL) dataVersion = tagDataVersion->payload;

		nbt::TagString* tagMaterials = root.findTag<nbt::TagString>("Materials", nbt::TAG_STRING);

		if (dataVersion > 2840) {
			nbt::TagCompound* tagPalette = root.findTag<nbt::TagCompound>("Palette", nbt::TAG_COMPOUND);
			nbt::TagInt* tagPaletteMax = root.findTag<nbt::TagInt>("PaletteMax", nbt::TAG_INT);
			nbt::TagByteArray* tagBlockData = root.findTag<nbt::TagByteArray>("BlockData", nbt::TAG_BYTE_ARRAY);
			if (tagPalette == NULL || tagPaletteMax == NULL || tagBlockData == NULL) return false;
			std::vector<std::string> blockNames(tagPaletteMax->payload);
			m_block_names.resize(tagPaletteMax->payload);
			std::map<std::string, nbt::NBTTag*> palette = tagPalette->payload;
			for (std::map<std::string, nbt::NBTTag*>::iterator it = palette.begin(); it != palette.end(); it++) {
				std::string name = it->first;
				int index = ((nbt::TagInt*)(it->second))->payload;
				blockNames[index] = name.substr(0, name.find_first_of('['));
				m_block_names[index] = name.substr(0, name.find_first_of('['));
			}
			std::vector<int8_t>& blockIndexs = tagBlockData->payload;
			for (int i = 0; i < size; i++) {
				int index = blockIndexs[i];
				m_block_ids[i] = block::GetBlockIdByName(blockNames[index]);
				m_block_datas[i] = block::GetBlockDataByName(blockNames[index]);
				m_block_name_indexs[i] = index;
			}
		}
		else if (tagMaterials && tagMaterials->payload == "Alpha") {
			nbt::TagByteArray* tagBlocks = root.findTag<nbt::TagByteArray>("Blocks", nbt::TAG_BYTE_ARRAY);
			nbt::TagByteArray* tagData = root.findTag<nbt::TagByteArray>("Data", nbt::TAG_BYTE_ARRAY);
			nbt::TagByteArray* tagAdd = root.findTag<nbt::TagByteArray>("Add", nbt::TAG_BYTE_ARRAY);
			nbt::TagByteArray* tagAddBlocks = root.findTag<nbt::TagByteArray>("AddBlocks", nbt::TAG_BYTE_ARRAY);

			if (tagBlocks == NULL || tagData == NULL) return false;
			tagAdd = tagAdd != NULL ? tagAdd : tagAddBlocks;
			for (int i = 0; i < size; i++) {
				int16_t add = 0;
				if (tagAdd) {
					if ((i % 2) == 0)
						add = (tagAdd->payload)[i / 2] & 0x0f;
					else
						add = ((tagAdd->payload)[i / 2] >> 4) & 0x0f;

				}
				m_block_ids[i] = (tagBlocks->payload)[i] + (add << 8);
				int8_t data = 0;
				if ((i % 2) == 0)
					data = (tagData->payload)[i / 2] & 0x0f;
				else
					data = ((tagData->payload)[i / 2] >> 4) & 0x0f;
				m_block_datas[i] = data;
			}
		}

		return true;
	}

	std::string Schematics::GetBlockName(int x, int y, int z) {
		uint32_t index = GetBlockIndex(x, y, z);
		if (index >= m_block_name_indexs.size()) return "";
		index = m_block_name_indexs[index];
		if (index >= m_block_names.size()) return "";
		return m_block_names[index];
	}

	bool Schematics::GetParaCraftBlockIdAndData(int x, int y, int z, uint16_t& block_id, uint16_t& block_data)
	{
		std::string mc_name = GetBlockName(x, y, z);
		if (mc_name.empty()) return false;
		
		block_id = 0;
		block_data = 0;
		
		if (mc_name == "minecraft:air") return true;

		block_id = block::GetParaCraftBlockIdByMineCraftName(mc_name);
		if (block_id == 0) return false;
		block_data = block::GetParaCraftBlockDataByMineCraftName(mc_name);
		return true;
	}
}


//https://minecraft.fandom.com/zh/wiki/Schematic%E6%96%87%E4%BB%B6%E6%A0%BC%E5%BC%8F
//https://www.minecraft-schematics.com/schematic/17599/
//int StartX = (a_Item.m_StartX == -1) ? 0 : std::min(Width, std::max(a_Item.m_StartX, 0));
//int StartY = (a_Item.m_StartY == -1) ? 0 : std::min(Height, std::max(a_Item.m_StartY, 0));
//int StartZ = (a_Item.m_StartZ == -1) ? 0 : std::min(Length, std::max(a_Item.m_StartZ, 0));
//int EndX = (a_Item.m_EndX == -1) ? Width - 1 : std::min(Width - 1, std::max(a_Item.m_EndX, 0));
//int EndY = (a_Item.m_EndY == -1) ? Height - 1 : std::min(Height - 1, std::max(a_Item.m_EndY, 0));
//int EndZ = (a_Item.m_EndZ == -1) ? Length - 1 : std::min(Length - 1, std::max(a_Item.m_EndZ, 0));
//for (int y = 0; y < SizeY; y++)
//{
//	for (int z = 0; z < SizeZ; z++)
//	{
//		for (int x = 0; x < SizeX; x++)
//		{
//			int idx = StartX + x + (StartZ + z) * Width + (StartY + y) * Width * Length;
//			Byte BlockType = Blocks[idx];
//			Byte BlockMeta = Metas[idx] & 0x0f;
//			Img.SetBlock(x, y, z, BlockType, BlockMeta);
//		}
//	}
//}