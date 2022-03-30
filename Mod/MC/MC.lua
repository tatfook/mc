

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
	self:LoadConfig();
end

function MC:LoadConfig(filepath)
	filepath = filepath or "Mod/MC/blocks.xml";
	local xmlRoot = ParaXML.LuaXML_ParseFile(filepath);
	if (type(xmlRoot) ~= "table") then return print("invalid blocks.xml") end
	local blocks = commonlib.XPath.selectNode(xmlRoot, "/blocks");
	if (type(blocks) ~= "table") then return print("blocks.xml format error") end
	for _, block in ipairs(blocks) do
		self:AddTranslateRule(block.attr);
	end
end

function MC:AddTranslateRule(rule)
	if (type(rule) ~= "table" or not rule.mc_name or not rule.pc_id) then return end

	print("AddTranslateRule:", rule.mc_name, rule.mc_id, rule.mc_data, rule.mc_state, rule.pc_id, rule.pc_data, rule.pc_side);
    
	self:Call({
		cmd="AddTranslateRule",
		mc_name = rule.mc_name,
		mc_id = tonumber(rule.mc_id) or 0,
		mc_data = tonumber(rule.mc_data) or 0,
		mc_state = tonumber(rule.mc_state) or 0,
		pc_id = tonumber(rule.pc_id) or 0,
		pc_data = tonumber(rule.pc_data) or 0,
		pc_side = tonumber(rule.pc_side) or 0,
	});
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

	return self.__result__;
end

function MC:LoadMCChunk(x, z)
	self.__result__ = nil;
	self:Call({cmd="GetChunkBlocks", x = x, z = z});
	return self.__result__;
end

function MC:Debug()
	self.__result__ = nil;
	self:Call({cmd="Debug"});
	if (not self.__result__) then return end 

	local not_exist_blocknames, not_exist_blockids = self.__result__.not_exist_blocknames, self.__result__.not_exist_blockids;
	print("================未配置的MineCraft方块名称=================", #not_exist_blocknames);
	for _, blockname in ipairs(not_exist_blocknames) do print(blockname) end 
	print("================未配置的MineCraft方块Id Data State=================", #not_exist_blockids);
	for _, blocktag in ipairs(not_exist_blockids) do
		local blockId = math.floor(blocktag / (256 * 256));
		local dataAndState = blocktag % (256 * 256);
		local blockData = math.floor(dataAndState / 256);
		local blockState = dataAndState % 256;
		print(blocktag, blockId, blockData, blockState);
	end
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
	if (cmd == "Debug") then
		MC.__result__ = {not_exist_blocknames = msg.not_exist_blocknames, not_exist_blockids = msg.not_exist_blockids};
	end

end

NPL.this(activate);

-- NPL.load("Mod/MCMap/")
-- local MC = NPL.load("Mod/MCMap/MC.lua", true);
-- MC:LoadSchematics("D:/mcworld/schem/gracefieldhouse.schem");
-- MC:LoadSchematics("D:/mcworld/schem/materials-alpha.schematic");
-- /mc import_schematics D:/mcworld/schem/gracefieldhouse.schem
-- /mc import_world D:/mcworld/world
-- /mc load_block_config Mod/MC/blocks.xml
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


-- >cmake -A x64 -S ./ -B ./build_x64
-- > devenv ./build_x64/MCImporter.sln /Build "Release|x64"

-- > cmake -A win32 -S ./ -B ./build_win32
-- > devenv ./build_win32/MCImporter.sln /Build "Release|win32"
