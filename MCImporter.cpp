
#include "MCImporter.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "INPLRuntimeState.h"
#include "NPLInterface.hpp"
#include "mc/block.h"
#include "mc/cache.h"
#include "mc/schematics.h"
#include "MCblock.h"

using namespace ParaEngine;

#define TEMP_MCR_FILENAME "temp/mcimporter.mcr.tmp"

#define MCIMPORTER_DEBUGER true

#pragma region PE_DLL 

#ifdef WIN32
#define CORE_EXPORT_DECL    __declspec(dllexport)
#else
#define CORE_EXPORT_DECL
#endif

// forware declare of exported functions. 
#ifdef __cplusplus
extern "C" {
#endif
	CORE_EXPORT_DECL const char* LibDescription();
	CORE_EXPORT_DECL int LibNumberClasses();
	CORE_EXPORT_DECL unsigned long LibVersion();
	CORE_EXPORT_DECL ParaEngine::ClassDescriptor* LibClassDesc(int i);
	CORE_EXPORT_DECL void LibInit();
	CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid);
	CORE_EXPORT_DECL bool LoadMCWorld(const std::string& sFolderName);
	CORE_EXPORT_DECL bool GetRegionBlocks(int regionX, int regionZ, std::vector<int>* blocks);
	CORE_EXPORT_DECL bool GetChunkBlocks(int chunkX, int chunkZ, std::vector<int>* blocks);
	CORE_EXPORT_DECL void GetRegionOffset(int& offsetRegionX, int& offsetRegionZ);
	CORE_EXPORT_DECL bool GetSpawnPosition(int& spawnX, int& spawnY, int& spawnZ);

#ifdef __cplusplus
}   /* extern "C" */
#endif

HINSTANCE Instance = NULL;

ClassDescriptor* MCImporter_GetClassDesc();
typedef ClassDescriptor* (*GetClassDescMethod)();

GetClassDescMethod Plugins[] =
{
	MCImporter_GetClassDesc,
};

#define MCImporter_CLASS_ID Class_ID(0x2b305a29, 0x47a409ce)

class MCImporterDesc :public ClassDescriptor
{
public:
	void* Create(bool loading = FALSE)
	{
		return new MCImporter();
	}

	const char* ClassName()
	{
		return "IMCImporter";
	}

	SClass_ID SuperClassID()
	{
		return OBJECT_MODIFIER_CLASS_ID;
	}

	Class_ID ClassID()
	{
		return MCImporter_CLASS_ID;
	}

	const char* Category()
	{
		return "Model Importer";
	}

	const char* InternalName()
	{
		return "Model Importer";
	}

	HINSTANCE HInstance()
	{
		extern HINSTANCE Instance;
		return Instance;
	}
};

ClassDescriptor* MCImporter_GetClassDesc()
{
	static MCImporterDesc s_desc;
	return &s_desc;
}

CORE_EXPORT_DECL const char* LibDescription()
{
	return "ParaEngine MCImporter Ver 1.0.0";
}

CORE_EXPORT_DECL unsigned long LibVersion()
{
	return 1;
}

CORE_EXPORT_DECL int LibNumberClasses()
{
	return sizeof(Plugins) / sizeof(Plugins[0]);
}

CORE_EXPORT_DECL ClassDescriptor* LibClassDesc(int i)
{
	if (i < LibNumberClasses() && Plugins[i])
	{
		return Plugins[i]();
	}
	else
	{
		return NULL;
	}
}

CORE_EXPORT_DECL void LibInit()
{
}
#ifdef WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
#else
void __attribute__((constructor)) DllMain()
#endif
{
	// TODO: dll start up code here
#ifdef WIN32
	Instance = hinstDLL;				// Hang on to this DLL's instance handle.
	return (TRUE);
#endif
}
#pragma endregion PE_DLL 

MCImporter::MCImporter()
{
	MCBlock::StaticInit();
	block::StaticInit();
}

MCImporter::~MCImporter()
{

}

MCImporter& MCImporter::GetSingleton()
{
	static MCImporter g_singleton;
	return g_singleton;
}

bool MCImporter::LoadWorld(const std::string& sFolderName)
{
	if (m_world.load(sFolderName))
	{
		m_world_cache = std::unique_ptr<mc_map::WorldCache>(new mc_map::WorldCache(&m_world));
		return true;
	}
	return false;
}

uint16_t MCImporter::getBlock(const mc_map::BlockPos& pos, mc_map::Chunk* chunk) const
{
	if (pos.y < 0)
		return 0;
	mc_map::ChunkPos chunk_pos(pos);
	mc_map::Chunk* mychunk = chunk;
	if (chunk == NULL || chunk_pos != chunk->getPos())
		mychunk = m_world_cache->getChunk(chunk_pos);
	// chunk may be NULL
	if (mychunk == NULL) {
		return 0;
		// otherwise get id and data
	}
	else {
		mc_map::LocalBlockPos local(pos);
		uint16_t id = mychunk->getBlockID(local);
		// assume that air does not have any data
		if (id == 0)
			return 0;
		return id;
	}
}

/** TODO: this function is not correct.
block that is surrounded by non-opache blocks may be wrongly removed.
*/
bool MCImporter::isOccludedBlock(const mc_map::BlockPos& pos, mc_map::Chunk* chunk, uint16_t id) const
{
	// uint16_t north, south, east, west, top, bottom;
	if (!MCBlock::IsSolidBlock(getBlock(pos + mc_map::DIR_TOP, chunk)) ||
		!MCBlock::IsSolidBlock(getBlock(pos + mc_map::DIR_BOTTOM, chunk)) ||
		!MCBlock::IsSolidBlock(getBlock(pos + mc_map::DIR_NORTH, chunk)) ||
		!MCBlock::IsSolidBlock(getBlock(pos + mc_map::DIR_SOUTH, chunk)) ||
		!MCBlock::IsSolidBlock(getBlock(pos + mc_map::DIR_EAST, chunk)) ||
		!MCBlock::IsSolidBlock(getBlock(pos + mc_map::DIR_WEST, chunk)))
	{
		return false;
	}
	return true;
}

void MCImporter::initOffsetRegionPos()
{
	int originalParaCraftRegionX = 37;
	int originalParaCraftRegionZ = 37;
	int originalMCRegionX = 0;
	int originalMCRegionZ = 0;
	m_world.GetOriginalRegionPos(originalMCRegionX, originalMCRegionZ);
	offsetRegionX = originalMCRegionX - originalParaCraftRegionX;
	offsetRegionZ = originalMCRegionZ - originalParaCraftRegionZ;
}

void MCImporter::TranslateParacraftChunkPos(int& chunkX, int& chunkZ)
{
	int x = chunkX;
	int localX = x % 32 < 0 ? x % 32 + 32 : x % 32;
	int paracraftRegionX = x >> 5;
	int mcRegionX = paracraftRegionX + offsetRegionX;
	chunkX = mcRegionX * 32 + localX;
	int z = chunkZ;
	int localZ = z % 32 < 0 ? z % 32 + 32 : z % 32;
	int paracraftRegionZ = z >> 5;
	int mcRegionZ = paracraftRegionZ + offsetRegionZ;
	chunkZ = mcRegionZ * 32 + localZ;
}

void MCImporter::TranslateMCChunkPos(int& chunkX, int& chunkZ)
{
	int x = chunkX;
	int localX = x % 32 < 0 ? x % 32 + 32 : x % 32;
	int MCRegionX = x >> 5;
	int paracraftRegionX = MCRegionX - offsetRegionX;
	chunkX = paracraftRegionX * 32 + localX;
	int z = chunkZ;
	int localZ = z % 32 < 0 ? z % 32 + 32 : z % 32;
	int MCRegionZ = z >> 5;
	int paracraftRegionZ = MCRegionZ - offsetRegionZ;
	chunkZ = paracraftRegionZ * 32 + localZ;
}

void MCImporter::TranslateMCBlockPos(int& bx, int& bz)
{
	int local_x = bx % 16;
	local_x = local_x < 0 ? local_x + 16 : local_x;
	int local_z = bz % 16;
	local_z = local_z < 0 ? local_z + 16 : local_z;

	/*int x = bx;
	int z = bz;*/
	int ChunkX, ChunkZ;
	if (bx < 0)
		ChunkX = (bx - 15) / 16;
	else
		ChunkX = bx >> 4;
	if (bz < 0)
		ChunkZ = (bz - 15) / 16;
	else
		ChunkZ = bz >> 4;
	TranslateMCChunkPos(ChunkX, ChunkZ);

	bx = ChunkX * 16 + local_x;
	bz = ChunkZ * 16 + local_z;
}

bool GetSpawnPosition(int& spawnX, int& spawnY, int& spawnZ)
{
	MCImporter& mc_importer = MCImporter::GetSingleton();
	spawnX = mc_importer.m_world.GetSpawnX();
	spawnY = mc_importer.m_world.GetSpawnY();
	spawnZ = mc_importer.m_world.GetSpawnZ();
	mc_importer.TranslateMCBlockPos(spawnX, spawnZ);
	spawnY += mc_importer.m_world.GetOffsetY();
	return true;
}

/*
  ** face value and the corresponding coordinate direction and the corresponding block data:
		0(x-)(data:0,4),1(x+)(data:1,5),2(z-)(data:2,6),3(z+)(data:3,7)
  ** half value:
		upward(0),downward(1)
*/
template<typename pObjectType, typename PosType>
bool HasStairsBlock(pObjectType& pobj, PosType pos, uint8_t face, uint8_t half)
{
	uint8_t data;
	if (half == 0)
	{
		if (face == 0)
		{
			data = 0;
		}
		else if (face == 1)
		{
			data = 1;
		}
		else if (face == 2)
		{
			data = 2;
		}
		else if (face == 3)
		{
			data = 3;
		}
	}
	else if (half == 1)
	{
		if (face == 0)
		{
			data = 4;
		}
		else if (face == 1)
		{
			data = 5;
		}
		else if (face == 2)
		{
			data = 6;
		}
		else if (face == 3)
		{
			data = 7;
		}
	}
	uint16_t real_block_id = 0;
	uint8_t real_data = 0;
	uint8_t real_state = 0;
	pobj->GetBlockInfo(pos, real_block_id, real_data, real_state);
	if (MCBlock::IsStairBlock(real_block_id))
	{
		if (data == real_data)
			return true;
	}
	return false;
}

/*
53(oak_stairs),67(stone_stairs),108(brick_stairs),109(stone_brick_stairs),114(nether_brick_stairs),128(sandstone_stairs),134(spruce_stairs),135(birch_stairs),
136(jungle_stairs),156(quartz_stairs),163(acacia_stairs),164(dark_oak_stairs),180(red_sandstone_stairs)
*/
template<typename pObjectType, typename PosType>
uint8_t getStairsBlockState(pObjectType& pobj, PosType pos, uint16_t block_id, uint16_t data)
{
	uint8_t half = data < 4 ? 0 : 1;

	uint8_t state = 0;
	// facing "x-"
	if (data == 0 || data == 4)
	{
		if (HasStairsBlock(pobj, PosType(pos.x + 1, pos.z, pos.y), 2, half))
		{
			state = 0;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x + 1, pos.z, pos.y), 3, half))   // state:outter 
		{
			state = 1;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x - 1, pos.z, pos.y), 2, half))   // state:inner 
		{
			state = 2;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x - 1, pos.z, pos.y), 3, half))   // state:inner
		{
			state = 3;
		}
		else
			state = 7;

	}

	// facing "x+"
	if (data == 1 || data == 5)
	{
		if (HasStairsBlock(pobj, PosType(pos.x - 1, pos.z, pos.y), 2, half))   // state:outter 
		{
			state = 0;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x - 1, pos.z, pos.y), 3, half))   // state:outter
		{
			state = 1;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x + 1, pos.z, pos.y), 2, half))        // state:inner 
		{
			state = 2;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x + 1, pos.z, pos.y), 3, half))   // state:inner 
		{
			state = 3;
		}
		else
			state = 7;
	}

	// facing "z-"
	if (data == 2 || data == 6)
	{
		if (HasStairsBlock(pobj, PosType(pos.x, pos.z + 1, pos.y), 0, half))        // state:outter 
		{
			state = 0;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x, pos.z + 1, pos.y), 1, half))   // state:outter 
		{
			state = 1;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x, pos.z - 1, pos.y), 0, half))   // state:inner 
		{
			state = 2;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x, pos.z - 1, pos.y), 1, half))   // state:inner
		{
			state = 3;
		}
		else
			state = 7;

	}

	// facing "z+"
	if (data == 3 || data == 7)
	{
		if (HasStairsBlock(pobj, PosType(pos.x, pos.z - 1, pos.y), 0, half))   // state:outter 
		{
			state = 0;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x, pos.z - 1, pos.y), 1, half))   // state:outter
		{
			state = 1;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x, pos.z + 1, pos.y), 0, half))        // state:inner 
		{
			state = 2;
		}
		else if (HasStairsBlock(pobj, PosType(pos.x, pos.z + 1, pos.y), 1, half))   // state:inner 
		{
			state = 3;
		}
		else
			state = 7;
	}
	return state;
}

// fence
template<typename pObjectType, typename PosType>
uint8_t getFenceBlockState(pObjectType& pobj, PosType pos, uint16_t block_id, uint16_t data)
{
	uint8_t state = 0;
	//uint8_t data = getBlockData(pos);
	// facing "x-"
	bool hasSolidBlockInPositiveX = pobj->hasBlock(PosType(pos.x + 1, pos.z, pos.y), block_id) || pobj->hasSolidBlock(PosType(pos.x + 1, pos.z, pos.y));
	bool hasSolidBlockInNegativeX = pobj->hasBlock(PosType(pos.x - 1, pos.z, pos.y), block_id) || pobj->hasSolidBlock(PosType(pos.x - 1, pos.z, pos.y));
	bool hasSolidBlockInPositiveZ = pobj->hasBlock(PosType(pos.x, pos.z + 1, pos.y), block_id) || pobj->hasSolidBlock(PosType(pos.x, pos.z + 1, pos.y));
	bool hasSolidBlockInNegativeZ = pobj->hasBlock(PosType(pos.x, pos.z - 1, pos.y), block_id) || pobj->hasSolidBlock(PosType(pos.x, pos.z - 1, pos.y));

	if ((!hasSolidBlockInPositiveX) && (!hasSolidBlockInNegativeX) && (!hasSolidBlockInPositiveZ) && (!hasSolidBlockInNegativeZ))
	{
		state = 0;
	}
	else if (hasSolidBlockInPositiveX && (!hasSolidBlockInNegativeX) && (!hasSolidBlockInPositiveZ) && (!hasSolidBlockInNegativeZ))
	{
		state = 1;
	}
	else if ((!hasSolidBlockInPositiveX) && hasSolidBlockInNegativeX && (!hasSolidBlockInPositiveZ) && (!hasSolidBlockInNegativeZ))
	{
		state = 2;
	}
	else if ((!hasSolidBlockInPositiveX) && (!hasSolidBlockInNegativeX) && hasSolidBlockInPositiveZ && (!hasSolidBlockInNegativeZ))
	{
		state = 3;
	}
	else if ((!hasSolidBlockInPositiveX) && (!hasSolidBlockInNegativeX) && (!hasSolidBlockInPositiveZ) && hasSolidBlockInNegativeZ)
	{
		state = 4;
	}
	else if (hasSolidBlockInPositiveX && hasSolidBlockInNegativeX && (!hasSolidBlockInPositiveZ) && (!hasSolidBlockInNegativeZ))
	{
		state = 5;
	}
	else if (hasSolidBlockInPositiveX && (!hasSolidBlockInNegativeX) && hasSolidBlockInPositiveZ && (!hasSolidBlockInNegativeZ))
	{
		state = 6;
	}
	else if (hasSolidBlockInPositiveX && (!hasSolidBlockInNegativeX) && (!hasSolidBlockInPositiveZ) && hasSolidBlockInNegativeZ)
	{
		state = 7;
	}
	else if ((!hasSolidBlockInPositiveX) && hasSolidBlockInNegativeX && hasSolidBlockInPositiveZ && (!hasSolidBlockInNegativeZ))
	{
		state = 8;
	}
	else if ((!hasSolidBlockInPositiveX) && hasSolidBlockInNegativeX && (!hasSolidBlockInPositiveZ) && hasSolidBlockInNegativeZ)
	{
		state = 9;
	}
	else if ((!hasSolidBlockInPositiveX) && (!hasSolidBlockInNegativeX) && hasSolidBlockInPositiveZ && hasSolidBlockInNegativeZ)
	{
		state = 10;
	}
	else if ((!hasSolidBlockInPositiveX) && hasSolidBlockInNegativeX && hasSolidBlockInPositiveZ && hasSolidBlockInNegativeZ)
	{
		state = 11;
	}
	else if (hasSolidBlockInPositiveX && (!hasSolidBlockInNegativeX) && hasSolidBlockInPositiveZ && hasSolidBlockInNegativeZ)
	{
		state = 12;
	}
	else if (hasSolidBlockInPositiveX && hasSolidBlockInNegativeX && (!hasSolidBlockInPositiveZ) && hasSolidBlockInNegativeZ)
	{
		state = 13;
	}
	else if (hasSolidBlockInPositiveX && hasSolidBlockInNegativeX && hasSolidBlockInPositiveZ && (!hasSolidBlockInNegativeZ))
	{
		state = 14;
	}
	else
	{
		state = 15;
	}

	return state;
}

template<typename pObjectType, typename PosType>
uint8_t getBlockState(pObjectType& pobj, PosType pos, uint16_t block_id, uint16_t data)
{
	uint8_t state = 0;
	//uint16_t block_id = getBlockID(pos);
	if (MCBlock::IsStairBlock(block_id))
	{
		state = getStairsBlockState(pobj, pos, block_id, data);
	}
	else if (block_id == 85 || block_id == 113 || block_id == 188 || block_id == 189 || block_id == 190 || block_id == 191 || block_id == 192)
	{
		state = getFenceBlockState(pobj, pos, block_id, data);
	}
	else if (block_id == 101 || block_id == 102 || block_id == 139)
	{
		// we deal with the ��iron_bars�� and ��glass_pane�� ��cobblestone_wall�� block including "16 states";
		state = getFenceBlockState(pobj, pos, block_id, data);
	}
	return state;
}

CORE_EXPORT_DECL void GetRegionOffset(int& offsetRegionX, int& offsetRegionZ)
{
	MCImporter& mc_importer = MCImporter::GetSingleton();
	offsetRegionX = mc_importer.offsetRegionX;
	offsetRegionZ = mc_importer.offsetRegionZ;
}

bool LoadMCWorld(const std::string& sFolderName)
{
	MCImporter& mc_importer = MCImporter::GetSingleton();
	if (!sFolderName.empty())
	{
		if (mc_importer.m_world.load(sFolderName))
		{
			mc_importer.m_world_cache = std::unique_ptr<mc_map::WorldCache>(new mc_map::WorldCache(&(mc_importer.m_world)));
			mc_importer.initOffsetRegionPos();
			return true;
		}
		return false;
	}
	return false;
}

bool GetRegionBlocks(int regionX, int regionZ, std::vector<int>* blocks)
{
	//LoadMCWorld("F:/game/Minecraft1.8.8/.minecraft/saves/test");
	MCImporter& mc_importer = MCImporter::GetSingleton();

	int mcRegionX = regionX + mc_importer.offsetRegionX;
	int mcRegionZ = regionZ + mc_importer.offsetRegionZ;
	mc_map::RegionFile* region = mc_importer.m_world_cache->getRegion(mc_map::RegionPos(mcRegionX, mcRegionZ));
	if (!region) {
		return false;
	}

	auto region_chunks = region->getContainingChunks();

	uint16_t min_y = MCImporter::min_y + mc_importer.m_world.GetOffsetY();
	uint16_t max_y = MCImporter::max_y + mc_importer.m_world.GetOffsetY();

	// go through all chunks in the region
	for (auto chunk_it = region_chunks.begin(); chunk_it != region_chunks.end(); ++chunk_it)
	{
		const mc_map::ChunkPos& chuck_pos = *chunk_it;
		mc_map::Chunk* mychunk = mc_importer.m_world_cache->getChunk(chuck_pos);
		if (mychunk)
		{
			for (int x = 0; x < 16; ++x)
			{
				for (int z = 0; z < 16; ++z)
				{
					for (int y = min_y; y < max_y; ++y)
					{
						mc_map::LocalBlockPos pos(x, z, y);
						uint16_t block_id = mychunk->getBlockID(pos);
						if (block_id != 0)
							//if (block_id == 53)
						{
							uint16_t block_data = mychunk->getBlockData(pos);
							BlockPos gpos = pos.toGlobalPos(chuck_pos);
							uint16_t block_state = 0;
							if (pos.beBorder())
								block_state = getBlockState(mc_importer.m_world_cache, gpos, block_id, block_data);
							else
								block_state = getBlockState(mychunk, pos, block_id, block_data);
							/*blocks->push_back(gpos.x);
							blocks->push_back(gpos.y);
							blocks->push_back(gpos.z);*/
							blocks->push_back(x);
							blocks->push_back(y);
							blocks->push_back(z);
							uint16_t block_side = 0;
							if (!MCBlock::TranslateMCBlock(block_id, block_data, block_state, block_side))
							{
								char sMsg[130];
								snprintf(sMsg, 100, "mc region x=%d,z=%d,block_x=%d,block_y=%d,block_z=%d,block_id=%d,block_data=%d,block_state=%d translate failed!;", x, z, gpos.x, gpos.y, gpos.z, block_id, block_data, block_state);
							}
							blocks->push_back((int)block_id);
							blocks->push_back((int)block_data);
							blocks->push_back((int)block_side);
						}
					}
				}
			}
		}
	}
	return true;
}

bool GetChunkBlocks(int chunkX, int chunkZ, std::vector<int>* blocks)
{
	//LoadMCWorld("F:/game/Minecraft1.8.8/.minecraft/saves/test");
	MCImporter& mc_importer = MCImporter::GetSingleton();

	uint16_t min_y = MCImporter::min_y + mc_importer.m_world.GetOffsetY();
	uint16_t max_y = MCImporter::max_y + mc_importer.m_world.GetOffsetY();

	int mcChunkX = chunkX;
	int mcChunkZ = chunkZ;
	mc_importer.TranslateParacraftChunkPos(mcChunkX, mcChunkZ);

	const mc_map::ChunkPos chuck_pos(mcChunkX, mcChunkZ);
	mc_map::Chunk* mychunk = mc_importer.m_world_cache->getChunk(chuck_pos);
	if (mychunk)
	{
		for (int x = 0; x < 16; ++x)
		{
			for (int z = 0; z < 16; ++z)
			{
				for (int nSection = min_y / 16; nSection < (max_y / 16); ++nSection)
				{
					Section* section = mychunk->getSection(nSection);
					if (!section) continue;
					for (int dy = 0; dy < 16; ++dy)
					{
						int y = nSection * 16 + dy;
						uint16_t block_id = 0, block_data = 0, block_side = 0;
						if (section->GetParaCraftBlockIdAndData(x, dy, z, block_id, block_data)) {
							blocks->push_back(x);
							blocks->push_back(y);
							blocks->push_back(z);
							blocks->push_back((int)block_id);
							blocks->push_back((int)block_data);
							blocks->push_back((int)block_side);
							continue;
						}
						mc_map::LocalBlockPos pos(x, z, y);
						block_id = mychunk->getBlockID(pos);
						if (block_id != 0)
						{
							uint16_t block_data = mychunk->getBlockData(pos);
							BlockPos gpos = pos.toGlobalPos(chuck_pos);
							uint16_t block_state = 0;
							if (pos.beBorder())
								block_state = getBlockState(mc_importer.m_world_cache, gpos, block_id, block_data);
							else
								block_state = getBlockState(mychunk, pos, block_id, block_data);
							blocks->push_back(x);
							blocks->push_back(y);
							blocks->push_back(z);
							uint16_t block_side = 0;
							if (!MCBlock::TranslateMCBlock(block_id, block_data, block_state, block_side))
							{
								char sMsg[130];
								snprintf(sMsg, 100, "mc region x=%d,z=%d,block_x=%d,block_y=%d,block_z=%d,block_id=%d,block_data=%d,block_state=%d translate failed!;", x, z, gpos.x, gpos.y, gpos.z, block_id, block_data, block_state);
							}
							blocks->push_back((int)block_id);
							blocks->push_back((int)block_data);
							blocks->push_back((int)block_side);
						}
					}
				}
			}
		}
	}
	else
		return false;
	return true;
}

bool GetSchematicsBlocks(const std::string& filepath, std::vector<uint16_t>* blocks, int& width, int& height, int& length)
{
	Schematics schem;
	if (!schem.Load(filepath)) return false;
	width = schem.GetWidth();
	height = schem.GetHeight();
	length = schem.GetLength();
	int size = width * height * length * 3;
	int count = 0;
	for (int y = 0; y < height; y++) {
		for (int z = 0; z < length; z++) {
			for (int x = 0; x < width; x++) {
				count++;
				uint16_t id = 0; 
				uint16_t data = 0;  
				uint16_t state = 0;
				uint16_t side = 0;
				if (!schem.GetParaCraftBlockIdAndData(x, y, z, id, data)) {
					id = schem.GetBlockId(x, y, z);
					data = schem.GetBlockData(x, y, z);
					MCBlock::TranslateMCBlock(id, data, state, side);
				}
				blocks->push_back(id);
				blocks->push_back(data);
				blocks->push_back(side);
			}
		}
	}
	return true;
}

CORE_EXPORT_DECL void LibActivate(int nType, void* pVoid)
{
	if (nType == ParaEngine::PluginActType_STATE)
	{
		NPL::INPLRuntimeState* pState = (NPL::INPLRuntimeState*)pVoid;
		const char* sMsg = pState->GetCurrentMsg();
		int nMsgLength = pState->GetCurrentMsgLength();

		NPLInterface::NPLObjectProxy tabMsg = NPLInterface::NPLHelper::MsgStringToNPLTable(sMsg);
		const std::string& sCmd = tabMsg["cmd"];

		MCImporter& mc_importer = MCImporter::GetSingleton();

		if (sCmd == "load")
		{
			const std::string& sFolder = tabMsg["folder"];
			uint16_t min_y = MCImporter::min_y;
			uint16_t max_y = MCImporter::max_y;
			if (tabMsg["min_y"].GetType() == NPLInterface::NPLObjectBase::NPLObjectType_Number)
			{
				min_y = (uint16_t)((double)(tabMsg["min_y"]));
			}
			if (tabMsg["max_y"].GetType() == NPLInterface::NPLObjectBase::NPLObjectType_Number)
			{
				max_y = (uint16_t)((double)(tabMsg["max_y"]));
			}
			bool bExportOpaque = true;
			if (tabMsg["bExportOpaque"].GetType() == NPLInterface::NPLObjectBase::NPLObjectType_Bool)
			{
				bExportOpaque = (bool)tabMsg["bExportOpaque"];
			}

			if (!sFolder.empty())
			{
				if (mc_importer.LoadWorld(sFolder))
				{
					NPLInterface::NPLObjectProxy msg;
					msg["succeed"] = "true";
					msg["region_count"] = (double)(mc_importer.m_world.getRegionCount());

					NPLInterface::NPLObjectProxy regions_table;

					std::ofstream file(TEMP_MCR_FILENAME, std::ios_base::binary);

					if (!file.is_open())
					{
						return;
					}
					// go through all regions in the world
					auto regions = mc_importer.m_world.getAvailableRegions();
					int count = 0;
					int i = 0;
					for (auto itCur = regions.cbegin(); itCur != regions.cend(); itCur++)
					{
						if (MCIMPORTER_DEBUGER)
						{
							NPLInterface::NPLObjectProxy region_table;
							region_table["x"] = (double)(itCur->x);
							region_table["z"] = (double)(itCur->z);
							std::stringstream sIndexStream;
							std::string sIndex;
							sIndexStream << i;
							sIndexStream >> sIndex;
							regions_table[sIndex] = region_table;

							{
								file << "region";
								file << ",";
								file << itCur->x;
								file << ",";
								file << itCur->z;
								file << ",";
								file << 0;
								file << ",";
								file << 0;
								file << ",";
								file << 0;
								file << "\n";
							}


							mc_map::RegionFile* region = mc_importer.m_world_cache->getRegion(*itCur);
							if (!region) {
								continue;
							}
							auto region_chunks = region->getContainingChunks();

							// go through all chunks in the region
							for (auto chunk_it = region_chunks.begin(); chunk_it != region_chunks.end(); ++chunk_it)
							{
								const mc_map::ChunkPos& chuck_pos = *chunk_it;
								mc_map::Chunk* mychunk = mc_importer.m_world_cache->getChunk(chuck_pos);
								if (mychunk)
								{
									for (int x = 0; x < 16; ++x)
									{
										for (int z = 0; z < 16; ++z)
										{
											for (int y = min_y; y < max_y; ++y)
											{
												mc_map::LocalBlockPos pos(x, z, y);
												uint16_t block_id = mychunk->getBlockID(pos);
												if (block_id != 0)
												{
													uint16_t block_data = mychunk->getBlockData(pos);
													BlockPos gpos = pos.toGlobalPos(chuck_pos);
													uint16_t block_state = 0;
													if (pos.beBorder())
														block_state = getBlockState(mc_importer.m_world_cache, gpos, block_id, block_data);
													else
														block_state = getBlockState(mychunk, pos, block_id, block_data);
													uint16_t block_side = 0;
													if (!MCBlock::TranslateMCBlock(block_id, block_data, block_state, block_side))
													{
														char sMsg[130];
														snprintf(sMsg, 100, "mc region x=%d,z=%d,block_x=%d,block_y=%d,block_z=%d,block_id=%d,block_data=%d,block_state=%d translate failed!;", x, z, gpos.x, gpos.y, gpos.z, block_id, block_data, block_state);
													}
													else
													{
														file << pos.x;
														file << ",";
														file << pos.y;
														file << ",";
														file << pos.z;
														file << ",";
														file << block_id;
														file << ",";
														file << block_data;
														file << ",";
														file << block_side;
														file << "\n";
														count++;
													}
												}
											}
										}
									}
									file.flush();
								}
							}
						}
						else
						{
							NPLInterface::NPLObjectProxy region_table;
							region_table["x"] = (double)(itCur->x);
							region_table["z"] = (double)(itCur->z);
							std::stringstream sIndexStream;
							std::string sIndex;
							sIndexStream << i;
							sIndexStream >> sIndex;
							regions_table[sIndex] = region_table;

							{
								file << "region";
								file << ",";
								file << itCur->x;
								file << ",";
								file << itCur->z;
								file << ",";
								file << 0;
								file << "\n";
							}


							mc_map::RegionFile* region = mc_importer.m_world_cache->getRegion(*itCur);
							if (!region) {
								continue;
							}
							auto region_chunks = region->getContainingChunks();

							// go through all chunks in the region
							for (auto chunk_it = region_chunks.begin(); chunk_it != region_chunks.end(); ++chunk_it)
							{
								const mc_map::ChunkPos& chuck_pos = *chunk_it;
								mc_map::Chunk* mychunk = mc_importer.m_world_cache->getChunk(chuck_pos);
								if (mychunk)
								{
									for (int x = 0; x < 16; ++x)
									{
										for (int z = 0; z < 16; ++z)
										{
											for (int y = min_y; y < max_y; ++y)
											{
												mc_map::LocalBlockPos pos(x, z, y);
												uint16_t block_id = mychunk->getBlockID(pos);
												uint16_t block_data = mychunk->getBlockData(pos);
												if (block_data > 0)
													block_id = block_id * 100 + block_data;

												if (block_id != 0)
												{
													BlockPos gpos = pos.toGlobalPos(chuck_pos);
													if (bExportOpaque || !mc_importer.isOccludedBlock(gpos, mychunk, block_id))
													{
														file << gpos.x;
														file << ",";
														file << gpos.y;
														file << ",";
														file << gpos.z;
														file << ",";
														file << block_id;
														file << "\n";
														count++;
													}
												}
											}
										}
									}
									file.flush();
								}
							}
						}

					}
					msg["regions"] = regions_table;
					msg["filename"] = TEMP_MCR_FILENAME;
					msg["count"] = (double)count;
					file.close();
					std::string output;
					NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
					pState->activate("script/apps/Aries/Creator/Game/Tasks/MCImporterTask.lua", output.c_str(), output.size());
				}
			}
		}
		else if (sCmd == "getregion")
		{
			int x = tabMsg["x"];
			int y = tabMsg["y"];
			RegionPos region_pos(x, y);
			if (mc_importer.m_world.hasRegion(region_pos))
			{
				NPLInterface::NPLObjectProxy msg;
				msg["succeed"] = "true";

				std::string output;
				NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
				pState->activate("script/apps/Aries/Creator/Game/Tasks/MCImporterTask.lua", output.c_str(), output.size());
			}
		}
		else if (sCmd == "GetSpawnPosition")
		{
			int nSpawnPosX = 0, nSpawnPosY = 0, nSpawnPosZ = 0;
			const std::string& sCallback = tabMsg["callback"];
			NPLInterface::NPLObjectProxy msg;
			msg["cmd"] = sCmd;
			if (GetSpawnPosition(nSpawnPosX, nSpawnPosY, nSpawnPosZ))
			{
				msg["x"] = (double)nSpawnPosX;
				msg["y"] = (double)nSpawnPosY;
				msg["z"] = (double)nSpawnPosZ;
			}
			else
			{
				msg["success"] = false;
			}
			std::string output;
			NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
			pState->call(sCallback.c_str(), output.c_str(), (int)output.size());
		}
		else if (sCmd == "LoadMCWorld")
		{
			const std::string& sPath = tabMsg["path"];
			const std::string& sCallback = tabMsg["callback"];
			NPLInterface::NPLObjectProxy msg;
			msg["cmd"] = sCmd;
			msg["succeed"] = LoadMCWorld(sPath);
			int nSpawnPosX = 0, nSpawnPosY = 0, nSpawnPosZ = 0;
			if (GetSpawnPosition(nSpawnPosX, nSpawnPosY, nSpawnPosZ)) {
				msg["spawn_x"] = (double)nSpawnPosX;
				msg["spawn_y"] = (double)nSpawnPosY;
				msg["spawn_z"] = (double)nSpawnPosZ;
			}
			std::string output;
			NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
			pState->call(sCallback.c_str(), output.c_str(), (int)output.size());
		}
		else if (sCmd == "GetChunkBlocks")
		{
			std::vector<int> blocks;

			int chunkX = (int)((double)tabMsg["x"]);
			int chunkZ = (int)((double)tabMsg["z"]);
			const std::string& sCallback = tabMsg["callback"];
			NPLInterface::NPLObjectProxy msg;
			msg["chunk_x"] = (double)chunkX;
			msg["chunk_z"] = (double)chunkZ;
			msg["cmd"] = sCmd;
			if (GetChunkBlocks(chunkX, chunkZ, &blocks))
			{
				int32_t blockCount = 0;
				NPLInterface::NPLObjectProxy xTable, yTable, zTable, templateTable, dataTable;
				for (vector<int>::iterator iter = blocks.begin(); iter != blocks.end(); iter++)
				{
					blockCount++;
					xTable[blockCount] = (double)(*iter++);
					yTable[blockCount] = (double)(*iter++);
					zTable[blockCount] = (double)(*iter++);
					templateTable[blockCount] = (double)(*iter++);
					dataTable[blockCount] = (double)(*iter++);
				}
				msg["x"] = xTable;
				msg["y"] = yTable;
				msg["z"] = zTable;
				msg["id"] = templateTable;
				msg["data"] = dataTable;
				msg["count"] = (double)blockCount;
			}
			std::string output;
			NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
			pState->call(sCallback.c_str(), output.c_str(), (int)output.size());
		}
		else if (sCmd == "LoadSchematics")
		{
			const std::string& filepath = tabMsg["filepath"];
			const std::string& sCallback = tabMsg["callback"];
			std::vector<uint16_t> blocks;
			int width = 0, height = 0, length = 0, blockCount = 0;
			NPLInterface::NPLObjectProxy msg;
			msg["cmd"] = sCmd;

			if (GetSchematicsBlocks(filepath, &blocks, width, height, length)) {
				msg["succeed"] = true;
				NPLInterface::NPLObjectProxy idTable, dataTable;
				for (vector<uint16_t>::iterator iter = blocks.begin(); iter != blocks.end(); iter++) {
					blockCount++;
					idTable[blockCount] = (double)(*iter++);
					dataTable[blockCount] = (double)(*iter++);
				}
				msg["id"] = idTable;
				msg["data"] = dataTable;
				msg["count"] = (double)blockCount;
			}
			else {
				msg["succeed"] = false;
			}

			msg["width"] = (double)width;
			msg["height"] = (double)height;
			msg["length"] = (double)length;

			std::string output;
			NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
			pState->call(sCallback.c_str(), output.c_str(), (int)output.size());
		}
		else if (sCmd == "AddTranslateRule") {
			std::string mc_name = tabMsg["mc_name"];
		
			int mc_id = (int)((double)tabMsg["mc_id"]);
			int mc_data = (int)((double)tabMsg["mc_data"]);
			int mc_state = (int)((double)tabMsg["mc_state"]);
			int pc_id = (int)((double)tabMsg["pc_id"]);
			int pc_data = (int)((double)tabMsg["pc_data"]);
			int pc_side = (int)((double)tabMsg["pc_side"]);
			block::AddMineCraftBlock(mc_id, mc_data, mc_name);
			block::AddParaCraftBlock(pc_id, pc_data, mc_name);
			MCBlock::AddBlockInfoToMap(mc_id, mc_data, pc_id, pc_data, mc_state, pc_side);
		}
		else if (sCmd == "Debug") {
			const std::string& sCallback = tabMsg["callback"];
			NPLInterface::NPLObjectProxy msg, names, ids;
			
			int index = 0;
			std::set<std::string>& name_set = block::GetNotExistBlockNameSet();
			for (std::set<std::string>::iterator it = name_set.begin(); it != name_set.end(); it++) names[++index] = *it;
			
			index = 0;
			std::set<uint32_t>& id_set = MCBlock::GetNotExistBlockIdSet();
			for (std::set<std::uint32_t>::iterator it = id_set.begin(); it != id_set.end(); it++) ids[++index] = (double)*it;

			msg["not_exist_blocknames"] = names;
			msg["not_exist_blockids"] = ids;
			msg["cmd"] = sCmd;
			std::string output;
			NPLInterface::NPLHelper::NPLTableToString("msg", msg, output);
			pState->call(sCallback.c_str(), output.c_str(), (int)output.size());
		}
	}
}


