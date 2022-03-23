#include <iostream>

#include "mc/section.h"
#include "mc/nbt.h"

namespace mapcrafter {
	namespace mc {
		static std::map<std::string, uint16_t> s_block_name_id_map = {
			{"minecraft:air", 0},
			{"minecraft:sandstone", 1},
			{"minecraft:bedrock", 2},
		};

		static std::map<std::string, uint16_t> s_block_name_data_map = {
			{"minecraft:air", 0},
			{"minecraft:sandstone", 1},
			{"minecraft:bedrock", 2},
		};

		static void AddMinecraftBlock(int id, int data, std::string name)
		{
			s_block_name_id_map[name] = id;
			s_block_name_data_map[name] = data;
		}

		static void InitBlockMap()
		{
			static bool inited = false;
			if (inited) return;
			inited = true;

			AddMinecraftBlock(0, 0, "minecraft:air");
			AddMinecraftBlock(1, 0, "minecraft:stone");
			AddMinecraftBlock(1, 1, "minecraft:granite");
			AddMinecraftBlock(1, 2, "minecraft:polished_granite");
			AddMinecraftBlock(1, 3, "minecraft:diorite");
			AddMinecraftBlock(1, 4, "minecraft:polished_diorite");
			AddMinecraftBlock(1, 5, "minecraft:andesite");
			AddMinecraftBlock(1, 6, "minecraft:polished_andesite");
			AddMinecraftBlock(2, 0, "minecraft:grass");
			AddMinecraftBlock(3, 0, "minecraft:dirt");
			AddMinecraftBlock(3, 1, "minecraft:coarse_dirt");
			AddMinecraftBlock(3, 2, "minecraft:podzol");
			AddMinecraftBlock(4, 0, "minecraft:cobblestone");
			AddMinecraftBlock(5, 0, "minecraft:oak_wood_plank");
			AddMinecraftBlock(5, 1, "minecraft:spruce_wood_plank");
			AddMinecraftBlock(5, 2, "minecraft:birch_wood_plank");
			AddMinecraftBlock(5, 3, "minecraft:jungle_wood_plank");
			AddMinecraftBlock(5, 4, "minecraft:acacia_wood_plank");
			AddMinecraftBlock(5, 5, "minecraft:dark_oak_wood_plank");
			AddMinecraftBlock(6, 0, "minecraft:oak_sapling");
			AddMinecraftBlock(6, 1, "minecraft:spruce_sapling");
			AddMinecraftBlock(6, 2, "minecraft:birch_sapling");
			AddMinecraftBlock(6, 3, "minecraft:jungle_sapling");
			AddMinecraftBlock(6, 4, "minecraft:acacia_sapling");
			AddMinecraftBlock(6, 5, "minecraft:dark_oak_sapling");
			AddMinecraftBlock(7, 0, "minecraft:bedrock");
			AddMinecraftBlock(8, 0, "minecraft:flowing_water");
			AddMinecraftBlock(9, 0, "minecraft:still_water");
			AddMinecraftBlock(10, 0, "minecraft:flowing_lava");
			AddMinecraftBlock(11, 0, "minecraft:still_lava");
			AddMinecraftBlock(12, 0, "minecraft:sand");
			AddMinecraftBlock(12, 1, "minecraft:red_sand");
			AddMinecraftBlock(13, 0, "minecraft:gravel");
			AddMinecraftBlock(14, 0, "minecraft:gold_ore");
			AddMinecraftBlock(15, 0, "minecraft:iron_ore");
			AddMinecraftBlock(16, 0, "minecraft:coal_ore");
			AddMinecraftBlock(17, 0, "minecraft:oak_wood");
			AddMinecraftBlock(17, 1, "minecraft:spruce_wood");
			AddMinecraftBlock(17, 2, "minecraft:birch_wood");
			AddMinecraftBlock(17, 3, "minecraft:jungle_wood");
			AddMinecraftBlock(18, 0, "minecraft:oak_leaves");
			AddMinecraftBlock(18, 1, "minecraft:spruce_leaves");
			AddMinecraftBlock(18, 2, "minecraft:birch_leaves");
			AddMinecraftBlock(18, 3, "minecraft:jungle_leaves");
			AddMinecraftBlock(19, 0, "minecraft:sponge");
			AddMinecraftBlock(19, 1, "minecraft:wet_sponge");
			AddMinecraftBlock(20, 0, "minecraft:glass");
			AddMinecraftBlock(21, 0, "minecraft:lapis_lazuli_ore");
			AddMinecraftBlock(22, 0, "minecraft:lapis_lazuli_block");
			AddMinecraftBlock(23, 0, "minecraft:dispenser");
			AddMinecraftBlock(24, 0, "minecraft:sandstone");
			AddMinecraftBlock(24, 1, "minecraft:chiseled_sandstone");
			AddMinecraftBlock(24, 2, "minecraft:smooth_sandstone");
			AddMinecraftBlock(25, 0, "minecraft:note_block");
			AddMinecraftBlock(26, 0, "minecraft:bed");
			AddMinecraftBlock(27, 0, "minecraft:powered_rail");
			AddMinecraftBlock(28, 0, "minecraft:detector_rail");
			AddMinecraftBlock(29, 0, "minecraft:sticky_piston");
			AddMinecraftBlock(30, 0, "minecraft:cobweb");
			AddMinecraftBlock(31, 0, "minecraft:dead_shrub");
			AddMinecraftBlock(31, 1, "minecraft:grass");
			AddMinecraftBlock(31, 2, "minecraft:fern");
			AddMinecraftBlock(32, 0, "minecraft:dead_bush");
			AddMinecraftBlock(33, 0, "minecraft:piston");
			AddMinecraftBlock(34, 0, "minecraft:piston_head");
			AddMinecraftBlock(35, 0, "minecraft:white_wool");
			AddMinecraftBlock(35, 1, "minecraft:orange_wool");
			AddMinecraftBlock(35, 2, "minecraft:magenta_wool");
			AddMinecraftBlock(35, 3, "minecraft:light_blue_wool");
			AddMinecraftBlock(35, 4, "minecraft:yellow_wool");
			AddMinecraftBlock(35, 5, "minecraft:lime_wool");
			AddMinecraftBlock(35, 6, "minecraft:pink_wool");
			AddMinecraftBlock(35, 7, "minecraft:gray_wool");
			AddMinecraftBlock(35, 8, "minecraft:light_gray_wool");
			AddMinecraftBlock(35, 9, "minecraft:cyan_wool");
			AddMinecraftBlock(35, 10, "minecraft:purple_wool");
			AddMinecraftBlock(35, 11, "minecraft:blue_wool");
			AddMinecraftBlock(35, 12, "minecraft:brown_wool");
			AddMinecraftBlock(35, 13, "minecraft:green_wool");
			AddMinecraftBlock(35, 14, "minecraft:red_wool");
			AddMinecraftBlock(35, 15, "minecraft:black_wool");
			AddMinecraftBlock(37, 0, "minecraft:dandelion");
			AddMinecraftBlock(38, 0, "minecraft:poppy");
			AddMinecraftBlock(38, 1, "minecraft:blue_orchid");
			AddMinecraftBlock(38, 2, "minecraft:allium");
			AddMinecraftBlock(38, 3, "minecraft:azure_bluet");
			AddMinecraftBlock(38, 4, "minecraft:red_tulip");
			AddMinecraftBlock(38, 5, "minecraft:orange_tulip");
			AddMinecraftBlock(38, 6, "minecraft:white_tulip");
			AddMinecraftBlock(38, 7, "minecraft:pink_tulip");
			AddMinecraftBlock(38, 8, "minecraft:oxeye_daisy");
			AddMinecraftBlock(39, 0, "minecraft:brown_mushroom");
			AddMinecraftBlock(40, 0, "minecraft:red_mushroom");
			AddMinecraftBlock(41, 0, "minecraft:gold_block");
			AddMinecraftBlock(42, 0, "minecraft:iron_block");
			AddMinecraftBlock(43, 0, "minecraft:double_stone_slab");
			AddMinecraftBlock(43, 1, "minecraft:double_sandstone_slab");
			AddMinecraftBlock(43, 2, "minecraft:double_wooden_slab");
			AddMinecraftBlock(43, 3, "minecraft:double_cobblestone_slab");
			AddMinecraftBlock(43, 4, "minecraft:double_brick_slab");
			AddMinecraftBlock(43, 5, "minecraft:double_stone_brick_slab");
			AddMinecraftBlock(43, 6, "minecraft:double_nether_brick_slab");
			AddMinecraftBlock(43, 7, "minecraft:double_quartz_slab");
			AddMinecraftBlock(44, 0, "minecraft:stone_slab");
			AddMinecraftBlock(44, 1, "minecraft:sandstone_slab");
			AddMinecraftBlock(44, 2, "minecraft:wooden_slab");
			AddMinecraftBlock(44, 3, "minecraft:cobblestone_slab");
			AddMinecraftBlock(44, 4, "minecraft:brick_slab");
			AddMinecraftBlock(44, 5, "minecraft:stone_brick_slab");
			AddMinecraftBlock(44, 6, "minecraft:nether_brick_slab");
			AddMinecraftBlock(44, 7, "minecraft:quartz_slab");
			AddMinecraftBlock(45, 0, "minecraft:bricks");
			AddMinecraftBlock(46, 0, "minecraft:tnt");
			AddMinecraftBlock(47, 0, "minecraft:bookshelf");
			AddMinecraftBlock(48, 0, "minecraft:moss_stone");
			AddMinecraftBlock(49, 0, "minecraft:obsidian");
			AddMinecraftBlock(50, 0, "minecraft:torch");
			AddMinecraftBlock(51, 0, "minecraft:fire");
			AddMinecraftBlock(52, 0, "minecraft:monster_spawner");
			AddMinecraftBlock(53, 0, "minecraft:oak_wood_stairs");
			AddMinecraftBlock(54, 0, "minecraft:chest");
			AddMinecraftBlock(55, 0, "minecraft:redstone_wire");
			AddMinecraftBlock(56, 0, "minecraft:diamond_ore");
			AddMinecraftBlock(57, 0, "minecraft:diamond_block");
			AddMinecraftBlock(58, 0, "minecraft:crafting_table");
			AddMinecraftBlock(59, 0, "minecraft:wheat_crops");
			AddMinecraftBlock(60, 0, "minecraft:farmland");
			AddMinecraftBlock(61, 0, "minecraft:furnace");
			AddMinecraftBlock(62, 0, "minecraft:burning_furnace");
			AddMinecraftBlock(63, 0, "minecraft:standing_sign_block");
			AddMinecraftBlock(64, 0, "minecraft:oak_door_block");
			AddMinecraftBlock(65, 0, "minecraft:ladder");
			AddMinecraftBlock(66, 0, "minecraft:rail");
			AddMinecraftBlock(67, 0, "minecraft:cobblestone_stairs");
			AddMinecraftBlock(68, 0, "minecraft:wall-mounted_sign_block");
			AddMinecraftBlock(69, 0, "minecraft:lever");
			AddMinecraftBlock(70, 0, "minecraft:stone_pressure_plate");
			AddMinecraftBlock(71, 0, "minecraft:iron_door_block");
			AddMinecraftBlock(72, 0, "minecraft:wooden_pressure_plate");
			AddMinecraftBlock(73, 0, "minecraft:redstone_ore");
			AddMinecraftBlock(74, 0, "minecraft:glowing_redstone_ore");
			AddMinecraftBlock(75, 0, "minecraft:redstone_torch_(off)");
			AddMinecraftBlock(76, 0, "minecraft:redstone_torch_(on)");
			AddMinecraftBlock(77, 0, "minecraft:stone_button");
			AddMinecraftBlock(78, 0, "minecraft:snow");
			AddMinecraftBlock(79, 0, "minecraft:ice");
			AddMinecraftBlock(80, 0, "minecraft:snow_block");
			AddMinecraftBlock(81, 0, "minecraft:cactus");
			AddMinecraftBlock(82, 0, "minecraft:clay");
			AddMinecraftBlock(83, 0, "minecraft:sugar_canes");
			AddMinecraftBlock(84, 0, "minecraft:jukebox");
			AddMinecraftBlock(85, 0, "minecraft:oak_fence");
			AddMinecraftBlock(86, 0, "minecraft:pumpkin");
			AddMinecraftBlock(87, 0, "minecraft:netherrack");
			AddMinecraftBlock(88, 0, "minecraft:soul_sand");
			AddMinecraftBlock(89, 0, "minecraft:glowstone");
			AddMinecraftBlock(90, 0, "minecraft:nether_portal");
			AddMinecraftBlock(91, 0, "minecraft:jack_o'lantern");
			AddMinecraftBlock(92, 0, "minecraft:cake_block");
			AddMinecraftBlock(93, 0, "minecraft:redstone_repeater_block_(off)");
			AddMinecraftBlock(94, 0, "minecraft:redstone_repeater_block_(on)");
			AddMinecraftBlock(95, 0, "minecraft:white_stained_glass");
			AddMinecraftBlock(95, 1, "minecraft:orange_stained_glass");
			AddMinecraftBlock(95, 2, "minecraft:magenta_stained_glass");
			AddMinecraftBlock(95, 3, "minecraft:light_blue_stained_glass");
			AddMinecraftBlock(95, 4, "minecraft:yellow_stained_glass");
			AddMinecraftBlock(95, 5, "minecraft:lime_stained_glass");
			AddMinecraftBlock(95, 6, "minecraft:pink_stained_glass");
			AddMinecraftBlock(95, 7, "minecraft:gray_stained_glass");
			AddMinecraftBlock(95, 8, "minecraft:light_gray_stained_glass");
			AddMinecraftBlock(95, 9, "minecraft:cyan_stained_glass");
			AddMinecraftBlock(95, 10, "minecraft:purple_stained_glass");
			AddMinecraftBlock(95, 11, "minecraft:blue_stained_glass");
			AddMinecraftBlock(95, 12, "minecraft:brown_stained_glass");
			AddMinecraftBlock(95, 13, "minecraft:green_stained_glass");
			AddMinecraftBlock(95, 14, "minecraft:red_stained_glass");
			AddMinecraftBlock(95, 15, "minecraft:black_stained_glass");
			AddMinecraftBlock(96, 0, "minecraft:wooden_trapdoor");
			AddMinecraftBlock(97, 0, "minecraft:stone_monster_egg");
			AddMinecraftBlock(97, 1, "minecraft:cobblestone_monster_egg");
			AddMinecraftBlock(97, 2, "minecraft:stone_brick_monster_egg");
			AddMinecraftBlock(97, 3, "minecraft:mossy_stone_brick_monster_egg");
			AddMinecraftBlock(97, 4, "minecraft:cracked_stone_brick_monster_egg");
			AddMinecraftBlock(97, 5, "minecraft:chiseled_stone_brick_monster_egg");
			AddMinecraftBlock(98, 0, "minecraft:stone_bricks");
			AddMinecraftBlock(98, 1, "minecraft:mossy_stone_bricks");
			AddMinecraftBlock(98, 2, "minecraft:cracked_stone_bricks");
			AddMinecraftBlock(98, 3, "minecraft:chiseled_stone_bricks");
			AddMinecraftBlock(99, 0, "minecraft:brown_mushroom_block");
			AddMinecraftBlock(100, 0, "minecraft:red_mushroom_block");
			AddMinecraftBlock(101, 0, "minecraft:iron_bars");
			AddMinecraftBlock(102, 0, "minecraft:glass_pane");
			AddMinecraftBlock(103, 0, "minecraft:melon_block");
			AddMinecraftBlock(104, 0, "minecraft:pumpkin_stem");
			AddMinecraftBlock(105, 0, "minecraft:melon_stem");
			AddMinecraftBlock(106, 0, "minecraft:vines");
			AddMinecraftBlock(107, 0, "minecraft:oak_fence_gate");
			AddMinecraftBlock(108, 0, "minecraft:brick_stairs");
			AddMinecraftBlock(109, 0, "minecraft:stone_brick_stairs");
			AddMinecraftBlock(110, 0, "minecraft:mycelium");
			AddMinecraftBlock(111, 0, "minecraft:lily_pad");
			AddMinecraftBlock(112, 0, "minecraft:nether_brick");
			AddMinecraftBlock(113, 0, "minecraft:nether_brick_fence");
			AddMinecraftBlock(114, 0, "minecraft:nether_brick_stairs");
			AddMinecraftBlock(115, 0, "minecraft:nether_wart");
			AddMinecraftBlock(116, 0, "minecraft:enchantment_table");
			AddMinecraftBlock(117, 0, "minecraft:brewing_stand");
			AddMinecraftBlock(118, 0, "minecraft:cauldron");
			AddMinecraftBlock(119, 0, "minecraft:end_portal");
			AddMinecraftBlock(120, 0, "minecraft:end_portal_frame");
			AddMinecraftBlock(121, 0, "minecraft:end_stone");
			AddMinecraftBlock(122, 0, "minecraft:dragon_egg");
			AddMinecraftBlock(123, 0, "minecraft:redstone_lamp_(inactive)");
			AddMinecraftBlock(124, 0, "minecraft:redstone_lamp_(active)");
			AddMinecraftBlock(125, 0, "minecraft:double_oak_wood_slab");
			AddMinecraftBlock(125, 1, "minecraft:double_spruce_wood_slab");
			AddMinecraftBlock(125, 2, "minecraft:double_birch_wood_slab");
			AddMinecraftBlock(125, 3, "minecraft:double_jungle_wood_slab");
			AddMinecraftBlock(125, 4, "minecraft:double_acacia_wood_slab");
			AddMinecraftBlock(125, 5, "minecraft:double_dark_oak_wood_slab");
			AddMinecraftBlock(126, 0, "minecraft:oak_wood_slab");
			AddMinecraftBlock(126, 1, "minecraft:spruce_wood_slab");
			AddMinecraftBlock(126, 2, "minecraft:birch_wood_slab");
			AddMinecraftBlock(126, 3, "minecraft:jungle_wood_slab");
			AddMinecraftBlock(126, 4, "minecraft:acacia_wood_slab");
			AddMinecraftBlock(126, 5, "minecraft:dark_oak_wood_slab");
			AddMinecraftBlock(127, 0, "minecraft:cocoa");
			AddMinecraftBlock(128, 0, "minecraft:sandstone_stairs");
			AddMinecraftBlock(129, 0, "minecraft:emerald_ore");
			AddMinecraftBlock(130, 0, "minecraft:ender_chest");
			AddMinecraftBlock(131, 0, "minecraft:tripwire_hook");
			AddMinecraftBlock(132, 0, "minecraft:tripwire");
			AddMinecraftBlock(133, 0, "minecraft:emerald_block");
			AddMinecraftBlock(134, 0, "minecraft:spruce_wood_stairs");
			AddMinecraftBlock(135, 0, "minecraft:birch_wood_stairs");
			AddMinecraftBlock(136, 0, "minecraft:jungle_wood_stairs");
			AddMinecraftBlock(137, 0, "minecraft:command_block");
			AddMinecraftBlock(138, 0, "minecraft:beacon");
			AddMinecraftBlock(139, 0, "minecraft:cobblestone_wall");
			AddMinecraftBlock(139, 1, "minecraft:mossy_cobblestone_wall");
			AddMinecraftBlock(140, 0, "minecraft:flower_pot");
			AddMinecraftBlock(141, 0, "minecraft:carrots");
			AddMinecraftBlock(142, 0, "minecraft:potatoes");
			AddMinecraftBlock(143, 0, "minecraft:wooden_button");
			AddMinecraftBlock(144, 0, "minecraft:mob_head");
			AddMinecraftBlock(145, 0, "minecraft:anvil");
			AddMinecraftBlock(146, 0, "minecraft:trapped_chest");
			AddMinecraftBlock(147, 0, "minecraft:weighted_pressure_plate_(light)");
			AddMinecraftBlock(148, 0, "minecraft:weighted_pressure_plate_(heavy)");
			AddMinecraftBlock(149, 0, "minecraft:redstone_comparator_(inactive)");
			AddMinecraftBlock(150, 0, "minecraft:redstone_comparator_(active)");
			AddMinecraftBlock(151, 0, "minecraft:daylight_sensor");
			AddMinecraftBlock(152, 0, "minecraft:redstone_block");
			AddMinecraftBlock(153, 0, "minecraft:nether_quartz_ore");
			AddMinecraftBlock(154, 0, "minecraft:hopper");
			AddMinecraftBlock(155, 0, "minecraft:quartz_block");
			AddMinecraftBlock(155, 1, "minecraft:chiseled_quartz_block");
			AddMinecraftBlock(155, 2, "minecraft:pillar_quartz_block");
			AddMinecraftBlock(156, 0, "minecraft:quartz_stairs");
			AddMinecraftBlock(157, 0, "minecraft:activator_rail");
			AddMinecraftBlock(158, 0, "minecraft:dropper");
			AddMinecraftBlock(159, 0, "minecraft:white_hardened_clay");
			AddMinecraftBlock(159, 1, "minecraft:orange_hardened_clay");
			AddMinecraftBlock(159, 2, "minecraft:magenta_hardened_clay");
			AddMinecraftBlock(159, 3, "minecraft:light_blue_hardened_clay");
			AddMinecraftBlock(159, 4, "minecraft:yellow_hardened_clay");
			AddMinecraftBlock(159, 5, "minecraft:lime_hardened_clay");
			AddMinecraftBlock(159, 6, "minecraft:pink_hardened_clay");
			AddMinecraftBlock(159, 7, "minecraft:gray_hardened_clay");
			AddMinecraftBlock(159, 8, "minecraft:light_gray_hardened_clay");
			AddMinecraftBlock(159, 9, "minecraft:cyan_hardened_clay");
			AddMinecraftBlock(159, 10, "minecraft:purple_hardened_clay");
			AddMinecraftBlock(159, 11, "minecraft:blue_hardened_clay");
			AddMinecraftBlock(159, 12, "minecraft:brown_hardened_clay");
			AddMinecraftBlock(159, 13, "minecraft:green_hardened_clay");
			AddMinecraftBlock(159, 14, "minecraft:red_hardened_clay");
			AddMinecraftBlock(159, 15, "minecraft:black_hardened_clay");
			AddMinecraftBlock(160, 0, "minecraft:white_stained_glass_pane");
			AddMinecraftBlock(160, 1, "minecraft:orange_stained_glass_pane");
			AddMinecraftBlock(160, 2, "minecraft:magenta_stained_glass_pane");
			AddMinecraftBlock(160, 3, "minecraft:light_blue_stained_glass_pane");
			AddMinecraftBlock(160, 4, "minecraft:yellow_stained_glass_pane");
			AddMinecraftBlock(160, 5, "minecraft:lime_stained_glass_pane");
			AddMinecraftBlock(160, 6, "minecraft:pink_stained_glass_pane");
			AddMinecraftBlock(160, 7, "minecraft:gray_stained_glass_pane");
			AddMinecraftBlock(160, 8, "minecraft:light_gray_stained_glass_pane");
			AddMinecraftBlock(160, 9, "minecraft:cyan_stained_glass_pane");
			AddMinecraftBlock(160, 10, "minecraft:purple_stained_glass_pane");
			AddMinecraftBlock(160, 11, "minecraft:blue_stained_glass_pane");
			AddMinecraftBlock(160, 12, "minecraft:brown_stained_glass_pane");
			AddMinecraftBlock(160, 13, "minecraft:green_stained_glass_pane");
			AddMinecraftBlock(160, 14, "minecraft:red_stained_glass_pane");
			AddMinecraftBlock(160, 15, "minecraft:black_stained_glass_pane");
			AddMinecraftBlock(161, 0, "minecraft:acacia_leaves");
			AddMinecraftBlock(161, 1, "minecraft:dark_oak_leaves");
			AddMinecraftBlock(162, 0, "minecraft:acacia_wood");
			AddMinecraftBlock(162, 1, "minecraft:dark_oak_wood");
			AddMinecraftBlock(163, 0, "minecraft:acacia_wood_stairs");
			AddMinecraftBlock(164, 0, "minecraft:dark_oak_wood_stairs");
			AddMinecraftBlock(165, 0, "minecraft:slime_block");
			AddMinecraftBlock(166, 0, "minecraft:barrier");
			AddMinecraftBlock(167, 0, "minecraft:iron_trapdoor");
			AddMinecraftBlock(168, 0, "minecraft:prismarine");
			AddMinecraftBlock(168, 1, "minecraft:prismarine_bricks");
			AddMinecraftBlock(168, 2, "minecraft:dark_prismarine");
			AddMinecraftBlock(169, 0, "minecraft:sea_lantern");
			AddMinecraftBlock(170, 0, "minecraft:hay_bale");
			AddMinecraftBlock(171, 0, "minecraft:white_carpet");
			AddMinecraftBlock(171, 1, "minecraft:orange_carpet");
			AddMinecraftBlock(171, 2, "minecraft:magenta_carpet");
			AddMinecraftBlock(171, 3, "minecraft:light_blue_carpet");
			AddMinecraftBlock(171, 4, "minecraft:yellow_carpet");
			AddMinecraftBlock(171, 5, "minecraft:lime_carpet");
			AddMinecraftBlock(171, 6, "minecraft:pink_carpet");
			AddMinecraftBlock(171, 7, "minecraft:gray_carpet");
			AddMinecraftBlock(171, 8, "minecraft:light_gray_carpet");
			AddMinecraftBlock(171, 9, "minecraft:cyan_carpet");
			AddMinecraftBlock(171, 10, "minecraft:purple_carpet");
			AddMinecraftBlock(171, 11, "minecraft:blue_carpet");
			AddMinecraftBlock(171, 12, "minecraft:brown_carpet");
			AddMinecraftBlock(171, 13, "minecraft:green_carpet");
			AddMinecraftBlock(171, 14, "minecraft:red_carpet");
			AddMinecraftBlock(171, 15, "minecraft:black_carpet");
			AddMinecraftBlock(172, 0, "minecraft:hardened_clay");
			AddMinecraftBlock(173, 0, "minecraft:block_of_coal");
			AddMinecraftBlock(174, 0, "minecraft:packed_ice");
			AddMinecraftBlock(175, 0, "minecraft:sunflower");
			AddMinecraftBlock(175, 1, "minecraft:lilac");
			AddMinecraftBlock(175, 2, "minecraft:double_tallgrass");
			AddMinecraftBlock(175, 3, "minecraft:large_fern");
			AddMinecraftBlock(175, 4, "minecraft:rose_bush");
			AddMinecraftBlock(175, 5, "minecraft:peony");
			AddMinecraftBlock(176, 0, "minecraft:free-standing_banner");
			AddMinecraftBlock(177, 0, "minecraft:wall-mounted_banner");
			AddMinecraftBlock(178, 0, "minecraft:inverted_daylight_sensor");
			AddMinecraftBlock(179, 0, "minecraft:red_sandstone");
			AddMinecraftBlock(179, 1, "minecraft:chiseled_red_sandstone");
			AddMinecraftBlock(179, 2, "minecraft:smooth_red_sandstone");
			AddMinecraftBlock(180, 0, "minecraft:red_sandstone_stairs");
			AddMinecraftBlock(181, 0, "minecraft:double_red_sandstone_slab");
			AddMinecraftBlock(182, 0, "minecraft:red_sandstone_slab");
			AddMinecraftBlock(183, 0, "minecraft:spruce_fence_gate");
			AddMinecraftBlock(184, 0, "minecraft:birch_fence_gate");
			AddMinecraftBlock(185, 0, "minecraft:jungle_fence_gate");
			AddMinecraftBlock(186, 0, "minecraft:dark_oak_fence_gate");
			AddMinecraftBlock(187, 0, "minecraft:acacia_fence_gate");
			AddMinecraftBlock(188, 0, "minecraft:spruce_fence");
			AddMinecraftBlock(189, 0, "minecraft:birch_fence");
			AddMinecraftBlock(190, 0, "minecraft:jungle_fence");
			AddMinecraftBlock(191, 0, "minecraft:dark_oak_fence");
			AddMinecraftBlock(192, 0, "minecraft:acacia_fence");
			AddMinecraftBlock(193, 0, "minecraft:spruce_door_block");
			AddMinecraftBlock(194, 0, "minecraft:birch_door_block");
			AddMinecraftBlock(195, 0, "minecraft:jungle_door_block");
			AddMinecraftBlock(196, 0, "minecraft:acacia_door_block");
			AddMinecraftBlock(197, 0, "minecraft:dark_oak_door_block");
			AddMinecraftBlock(198, 0, "minecraft:end_rod");
			AddMinecraftBlock(199, 0, "minecraft:chorus_plant");
		}

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
			for (int i = 0; i < 4096; i++) {
				m_block_ids[i] = 0;
				m_block_datas[i] = 0;
			}
		}

		void Section::PlatteToIdAndData(nbt::TagList* tagPalettes, uint16_t* platte_indexs)
		{
			InitBlockMap();

			size_t palette_size = tagPalettes->payload.size();
			for (int i = 0; i < 4096; i++) {
				uint16_t palette_index = platte_indexs[i];
				nbt::TagCompound* tagBlock = (nbt::TagCompound*)tagPalettes->payload.at(palette_index);
				if (tagBlock == NULL) continue;
				nbt::TagString* tagName = tagBlock->findTag<nbt::TagString>("Name", nbt::TAG_STRING);
				if (tagName) {
					std::string blockName = tagName->payload;
					if (s_block_name_id_map.find(blockName) != s_block_name_id_map.end()) {
						m_block_ids[i] = s_block_name_id_map.at(blockName);
					}
					if (s_block_name_data_map.find(blockName) != s_block_name_data_map.end()) {
						m_block_datas[i] = s_block_name_data_map.at(blockName);
					}
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
}