#pragma once

#ifdef WIN32
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#endif


// ParaEngine includes
#include "PluginAPI.h"
#include <memory>
#include "mc/world.h"
#include "mc/cache.h"

using namespace mapcrafter;
using namespace mapcrafter::mc;

class MCImporter
{
public:
	MCImporter();
	~MCImporter();

	static MCImporter& GetSingleton();

	static MCImporter& CreateGetSingleton();

	/** load a given world folder */
	bool LoadWorld(const std::string& sFolderName);

	uint16_t getBlock(const mc::BlockPos& pos, mc::Chunk* chunk) const;

	/** return true if the block's nearby 4 blocks are all obstructed blocks. */
	bool isOccludedBlock(const mc::BlockPos& pos, mc::Chunk* chunk, uint16_t id) const;
public:
	mc::World m_world;
	std::unique_ptr<mc::WorldCache> m_world_cache;
	//mc::WorldCache* p_world_cache;
	void initOffsetRegionPos();
	void TranslateParacraftChunkPos(int &chunkX,int &chunkZ);
	void TranslateMCChunkPos(int &chunkX, int &chunkZ);
	void TranslateMCBlockPos(int &bx, int &bz);
	int offsetRegionX;
	int offsetRegionZ;
	int offsetChunkX;
	int offsetChunkZ;

	static const uint16_t min_y = 0;
	static const uint16_t max_y = 256;
};