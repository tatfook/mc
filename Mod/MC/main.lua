--[[

]]
local MC = commonlib.inherit(commonlib.gettable("Mod.ModBase"), commonlib.gettable("Mod.MC"));

-- name of the generator
MC.generator_name = "MC";

function MC:ctor()
end

-- virtual function get mod name

function MC:GetName()
	return "MC"
end

-- virtual function get mod description 

function MC:GetDesc()
	return "MC is a plugin in paracraft"
end

function MC:init()
	print("==============================MCMap plugin initialized============================");
	if (ParaEngine.IsDebugging()) then NPL.load("Mod/MC/") end 

	NPL.load("Mod/MC/Command.lua")
end

function MC:OnLogin()
end
-- called when a new world is loaded. 

function MC:OnWorldLoad()
end
-- called when a world is unloaded. 

function MC:OnLeaveWorld()
end

function MC:OnDestroy()
end
