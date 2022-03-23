
#include <iostream>
#include "mc/world.h"
#include "mc/cache.h"
#include "mc/schematics.h"

using namespace mc_map;
using namespace std;


int main()
{
	Schematics schematics;

	schematics.Load("D:/mcworld/gracefieldhouse.schem");
	
	return 0;
}

int test_mca()
{
	shared_ptr<World> world = make_shared<World>();
	unique_ptr<WorldCache> world_cache = make_unique<WorldCache>(*world);
	const char* world_path = "D:/mcworld/HG";
	if (!world->load(world_path))
	{
		cout << "Load World Failed" << endl;
		return 1;
	}

	unordered_set<RegionPos, hash_function> regions = world->getAvailableRegions();

	for (unordered_set<RegionPos, hash_function>::const_iterator it = regions.cbegin(); it != regions.cend(); it++)
	{
		RegionFile region_file;
		if (!world->getRegion(*it, region_file))
		{
			cout << "region not exist" << *it << endl;
		}
		else
		{
			region_file.loadAll();
			Chunk chunk;
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					ChunkPos pos(x, z);
					region_file.loadChunk(pos, chunk);
				}
			}
		}
	}
	return 0;
}