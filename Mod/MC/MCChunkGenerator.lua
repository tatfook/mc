
NPL.load("(gl)script/apps/Aries/Creator/Game/World/ChunkGenerator.lua");
local EntityManager = commonlib.gettable("MyCompany.Aries.Game.EntityManager");

local MC = NPL.load("./MC.lua");

local MCChunkGenerator = commonlib.inherit(commonlib.gettable("MyCompany.Aries.Game.World.ChunkGenerator"), NPL.export())

function MCChunkGenerator:ctor()
end

-- @param world: WorldManager, if nil, it means a local generator. 
-- @param seed: a number
function MCChunkGenerator:Init(world, seed)
	MCChunkGenerator._super.Init(self, world, seed);
	self:LoadMCWorld();
	return self;
end

function MCChunkGenerator:OnExit()
	MCChunkGenerator._super.OnExit(self);
end

function MCChunkGenerator:GetGenerateName()
	return "MCChunkGenerator";
end

function MCChunkGenerator:LoadMCWorld()
	local result = MC:LoadMCWorld(self:GetWorld():GetSeedString());
	if(not result) then return end

	local spawn_x, spawn_y, spawn_z = result.spawn_x, result.spawn_y, result.spawn_z;
	NPL.load("(gl)script/ide/timer.lua");
	local mytimer = commonlib.Timer:new({callbackFunc = function(timer)
		_guihelper.MessageBox("Do you want to teleport to spawn position in mc world?", function(res)
			if(res and res == _guihelper.DialogResult.Yes) then
				EntityManager.GetPlayer():SetBlockPos(spawn_x, spawn_y, spawn_z);
			end
		end, _guihelper.MessageBoxButtons.YesNo);
	end})
	mytimer:Change(2000);
end

-- generate chunk for the entire chunk column at x, z
function MCChunkGenerator:GenerateChunkImp(chunk, x, z, external)
	local result = MC:LoadMCChunk(x, z);
	if (not result) then return end 
	local xs, ys, zs, ids, datas = result.x, result.y, result.z, result.ids, result.datas;
	for i = 1, result.count do
		local bx, by, bz, block_id, block_data = xs[i], ys[i], zs[i], ids[i], datas[i];
		chunk:SetType(bx, by, bz, block_id, false);
		if(block_data and block_data~=0) then
			chunk:SetData(bx, by, bz, block_data);
		end
	end
end

