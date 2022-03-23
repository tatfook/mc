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

#include "mc/cache.h"
#include <math.h>
#include "MCblock.h"

namespace mapcrafter {
namespace mc {

WorldCache::WorldCache(const World& world)
		: world(world) {
	for (int i = 0; i < RSIZE; i++) {
		regioncache[i].used = false;
	}
	for (int i = 0; i < CSIZE; i++) {
		chunkcache[i].used = false;
	}
}

/**
 * Calculates the position of a region position in the cache.
 */
int WorldCache::getRegionCacheIndex(const RegionPos& pos) {
	return (((pos.x + 4096) & RMASK) * RWIDTH + (pos.z + 4096)) & RMASK;
}

/**
 * Calculates the position of a chunk position in the cache.
 */
int WorldCache::getChunkCacheIndex(const ChunkPos& pos) {
	//                4096*32
	return (((pos.x + 131072) & CMASK) * CWIDTH + (pos.z + 131072)) & CMASK;
}

RegionFile* WorldCache::getRegion(const RegionPos& pos) {
	CacheEntry<RegionPos, RegionFile>& entry = regioncache[getRegionCacheIndex(pos)];
	// check if region is already in cache
	if (entry.used && entry.key == pos) {
		//regionstats.hits++;
		return &entry.value;
	}

	// if not try to load the region

	// region does not exist, region in cache was not modified
	if (!world.getRegion(pos, entry.value))
		return NULL;

	if (!entry.value.loadAll()) {
		// the region is not valid, region in cache was probably modified
		entry.used = false;
		return NULL;
	}

	entry.used = true;
	entry.key = pos;
	//regionstats.misses++;
	return &entry.value;
}

Chunk* WorldCache::getChunk(const ChunkPos& pos) {
	CacheEntry<ChunkPos, Chunk>& entry = chunkcache[getChunkCacheIndex(pos)];
	// check if chunk is already in cache
	if (entry.used && entry.key == pos) {
		//chunkstats.hits++;
		return &entry.value;
	}

	// if not try to get the region of the chunk from the cache
	RegionFile* region = getRegion(pos.getRegion());
	if (region == NULL) {
		//chunkstats.unavailable++;
		return NULL;
	}

	// then try to load the chunk
	int status = region->loadChunk(pos, entry.value);
	// the chunk does not exist, chunk in cache was not modified
	if (status == RegionFile::CHUNK_DOES_NOT_EXIST)
		return NULL;

	if (status != RegionFile::CHUNK_OK) {
		//chunkstats.unavailable++;
		// the chunk is not valid, chunk in cache was probably modified
		entry.used = false;
		return NULL;
	}

	entry.used = true;
	entry.key = pos;
	//chunkstats.misses++;
	return &entry.value;
}

const CacheStats& WorldCache::getRegionCacheStats() const {
	return regionstats;
}

const CacheStats& WorldCache::getChunkCacheStats() const {
	return chunkstats;
}

//BlockPos LocalBlockPos::toGlobalPos(const ChunkPos& chunk) const {
//	return BlockPos(x + chunk.x * 16, z + chunk.z * 16, y);
//}

bool WorldCache::hasBlock(BlockPos blockpos, uint16_t block_id, uint8_t data, uint8_t state)
{
	ChunkPos chunkpos(blockpos);
	RegionPos &regionpos = chunkpos.getRegion();
	if (world.hasRegion(regionpos))
	{
		RegionFile* region = getRegion(regionpos);
		if (region)
		{
			if (region->hasChunk(chunkpos))
			{
				Chunk* chunk = getChunk(chunkpos);
				if (chunk)
				{
					if (chunk->hasBlock(blockpos.toLocalPos(), block_id, data, state))
						return true;
				}
			}
		}
	}
	return false;
}

bool WorldCache::GetBlockInfo(BlockPos blockpos, uint16_t &block_id, uint8_t &data, uint8_t &state)
{
	ChunkPos chunkpos(blockpos);
	RegionPos &regionpos = chunkpos.getRegion();
	if (world.hasRegion(regionpos))
	{
		RegionFile* region = getRegion(regionpos);
		if (region)
		{
			if (region->hasChunk(chunkpos))
			{
				Chunk* chunk = getChunk(chunkpos);
				if (chunk)
				{
					return chunk->GetBlockInfo(blockpos.toLocalPos(), block_id, data, state);
				}
			}
		}
	}
	return false;
}

uint16_t WorldCache::getBlockID(BlockPos& blockpos){
	ChunkPos chunkpos(blockpos);
	RegionPos &regionpos = chunkpos.getRegion();
	if (world.hasRegion(regionpos))
	{
		RegionFile* region = getRegion(regionpos);
		if (region)
		{
			if (region->hasChunk(chunkpos))
			{
				Chunk* chunk = getChunk(chunkpos);
				if (chunk)
				{
					return chunk->getBlockID(blockpos.toLocalPos());
				}
			}
		}
	}
	return 0;
}

bool WorldCache::hasSolidBlock(BlockPos& pos)
{
	uint16_t block_id = getBlockID(pos);
	if (block_id > 0)
	{
		if (MCBlock::IsSolidBlock(block_id))
			return true;
	}
	return false;
}

}
}
