
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

		return true;
	}
}


//https://minecraft.fandom.com/zh/wiki/Schematic%E6%96%87%E4%BB%B6%E6%A0%BC%E5%BC%8F

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