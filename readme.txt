Author: LiPeng, LiXizhi
Date: 2015.11

---++ Introduction
	added /mcimport command to import minecraft world directory. Please note, it does not import at once, instead it uses the ChunkGenerator interface to load the world dynamically according to current player position. 

---++ Install
	- copy MCImporter.dll to paracraft root directory. (currently only 32bits version is built)
	- copy MCImporter.zip to ./Mod folder and enable the importer plugin. 

---++ How to use
	- type /mcimport and select a valid minecraft world directory. 
	- a new world with the same name will be created locally, one may need to teleport the player to correct position to see the world load progressively as the player moves. 

---++ reference
	source code is based on mapcrafter-master
	https://github.com/m0r13/mapcrafter (all files in /mc folder)
	require boost zlib 

---++ build
	the only thing that I modified in mapcrafter is in nbt.cpp file line 252
	if(length>0)
	{
		stream.read(&payload[0], length);
	}
	VC compiler does not support 0 length array.  gcc has it. 

---++ change log

2015.11.18  by Lipeng

c++

1.open folder browser select mc world path

(1).EditorHelper.h         add function static bool OpenFolderDialog(string& FolderPath, string title);

(2).ParaScriptingScene.h   add function static bool InitMCWorldInfo(string mcWorldPath = "");

(3).ParaScripting.cpp add  def("InitMCWorldInfo", &ParaScene::InitMCWorldInfo),

2.read mc world spawn position

(1).MCImporterDLL add function  CORE_EXPORT_DECL bool GetSpawnPosition(int &spawnX, int &spawnY, int &spawnZ);

(2).BlockWorld.h add function  bool GetMCWorldSpawnPos(const luabind::adl::object& pos);

(3).ParaScriptingTerrain.h  add function  static bool GetMCWorldSpawnPosition(const luabind::adl::object& pos);

(4).ParaScripting3.cpp add def("GetMCWorldSpawnPosition", &ParaTerrain::GetMCWorldSpawnPosition),

script

1.open folder browser select mc world path

(1).InternetLoadWorld.lua  add   function InternetLoadWorld.LoadMCWorld()

(2).CreateNewWorld.lua   add   function CreateNewWorld.LoadMCWorld(worldname)

(3).script/apps/Aries/Creator/Game/main.lua  in "function Game.Start()" add  GameLogic.SetMCWorld(ParaWorld.BeMCWorld());

(4).game_logic.lua  add   GameLogic.SetMCWorld(beMCWorld)

2.set paracraft spawn position according the mc world spawn position

(1).script/apps/Aries/Creator/Game/World/World.lua  in  function World:GetSpawnPoint() use "ParaTerrain.GetMCWorldSpawnPosition" get the mc world spawn position