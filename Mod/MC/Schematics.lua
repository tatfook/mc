
local BlockEngine = commonlib.gettable("MyCompany.Aries.Game.BlockEngine");

local MC = NPL.load("./MC.lua");

local Schematics = commonlib.inherit(commonlib.gettable("System.Core.ToolBase"), NPL.export())

function Schematics:Init()
	self:LoadConfig();
end

function Schematics:LoadConfig(filepath)
	filepath = filepath or "Mod/MC/blocks.xml";
	local xmlRoot = ParaXML.LuaXML_ParseFile(filepath);
	if (type(xmlRoot) ~= "table") then return end
	local blocks = commonlib.XPath.selectNode(xmlRoot, "/blocks");
	if (type(blocks) ~= "table") then return end
	for _, block in ipairs(blocks) do
		self:AddTranslateRule(block.attr);
	end
end

function Schematics:AddTranslateRule(rule)
	if (type(rule) ~= "table" or not rule.mc_name or not rule.mc_id or not rule.mc_data or not rule.pc_id or not rule.pc_data) then return end 
	
    MC:Call({
		cmd="AddTranslateRule",
		mc_name = rule.mc_name,
		mc_id = rule.mc_id,
		mc_data = rule.mc_data,
		mc_state = rule.mc_state or 0,
		pc_id = rule.pc_id,
		pc_data = rule.pc_data,
		pc_side = rule.pc_side or 0,
	});
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
