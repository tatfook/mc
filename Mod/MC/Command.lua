NPL.load("(gl)script/apps/Aries/Creator/Game/World/ChunkGenerators.lua");
local ChunkGenerators = commonlib.gettable("MyCompany.Aries.Game.World.ChunkGenerators");

local Commands = commonlib.gettable("MyCompany.Aries.Game.Commands");
local CmdParser = commonlib.gettable("MyCompany.Aries.Game.CmdParser");	
local EntityManager = commonlib.gettable("MyCompany.Aries.Game.EntityManager");

local Command = commonlib.inherit(commonlib.gettable("System.Core.ToolBase"), NPL.export())

local MC = NPL.load("./MC.lua");
local MCChunkGenerator = NPL.load("./MCChunkGenerator.lua");
local Schematics = NPL.load("./Schematics.lua");

local function FormatPath(filename, platform)
    platform = platform or System.os.GetPlatform();

	if(platform == "win32") then
        filename = string.gsub(filename, "/+", "\\");
		filename = string.gsub(filename, "\\+", "\\");
	else
		filename = string.gsub(filename, "\\+", "/");
        filename = string.gsub(filename, "/+", "/");
	end
	
    return filename;
end

-- allow the user to choose which world to import. 
local function OnClickImportMCSchematicsCmd()
	NPL.load("(gl)script/ide/OpenFileDialog.lua");
	local filepath = CommonCtrl.OpenFileDialog.ShowDialog_Win32();
	if (not filepath or filepath == "") then return end 
    local x, y, z = EntityManager.GetPlayer():GetBlockPos();
    Schematics:Load(filepath, x, y, z);
end

local function ImportSchematics(cmd_text)
    local filepath, cmd_text = CmdParser.ParseString(cmd_text);
	if (not filepath or filepath == "") then return OnClickImportMCSchematicsCmd() end 
    
    local x, y, z, cmd_text = CmdParser.ParsePos(cmd_text);
    if (not x or not y or not z) then
        x, y, z = EntityManager.GetPlayer():GetBlockPos();
    end
    Schematics:Load(filepath, x, y, z);
end

local function CreateWorldFromMCWorld(worldname, src_dir)
	NPL.load("(gl)script/apps/Aries/Creator/Game/Login/CreateNewWorld.lua");
	local WorldCommon = commonlib.gettable("MyCompany.Aries.Creator.WorldCommon")
	local CreateNewWorld = commonlib.gettable("MyCompany.Aries.Game.MainLogin.CreateNewWorld")
	local GameLogic = commonlib.gettable("MyCompany.Aries.Game.GameLogic");

	local params = {
		worldname = worldname,
		title = worldname,
		creationfolder = CreateNewWorld.GetWorldFolder(),
		parentworld = "worlds/Templates/Empty/flatsandland",
		world_generator = MCChunkGenerator:GetGenerateName(),
		seed = src_dir,
	}
	LOG.std(nil, "info", "CreateNewWorld", params);

	local worldpath, error_msg = CreateNewWorld.CreateWorld(params);
	if(not worldpath) then
		if(error_msg) then
			_guihelper.MessageBox(error_msg);
		end
	else
		WorldCommon.OpenWorld(worldpath, true);
	end
end

-- allow the user to choose which world to import. 
local function OnClickImportMCWorldCmd()
	NPL.load("(gl)script/ide/OpenFileDialog.lua");
	local src_dir = CommonCtrl.OpenFileDialog.ShowOpenFolder_Win32();
	if(src_dir and src_dir~="") then
		src_dir = src_dir:gsub("[/\\]$", "");
		local worldname = src_dir:match("[^/\\]+$");
		if(worldname and src_dir) then
			if(ParaIO.DoesFileExist(src_dir.."\\level.dat", false)) then
				CreateWorldFromMCWorld(worldname, src_dir)
			else
				_guihelper.MessageBox("Directory is not a valid minecraft world directory, please select a valid folder.");
			end
		end
	end
end

local function ImportWorld(cmd_text)
    local worldpath, cmd_text = CmdParser.ParseString(cmd_text);
    if (not worldpath or worldpath == "") then return OnClickImportMCWorldCmd() end
    worldpath = worldpath:gsub("[/\\]$", "");
	local worldname = worldpath:match("[^/\\]+$");
    if (not worldname or worldname == "") then return end
	CreateWorldFromMCWorld(worldname, worldpath)
end

local function LoadBlockConfig(cmd_text)
    local filepath = CmdParser.ParseString(cmd_text);
    MC:LoadConfig(filepath);
end

function Command:Install()
    Commands["mc"] = {
        name="mc", 
        quick_ref="/mc import_schematics ...", 
        desc=[[minecraft map import and export
    subcmd:
    import_schematics schem_path                           -- 导入 MineCraft 的 Schematics 地图文件
        /mc import_schematics house.schem 19200 4 19200    -- 在19200 4 19200处加载
    import_world world_path
        /mc import_world D:/world
    load_block_config                                      -- 加载方块配置 
        /mc load_block_config blocks.xml 
    debug                                                  -- 打印未配置的 MineCraft 方块信息
        /mc debug
        ]] ,
        handler = function(cmd_name, cmd_text, cmd_params, fromEntity)
            local cmd, cmd_text = CmdParser.ParseString(cmd_text);
    
            if (cmd == "import_schematics") then
                ImportSchematics(cmd_text);
            elseif (cmd == "import_world") then
                ImportWorld(cmd_text);
            elseif (cmd == "load_block_config") then
                LoadBlockConfig(cmd_text);
            elseif (cmd == "debug") then
                MC:Debug();
            end
        end,
    };
end

function Command:Init()
    self:Install();

    ChunkGenerators:Register(MCChunkGenerator:GetGenerateName(), MCChunkGenerator);
end

Command:InitSingleton():Init();


-- /mc import_world D:/mcworld/world