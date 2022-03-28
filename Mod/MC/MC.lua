

local MC = commonlib.inherit(commonlib.gettable("System.Core.ToolBase"), NPL.export())

function MC:ctor()
	-- please note this mode only works with 32bits dll.
    local dll_name = "MCImporter";
	if (ParaEngine.IsDebugging()) then
		self.__dll_path__ = dll_name .. "_d.dll";
	elseif (ParaEngine.GetAttributeObject():GetField("Is64BitsSystem", false)) then
		self.__dll_path__ = dll_name .. "_64bits.dll";
	else
		self.__dll_path__ = dll_name .. ".dll";
	end
end

function MC:Call(data)
	if (type(data) ~= "table") then return end

	data.callback = self:GetCallBackPath();

	NPL.call(self.__dll_path__, data);
end

function MC:Init()
end


function MC:GetCallBackPath()
	return "Mod/MC/MC.lua";
end

function MC:LoadSchematics(filepath)
	self.__result__ = nil;
	self:Call({
		cmd="LoadSchematics", 
		filepath = filepath,
	});
	return self.__result__;
end


function MC:LoadMCWorld(worldpath)
	self.__result__ = nil;

	self:Call({
		cmd="LoadMCWorld",
		path = worldpath,
	});

	if(not self.isworldloaded) then
		LOG.std(nil, "info", "MCImporterChunkGenerator", "failed to load world at path %s", src_path);
	else
		LOG.std(nil, "info", "MCImporterChunkGenerator", "successfully load world at path %s", src_path);
	end

	return self.__result__;
end

function MC:LoadMCChunk(x, z)
	self.__result__ = nil;
	self:Call({cmd="GetChunkBlocks", x = x, z = z});
	return self.__result__;
end

MC:InitSingleton():Init();

local function activate()
	if (type(msg) ~= "table") then return end 
	local cmd = msg.cmd;

	if(cmd == "LoadSchematics" and msg.width and msg.height and msg.length and msg.id and msg.data) then
		MC.__result__ = {width = msg.width, height = msg.height, length = msg.length, ids = msg.id, datas = msg.data};
	end
	if (cmd == "LoadMCWorld" and msg.succeed) then
		MC.__result__ = {spawn_x = msg.spawn_x, spawn_y = msg.spawn_y, spawn_z = msg.spawn_z};
	end
	if (cmd == "GetChunkBlocks" and msg.count) then
		MC.__result__ = {x = msg.x, y = msg.y, z = msg.z, ids = msg.id, datas = msg.data, count = msg.count};
	end
end

NPL.this(activate);

-- NPL.load("Mod/MCMap/")
-- local MC = NPL.load("Mod/MCMap/MC.lua", true);
-- MC:LoadSchematics("D:/mcworld/schem/gracefieldhouse.schem");
-- MC:LoadSchematics("D:/mcworld/schem/materials-alpha.schematic");
-- /mc import_schematics D:/mcworld/schem/gracefieldhouse.schem
-- /mc import_world D:/mcworld/HG
--[[
-- mc_name minecraft 方块名称
-- mc_id  minecraft 方块ID
-- mc_data minecraft 方块数据
-- mc_state minecraft 方块状态
-- pc_id paracraft 方块ID
-- pc_data paracraft 方块数据
-- pc_data paracraft 方块面向
<blocks>
	<block mc_name="minecraft:air" mc_id="0" mc_data="0" mc_state="0" pc_id="0" pc_data="0" pc_side="0"></block>
</blocks>
]]
