
#include "NPLInterface.hpp"
#include "block.h"
#include <iostream>
#include <set>
namespace mc_map {
	namespace block {

		static std::set<std::string> s_not_exist_block_name_set;

		static std::map<std::string, uint16_t> s_block_name_id_map = {};
		static std::map<std::string, int8_t> s_block_name_data_map = {};
		static std::map<std::string, std::string> s_block_id_data_to_name_map = {};
		
		static bool s_solid_block_map[512];

		void AddMineCraftBlock(int id, int data, std::string name)
		{
			s_block_name_id_map[name] = id;
			s_block_name_data_map[name] = data;

			std::string id_data;
			id_data = id_data + std::to_string(id) + ";" + std::to_string(data);
			s_block_id_data_to_name_map[id_data] = name;
		}

		bool IsStairBlock(uint16_t block_id)
		{
			if (block_id == 53 || block_id == 67 || block_id == 108 || block_id == 109 || block_id == 114 || block_id == 128 ||
				block_id == 134 || block_id == 135 || block_id == 136 || block_id == 156 || block_id == 163 || block_id == 164 || block_id == 180)
				return true;
			else
				return false;
		}

		std::set<std::string>& GetNotExistBlockNameSet()
		{
			return s_not_exist_block_name_set;
		}

		int16_t GetBlockIdByName(std::string name)
		{
			if (s_block_name_id_map.find(name) != s_block_name_id_map.end()) {
				return s_block_name_id_map.at(name);
			}
			
			s_not_exist_block_name_set.insert(name);

			char Msg[1024];
			snprintf(Msg, 1000, "MineCraft Block Not Exist!!! Name: %s\n", name.c_str());
			OUTPUT_LOG(Msg);

			return 0;
		}

		int8_t GetBlockDataByName(std::string name)
		{
			if (s_block_name_data_map.find(name) != s_block_name_data_map.end()) {
				return s_block_name_data_map.at(name);
			}
			return 0;
		}

		void StaticInit()
		{
			static bool inited = false;
			if (inited) return;
			inited = true;

			AddMineCraftBlock(0, 0, "minecraft:air");
			AddMineCraftBlock(1, 0, "minecraft:stone");
			AddMineCraftBlock(1, 1, "minecraft:granite");
			AddMineCraftBlock(1, 2, "minecraft:polished_granite");
			AddMineCraftBlock(1, 3, "minecraft:diorite");
			AddMineCraftBlock(1, 4, "minecraft:polished_diorite");
			AddMineCraftBlock(1, 5, "minecraft:andesite");
			AddMineCraftBlock(1, 6, "minecraft:polished_andesite");
			AddMineCraftBlock(2, 0, "minecraft:grass");
			AddMineCraftBlock(3, 0, "minecraft:dirt");
			AddMineCraftBlock(3, 1, "minecraft:coarse_dirt");
			AddMineCraftBlock(3, 2, "minecraft:podzol");
			AddMineCraftBlock(4, 0, "minecraft:cobblestone");
			AddMineCraftBlock(5, 0, "minecraft:oak_wood_plank");
			AddMineCraftBlock(5, 1, "minecraft:spruce_wood_plank");
			AddMineCraftBlock(5, 2, "minecraft:birch_wood_plank");
			AddMineCraftBlock(5, 3, "minecraft:jungle_wood_plank");
			AddMineCraftBlock(5, 4, "minecraft:acacia_wood_plank");
			AddMineCraftBlock(5, 5, "minecraft:dark_oak_wood_plank");
			AddMineCraftBlock(6, 0, "minecraft:oak_sapling");
			AddMineCraftBlock(6, 1, "minecraft:spruce_sapling");
			AddMineCraftBlock(6, 2, "minecraft:birch_sapling");
			AddMineCraftBlock(6, 3, "minecraft:jungle_sapling");
			AddMineCraftBlock(6, 4, "minecraft:acacia_sapling");
			AddMineCraftBlock(6, 5, "minecraft:dark_oak_sapling");
			AddMineCraftBlock(7, 0, "minecraft:bedrock");
			AddMineCraftBlock(8, 0, "minecraft:flowing_water");
			AddMineCraftBlock(9, 0, "minecraft:still_water");
			AddMineCraftBlock(10, 0, "minecraft:flowing_lava");
			AddMineCraftBlock(11, 0, "minecraft:still_lava");
			AddMineCraftBlock(12, 0, "minecraft:sand");
			AddMineCraftBlock(12, 1, "minecraft:red_sand");
			AddMineCraftBlock(13, 0, "minecraft:gravel");
			AddMineCraftBlock(14, 0, "minecraft:gold_ore");
			AddMineCraftBlock(15, 0, "minecraft:iron_ore");
			AddMineCraftBlock(16, 0, "minecraft:coal_ore");
			AddMineCraftBlock(17, 0, "minecraft:oak_wood");
			AddMineCraftBlock(17, 1, "minecraft:spruce_wood");
			AddMineCraftBlock(17, 2, "minecraft:birch_wood");
			AddMineCraftBlock(17, 3, "minecraft:jungle_wood");
			AddMineCraftBlock(18, 0, "minecraft:oak_leaves");
			AddMineCraftBlock(18, 1, "minecraft:spruce_leaves");
			AddMineCraftBlock(18, 2, "minecraft:birch_leaves");
			AddMineCraftBlock(18, 3, "minecraft:jungle_leaves");
			AddMineCraftBlock(19, 0, "minecraft:sponge");
			AddMineCraftBlock(19, 1, "minecraft:wet_sponge");
			AddMineCraftBlock(20, 0, "minecraft:glass");
			AddMineCraftBlock(21, 0, "minecraft:lapis_lazuli_ore");
			AddMineCraftBlock(22, 0, "minecraft:lapis_lazuli_block");
			AddMineCraftBlock(23, 0, "minecraft:dispenser");
			AddMineCraftBlock(24, 0, "minecraft:sandstone");
			AddMineCraftBlock(24, 1, "minecraft:chiseled_sandstone");
			AddMineCraftBlock(24, 2, "minecraft:smooth_sandstone");
			AddMineCraftBlock(25, 0, "minecraft:note_block");
			AddMineCraftBlock(26, 0, "minecraft:bed");
			AddMineCraftBlock(27, 0, "minecraft:powered_rail");
			AddMineCraftBlock(28, 0, "minecraft:detector_rail");
			AddMineCraftBlock(29, 0, "minecraft:sticky_piston");
			AddMineCraftBlock(30, 0, "minecraft:cobweb");
			AddMineCraftBlock(31, 0, "minecraft:dead_shrub");
			AddMineCraftBlock(31, 1, "minecraft:grass");
			AddMineCraftBlock(31, 2, "minecraft:fern");
			AddMineCraftBlock(32, 0, "minecraft:dead_bush");
			AddMineCraftBlock(33, 0, "minecraft:piston");
			AddMineCraftBlock(34, 0, "minecraft:piston_head");
			AddMineCraftBlock(35, 0, "minecraft:white_wool");
			AddMineCraftBlock(35, 1, "minecraft:orange_wool");
			AddMineCraftBlock(35, 2, "minecraft:magenta_wool");
			AddMineCraftBlock(35, 3, "minecraft:light_blue_wool");
			AddMineCraftBlock(35, 4, "minecraft:yellow_wool");
			AddMineCraftBlock(35, 5, "minecraft:lime_wool");
			AddMineCraftBlock(35, 6, "minecraft:pink_wool");
			AddMineCraftBlock(35, 7, "minecraft:gray_wool");
			AddMineCraftBlock(35, 8, "minecraft:light_gray_wool");
			AddMineCraftBlock(35, 9, "minecraft:cyan_wool");
			AddMineCraftBlock(35, 10, "minecraft:purple_wool");
			AddMineCraftBlock(35, 11, "minecraft:blue_wool");
			AddMineCraftBlock(35, 12, "minecraft:brown_wool");
			AddMineCraftBlock(35, 13, "minecraft:green_wool");
			AddMineCraftBlock(35, 14, "minecraft:red_wool");
			AddMineCraftBlock(35, 15, "minecraft:black_wool");
			AddMineCraftBlock(37, 0, "minecraft:dandelion");
			AddMineCraftBlock(38, 0, "minecraft:poppy");
			AddMineCraftBlock(38, 1, "minecraft:blue_orchid");
			AddMineCraftBlock(38, 2, "minecraft:allium");
			AddMineCraftBlock(38, 3, "minecraft:azure_bluet");
			AddMineCraftBlock(38, 4, "minecraft:red_tulip");
			AddMineCraftBlock(38, 5, "minecraft:orange_tulip");
			AddMineCraftBlock(38, 6, "minecraft:white_tulip");
			AddMineCraftBlock(38, 7, "minecraft:pink_tulip");
			AddMineCraftBlock(38, 8, "minecraft:oxeye_daisy");
			AddMineCraftBlock(39, 0, "minecraft:brown_mushroom");
			AddMineCraftBlock(40, 0, "minecraft:red_mushroom");
			AddMineCraftBlock(41, 0, "minecraft:gold_block");
			AddMineCraftBlock(42, 0, "minecraft:iron_block");
			AddMineCraftBlock(43, 0, "minecraft:double_stone_slab");
			AddMineCraftBlock(43, 1, "minecraft:double_sandstone_slab");
			AddMineCraftBlock(43, 2, "minecraft:double_wooden_slab");
			AddMineCraftBlock(43, 3, "minecraft:double_cobblestone_slab");
			AddMineCraftBlock(43, 4, "minecraft:double_brick_slab");
			AddMineCraftBlock(43, 5, "minecraft:double_stone_brick_slab");
			AddMineCraftBlock(43, 6, "minecraft:double_nether_brick_slab");
			AddMineCraftBlock(43, 7, "minecraft:double_quartz_slab");
			AddMineCraftBlock(44, 0, "minecraft:stone_slab");
			AddMineCraftBlock(44, 1, "minecraft:sandstone_slab");
			AddMineCraftBlock(44, 2, "minecraft:wooden_slab");
			AddMineCraftBlock(44, 3, "minecraft:cobblestone_slab");
			AddMineCraftBlock(44, 4, "minecraft:brick_slab");
			AddMineCraftBlock(44, 5, "minecraft:stone_brick_slab");
			AddMineCraftBlock(44, 6, "minecraft:nether_brick_slab");
			AddMineCraftBlock(44, 7, "minecraft:quartz_slab");
			AddMineCraftBlock(45, 0, "minecraft:bricks");
			AddMineCraftBlock(46, 0, "minecraft:tnt");
			AddMineCraftBlock(47, 0, "minecraft:bookshelf");
			AddMineCraftBlock(48, 0, "minecraft:moss_stone");
			AddMineCraftBlock(49, 0, "minecraft:obsidian");
			AddMineCraftBlock(50, 0, "minecraft:torch");
			AddMineCraftBlock(51, 0, "minecraft:fire");
			AddMineCraftBlock(52, 0, "minecraft:monster_spawner");
			AddMineCraftBlock(53, 0, "minecraft:oak_wood_stairs");
			AddMineCraftBlock(54, 0, "minecraft:chest");
			AddMineCraftBlock(55, 0, "minecraft:redstone_wire");
			AddMineCraftBlock(56, 0, "minecraft:diamond_ore");
			AddMineCraftBlock(57, 0, "minecraft:diamond_block");
			AddMineCraftBlock(58, 0, "minecraft:crafting_table");
			AddMineCraftBlock(59, 0, "minecraft:wheat_crops");
			AddMineCraftBlock(60, 0, "minecraft:farmland");
			AddMineCraftBlock(61, 0, "minecraft:furnace");
			AddMineCraftBlock(62, 0, "minecraft:burning_furnace");
			AddMineCraftBlock(63, 0, "minecraft:standing_sign_block");
			AddMineCraftBlock(64, 0, "minecraft:oak_door_block");
			AddMineCraftBlock(65, 0, "minecraft:ladder");
			AddMineCraftBlock(66, 0, "minecraft:rail");
			AddMineCraftBlock(67, 0, "minecraft:cobblestone_stairs");
			AddMineCraftBlock(68, 0, "minecraft:wall-mounted_sign_block");
			AddMineCraftBlock(69, 0, "minecraft:lever");
			AddMineCraftBlock(70, 0, "minecraft:stone_pressure_plate");
			AddMineCraftBlock(71, 0, "minecraft:iron_door_block");
			AddMineCraftBlock(72, 0, "minecraft:wooden_pressure_plate");
			AddMineCraftBlock(73, 0, "minecraft:redstone_ore");
			AddMineCraftBlock(74, 0, "minecraft:glowing_redstone_ore");
			AddMineCraftBlock(75, 0, "minecraft:redstone_torch_(off)");
			AddMineCraftBlock(76, 0, "minecraft:redstone_torch_(on)");
			AddMineCraftBlock(77, 0, "minecraft:stone_button");
			AddMineCraftBlock(78, 0, "minecraft:snow");
			AddMineCraftBlock(79, 0, "minecraft:ice");
			AddMineCraftBlock(80, 0, "minecraft:snow_block");
			AddMineCraftBlock(81, 0, "minecraft:cactus");
			AddMineCraftBlock(82, 0, "minecraft:clay");
			AddMineCraftBlock(83, 0, "minecraft:sugar_canes");
			AddMineCraftBlock(84, 0, "minecraft:jukebox");
			AddMineCraftBlock(85, 0, "minecraft:oak_fence");
			AddMineCraftBlock(86, 0, "minecraft:pumpkin");
			AddMineCraftBlock(87, 0, "minecraft:netherrack");
			AddMineCraftBlock(88, 0, "minecraft:soul_sand");
			AddMineCraftBlock(89, 0, "minecraft:glowstone");
			AddMineCraftBlock(90, 0, "minecraft:nether_portal");
			AddMineCraftBlock(91, 0, "minecraft:jack_o'lantern");
			AddMineCraftBlock(92, 0, "minecraft:cake_block");
			AddMineCraftBlock(93, 0, "minecraft:redstone_repeater_block_(off)");
			AddMineCraftBlock(94, 0, "minecraft:redstone_repeater_block_(on)");
			AddMineCraftBlock(95, 0, "minecraft:white_stained_glass");
			AddMineCraftBlock(95, 1, "minecraft:orange_stained_glass");
			AddMineCraftBlock(95, 2, "minecraft:magenta_stained_glass");
			AddMineCraftBlock(95, 3, "minecraft:light_blue_stained_glass");
			AddMineCraftBlock(95, 4, "minecraft:yellow_stained_glass");
			AddMineCraftBlock(95, 5, "minecraft:lime_stained_glass");
			AddMineCraftBlock(95, 6, "minecraft:pink_stained_glass");
			AddMineCraftBlock(95, 7, "minecraft:gray_stained_glass");
			AddMineCraftBlock(95, 8, "minecraft:light_gray_stained_glass");
			AddMineCraftBlock(95, 9, "minecraft:cyan_stained_glass");
			AddMineCraftBlock(95, 10, "minecraft:purple_stained_glass");
			AddMineCraftBlock(95, 11, "minecraft:blue_stained_glass");
			AddMineCraftBlock(95, 12, "minecraft:brown_stained_glass");
			AddMineCraftBlock(95, 13, "minecraft:green_stained_glass");
			AddMineCraftBlock(95, 14, "minecraft:red_stained_glass");
			AddMineCraftBlock(95, 15, "minecraft:black_stained_glass");
			AddMineCraftBlock(96, 0, "minecraft:wooden_trapdoor");
			AddMineCraftBlock(97, 0, "minecraft:stone_monster_egg");
			AddMineCraftBlock(97, 1, "minecraft:cobblestone_monster_egg");
			AddMineCraftBlock(97, 2, "minecraft:stone_brick_monster_egg");
			AddMineCraftBlock(97, 3, "minecraft:mossy_stone_brick_monster_egg");
			AddMineCraftBlock(97, 4, "minecraft:cracked_stone_brick_monster_egg");
			AddMineCraftBlock(97, 5, "minecraft:chiseled_stone_brick_monster_egg");
			AddMineCraftBlock(98, 0, "minecraft:stone_bricks");
			AddMineCraftBlock(98, 1, "minecraft:mossy_stone_bricks");
			AddMineCraftBlock(98, 2, "minecraft:cracked_stone_bricks");
			AddMineCraftBlock(98, 3, "minecraft:chiseled_stone_bricks");
			AddMineCraftBlock(99, 0, "minecraft:brown_mushroom_block");
			AddMineCraftBlock(100, 0, "minecraft:red_mushroom_block");
			AddMineCraftBlock(101, 0, "minecraft:iron_bars");
			AddMineCraftBlock(102, 0, "minecraft:glass_pane");
			AddMineCraftBlock(103, 0, "minecraft:melon_block");
			AddMineCraftBlock(104, 0, "minecraft:pumpkin_stem");
			AddMineCraftBlock(105, 0, "minecraft:melon_stem");
			AddMineCraftBlock(106, 0, "minecraft:vines");
			AddMineCraftBlock(107, 0, "minecraft:oak_fence_gate");
			AddMineCraftBlock(108, 0, "minecraft:brick_stairs");
			AddMineCraftBlock(109, 0, "minecraft:stone_brick_stairs");
			AddMineCraftBlock(110, 0, "minecraft:mycelium");
			AddMineCraftBlock(111, 0, "minecraft:lily_pad");
			AddMineCraftBlock(112, 0, "minecraft:nether_brick");
			AddMineCraftBlock(113, 0, "minecraft:nether_brick_fence");
			AddMineCraftBlock(114, 0, "minecraft:nether_brick_stairs");
			AddMineCraftBlock(115, 0, "minecraft:nether_wart");
			AddMineCraftBlock(116, 0, "minecraft:enchantment_table");
			AddMineCraftBlock(117, 0, "minecraft:brewing_stand");
			AddMineCraftBlock(118, 0, "minecraft:cauldron");
			AddMineCraftBlock(119, 0, "minecraft:end_portal");
			AddMineCraftBlock(120, 0, "minecraft:end_portal_frame");
			AddMineCraftBlock(121, 0, "minecraft:end_stone");
			AddMineCraftBlock(122, 0, "minecraft:dragon_egg");
			AddMineCraftBlock(123, 0, "minecraft:redstone_lamp_(inactive)");
			AddMineCraftBlock(124, 0, "minecraft:redstone_lamp_(active)");
			AddMineCraftBlock(125, 0, "minecraft:double_oak_wood_slab");
			AddMineCraftBlock(125, 1, "minecraft:double_spruce_wood_slab");
			AddMineCraftBlock(125, 2, "minecraft:double_birch_wood_slab");
			AddMineCraftBlock(125, 3, "minecraft:double_jungle_wood_slab");
			AddMineCraftBlock(125, 4, "minecraft:double_acacia_wood_slab");
			AddMineCraftBlock(125, 5, "minecraft:double_dark_oak_wood_slab");
			AddMineCraftBlock(126, 0, "minecraft:oak_wood_slab");
			AddMineCraftBlock(126, 1, "minecraft:spruce_wood_slab");
			AddMineCraftBlock(126, 2, "minecraft:birch_wood_slab");
			AddMineCraftBlock(126, 3, "minecraft:jungle_wood_slab");
			AddMineCraftBlock(126, 4, "minecraft:acacia_wood_slab");
			AddMineCraftBlock(126, 5, "minecraft:dark_oak_wood_slab");
			AddMineCraftBlock(127, 0, "minecraft:cocoa");
			AddMineCraftBlock(128, 0, "minecraft:sandstone_stairs");
			AddMineCraftBlock(129, 0, "minecraft:emerald_ore");
			AddMineCraftBlock(130, 0, "minecraft:ender_chest");
			AddMineCraftBlock(131, 0, "minecraft:tripwire_hook");
			AddMineCraftBlock(132, 0, "minecraft:tripwire");
			AddMineCraftBlock(133, 0, "minecraft:emerald_block");
			AddMineCraftBlock(134, 0, "minecraft:spruce_wood_stairs");
			AddMineCraftBlock(135, 0, "minecraft:birch_wood_stairs");
			AddMineCraftBlock(136, 0, "minecraft:jungle_wood_stairs");
			AddMineCraftBlock(137, 0, "minecraft:command_block");
			AddMineCraftBlock(138, 0, "minecraft:beacon");
			AddMineCraftBlock(139, 0, "minecraft:cobblestone_wall");
			AddMineCraftBlock(139, 1, "minecraft:mossy_cobblestone_wall");
			AddMineCraftBlock(140, 0, "minecraft:flower_pot");
			AddMineCraftBlock(141, 0, "minecraft:carrots");
			AddMineCraftBlock(142, 0, "minecraft:potatoes");
			AddMineCraftBlock(143, 0, "minecraft:wooden_button");
			AddMineCraftBlock(144, 0, "minecraft:mob_head");
			AddMineCraftBlock(145, 0, "minecraft:anvil");
			AddMineCraftBlock(146, 0, "minecraft:trapped_chest");
			AddMineCraftBlock(147, 0, "minecraft:weighted_pressure_plate_(light)");
			AddMineCraftBlock(148, 0, "minecraft:weighted_pressure_plate_(heavy)");
			AddMineCraftBlock(149, 0, "minecraft:redstone_comparator_(inactive)");
			AddMineCraftBlock(150, 0, "minecraft:redstone_comparator_(active)");
			AddMineCraftBlock(151, 0, "minecraft:daylight_sensor");
			AddMineCraftBlock(152, 0, "minecraft:redstone_block");
			AddMineCraftBlock(153, 0, "minecraft:nether_quartz_ore");
			AddMineCraftBlock(154, 0, "minecraft:hopper");
			AddMineCraftBlock(155, 0, "minecraft:quartz_block");
			AddMineCraftBlock(155, 1, "minecraft:chiseled_quartz_block");
			AddMineCraftBlock(155, 2, "minecraft:pillar_quartz_block");
			AddMineCraftBlock(156, 0, "minecraft:quartz_stairs");
			AddMineCraftBlock(157, 0, "minecraft:activator_rail");
			AddMineCraftBlock(158, 0, "minecraft:dropper");
			AddMineCraftBlock(159, 0, "minecraft:white_hardened_clay");
			AddMineCraftBlock(159, 1, "minecraft:orange_hardened_clay");
			AddMineCraftBlock(159, 2, "minecraft:magenta_hardened_clay");
			AddMineCraftBlock(159, 3, "minecraft:light_blue_hardened_clay");
			AddMineCraftBlock(159, 4, "minecraft:yellow_hardened_clay");
			AddMineCraftBlock(159, 5, "minecraft:lime_hardened_clay");
			AddMineCraftBlock(159, 6, "minecraft:pink_hardened_clay");
			AddMineCraftBlock(159, 7, "minecraft:gray_hardened_clay");
			AddMineCraftBlock(159, 8, "minecraft:light_gray_hardened_clay");
			AddMineCraftBlock(159, 9, "minecraft:cyan_hardened_clay");
			AddMineCraftBlock(159, 10, "minecraft:purple_hardened_clay");
			AddMineCraftBlock(159, 11, "minecraft:blue_hardened_clay");
			AddMineCraftBlock(159, 12, "minecraft:brown_hardened_clay");
			AddMineCraftBlock(159, 13, "minecraft:green_hardened_clay");
			AddMineCraftBlock(159, 14, "minecraft:red_hardened_clay");
			AddMineCraftBlock(159, 15, "minecraft:black_hardened_clay");
			AddMineCraftBlock(160, 0, "minecraft:white_stained_glass_pane");
			AddMineCraftBlock(160, 1, "minecraft:orange_stained_glass_pane");
			AddMineCraftBlock(160, 2, "minecraft:magenta_stained_glass_pane");
			AddMineCraftBlock(160, 3, "minecraft:light_blue_stained_glass_pane");
			AddMineCraftBlock(160, 4, "minecraft:yellow_stained_glass_pane");
			AddMineCraftBlock(160, 5, "minecraft:lime_stained_glass_pane");
			AddMineCraftBlock(160, 6, "minecraft:pink_stained_glass_pane");
			AddMineCraftBlock(160, 7, "minecraft:gray_stained_glass_pane");
			AddMineCraftBlock(160, 8, "minecraft:light_gray_stained_glass_pane");
			AddMineCraftBlock(160, 9, "minecraft:cyan_stained_glass_pane");
			AddMineCraftBlock(160, 10, "minecraft:purple_stained_glass_pane");
			AddMineCraftBlock(160, 11, "minecraft:blue_stained_glass_pane");
			AddMineCraftBlock(160, 12, "minecraft:brown_stained_glass_pane");
			AddMineCraftBlock(160, 13, "minecraft:green_stained_glass_pane");
			AddMineCraftBlock(160, 14, "minecraft:red_stained_glass_pane");
			AddMineCraftBlock(160, 15, "minecraft:black_stained_glass_pane");
			AddMineCraftBlock(161, 0, "minecraft:acacia_leaves");
			AddMineCraftBlock(161, 1, "minecraft:dark_oak_leaves");
			AddMineCraftBlock(162, 0, "minecraft:acacia_wood");
			AddMineCraftBlock(162, 1, "minecraft:dark_oak_wood");
			AddMineCraftBlock(163, 0, "minecraft:acacia_wood_stairs");
			AddMineCraftBlock(164, 0, "minecraft:dark_oak_wood_stairs");
			AddMineCraftBlock(165, 0, "minecraft:slime_block");
			AddMineCraftBlock(166, 0, "minecraft:barrier");
			AddMineCraftBlock(167, 0, "minecraft:iron_trapdoor");
			AddMineCraftBlock(168, 0, "minecraft:prismarine");
			AddMineCraftBlock(168, 1, "minecraft:prismarine_bricks");
			AddMineCraftBlock(168, 2, "minecraft:dark_prismarine");
			AddMineCraftBlock(169, 0, "minecraft:sea_lantern");
			AddMineCraftBlock(170, 0, "minecraft:hay_bale");
			AddMineCraftBlock(171, 0, "minecraft:white_carpet");
			AddMineCraftBlock(171, 1, "minecraft:orange_carpet");
			AddMineCraftBlock(171, 2, "minecraft:magenta_carpet");
			AddMineCraftBlock(171, 3, "minecraft:light_blue_carpet");
			AddMineCraftBlock(171, 4, "minecraft:yellow_carpet");
			AddMineCraftBlock(171, 5, "minecraft:lime_carpet");
			AddMineCraftBlock(171, 6, "minecraft:pink_carpet");
			AddMineCraftBlock(171, 7, "minecraft:gray_carpet");
			AddMineCraftBlock(171, 8, "minecraft:light_gray_carpet");
			AddMineCraftBlock(171, 9, "minecraft:cyan_carpet");
			AddMineCraftBlock(171, 10, "minecraft:purple_carpet");
			AddMineCraftBlock(171, 11, "minecraft:blue_carpet");
			AddMineCraftBlock(171, 12, "minecraft:brown_carpet");
			AddMineCraftBlock(171, 13, "minecraft:green_carpet");
			AddMineCraftBlock(171, 14, "minecraft:red_carpet");
			AddMineCraftBlock(171, 15, "minecraft:black_carpet");
			AddMineCraftBlock(172, 0, "minecraft:hardened_clay");
			AddMineCraftBlock(173, 0, "minecraft:block_of_coal");
			AddMineCraftBlock(174, 0, "minecraft:packed_ice");
			AddMineCraftBlock(175, 0, "minecraft:sunflower");
			AddMineCraftBlock(175, 1, "minecraft:lilac");
			AddMineCraftBlock(175, 2, "minecraft:double_tallgrass");
			AddMineCraftBlock(175, 3, "minecraft:large_fern");
			AddMineCraftBlock(175, 4, "minecraft:rose_bush");
			AddMineCraftBlock(175, 5, "minecraft:peony");
			AddMineCraftBlock(176, 0, "minecraft:free-standing_banner");
			AddMineCraftBlock(177, 0, "minecraft:wall-mounted_banner");
			AddMineCraftBlock(178, 0, "minecraft:inverted_daylight_sensor");
			AddMineCraftBlock(179, 0, "minecraft:red_sandstone");
			AddMineCraftBlock(179, 1, "minecraft:chiseled_red_sandstone");
			AddMineCraftBlock(179, 2, "minecraft:smooth_red_sandstone");
			AddMineCraftBlock(180, 0, "minecraft:red_sandstone_stairs");
			AddMineCraftBlock(181, 0, "minecraft:double_red_sandstone_slab");
			AddMineCraftBlock(182, 0, "minecraft:red_sandstone_slab");
			AddMineCraftBlock(183, 0, "minecraft:spruce_fence_gate");
			AddMineCraftBlock(184, 0, "minecraft:birch_fence_gate");
			AddMineCraftBlock(185, 0, "minecraft:jungle_fence_gate");
			AddMineCraftBlock(186, 0, "minecraft:dark_oak_fence_gate");
			AddMineCraftBlock(187, 0, "minecraft:acacia_fence_gate");
			AddMineCraftBlock(188, 0, "minecraft:spruce_fence");
			AddMineCraftBlock(189, 0, "minecraft:birch_fence");
			AddMineCraftBlock(190, 0, "minecraft:jungle_fence");
			AddMineCraftBlock(191, 0, "minecraft:dark_oak_fence");
			AddMineCraftBlock(192, 0, "minecraft:acacia_fence");
			AddMineCraftBlock(193, 0, "minecraft:spruce_door_block");
			AddMineCraftBlock(194, 0, "minecraft:birch_door_block");
			AddMineCraftBlock(195, 0, "minecraft:jungle_door_block");
			AddMineCraftBlock(196, 0, "minecraft:acacia_door_block");
			AddMineCraftBlock(197, 0, "minecraft:dark_oak_door_block");
			AddMineCraftBlock(198, 0, "minecraft:end_rod");
			AddMineCraftBlock(199, 0, "minecraft:chorus_plant");


			for (int i = 0; i < 512; ++i) {
				s_solid_block_map[i] = true;
			}

			s_solid_block_map[0] = false;
			s_solid_block_map[6] = false;
			s_solid_block_map[18] = false;
			s_solid_block_map[19] = false;

			s_solid_block_map[30] = false;
			s_solid_block_map[31] = false;
			s_solid_block_map[32] = false;
			s_solid_block_map[37] = false;
			s_solid_block_map[38] = false;
			s_solid_block_map[39] = false;
			s_solid_block_map[40] = false;
			s_solid_block_map[59] = false;

			s_solid_block_map[65] = false;
			s_solid_block_map[66] = false;
			s_solid_block_map[81] = false;
			s_solid_block_map[83] = false;
			s_solid_block_map[85] = false;

			s_solid_block_map[50] = false; // torch
			s_solid_block_map[51] = false;
			s_solid_block_map[52] = false;
			s_solid_block_map[53] = false;
			s_solid_block_map[67] = false;
			s_solid_block_map[69] = false;
			s_solid_block_map[85] = false;
			s_solid_block_map[20] = false;  // glass
			s_solid_block_map[65] = false;
			s_solid_block_map[66] = false;
			s_solid_block_map[63] = false;
			s_solid_block_map[64] = false;
			s_solid_block_map[67] = false;
			s_solid_block_map[68] = false;
			s_solid_block_map[69] = false;

			s_solid_block_map[71] = false;
			s_solid_block_map[75] = false;
			s_solid_block_map[76] = false;
			s_solid_block_map[79] = false;

			s_solid_block_map[101] = false;
			s_solid_block_map[102] = false;
			s_solid_block_map[106] = false;
			s_solid_block_map[107] = false;
			s_solid_block_map[108] = false;
			s_solid_block_map[109] = false;

			s_solid_block_map[113] = false;
			s_solid_block_map[114] = false;
			s_solid_block_map[127] = false;
			s_solid_block_map[128] = false;

			s_solid_block_map[134] = false;
			s_solid_block_map[135] = false;
			s_solid_block_map[136] = false;
			s_solid_block_map[138] = false;
			s_solid_block_map[156] = false;
			s_solid_block_map[157] = false;
		}

		bool IsSolidBlock(int block_id)
		{
			if (block_id >= 0 && block_id < 512) return s_solid_block_map[block_id];
			return true;
		}
	}

}


