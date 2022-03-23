#include "PluginAPI.h"
#include "MCBlock.h"


struct MCBlockInfo
{
	uint16_t block_id;
	uint16_t data;
	uint16_t state;
	uint16_t pe_block_id;
	uint16_t pe_data;
	uint16_t pe_side;
};

std::map<uint32_t, MCBlockInfo*> MCBlockMaps;

void MCBlock::AddBlockInfoToMap(uint16_t block_id, uint16_t data, uint16_t pe_block_id, uint16_t pe_data, uint16_t state, uint16_t pe_side)
{
	MCBlockInfo* info = new MCBlockInfo;
	info->block_id = block_id;
	info->data = data;
	info->state = state;
	info->pe_block_id = pe_block_id;
	info->pe_data = pe_data;
	info->pe_side = pe_side;
	//MCBlockMaps.insert()
	/*char s[16];
	sprintf(s,"%d,%d,%d", block_id, data, sec_block_id);*/
	//std::string block_tag(s);
	uint32_t id_32 = block_id;
	id_32 = id_32 << 16;
	uint32_t data_32 = data;
	data_32 = data_32 << 8;
	uint32_t state_32 = state;
	uint32_t block_tag = id_32 + data_32 + state_32;
	//uint32_t block_tag = (uint32_t)block_id << 6 + data << 3 + state;
	MCBlockMaps[block_tag] = info;
}

void MCBlock::AddStairBlockInfoToMap(uint16_t block_id, uint16_t pe_block_id)
{
	// stair block
	//half:top
	// face x-
	AddBlockInfoToMap(block_id, 0, pe_block_id, 5, 0); // state:outter,corner face:z- 
	AddBlockInfoToMap(block_id, 0, pe_block_id, 8, 1); // state:outter,corner face:z+ 
	AddBlockInfoToMap(block_id, 0, pe_block_id, 18, 2);// state:inner	,corner face:z-
	AddBlockInfoToMap(block_id, 0, pe_block_id, 21, 3);// state:inner	,corner face:z+
	AddBlockInfoToMap(block_id, 0, pe_block_id, 1, 7); // state:normal
	// face x+		 
	AddBlockInfoToMap(block_id, 1, pe_block_id, 6, 0); // state:outter,corner face:z-
	AddBlockInfoToMap(block_id, 1, pe_block_id, 7, 1); // state:outter,corner face:z+
	AddBlockInfoToMap(block_id, 1, pe_block_id, 19, 2);// state:inner	,corner face:z-
	AddBlockInfoToMap(block_id, 1, pe_block_id, 20, 3);// state:inner	,corner face:z+
	AddBlockInfoToMap(block_id, 1, pe_block_id, 2, 7); // state:normal
	// face z-		  
	AddBlockInfoToMap(block_id, 2, pe_block_id, 5, 0); // state:outter,corner face:x-
	AddBlockInfoToMap(block_id, 2, pe_block_id, 6, 1); // state:outter,corner face:x+
	AddBlockInfoToMap(block_id, 2, pe_block_id, 18, 2);// state:inner	,corner face:x-
	AddBlockInfoToMap(block_id, 2, pe_block_id, 19, 3);// state:inner	,corner face:x+
	AddBlockInfoToMap(block_id, 2, pe_block_id, 3, 7); // state:normal
	// face z+		
	AddBlockInfoToMap(block_id, 3, pe_block_id, 6, 0); // state:outter,corner face:x-
	AddBlockInfoToMap(block_id, 3, pe_block_id, 7, 1); // state:outter,corner face:x+
	AddBlockInfoToMap(block_id, 3, pe_block_id, 19, 2);// state:inner	,corner face:x-
	AddBlockInfoToMap(block_id, 3, pe_block_id, 20, 3);// state:inner	,corner face:x+
	AddBlockInfoToMap(block_id, 3, pe_block_id, 4, 7); // state:normal
	//half:bottom	  
	// face x-		  
	AddBlockInfoToMap(block_id, 4, pe_block_id, 14, 0); // state:outter,corner face:z- 
	AddBlockInfoToMap(block_id, 4, pe_block_id, 17, 1); // state:outter,corner face:z+
	AddBlockInfoToMap(block_id, 4, pe_block_id, 22, 2); // state:inner ,corner face:z-
	AddBlockInfoToMap(block_id, 4, pe_block_id, 25, 3); // state:inner ,corner face:z+
	AddBlockInfoToMap(block_id, 4, pe_block_id, 12, 7); // state:normal
	// face x+		
	AddBlockInfoToMap(block_id, 5, pe_block_id, 15, 0); // state:outter,corner face:z-
	AddBlockInfoToMap(block_id, 5, pe_block_id, 16, 1); // state:outter,corner face:z+
	AddBlockInfoToMap(block_id, 5, pe_block_id, 23, 2); // state:inner ,corner face:z-
	AddBlockInfoToMap(block_id, 5, pe_block_id, 24, 3); // state:inner ,corner face:z+
	AddBlockInfoToMap(block_id, 5, pe_block_id, 10, 7); // state:normal
	// face z-		 
	AddBlockInfoToMap(block_id, 6, pe_block_id, 14, 0);  // state:outter,corner face:x-
	AddBlockInfoToMap(block_id, 6, pe_block_id, 15, 1);  // state:outter,corner face:x+
	AddBlockInfoToMap(block_id, 6, pe_block_id, 22, 2);  // state:inner ,corner face:x-
	AddBlockInfoToMap(block_id, 6, pe_block_id, 23, 3);  // state:inner ,corner face:x+
	AddBlockInfoToMap(block_id, 6, pe_block_id, 11, 7);  // state:normal
	// face z+		
	AddBlockInfoToMap(block_id, 7, pe_block_id, 17, 0);  // state:outter,corner face:x-
	AddBlockInfoToMap(block_id, 7, pe_block_id, 16, 1);  // state:outter,corner face:x+
	AddBlockInfoToMap(block_id, 7, pe_block_id, 25, 2);  // state:inner ,corner face:x-
	AddBlockInfoToMap(block_id, 7, pe_block_id, 24, 3);  // state:inner ,corner face:x+
	AddBlockInfoToMap(block_id, 7, pe_block_id, 13, 7);  // state:normal
}

void MCBlock::InitTranslateMap()
{
	/*
	 * fill the mc_to_pe TranslateMap
	 * params:mc_block_id,mc_block_data or sec_mc_block_id,pe_block_id,pe_block_data
	 */
	// air
	//AddBlockInfoToMap(0, 0, 0, 0);
	// stone
	AddBlockInfoToMap(1, 0, 56, 0);
	AddBlockInfoToMap(1, 1, 56, 0);
	AddBlockInfoToMap(1, 2, 56, 0);
	AddBlockInfoToMap(1, 3, 56, 0);
	AddBlockInfoToMap(1, 4, 56, 0);
	AddBlockInfoToMap(1, 5, 56, 0);
	AddBlockInfoToMap(1, 6, 56, 0);

	// grass
	AddBlockInfoToMap(2, 0, 62, 0);

	// dirt
	AddBlockInfoToMap(3, 0, 55, 0);
	AddBlockInfoToMap(3, 1, 55, 0);
	AddBlockInfoToMap(3, 2, 55, 0);

	// cobblestone
	AddBlockInfoToMap(4, 0, 58, 0);

	// planks
	AddBlockInfoToMap(5, 0, 81, 0);
	AddBlockInfoToMap(5, 1, 81, 0);
	AddBlockInfoToMap(5, 2, 81, 0);
	AddBlockInfoToMap(5, 3, 81, 0);
	AddBlockInfoToMap(5, 4, 81, 0);
	AddBlockInfoToMap(5, 5, 81, 0);

	// sapling
	AddBlockInfoToMap(6, 0, 119, 0);
	AddBlockInfoToMap(6, 1, 120, 0);
	AddBlockInfoToMap(6, 2, 121, 0);
	AddBlockInfoToMap(6, 3, 122, 0);
	AddBlockInfoToMap(6, 4, 120, 0);
	AddBlockInfoToMap(6, 5, 119, 0);

	// bedrock
	AddBlockInfoToMap(7, 0, 123, 0);

	// flowing_water
	AddBlockInfoToMap(8, 0, 75, 0);

	// water
	AddBlockInfoToMap(9, 0, 75, 0);
	AddBlockInfoToMap(9, 1, 75, 0);
	AddBlockInfoToMap(9, 2, 75, 0);
	AddBlockInfoToMap(9, 3, 75, 0);
	AddBlockInfoToMap(9, 4, 75, 0);
	AddBlockInfoToMap(9, 5, 75, 0);
	AddBlockInfoToMap(9, 6, 75, 0);
	AddBlockInfoToMap(9, 7, 75, 0);

	// flowing_lava
	AddBlockInfoToMap(10, 0, 82, 0);

	// lava
	AddBlockInfoToMap(11, 0, 82, 0);
	AddBlockInfoToMap(11, 2, 82, 0);
	AddBlockInfoToMap(11, 4, 82, 0);
	AddBlockInfoToMap(11, 6, 82, 0);

	// sand
	AddBlockInfoToMap(12, 0, 51, 0);
	AddBlockInfoToMap(12, 0, 51, 0);

	// gravel
	AddBlockInfoToMap(13, 0, 12, 0);

	// gold_ore
	AddBlockInfoToMap(14, 0, 18, 0);

	// iron_ore 
	AddBlockInfoToMap(15, 0, 124, 0);

	// coal_ore
	AddBlockInfoToMap(16, 0, 125, 0);

	// log 
	AddBlockInfoToMap(17, 0, 98, 0);
	AddBlockInfoToMap(17, 1, 98, 0);
	AddBlockInfoToMap(17, 2, 98, 0);
	AddBlockInfoToMap(17, 3, 98, 0);

	// leaves 
	AddBlockInfoToMap(18, 0, 86, 0);
	AddBlockInfoToMap(18, 1, 91, 0);
	AddBlockInfoToMap(18, 2, 129, 0);
	AddBlockInfoToMap(18, 3, 85, 0);

	// sponge
	AddBlockInfoToMap(19, 0, 174, 0);
	AddBlockInfoToMap(19, 1, 174, 0);

	// glass
	AddBlockInfoToMap(20, 0, 95, 0);

	// lapis_ore
	AddBlockInfoToMap(21, 0, 130, 0);

	// lapis_block
	AddBlockInfoToMap(22, 0, 131, 0);

	// dispenser (can not find the replace block in paracraft)
	AddBlockInfoToMap(23, 0, 155, 0);

	// lapis_block 
	AddBlockInfoToMap(24, 0, 4, 0);
	AddBlockInfoToMap(24, 1, 4, 0);
	AddBlockInfoToMap(24, 2, 4, 0);

	// noteblock
	AddBlockInfoToMap(25, 0, 216, 0);

	// bed (can not find the replace block in paracraft)
	AddBlockInfoToMap(26, 0, 0, 0);

	// golden_rail
	AddBlockInfoToMap(27, 0, 250, 2); //flat and around z 
	AddBlockInfoToMap(27, 1, 250, 1); //flat and around x  
	AddBlockInfoToMap(27, 2, 250, 7);//from low to high and around x
	AddBlockInfoToMap(27, 3, 250, 9);//from high to low and around x
	AddBlockInfoToMap(27, 4, 250, 10);//from high to low and around z
	AddBlockInfoToMap(27, 5, 250, 8);//from low to high and around z


	// detector_rail
	AddBlockInfoToMap(28, 0, 251, 2); //flat and around z 
	AddBlockInfoToMap(28, 1, 251, 1); //flat and around x  
	AddBlockInfoToMap(28, 2, 251, 7);//from low to high and around x
	AddBlockInfoToMap(28, 3, 251, 9);//from high to low and around x
	AddBlockInfoToMap(28, 4, 251, 10);//from high to low and around z
	AddBlockInfoToMap(28, 5, 251, 8);//from low to high and around z

	// sticky_piston
	AddBlockInfoToMap(29, 2, 202, 2);  // side£ºz-   state£ºoff
	AddBlockInfoToMap(29, 3, 202, 3);  // side£ºz+   state£ºoff
	AddBlockInfoToMap(29, 4, 202, 0);  // side£ºx-   state£ºoff
	AddBlockInfoToMap(29, 5, 202, 1);  // side£ºx+   state£ºoff
	AddBlockInfoToMap(29, 10, 202, 10); // side£ºz-   state£ºon
	AddBlockInfoToMap(29, 11, 202, 11); // side£ºz+   state£ºon
	AddBlockInfoToMap(29, 12, 202, 8); // side£ºx-   state£ºon
	AddBlockInfoToMap(29, 13, 202, 9); // side£ºx+   state£ºon

	// web
	AddBlockInfoToMap(30, 0, 118, 0);

	// tallgrass
	AddBlockInfoToMap(31, 1, 113, 0);
	AddBlockInfoToMap(31, 2, 114, 0);

	// deadbush
	AddBlockInfoToMap(32, 0, 132, 0);

	// piston
	AddBlockInfoToMap(33, 2, 203, 2);  // side£ºz-   state£ºoff
	AddBlockInfoToMap(33, 3, 203, 3);  // side£ºz+   state£ºoff
	AddBlockInfoToMap(33, 4, 203, 0);  // side£ºx-   state£ºoff
	AddBlockInfoToMap(33, 5, 203, 1);  // side£ºx+   state£ºoff
	AddBlockInfoToMap(33, 10, 203, 10); // side£ºz-   state£ºon
	AddBlockInfoToMap(33, 11, 203, 11); // side£ºz+   state£ºon
	AddBlockInfoToMap(33, 12, 203, 8); // side£ºx-   state£ºon
	AddBlockInfoToMap(33, 13, 203, 9); // side£ºx+   state£ºon

	// piston_head
	AddBlockInfoToMap(34, 2, 204, 10);  // side£ºz-  piston_head
	AddBlockInfoToMap(34, 3, 204, 11);  // side£ºz+  piston_head
	AddBlockInfoToMap(34, 4, 204, 8);  // side£ºx-  piston_head
	AddBlockInfoToMap(34, 5, 204, 9);  // side£ºx+  piston_head
	AddBlockInfoToMap(34, 10, 204, 10); // side£ºz-  sticky_piston 
	AddBlockInfoToMap(34, 11, 204, 11); // side£ºz+  sticky_piston
	AddBlockInfoToMap(34, 12, 204, 8); // side£ºx-  sticky_piston
	AddBlockInfoToMap(34, 13, 204, 9); // side£ºx+  sticky_piston

	// wool 
	AddBlockInfoToMap(35, 0, 133, 0);
	AddBlockInfoToMap(35, 1, 94, 0);
	AddBlockInfoToMap(35, 2, 25, 0);
	AddBlockInfoToMap(35, 3, 21, 0);
	AddBlockInfoToMap(35, 4, 27, 0);
	AddBlockInfoToMap(35, 5, 93, 0);
	AddBlockInfoToMap(35, 6, 96, 0);
	AddBlockInfoToMap(35, 7, 134, 0);
	AddBlockInfoToMap(35, 8, 135, 0);
	AddBlockInfoToMap(35, 9, 20, 0);
	AddBlockInfoToMap(35, 10, 24, 0);
	AddBlockInfoToMap(35, 11, 19, 0);
	AddBlockInfoToMap(35, 12, 136, 0);
	AddBlockInfoToMap(35, 13, 137, 0);
	AddBlockInfoToMap(35, 14, 23, 0);
	AddBlockInfoToMap(35, 15, 71, 0);

	// piston_extension (can find this block in version 1.8.8 mc,replace it with 34<piston_head>) 
	AddBlockInfoToMap(36, 0, 0, 0);

	// yellow_flower
	AddBlockInfoToMap(37, 0, 116, 0);

	// red_flower
	AddBlockInfoToMap(38, 0, 115, 0);
	AddBlockInfoToMap(38, 1, 115, 0);
	AddBlockInfoToMap(38, 2, 115, 0);
	AddBlockInfoToMap(38, 3, 115, 0);
	AddBlockInfoToMap(38, 4, 115, 0);
	AddBlockInfoToMap(38, 5, 115, 0);
	AddBlockInfoToMap(38, 6, 115, 0);
	AddBlockInfoToMap(38, 7, 115, 0);
	AddBlockInfoToMap(38, 8, 115, 0);

	// brown_mushroom
	AddBlockInfoToMap(39, 0, 141, 0);

	// red_mushroom
	AddBlockInfoToMap(40, 0, 117, 0);

	// gold_block
	AddBlockInfoToMap(41, 0, 142, 0);

	// iron_block 
	AddBlockInfoToMap(42, 0, 143, 0);

	// double_stone_slab (can find this block in version 1.8.8 mc)
	AddBlockInfoToMap(43, 0, 59, 0);
	AddBlockInfoToMap(43, 1, 51, 0);
	AddBlockInfoToMap(43, 2, 138, 0);
	AddBlockInfoToMap(43, 3, 155, 0);
	AddBlockInfoToMap(43, 4, 70, 0);
	AddBlockInfoToMap(43, 5, 68, 0);
	AddBlockInfoToMap(43, 6, 154, 0);
	AddBlockInfoToMap(43, 7, 97, 0);

	// stone_slab
	AddBlockInfoToMap(44, 0, 176, 0);
	AddBlockInfoToMap(44, 1, 177, 0);
	// 44/2 block doesn't exist
	AddBlockInfoToMap(44, 2, 184, 0);
	AddBlockInfoToMap(44, 3, 178, 0);
	AddBlockInfoToMap(44, 4, 179, 0);
	AddBlockInfoToMap(44, 5, 180, 0);
	AddBlockInfoToMap(44, 6, 181, 0);
	AddBlockInfoToMap(44, 7, 182, 0);

	// brick_block
	AddBlockInfoToMap(45, 0, 70, 0);

	// tnt
	AddBlockInfoToMap(46, 0, 23, 0);

	// bookshelf
	AddBlockInfoToMap(47, 0, 144, 0);

	// mossy_cobblestone
	AddBlockInfoToMap(48, 0, 145, 0);

	// obsidian
	AddBlockInfoToMap(49, 0, 146, 0);

	// torch
	AddBlockInfoToMap(50, 1, 100, 3); //facing:x+
	AddBlockInfoToMap(50, 2, 100, 1); //facing:x-
	AddBlockInfoToMap(50, 3, 100, 2); //facing:z+
	AddBlockInfoToMap(50, 4, 100, 4); //facing:z-
	AddBlockInfoToMap(50, 5, 100, 5); //facing:up

	// fire (can not find the replace block in paracraft)
	AddBlockInfoToMap(51, 0, 0, 0);

	// mob_spawner (can not find the replace block in paracraft)
	AddBlockInfoToMap(52, 0, 155, 0);

	// oak_stairs
	AddStairBlockInfoToMap(53, 112);

	// chest
	// face z-
	AddBlockInfoToMap(54, 2, 215, 0, 0, 2);
	// face z+
	AddBlockInfoToMap(54, 3, 215, 0, 0, 3);
	// face x-
	AddBlockInfoToMap(54, 4, 215, 0, 0, 0);
	// face x+
	AddBlockInfoToMap(54, 5, 215, 0, 0, 1);

	// redstone_wire (can find this block in version 1.8.8 mc)
	AddBlockInfoToMap(55, 0, 189, 0);

	// diamond_ore
	AddBlockInfoToMap(56, 0, 147, 0);

	// diamond_block
	AddBlockInfoToMap(57, 0, 148, 0);

	// crafting_table (can not find the replace block in paracraft)
	AddBlockInfoToMap(58, 0, 128, 0);

	// wheat
	AddBlockInfoToMap(59, 0, 164, 3);
	AddBlockInfoToMap(59, 1, 164, 3);
	AddBlockInfoToMap(59, 2, 164, 2);
	AddBlockInfoToMap(59, 3, 164, 2);
	AddBlockInfoToMap(59, 4, 164, 1);
	AddBlockInfoToMap(59, 5, 164, 1);
	AddBlockInfoToMap(59, 6, 164, 0);
	AddBlockInfoToMap(59, 7, 164, 0);

	// farmland
	AddBlockInfoToMap(60, 0, 13, 0); //dry
	AddBlockInfoToMap(60, 7, 13, 0); //moist

	// furnace (can not find the replace block in paracraft)
	// face z-
	AddBlockInfoToMap(61, 2, 155, 0);
	// face z+			   
	AddBlockInfoToMap(61, 3, 155, 0);
	// face x-			   
	AddBlockInfoToMap(61, 4, 155, 0);
	// face x+			   
	AddBlockInfoToMap(61, 5, 155, 0);

	// lit_furnace (can not find the replace block in paracraft)
	AddBlockInfoToMap(62, 0, 155, 0);

	// standing_sign ()
	AddBlockInfoToMap(63, 0, 211, 2);   // south 
	AddBlockInfoToMap(63, 1, 211, 2);	// south - southwest
	AddBlockInfoToMap(63, 2, 211, 2);	// southwest
	AddBlockInfoToMap(63, 3, 211, 1);	// west - southwest
	AddBlockInfoToMap(63, 4, 211, 1);	// west
	AddBlockInfoToMap(63, 5, 211, 1);	// west - northwest
	AddBlockInfoToMap(63, 6, 211, 1);	// northwest
	AddBlockInfoToMap(63, 7, 211, 3);	// north - northwest
	AddBlockInfoToMap(63, 8, 211, 3);	// north
	AddBlockInfoToMap(63, 9, 211, 3);	// north - northeast
	AddBlockInfoToMap(63, 10, 211, 3);	// northeast
	AddBlockInfoToMap(63, 11, 211, 0);	// east - northeast
	AddBlockInfoToMap(63, 12, 211, 0);	// east
	AddBlockInfoToMap(63, 13, 211, 0);	// east - southeast
	AddBlockInfoToMap(63, 14, 211, 0);	// southeast
	AddBlockInfoToMap(63, 15, 211, 2);	// south - southeast

	// wooden_door  (not complete)
	AddBlockInfoToMap(64, 0, 0, 0);  //facing:;hinge:;half:;open/powered:;
	AddBlockInfoToMap(64, 1, 0, 0);  //facing:z-;hinge:left;half:lower;open/powered:off;
	AddBlockInfoToMap(64, 8, 0, 0);  //facing:z-;hinge:left;half:upper;open/powered:off;

	// ladder
	AddBlockInfoToMap(65, 2, 166, 5); //facing:z-
	AddBlockInfoToMap(65, 3, 166, 1); //facing:z+
	AddBlockInfoToMap(65, 4, 166, 4); //facing:x-
	AddBlockInfoToMap(65, 5, 166, 3); //facing:x+

	// rail
	AddBlockInfoToMap(66, 0, 103, 2); //flat and around z 
	AddBlockInfoToMap(66, 1, 103, 1); //flat and around x  
	AddBlockInfoToMap(66, 2, 103, 7);//from low to high and around x
	AddBlockInfoToMap(66, 3, 103, 9);//from high to low and around x
	AddBlockInfoToMap(66, 4, 103, 10);//from high to low and around z
	AddBlockInfoToMap(66, 5, 103, 8);//from low to high and around z

	// stone_stairs
	AddStairBlockInfoToMap(67, 175);

	// wall_sign
	AddBlockInfoToMap(68, 2, 211, 3); //facing:z-
	AddBlockInfoToMap(68, 3, 211, 2); //facing:z+
	AddBlockInfoToMap(68, 4, 211, 1); //facing:x-
	AddBlockInfoToMap(68, 5, 211, 0); //facing:x+

	// lever
	AddBlockInfoToMap(69, 0, 190, 6); //facing:down;open:off;around:x
	AddBlockInfoToMap(69, 1, 190, 3); //facing:x+;open:off
	AddBlockInfoToMap(69, 2, 190, 1); //facing:x-;open:off
	AddBlockInfoToMap(69, 3, 190, 2); //facing:z+;open:off
	AddBlockInfoToMap(69, 4, 190, 4); //facing:z-;open:off
	AddBlockInfoToMap(69, 5, 190, 5); //facing:up;open:off;around:z
	AddBlockInfoToMap(69, 6, 190, 5); //facing:up;open:off;around:x
	AddBlockInfoToMap(69, 7, 190, 6); //facing:down;open:off;around:z
	AddBlockInfoToMap(69, 8, 190, 14); //facing:down;open:on;around:x
	AddBlockInfoToMap(69, 9, 190, 11);  //facing:x+;open:on 
	AddBlockInfoToMap(69, 10, 190, 9); //facing:x-;open:on
	AddBlockInfoToMap(69, 11, 190, 10); //facing:z+;open:on
	AddBlockInfoToMap(69, 12, 190, 12); //facing:z-;open:on
	AddBlockInfoToMap(69, 13, 190, 13); //facing:up;open:on;around:z
	AddBlockInfoToMap(69, 14, 190, 13); //facing:up;open:on;around:x
	AddBlockInfoToMap(69, 15, 190, 14); //facing:down;open:on;around:z

	// stone_pressure_plate
	AddBlockInfoToMap(70, 0, 200, 0);
	AddBlockInfoToMap(70, 1, 200, 1);

	// iron_door
	AddBlockInfoToMap(71, 0, 230, 0);

	// wooden_pressure_plate
	AddBlockInfoToMap(72, 0, 201, 0);
	AddBlockInfoToMap(72, 1, 201, 1);

	// redstone_ore
	AddBlockInfoToMap(73, 0, 157, 0);

	// lit_redstone_ore
	AddBlockInfoToMap(74, 0, 157, 0);

	// unlit_redstone_torch
	AddBlockInfoToMap(75, 1, 191, 3); //facing:x+
	AddBlockInfoToMap(75, 2, 191, 1); //facing:x-
	AddBlockInfoToMap(75, 3, 191, 2); //facing:z+
	AddBlockInfoToMap(75, 4, 191, 4); //facing:z-
	AddBlockInfoToMap(75, 5, 191, 5); //facing:up

	// redstone_torch
	AddBlockInfoToMap(76, 1, 192, 3); //facing:x+
	AddBlockInfoToMap(76, 2, 192, 1); //facing:x-
	AddBlockInfoToMap(76, 3, 192, 2); //facing:z+
	AddBlockInfoToMap(76, 4, 192, 4); //facing:z-
	AddBlockInfoToMap(76, 5, 192, 5); //facing:up

	// stone_button
	AddBlockInfoToMap(77, 0, 0, 0); //facing:down
	AddBlockInfoToMap(77, 1, 0, 0); //facing:x+
	AddBlockInfoToMap(77, 2, 0, 0); //facing:x-
	AddBlockInfoToMap(77, 3, 0, 0); //facing:z+
	AddBlockInfoToMap(77, 4, 0, 0); //facing:z-
	AddBlockInfoToMap(77, 5, 0, 0); //facing:up

	// only translate block

	// snow_layer
	AddBlockInfoToMap(78, 0, 5, 0);
	AddBlockInfoToMap(78, 1, 5, 0);
	AddBlockInfoToMap(78, 2, 5, 0);
	AddBlockInfoToMap(78, 3, 5, 0);
	AddBlockInfoToMap(78, 4, 5, 0);
	AddBlockInfoToMap(78, 5, 5, 0);
	AddBlockInfoToMap(78, 6, 5, 0);
	AddBlockInfoToMap(78, 7, 5, 0);

	// ice
	AddBlockInfoToMap(79, 0, 17, 0);

	// snow
	AddBlockInfoToMap(80, 0, 161, 0);

	// cactus (can not find the replace block in paracraft)
	AddBlockInfoToMap(81, 0, 152, 0);

	// clay
	AddBlockInfoToMap(82, 0, 53, 0);

	// reeds
	AddBlockInfoToMap(83, 0, 161, 0);

	// jukebox (can not find the replace block in paracraft)
	AddBlockInfoToMap(84, 0, 217, 0);

	// fence
	AddBlockInfoToMap(85, 0, 101, 1, 0);
	AddBlockInfoToMap(85, 0, 101, 2, 1);
	AddBlockInfoToMap(85, 0, 101, 4, 2);
	AddBlockInfoToMap(85, 0, 101, 5, 3);
	AddBlockInfoToMap(85, 0, 101, 3, 4);
	AddBlockInfoToMap(85, 0, 101, 14, 5);
	AddBlockInfoToMap(85, 0, 101, 6, 6);
	AddBlockInfoToMap(85, 0, 101, 9, 7);
	AddBlockInfoToMap(85, 0, 101, 7, 8);
	AddBlockInfoToMap(85, 0, 101, 8, 9);
	AddBlockInfoToMap(85, 0, 101, 15, 10);
	AddBlockInfoToMap(85, 0, 101, 11, 11);
	AddBlockInfoToMap(85, 0, 101, 13, 12);
	AddBlockInfoToMap(85, 0, 101, 10, 13);
	AddBlockInfoToMap(85, 0, 101, 12, 14);
	AddBlockInfoToMap(85, 0, 101, 16, 15);

	// pumpkin
	AddBlockInfoToMap(86, 0, 149, 0);

	// netherrack
	AddBlockInfoToMap(87, 0, 150, 0);

	// soul_sand
	AddBlockInfoToMap(88, 0, 151, 0);

	// glowstone
	AddBlockInfoToMap(89, 0, 87, 0);

	// portal (can not find the replace block in paracraft)
	AddBlockInfoToMap(90, 0, 146, 0);

	// lit_pumpkin
	AddBlockInfoToMap(91, 0, 220, 0);

	// cake
	AddBlockInfoToMap(92, 0, 84, 0);

	// unpowered_repeater
	AddBlockInfoToMap(93, 0, 0, 0);

	// powered_repeater
	AddBlockInfoToMap(94, 0, 0, 0);

	// stained_glass
	AddBlockInfoToMap(95, 0, 95, 0);
	AddBlockInfoToMap(95, 1, 95, 0);
	AddBlockInfoToMap(95, 2, 95, 0);
	AddBlockInfoToMap(95, 3, 95, 0);
	AddBlockInfoToMap(95, 4, 95, 0);
	AddBlockInfoToMap(95, 5, 95, 0);
	AddBlockInfoToMap(95, 6, 95, 0);
	AddBlockInfoToMap(95, 7, 95, 0);
	AddBlockInfoToMap(95, 8, 95, 0);
	AddBlockInfoToMap(95, 9, 95, 0);
	AddBlockInfoToMap(95, 10, 95, 0);
	AddBlockInfoToMap(95, 11, 95, 0);
	AddBlockInfoToMap(95, 12, 95, 0);
	AddBlockInfoToMap(95, 13, 95, 0);
	AddBlockInfoToMap(95, 14, 95, 0);
	AddBlockInfoToMap(95, 15, 95, 0);


	// trapdoor
	AddBlockInfoToMap(96, 8, 108, 0);
	AddBlockInfoToMap(96, 9, 108, 0);
	AddBlockInfoToMap(96, 10, 108, 0);
	AddBlockInfoToMap(96, 11, 108, 0);

	// monster_egg (can not find the replace block in paracraft)
	AddBlockInfoToMap(97, 0, 58, 0);
	AddBlockInfoToMap(97, 1, 58, 0);
	AddBlockInfoToMap(97, 2, 68, 0);
	AddBlockInfoToMap(97, 3, 69, 0);
	AddBlockInfoToMap(97, 4, 66, 0);
	AddBlockInfoToMap(97, 5, 66, 0);

	// stonebrick
	AddBlockInfoToMap(98, 0, 68, 0);
	AddBlockInfoToMap(98, 1, 68, 0);
	AddBlockInfoToMap(98, 2, 68, 0);
	AddBlockInfoToMap(98, 3, 68, 0);

	// brown_mushroom_block
	AddBlockInfoToMap(99, 0, 141, 0);

	// red_mushroom_block
	AddBlockInfoToMap(100, 0, 117, 0);

	// iron_bars
	AddBlockInfoToMap(101, 0, 209, 11, 0);
	AddBlockInfoToMap(101, 0, 209, 1, 1);
	AddBlockInfoToMap(101, 0, 209, 1, 2);
	AddBlockInfoToMap(101, 0, 209, 2, 3);
	AddBlockInfoToMap(101, 0, 209, 2, 4);
	AddBlockInfoToMap(101, 0, 209, 1, 5);
	AddBlockInfoToMap(101, 0, 209, 3, 6);
	AddBlockInfoToMap(101, 0, 209, 6, 7);
	AddBlockInfoToMap(101, 0, 209, 4, 8);
	AddBlockInfoToMap(101, 0, 209, 5, 9);
	AddBlockInfoToMap(101, 0, 209, 2, 10);
	AddBlockInfoToMap(101, 0, 209, 8, 11);
	AddBlockInfoToMap(101, 0, 209, 10, 12);
	AddBlockInfoToMap(101, 0, 209, 7, 13);
	AddBlockInfoToMap(101, 0, 209, 9, 14);
	AddBlockInfoToMap(101, 0, 209, 11, 15);

	// glass_pane
	AddBlockInfoToMap(102, 0, 102, 11, 0);
	AddBlockInfoToMap(102, 0, 102, 1, 1);
	AddBlockInfoToMap(102, 0, 102, 1, 2);
	AddBlockInfoToMap(102, 0, 102, 2, 3);
	AddBlockInfoToMap(102, 0, 102, 2, 4);
	AddBlockInfoToMap(102, 0, 102, 1, 5);
	AddBlockInfoToMap(102, 0, 102, 3, 6);
	AddBlockInfoToMap(102, 0, 102, 6, 7);
	AddBlockInfoToMap(102, 0, 102, 4, 8);
	AddBlockInfoToMap(102, 0, 102, 5, 9);
	AddBlockInfoToMap(102, 0, 102, 2, 10);
	AddBlockInfoToMap(102, 0, 102, 8, 11);
	AddBlockInfoToMap(102, 0, 102, 10, 12);
	AddBlockInfoToMap(102, 0, 102, 7, 13);
	AddBlockInfoToMap(102, 0, 102, 9, 14);
	AddBlockInfoToMap(102, 0, 102, 11, 15);

	// melon_block
	AddBlockInfoToMap(103, 0, 152, 0);

	// pumpkin_stem (can not find the replace block in paracraft)
	AddBlockInfoToMap(104, 0, 117, 0);

	// melon_stem (can not find the replace block in paracraft)
	AddBlockInfoToMap(105, 0, 117, 0);

	// vine
	AddBlockInfoToMap(106, 1, 162, 5); //facing:z-
	AddBlockInfoToMap(106, 2, 162, 3); //facing:x+
	AddBlockInfoToMap(106, 4, 162, 1); //facing:z+
	AddBlockInfoToMap(106, 8, 162, 4); //facing:x-

	// fence_gate (can not find the replace block in paracraft)
	AddBlockInfoToMap(107, 0, 108, 0);

	// brick_stairs
	AddStairBlockInfoToMap(108, 167);

	// stone_brick_stairs
	AddStairBlockInfoToMap(109, 168);

	// mycelium (can not find the replace block in paracraft)
	AddBlockInfoToMap(110, 0, 153, 0);

	// waterlily
	AddBlockInfoToMap(111, 0, 222, 0);

	// nether_brick
	AddBlockInfoToMap(112, 0, 154, 0);

	// nether_brick_fence
	AddBlockInfoToMap(113, 0, 101, 1, 0);
	AddBlockInfoToMap(113, 0, 101, 2, 1);
	AddBlockInfoToMap(113, 0, 101, 4, 2);
	AddBlockInfoToMap(113, 0, 101, 5, 3);
	AddBlockInfoToMap(113, 0, 101, 3, 4);
	AddBlockInfoToMap(113, 0, 101, 14, 5);
	AddBlockInfoToMap(113, 0, 101, 6, 6);
	AddBlockInfoToMap(113, 0, 101, 9, 7);
	AddBlockInfoToMap(113, 0, 101, 7, 8);
	AddBlockInfoToMap(113, 0, 101, 8, 9);
	AddBlockInfoToMap(113, 0, 101, 15, 10);
	AddBlockInfoToMap(113, 0, 101, 11, 11);
	AddBlockInfoToMap(113, 0, 101, 13, 12);
	AddBlockInfoToMap(113, 0, 101, 10, 13);
	AddBlockInfoToMap(113, 0, 101, 12, 14);
	AddBlockInfoToMap(113, 0, 101, 16, 15);

	// nether_brick_stairs
	AddStairBlockInfoToMap(114, 169);

	// nether_wart (can not find the replace block in paracraft)
	AddBlockInfoToMap(115, 0, 154, 0);

	// enchanting_table (can not find the replace block in paracraft)
	AddBlockInfoToMap(116, 0, 131, 0);

	// brewing_stand (can not find the replace block in paracraft)
	AddBlockInfoToMap(117, 0, 131, 0);

	// cauldron (can not find the replace block in paracraft)
	AddBlockInfoToMap(118, 0, 124, 0);

	// end_portal (can not find the replace block in paracraft)
	AddBlockInfoToMap(119, 0, 0, 0);

	// end_portal_frame (can not find the replace block in paracraft)
	AddBlockInfoToMap(120, 0, 146, 0);

	// end_stone
	AddBlockInfoToMap(121, 0, 155, 0);

	// dragon_egg (can not find the replace block in paracraft)
	AddBlockInfoToMap(122, 0, 0, 0);

	// redstone_lamp
	AddBlockInfoToMap(123, 0, 6, 0);

	// lit_redstone_lamp
	AddBlockInfoToMap(124, 0, 6, 0);

	// double_wooden_slab
	AddBlockInfoToMap(125, 0, 81, 0);
	AddBlockInfoToMap(125, 1, 138, 0);
	AddBlockInfoToMap(125, 2, 139, 0);
	AddBlockInfoToMap(125, 3, 140, 0);
	AddBlockInfoToMap(125, 4, 81, 0);
	AddBlockInfoToMap(125, 5, 70, 0);

	// wooden_slab
	AddBlockInfoToMap(126, 0, 160, 0);
	AddBlockInfoToMap(126, 1, 183, 0);
	AddBlockInfoToMap(126, 2, 184, 0);
	AddBlockInfoToMap(126, 3, 185, 0);
	AddBlockInfoToMap(126, 4, 160, 0);
	AddBlockInfoToMap(126, 5, 179, 0);

	// cocoa (can not find the replace block in paracraft)
	AddBlockInfoToMap(127, 0, 149, 0);

	// sandstone_stairs
	AddStairBlockInfoToMap(128, 104);

	// emerald_ore
	AddBlockInfoToMap(129, 0, 2, 0);

	// ender_chest (can not find the replace block in paracraft)
	// face z-
	AddBlockInfoToMap(130, 2, 215, 0, 0, 2);
	// face z+
	AddBlockInfoToMap(130, 3, 215, 0, 0, 3);
	// face x-
	AddBlockInfoToMap(130, 4, 215, 0, 0, 0);
	// face x+
	AddBlockInfoToMap(130, 5, 215, 0, 0, 1);

	// tripwire_hook (can not find the replace block in paracraft)
	AddBlockInfoToMap(131, 0, 0, 0);

	// tripwire (can not find the replace block in paracraft)
	AddBlockInfoToMap(132, 0, 0, 0);

	// emerald_block
	AddBlockInfoToMap(133, 0, 156, 0);

	// spruce_stairs
	AddStairBlockInfoToMap(134, 117);

	// birch_stairs
	AddStairBlockInfoToMap(135, 172);

	// jungle_stairs
	AddStairBlockInfoToMap(136, 188);

	// command_block
	AddBlockInfoToMap(137, 0, 212, 0);

	// beacon (can not find the replace block in paracraft)
	AddBlockInfoToMap(138, 0, 148, 0);

	// cobblestone_wall
	AddBlockInfoToMap(139, 0, 111, 1, 0);
	AddBlockInfoToMap(139, 0, 111, 2, 1);
	AddBlockInfoToMap(139, 0, 111, 4, 2);
	AddBlockInfoToMap(139, 0, 111, 5, 3);
	AddBlockInfoToMap(139, 0, 111, 3, 4);
	AddBlockInfoToMap(139, 0, 111, 16, 5);
	AddBlockInfoToMap(139, 0, 111, 6, 6);
	AddBlockInfoToMap(139, 0, 111, 9, 7);
	AddBlockInfoToMap(139, 0, 111, 7, 8);
	AddBlockInfoToMap(139, 0, 111, 8, 9);
	AddBlockInfoToMap(139, 0, 111, 17, 10);
	AddBlockInfoToMap(139, 0, 111, 11, 11);
	AddBlockInfoToMap(139, 0, 111, 13, 12);
	AddBlockInfoToMap(139, 0, 111, 10, 13);
	AddBlockInfoToMap(139, 0, 111, 12, 14);
	AddBlockInfoToMap(139, 0, 111, 18, 15);

	// flower_pot (can not find the replace block in paracraft)
	AddBlockInfoToMap(140, 0, 98, 0);

	// carrots (can not find the replace block in paracraft)
	AddBlockInfoToMap(141, 0, 113, 0);

	// potatoes (can not find the replace block in paracraft)
	AddBlockInfoToMap(142, 0, 113, 0);

	// wooden_button 
	AddBlockInfoToMap(143, 0, 117, 0);

	// skull (can not find the replace block in paracraft)
	AddBlockInfoToMap(144, 0, 123, 0);

	// anvil (can not find the replace block in paracraft)
	AddBlockInfoToMap(145, 0, 124, 0);
	AddBlockInfoToMap(145, 1, 124, 0);
	AddBlockInfoToMap(145, 2, 124, 0);

	// trapped_chest (can not find the replace block in paracraft)
	// face z-
	AddBlockInfoToMap(146, 2, 215, 0, 0, 2);
	// face z+
	AddBlockInfoToMap(146, 3, 215, 0, 0, 3);
	// face x-
	AddBlockInfoToMap(146, 4, 215, 0, 0, 0);
	// face x+
	AddBlockInfoToMap(146, 5, 215, 0, 0, 1);

	// light_weighted_pressure_plate (can not find the replace block in paracraft)
	AddBlockInfoToMap(147, 0, 201, 0);
	AddBlockInfoToMap(147, 1, 201, 1);

	// heavy_weighted_pressure_plate (can not find the replace block in paracraft)
	AddBlockInfoToMap(148, 0, 200, 0);
	AddBlockInfoToMap(148, 1, 200, 1);

	// unpowered_comparator (can not find the replace block in paracraft)
	AddBlockInfoToMap(149, 0, 0, 0);

	// powered_comparator (can not find the replace block in paracraft)
	AddBlockInfoToMap(150, 0, 0, 0);

	// daylight_detector (can not find the replace block in paracraft)
	AddBlockInfoToMap(151, 0, 74, 0);

	// redstone_block
	AddBlockInfoToMap(152, 0, 157, 0);

	// quartz_ore
	AddBlockInfoToMap(153, 0, 158, 0);

	// hopper (can not find the replace block in paracraft)
	AddBlockInfoToMap(154, 0, 155, 0);

	// quartz_block
	AddBlockInfoToMap(155, 0, 97, 0);
	AddBlockInfoToMap(155, 1, 97, 0);
	AddBlockInfoToMap(155, 2, 97, 0);
	AddBlockInfoToMap(155, 3, 97, 0);
	AddBlockInfoToMap(155, 4, 97, 0);
	AddBlockInfoToMap(155, 5, 97, 0);
	AddBlockInfoToMap(155, 6, 97, 0);
	AddBlockInfoToMap(155, 7, 97, 0);
	AddBlockInfoToMap(155, 8, 97, 0);
	AddBlockInfoToMap(155, 9, 97, 0);
	AddBlockInfoToMap(155, 10, 97, 0);
	AddBlockInfoToMap(155, 11, 97, 0);
	AddBlockInfoToMap(155, 12, 97, 0);
	AddBlockInfoToMap(155, 13, 97, 0);
	AddBlockInfoToMap(155, 14, 97, 0);
	AddBlockInfoToMap(155, 15, 97, 0);

	// quartz_stairs
	AddStairBlockInfoToMap(156, 187);

	// face x-
	AddBlockInfoToMap(156, 0, 187, 5, 0); // state:outter 
	AddBlockInfoToMap(156, 0, 187, 8, 1); // state:outter 
	AddBlockInfoToMap(156, 0, 187, 18, 2);// state:inner
	AddBlockInfoToMap(156, 0, 187, 21, 3);// state:inner
	AddBlockInfoToMap(156, 0, 187, 1, 7); // state:normal
	// face x+		   	   	   
	AddBlockInfoToMap(156, 1, 187, 6, 0); // state:outter
	AddBlockInfoToMap(156, 1, 187, 7, 1); // state:outter
	AddBlockInfoToMap(156, 1, 187, 19, 2);// state:inner
	AddBlockInfoToMap(156, 1, 187, 20, 3);// state:inner
	AddBlockInfoToMap(156, 1, 187, 2, 7); // state:normal
	// face z-		     	   
	AddBlockInfoToMap(156, 2, 187, 5, 0); // state:outter
	AddBlockInfoToMap(156, 2, 187, 8, 1); // state:outter
	AddBlockInfoToMap(156, 2, 187, 18, 2);// state:inner
	AddBlockInfoToMap(156, 2, 187, 21, 3);// state:inner
	AddBlockInfoToMap(156, 2, 187, 4, 7); // state:normal
	// face z-		  		   
	AddBlockInfoToMap(156, 3, 187, 6, 0); // state:outter
	AddBlockInfoToMap(156, 3, 187, 7, 1); // state:outter
	AddBlockInfoToMap(156, 3, 187, 19, 2);// state:inner
	AddBlockInfoToMap(156, 3, 187, 20, 3);// state:inner
	AddBlockInfoToMap(156, 3, 187, 3, 7); // state:normal

	// activator_rail (can not find the replace block in paracraft)
	AddBlockInfoToMap(157, 0, 250, 2); //flat and around z 
	AddBlockInfoToMap(157, 1, 250, 1); //flat and around x  
	AddBlockInfoToMap(157, 2, 250, 7);//from low to high and around x
	AddBlockInfoToMap(157, 3, 250, 9);//from high to low and around x
	AddBlockInfoToMap(157, 4, 250, 10);//from high to low and around z
	AddBlockInfoToMap(157, 5, 250, 8);//from low to high and around z

	// dropper (can not find the replace block in paracraft)
	AddBlockInfoToMap(158, 0, 155, 0);

	// stained_hardened_clay (can not find the replace block in paracraft)
	
	AddBlockInfoToMap(159, 0, 133, 0);
	AddBlockInfoToMap(159, 1, 94, 0);
	AddBlockInfoToMap(159, 2, 25, 0);
	AddBlockInfoToMap(159, 3, 21, 0);
	AddBlockInfoToMap(159, 4, 27, 0);
	AddBlockInfoToMap(159, 5, 20, 0);
	AddBlockInfoToMap(159, 6, 96, 0);
	AddBlockInfoToMap(159, 7, 134, 0);
	AddBlockInfoToMap(159, 8, 134, 0);
	AddBlockInfoToMap(159, 9, 20, 0);
	AddBlockInfoToMap(159, 10, 24, 0);
	AddBlockInfoToMap(159, 11, 19, 0);
	AddBlockInfoToMap(159, 12, 136, 0);
	AddBlockInfoToMap(159, 13, 137, 0);
	AddBlockInfoToMap(159, 14, 23, 0);
	AddBlockInfoToMap(159, 15, 71, 0);
	

	// stained_glass_pan
	//data:0-15, have 16 color glass plane
	AddBlockInfoToMap(160, 0, 102, 11, 0);
	AddBlockInfoToMap(160, 0, 102, 1, 1);
	AddBlockInfoToMap(160, 0, 102, 1, 2);
	AddBlockInfoToMap(160, 0, 102, 2, 3);
	AddBlockInfoToMap(160, 0, 102, 2, 4);
	AddBlockInfoToMap(160, 0, 102, 1, 5);
	AddBlockInfoToMap(160, 0, 102, 3, 6);
	AddBlockInfoToMap(160, 0, 102, 6, 7);
	AddBlockInfoToMap(160, 0, 102, 4, 8);
	AddBlockInfoToMap(160, 0, 102, 5, 9);
	AddBlockInfoToMap(160, 0, 102, 2, 10);
	AddBlockInfoToMap(160, 0, 102, 8, 11);
	AddBlockInfoToMap(160, 0, 102, 10, 12);
	AddBlockInfoToMap(160, 0, 102, 7, 13);
	AddBlockInfoToMap(160, 0, 102, 9, 14);
	AddBlockInfoToMap(160, 0, 102, 11, 15);

	AddBlockInfoToMap(160, 1, 102, 11, 0);
	AddBlockInfoToMap(160, 1, 102, 1, 1);
	AddBlockInfoToMap(160, 1, 102, 1, 2);
	AddBlockInfoToMap(160, 1, 102, 2, 3);
	AddBlockInfoToMap(160, 1, 102, 2, 4);
	AddBlockInfoToMap(160, 1, 102, 1, 5);
	AddBlockInfoToMap(160, 1, 102, 3, 6);
	AddBlockInfoToMap(160, 1, 102, 6, 7);
	AddBlockInfoToMap(160, 1, 102, 4, 8);
	AddBlockInfoToMap(160, 1, 102, 5, 9);
	AddBlockInfoToMap(160, 1, 102, 2, 10);
	AddBlockInfoToMap(160, 1, 102, 8, 11);
	AddBlockInfoToMap(160, 1, 102, 10, 12);
	AddBlockInfoToMap(160, 1, 102, 7, 13);
	AddBlockInfoToMap(160, 1, 102, 9, 14);
	AddBlockInfoToMap(160, 1, 102, 11, 15);

	AddBlockInfoToMap(160, 2, 102, 11, 0);
	AddBlockInfoToMap(160, 2, 102, 1, 1);
	AddBlockInfoToMap(160, 2, 102, 1, 2);
	AddBlockInfoToMap(160, 2, 102, 2, 3);
	AddBlockInfoToMap(160, 2, 102, 2, 4);
	AddBlockInfoToMap(160, 2, 102, 1, 5);
	AddBlockInfoToMap(160, 2, 102, 3, 6);
	AddBlockInfoToMap(160, 2, 102, 6, 7);
	AddBlockInfoToMap(160, 2, 102, 4, 8);
	AddBlockInfoToMap(160, 2, 102, 5, 9);
	AddBlockInfoToMap(160, 2, 102, 2, 10);
	AddBlockInfoToMap(160, 2, 102, 8, 11);
	AddBlockInfoToMap(160, 2, 102, 10, 12);
	AddBlockInfoToMap(160, 2, 102, 7, 13);
	AddBlockInfoToMap(160, 2, 102, 9, 14);
	AddBlockInfoToMap(160, 2, 102, 11, 15);

	AddBlockInfoToMap(160, 3, 102, 11, 0);
	AddBlockInfoToMap(160, 3, 102, 1, 1);
	AddBlockInfoToMap(160, 3, 102, 1, 2);
	AddBlockInfoToMap(160, 3, 102, 2, 3);
	AddBlockInfoToMap(160, 3, 102, 2, 4);
	AddBlockInfoToMap(160, 3, 102, 1, 5);
	AddBlockInfoToMap(160, 3, 102, 3, 6);
	AddBlockInfoToMap(160, 3, 102, 6, 7);
	AddBlockInfoToMap(160, 3, 102, 4, 8);
	AddBlockInfoToMap(160, 3, 102, 5, 9);
	AddBlockInfoToMap(160, 3, 102, 2, 10);
	AddBlockInfoToMap(160, 3, 102, 8, 11);
	AddBlockInfoToMap(160, 3, 102, 10, 12);
	AddBlockInfoToMap(160, 3, 102, 7, 13);
	AddBlockInfoToMap(160, 3, 102, 9, 14);
	AddBlockInfoToMap(160, 3, 102, 11, 15);

	AddBlockInfoToMap(160, 4, 102, 11, 0);
	AddBlockInfoToMap(160, 4, 102, 1, 1);
	AddBlockInfoToMap(160, 4, 102, 1, 2);
	AddBlockInfoToMap(160, 4, 102, 2, 3);
	AddBlockInfoToMap(160, 4, 102, 2, 4);
	AddBlockInfoToMap(160, 4, 102, 1, 5);
	AddBlockInfoToMap(160, 4, 102, 3, 6);
	AddBlockInfoToMap(160, 4, 102, 6, 7);
	AddBlockInfoToMap(160, 4, 102, 4, 8);
	AddBlockInfoToMap(160, 4, 102, 5, 9);
	AddBlockInfoToMap(160, 4, 102, 2, 10);
	AddBlockInfoToMap(160, 4, 102, 8, 11);
	AddBlockInfoToMap(160, 4, 102, 10, 12);
	AddBlockInfoToMap(160, 4, 102, 7, 13);
	AddBlockInfoToMap(160, 4, 102, 9, 14);
	AddBlockInfoToMap(160, 4, 102, 11, 15);

	AddBlockInfoToMap(160, 5, 102, 11, 0);
	AddBlockInfoToMap(160, 5, 102, 1, 1);
	AddBlockInfoToMap(160, 5, 102, 1, 2);
	AddBlockInfoToMap(160, 5, 102, 2, 3);
	AddBlockInfoToMap(160, 5, 102, 2, 4);
	AddBlockInfoToMap(160, 5, 102, 1, 5);
	AddBlockInfoToMap(160, 5, 102, 3, 6);
	AddBlockInfoToMap(160, 5, 102, 6, 7);
	AddBlockInfoToMap(160, 5, 102, 4, 8);
	AddBlockInfoToMap(160, 5, 102, 5, 9);
	AddBlockInfoToMap(160, 5, 102, 2, 10);
	AddBlockInfoToMap(160, 5, 102, 8, 11);
	AddBlockInfoToMap(160, 5, 102, 10, 12);
	AddBlockInfoToMap(160, 5, 102, 7, 13);
	AddBlockInfoToMap(160, 5, 102, 9, 14);
	AddBlockInfoToMap(160, 5, 102, 11, 15);

	AddBlockInfoToMap(160, 6, 102, 11, 0);
	AddBlockInfoToMap(160, 6, 102, 1, 1);
	AddBlockInfoToMap(160, 6, 102, 1, 2);
	AddBlockInfoToMap(160, 6, 102, 2, 3);
	AddBlockInfoToMap(160, 6, 102, 2, 4);
	AddBlockInfoToMap(160, 6, 102, 1, 5);
	AddBlockInfoToMap(160, 6, 102, 3, 6);
	AddBlockInfoToMap(160, 6, 102, 6, 7);
	AddBlockInfoToMap(160, 6, 102, 4, 8);
	AddBlockInfoToMap(160, 6, 102, 5, 9);
	AddBlockInfoToMap(160, 6, 102, 2, 10);
	AddBlockInfoToMap(160, 6, 102, 8, 11);
	AddBlockInfoToMap(160, 6, 102, 10, 12);
	AddBlockInfoToMap(160, 6, 102, 7, 13);
	AddBlockInfoToMap(160, 6, 102, 9, 14);
	AddBlockInfoToMap(160, 6, 102, 11, 15);

	AddBlockInfoToMap(160, 7, 102, 11, 0);
	AddBlockInfoToMap(160, 7, 102, 1, 1);
	AddBlockInfoToMap(160, 7, 102, 1, 2);
	AddBlockInfoToMap(160, 7, 102, 2, 3);
	AddBlockInfoToMap(160, 7, 102, 2, 4);
	AddBlockInfoToMap(160, 7, 102, 1, 5);
	AddBlockInfoToMap(160, 7, 102, 3, 6);
	AddBlockInfoToMap(160, 7, 102, 6, 7);
	AddBlockInfoToMap(160, 7, 102, 4, 8);
	AddBlockInfoToMap(160, 7, 102, 5, 9);
	AddBlockInfoToMap(160, 7, 102, 2, 10);
	AddBlockInfoToMap(160, 7, 102, 8, 11);
	AddBlockInfoToMap(160, 7, 102, 10, 12);
	AddBlockInfoToMap(160, 7, 102, 7, 13);
	AddBlockInfoToMap(160, 7, 102, 9, 14);
	AddBlockInfoToMap(160, 7, 102, 11, 15);

	AddBlockInfoToMap(160, 8, 102, 11, 0);
	AddBlockInfoToMap(160, 8, 102, 1, 1);
	AddBlockInfoToMap(160, 8, 102, 1, 2);
	AddBlockInfoToMap(160, 8, 102, 2, 3);
	AddBlockInfoToMap(160, 8, 102, 2, 4);
	AddBlockInfoToMap(160, 8, 102, 1, 5);
	AddBlockInfoToMap(160, 8, 102, 3, 6);
	AddBlockInfoToMap(160, 8, 102, 6, 7);
	AddBlockInfoToMap(160, 8, 102, 4, 8);
	AddBlockInfoToMap(160, 8, 102, 5, 9);
	AddBlockInfoToMap(160, 8, 102, 2, 10);
	AddBlockInfoToMap(160, 8, 102, 8, 11);
	AddBlockInfoToMap(160, 8, 102, 10, 12);
	AddBlockInfoToMap(160, 8, 102, 7, 13);
	AddBlockInfoToMap(160, 8, 102, 9, 14);
	AddBlockInfoToMap(160, 8, 102, 11, 15);

	AddBlockInfoToMap(160, 9, 102, 11, 0);
	AddBlockInfoToMap(160, 9, 102, 1, 1);
	AddBlockInfoToMap(160, 9, 102, 1, 2);
	AddBlockInfoToMap(160, 9, 102, 2, 3);
	AddBlockInfoToMap(160, 9, 102, 2, 4);
	AddBlockInfoToMap(160, 9, 102, 1, 5);
	AddBlockInfoToMap(160, 9, 102, 3, 6);
	AddBlockInfoToMap(160, 9, 102, 6, 7);
	AddBlockInfoToMap(160, 9, 102, 4, 8);
	AddBlockInfoToMap(160, 9, 102, 5, 9);
	AddBlockInfoToMap(160, 9, 102, 2, 10);
	AddBlockInfoToMap(160, 9, 102, 8, 11);
	AddBlockInfoToMap(160, 9, 102, 10, 12);
	AddBlockInfoToMap(160, 9, 102, 7, 13);
	AddBlockInfoToMap(160, 9, 102, 9, 14);
	AddBlockInfoToMap(160, 9, 102, 11, 15);

	AddBlockInfoToMap(160, 10, 102, 11, 0);
	AddBlockInfoToMap(160, 10, 102, 1, 1);
	AddBlockInfoToMap(160, 10, 102, 1, 2);
	AddBlockInfoToMap(160, 10, 102, 2, 3);
	AddBlockInfoToMap(160, 10, 102, 2, 4);
	AddBlockInfoToMap(160, 10, 102, 1, 5);
	AddBlockInfoToMap(160, 10, 102, 3, 6);
	AddBlockInfoToMap(160, 10, 102, 6, 7);
	AddBlockInfoToMap(160, 10, 102, 4, 8);
	AddBlockInfoToMap(160, 10, 102, 5, 9);
	AddBlockInfoToMap(160, 10, 102, 2, 10);
	AddBlockInfoToMap(160, 10, 102, 8, 11);
	AddBlockInfoToMap(160, 10, 102, 10, 12);
	AddBlockInfoToMap(160, 10, 102, 7, 13);
	AddBlockInfoToMap(160, 10, 102, 9, 14);
	AddBlockInfoToMap(160, 10, 102, 11, 15);

	AddBlockInfoToMap(160, 11, 102, 11, 0);
	AddBlockInfoToMap(160, 11, 102, 1, 1);
	AddBlockInfoToMap(160, 11, 102, 1, 2);
	AddBlockInfoToMap(160, 11, 102, 2, 3);
	AddBlockInfoToMap(160, 11, 102, 2, 4);
	AddBlockInfoToMap(160, 11, 102, 1, 5);
	AddBlockInfoToMap(160, 11, 102, 3, 6);
	AddBlockInfoToMap(160, 11, 102, 6, 7);
	AddBlockInfoToMap(160, 11, 102, 4, 8);
	AddBlockInfoToMap(160, 11, 102, 5, 9);
	AddBlockInfoToMap(160, 11, 102, 2, 10);
	AddBlockInfoToMap(160, 11, 102, 8, 11);
	AddBlockInfoToMap(160, 11, 102, 10, 12);
	AddBlockInfoToMap(160, 11, 102, 7, 13);
	AddBlockInfoToMap(160, 11, 102, 9, 14);
	AddBlockInfoToMap(160, 11, 102, 11, 15);

	AddBlockInfoToMap(160, 12, 102, 11, 0);
	AddBlockInfoToMap(160, 12, 102, 1, 1);
	AddBlockInfoToMap(160, 12, 102, 1, 2);
	AddBlockInfoToMap(160, 12, 102, 2, 3);
	AddBlockInfoToMap(160, 12, 102, 2, 4);
	AddBlockInfoToMap(160, 12, 102, 1, 5);
	AddBlockInfoToMap(160, 12, 102, 3, 6);
	AddBlockInfoToMap(160, 12, 102, 6, 7);
	AddBlockInfoToMap(160, 12, 102, 4, 8);
	AddBlockInfoToMap(160, 12, 102, 5, 9);
	AddBlockInfoToMap(160, 12, 102, 2, 10);
	AddBlockInfoToMap(160, 12, 102, 8, 11);
	AddBlockInfoToMap(160, 12, 102, 10, 12);
	AddBlockInfoToMap(160, 12, 102, 7, 13);
	AddBlockInfoToMap(160, 12, 102, 9, 14);
	AddBlockInfoToMap(160, 12, 102, 11, 15);

	AddBlockInfoToMap(160, 13, 102, 11, 0);
	AddBlockInfoToMap(160, 13, 102, 1, 1);
	AddBlockInfoToMap(160, 13, 102, 1, 2);
	AddBlockInfoToMap(160, 13, 102, 2, 3);
	AddBlockInfoToMap(160, 13, 102, 2, 4);
	AddBlockInfoToMap(160, 13, 102, 1, 5);
	AddBlockInfoToMap(160, 13, 102, 3, 6);
	AddBlockInfoToMap(160, 13, 102, 6, 7);
	AddBlockInfoToMap(160, 13, 102, 4, 8);
	AddBlockInfoToMap(160, 13, 102, 5, 9);
	AddBlockInfoToMap(160, 13, 102, 2, 10);
	AddBlockInfoToMap(160, 13, 102, 8, 11);
	AddBlockInfoToMap(160, 13, 102, 10, 12);
	AddBlockInfoToMap(160, 13, 102, 7, 13);
	AddBlockInfoToMap(160, 13, 102, 9, 14);
	AddBlockInfoToMap(160, 13, 102, 11, 15);

	AddBlockInfoToMap(160, 14, 102, 11, 0);
	AddBlockInfoToMap(160, 14, 102, 1, 1);
	AddBlockInfoToMap(160, 14, 102, 1, 2);
	AddBlockInfoToMap(160, 14, 102, 2, 3);
	AddBlockInfoToMap(160, 14, 102, 2, 4);
	AddBlockInfoToMap(160, 14, 102, 1, 5);
	AddBlockInfoToMap(160, 14, 102, 3, 6);
	AddBlockInfoToMap(160, 14, 102, 6, 7);
	AddBlockInfoToMap(160, 14, 102, 4, 8);
	AddBlockInfoToMap(160, 14, 102, 5, 9);
	AddBlockInfoToMap(160, 14, 102, 2, 10);
	AddBlockInfoToMap(160, 14, 102, 8, 11);
	AddBlockInfoToMap(160, 14, 102, 10, 12);
	AddBlockInfoToMap(160, 14, 102, 7, 13);
	AddBlockInfoToMap(160, 14, 102, 9, 14);
	AddBlockInfoToMap(160, 14, 102, 11, 15);

	AddBlockInfoToMap(160, 15, 102, 11, 0);
	AddBlockInfoToMap(160, 15, 102, 1, 1);
	AddBlockInfoToMap(160, 15, 102, 1, 2);
	AddBlockInfoToMap(160, 15, 102, 2, 3);
	AddBlockInfoToMap(160, 15, 102, 2, 4);
	AddBlockInfoToMap(160, 15, 102, 1, 5);
	AddBlockInfoToMap(160, 15, 102, 3, 6);
	AddBlockInfoToMap(160, 15, 102, 6, 7);
	AddBlockInfoToMap(160, 15, 102, 4, 8);
	AddBlockInfoToMap(160, 15, 102, 5, 9);
	AddBlockInfoToMap(160, 15, 102, 2, 10);
	AddBlockInfoToMap(160, 15, 102, 8, 11);
	AddBlockInfoToMap(160, 15, 102, 10, 12);
	AddBlockInfoToMap(160, 15, 102, 7, 13);
	AddBlockInfoToMap(160, 15, 102, 9, 14);
	AddBlockInfoToMap(160, 15, 102, 11, 15);

	// leaves2
	AddBlockInfoToMap(161, 0, 129, 0);
	AddBlockInfoToMap(161, 1, 129, 0);

	// log2
	AddBlockInfoToMap(162, 0, 140, 0);
	AddBlockInfoToMap(162, 1, 140, 0);

	// acacia_stairs
	AddStairBlockInfoToMap(163, 172);

	// dark_oak_stairs
	AddStairBlockInfoToMap(164, 188);

	// slime (can not find the replace block in paracraft)
	AddBlockInfoToMap(165, 0, 145, 0);

	// barrier (can not find the replace block in paracraft)
	AddBlockInfoToMap(166, 0, 0, 0);

	// iron_trapdoor
	AddBlockInfoToMap(167, 0, 117, 0);

	// prismarine
	AddBlockInfoToMap(168, 0, 148, 0);
	AddBlockInfoToMap(168, 1, 148, 0);
	AddBlockInfoToMap(168, 2, 148, 0);

	// sea_lantern
	AddBlockInfoToMap(169, 0, 6, 0);

	// hay_block (can not find the replace block in paracraft)
	AddBlockInfoToMap(170, 0, 186, 0);

	// carpet
	AddBlockInfoToMap(171, 0, 234, 0);
	AddBlockInfoToMap(171, 1, 235, 0);
	AddBlockInfoToMap(171, 2, 236, 0);
	AddBlockInfoToMap(171, 3, 237, 0);
	AddBlockInfoToMap(171, 4, 238, 0);
	AddBlockInfoToMap(171, 5, 239, 0);
	AddBlockInfoToMap(171, 6, 240, 0);
	AddBlockInfoToMap(171, 7, 241, 0);
	AddBlockInfoToMap(171, 8, 242, 0);
	AddBlockInfoToMap(171, 9, 243, 0);
	AddBlockInfoToMap(171, 10, 244, 0);
	AddBlockInfoToMap(171, 11, 245, 0);
	AddBlockInfoToMap(171, 12, 246, 0);
	AddBlockInfoToMap(171, 13, 247, 0);
	AddBlockInfoToMap(171, 14, 248, 0);
	AddBlockInfoToMap(171, 15, 249, 0);

	// hardened_clay
	AddBlockInfoToMap(172, 0, 53, 0);

	// coal_block
	AddBlockInfoToMap(173, 0, 71, 0);

	// packed_ice
	AddBlockInfoToMap(174, 0, 17, 0);

	// large_flowers
	AddBlockInfoToMap(175, 0, 116, 0);
	AddBlockInfoToMap(175, 1, 115, 0);
	AddBlockInfoToMap(175, 2, 113, 0);
	AddBlockInfoToMap(175, 3, 114, 0);
	AddBlockInfoToMap(175, 4, 115, 0);
	AddBlockInfoToMap(175, 5, 115, 0);

	// standing_banner (can not find the replace block in paracraft)
	AddBlockInfoToMap(176, 0, 0, 0);

	// wall_banner (can not find the replace block in paracraft)
	AddBlockInfoToMap(177, 0, 0, 0);

	// daylight_detector_inverted (can not find the replace block in paracraft)
	AddBlockInfoToMap(178, 0, 74, 0);

	// red_sandstone
	AddBlockInfoToMap(179, 0, 12, 0);
	AddBlockInfoToMap(179, 1, 12, 0);
	AddBlockInfoToMap(179, 2, 12, 0);

	// red_sandstone_stairs 
	AddStairBlockInfoToMap(180, 188);

	// double_stone_slab2
	AddBlockInfoToMap(181, 0, 23, 0);

	// stone_slab2
	AddBlockInfoToMap(182, 0, 181, 0);

	// spruce_fence_gate
	AddBlockInfoToMap(183, 0, 117, 0);

	// birch_fence_gate
	AddBlockInfoToMap(184, 0, 117, 0);

	// jungle_fence_gate
	AddBlockInfoToMap(185, 0, 117, 0);

	// dark_oak_fence_gate
	AddBlockInfoToMap(186, 0, 117, 0);

	// acacia_fence_gate
	AddBlockInfoToMap(187, 0, 117, 0);

	// spruce_fence
	AddBlockInfoToMap(188, 0, 101, 1, 0);
	AddBlockInfoToMap(188, 0, 101, 2, 1);
	AddBlockInfoToMap(188, 0, 101, 4, 2);
	AddBlockInfoToMap(188, 0, 101, 5, 3);
	AddBlockInfoToMap(188, 0, 101, 3, 4);
	AddBlockInfoToMap(188, 0, 101, 14, 5);
	AddBlockInfoToMap(188, 0, 101, 6, 6);
	AddBlockInfoToMap(188, 0, 101, 9, 7);
	AddBlockInfoToMap(188, 0, 101, 7, 8);
	AddBlockInfoToMap(188, 0, 101, 8, 9);
	AddBlockInfoToMap(188, 0, 101, 15, 10);
	AddBlockInfoToMap(188, 0, 101, 11, 11);
	AddBlockInfoToMap(188, 0, 101, 13, 12);
	AddBlockInfoToMap(188, 0, 101, 10, 13);
	AddBlockInfoToMap(188, 0, 101, 12, 14);
	AddBlockInfoToMap(188, 0, 101, 16, 15);

	// birch_fence
	AddBlockInfoToMap(189, 0, 101, 1, 0);
	AddBlockInfoToMap(189, 0, 101, 2, 1);
	AddBlockInfoToMap(189, 0, 101, 4, 2);
	AddBlockInfoToMap(189, 0, 101, 5, 3);
	AddBlockInfoToMap(189, 0, 101, 3, 4);
	AddBlockInfoToMap(189, 0, 101, 14, 5);
	AddBlockInfoToMap(189, 0, 101, 6, 6);
	AddBlockInfoToMap(189, 0, 101, 9, 7);
	AddBlockInfoToMap(189, 0, 101, 7, 8);
	AddBlockInfoToMap(189, 0, 101, 8, 9);
	AddBlockInfoToMap(189, 0, 101, 15, 10);
	AddBlockInfoToMap(189, 0, 101, 11, 11);
	AddBlockInfoToMap(189, 0, 101, 13, 12);
	AddBlockInfoToMap(189, 0, 101, 10, 13);
	AddBlockInfoToMap(189, 0, 101, 12, 14);
	AddBlockInfoToMap(189, 0, 101, 16, 15);

	// jungle_fence
	AddBlockInfoToMap(190, 0, 101, 1, 0);
	AddBlockInfoToMap(190, 0, 101, 2, 1);
	AddBlockInfoToMap(190, 0, 101, 4, 2);
	AddBlockInfoToMap(190, 0, 101, 5, 3);
	AddBlockInfoToMap(190, 0, 101, 3, 4);
	AddBlockInfoToMap(190, 0, 101, 14, 5);
	AddBlockInfoToMap(190, 0, 101, 6, 6);
	AddBlockInfoToMap(190, 0, 101, 9, 7);
	AddBlockInfoToMap(190, 0, 101, 7, 8);
	AddBlockInfoToMap(190, 0, 101, 8, 9);
	AddBlockInfoToMap(190, 0, 101, 15, 10);
	AddBlockInfoToMap(190, 0, 101, 11, 11);
	AddBlockInfoToMap(190, 0, 101, 13, 12);
	AddBlockInfoToMap(190, 0, 101, 10, 13);
	AddBlockInfoToMap(190, 0, 101, 12, 14);
	AddBlockInfoToMap(190, 0, 101, 16, 15);

	// dark_oak_fence
	AddBlockInfoToMap(191, 0, 214, 1, 0);
	AddBlockInfoToMap(191, 0, 214, 2, 1);
	AddBlockInfoToMap(191, 0, 214, 4, 2);
	AddBlockInfoToMap(191, 0, 214, 5, 3);
	AddBlockInfoToMap(191, 0, 214, 3, 4);
	AddBlockInfoToMap(191, 0, 214, 14, 5);
	AddBlockInfoToMap(191, 0, 214, 6, 6);
	AddBlockInfoToMap(191, 0, 214, 9, 7);
	AddBlockInfoToMap(191, 0, 214, 7, 8);
	AddBlockInfoToMap(191, 0, 214, 8, 9);
	AddBlockInfoToMap(191, 0, 214, 15, 10);
	AddBlockInfoToMap(191, 0, 214, 11, 11);
	AddBlockInfoToMap(191, 0, 214, 13, 12);
	AddBlockInfoToMap(191, 0, 214, 10, 13);
	AddBlockInfoToMap(191, 0, 214, 12, 14);
	AddBlockInfoToMap(191, 0, 214, 16, 15);

	// acacia_fence
	AddBlockInfoToMap(192, 0, 101, 1, 0);
	AddBlockInfoToMap(192, 0, 101, 2, 1);
	AddBlockInfoToMap(192, 0, 101, 4, 2);
	AddBlockInfoToMap(192, 0, 101, 5, 3);
	AddBlockInfoToMap(192, 0, 101, 3, 4);
	AddBlockInfoToMap(192, 0, 101, 14, 5);
	AddBlockInfoToMap(192, 0, 101, 6, 6);
	AddBlockInfoToMap(192, 0, 101, 9, 7);
	AddBlockInfoToMap(192, 0, 101, 7, 8);
	AddBlockInfoToMap(192, 0, 101, 8, 9);
	AddBlockInfoToMap(192, 0, 101, 15, 10);
	AddBlockInfoToMap(192, 0, 101, 11, 11);
	AddBlockInfoToMap(192, 0, 101, 13, 12);
	AddBlockInfoToMap(192, 0, 101, 10, 13);
	AddBlockInfoToMap(192, 0, 101, 12, 14);
	AddBlockInfoToMap(192, 0, 101, 16, 15);

	// spruce_door
	AddBlockInfoToMap(193, 0, 117, 0);

	// birch_door
	AddBlockInfoToMap(194, 0, 117, 0);

	// jungle_door
	AddBlockInfoToMap(195, 0, 117, 0);

	// acacia_door
	AddBlockInfoToMap(196, 0, 117, 0);

	// dark_oak_door
	AddBlockInfoToMap(197, 0, 117, 0);

}

int block_maps[512];

void MCBlock::StaticInit()
{
	for(int i=0; i < 512; ++i)
	{
		block_maps[i] = 1;
	}
	block_maps[0] = 0;
	block_maps[6] = 0;
	block_maps[18] = 0;
	block_maps[19] = 0;

	block_maps[30] = 0;
	block_maps[31] = 0;
	block_maps[32] = 0;
	block_maps[37] = 0;
	block_maps[38] = 0;
	block_maps[39] = 0;
	block_maps[40] = 0;
	block_maps[59] = 0;

	block_maps[65] = 0;
	block_maps[66] = 0;
	block_maps[81] = 0;
	block_maps[83] = 0;
	block_maps[85] = 0;

	block_maps[50] = 0; // torch
	block_maps[51] = 0; 
	block_maps[52] = 0; 
	block_maps[53] = 0; 
	block_maps[67] = 0; 
	block_maps[69] = 0; 
	block_maps[85] = 0; 
	block_maps[20] = 0;  // glass
	block_maps[65] = 0;  
	block_maps[66] = 0;  
	block_maps[63] = 0;  
	block_maps[64] = 0;  
	block_maps[67] = 0;  
	block_maps[68] = 0;  
	block_maps[69] = 0;  

	block_maps[71] = 0;  
	block_maps[75] = 0;  
	block_maps[76] = 0;  
	block_maps[79] = 0;  

	block_maps[101] = 0;  
	block_maps[102] = 0;  
	block_maps[106] = 0;  
	block_maps[107] = 0;  
	block_maps[108] = 0;  
	block_maps[109] = 0;  

	block_maps[113] = 0; 
	block_maps[114] = 0; 
	block_maps[127] = 0;
	block_maps[128] = 0;

	block_maps[134] = 0; 
	block_maps[135] = 0; 
	block_maps[136] = 0; 
	block_maps[138] = 0; 
	block_maps[156] = 0; 
	block_maps[157] = 0; 

	InitTranslateMap();
}


bool MCBlock::IsSolidBlock( int block_id )
{
	if(block_id >=0 && block_id<256)
		return (block_maps[block_id] == 1);
	else
		return true;
}

bool MCBlock::IsStairBlock(uint16_t block_id)
{
	if (block_id == 53 || block_id == 67 || block_id == 108 || block_id == 109 || block_id == 114 || block_id == 128 || 
		block_id == 134 || block_id == 135 || block_id == 136 || block_id == 156 || block_id == 163 || block_id == 164 || block_id == 180)
		return true;
	else
		return false;
}

bool MCBlock::TranslateMCBlock(uint16_t &block_id, uint16_t &block_data, uint16_t block_state, uint16_t& block_side)
{
	uint32_t id_32 = block_id;
	id_32 = id_32 << 16;
	uint32_t data_32 = block_data;
	data_32 = data_32 << 8;
	uint32_t state_32 = block_state;
	uint32_t block_tag = id_32 + data_32 + state_32;
	if (MCBlockMaps[block_tag])
	{
		
		MCBlockInfo *info = MCBlockMaps[block_tag];
		if (info->pe_block_id == 0)
			return false;
		block_id = info->pe_block_id;
		block_data = info->pe_data;
		block_side = info->pe_side;
		return true;
	}
	block_id = 0;
	block_data = 0;
	block_side = 0;
	return false;
}