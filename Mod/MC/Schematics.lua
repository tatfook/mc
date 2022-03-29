
local BlockEngine = commonlib.gettable("MyCompany.Aries.Game.BlockEngine");

local MC = NPL.load("./MC.lua");

local Schematics = commonlib.inherit(commonlib.gettable("System.Core.ToolBase"), NPL.export())

function Schematics:Init()
end

function Schematics:Load(filepath, sx, sy, sz)
    sx, sy, sz = sx or 19200, sy or 4, sz or 19200;
    local result = MC:LoadSchematics(filepath);
    
    if (not result) then return end

    local width, height, length, ids, datas = result.width, result.height, result.length, result.ids, result.datas;

    for y = 1, height do
		for z = 1, length do
			for x = 1, width do
				local index = x + z * width + y * width * length;
				local id = ids[index];
				local data = datas[index];
				BlockEngine:SetBlock(sx + x, sy + y, sz + z, id, data);
			end
		end
	end
end

Schematics:InitSingleton():Init();
