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

#include "mc/world.h"

#include "mc/chunk.h"

#include "util.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace mc_map {

	World::World()
		: rotation(0) {
		m_spawn_x = 0;
		m_spawn_y = 0;
		m_spawn_z = 0;
		m_data_version = 0;
	}

	World::~World() {
	}

	/**
	 * Loads a world from a directory.
	 */
	bool World::load(const std::string& dir, int rotation) {
		worldpath = dir;

		this->rotation = rotation;
		fs::path world_dir(dir);
		fs::path region_dir = world_dir / "region";
		if (!fs::exists(world_dir)) {
			std::cerr << "Error: World directory " << world_dir << " does not exists!" << std::endl;
			return false;
		}
		else if (!fs::exists(region_dir)) {
			std::cerr << "Error: Region directory " << region_dir << " does not exists!" << std::endl;
			return false;
		}
		else {
			if (!readRegions(region_dir.string())) return false;
		}

		return readLevel();
	}

	bool World::readLevel()
	{
		std::string level_dat_dir = worldpath + "/level.dat";
		if (!fs::exists(level_dat_dir)) {
			std::cerr << "Error: level.dat file " << level_dat_dir << " does not exists!" << std::endl;
			return false;
		}
		nbt::NBTFile nbt;
		nbt.readNBT(level_dat_dir.c_str());
		nbt::TagCompound* data = nbt.findTag<nbt::TagCompound>("Data", nbt::TAG_COMPOUND);
		if (data == NULL) {
			std::cerr << "Warning: Corrupt level.dat (No data tag)!" << std::endl;
			return false;
		}

		nbt::TagInt* xpos = data->findTag<nbt::TagInt>("SpawnX", nbt::TAG_INT);
		nbt::TagInt* ypos = data->findTag<nbt::TagInt>("SpawnY", nbt::TAG_INT);
		nbt::TagInt* zpos = data->findTag<nbt::TagInt>("SpawnZ", nbt::TAG_INT);
		if (xpos == NULL || ypos == NULL || zpos == NULL) return false;

		m_spawn_x = xpos->payload;
		m_spawn_y = ypos->payload;
		m_spawn_z = zpos->payload;

		nbt::TagInt* tagDataVersion = data->findTag<nbt::TagInt>("DataVersion", nbt::TAG_INT);
		if (tagDataVersion != NULL) m_data_version = tagDataVersion->payload;

		return true;
	}
	/**
	 * Scans the region directory for available Anvil region files.
	 */
	bool World::readRegions(const std::string& path) {
		fs::path region_dir(path);
		if (!fs::exists(region_dir))
			return false;
		std::string ending = ".mca";
		for (fs::directory_iterator it(region_dir); it != fs::directory_iterator();
			++it) {
			std::string region_file = (*it).path().string();

#ifdef OLD_BOOST
			std::string filename = (*it).path().filename();
#else
			std::string filename = (*it).path().filename().string();
#endif

			if (!std::equal(ending.rbegin(), ending.rend(), filename.rbegin()))
				continue;
			int x = 0;
			int z = 0;
			if (sscanf(filename.c_str(), "r.%d.%d.mca", &x, &z) != 2)
				continue;
			RegionPos pos(x, z);
			if (rotation)
				pos.rotate(rotation);
			available_regions.insert(pos);
			region_files[pos] = it->path().string();
		}
		return true;
	}

	int World::getRegionCount() const {
		return available_regions.size();
	}

	int World::GetOffsetY() const {
		if (m_data_version >= 2975) return -4 * 16;

		return 0;
	}

	void World::GetOriginalRegionPos(int& originalRegionX, int& originalRegionZ)
	{
		auto regionPos = available_regions.begin();
		originalRegionX = regionPos->x;
		originalRegionZ = regionPos->z;
	}

	bool World::hasRegion(const RegionPos& pos) const {
		return available_regions.count(pos) != 0;
	}

	const std::unordered_set<RegionPos, hash_function>& World::getAvailableRegions() const {
		return available_regions;
	}

	/**
	 * Returns a region file with a specific position.
	 */
	bool World::getRegion(const RegionPos& pos, RegionFile& region) const {
		std::unordered_map<RegionPos, std::string, hash_function>::const_iterator it
			= region_files.find(pos);
		if (it == region_files.end())
			return false;
		region = RegionFile(it->second, rotation);
		return true;
	}
}
