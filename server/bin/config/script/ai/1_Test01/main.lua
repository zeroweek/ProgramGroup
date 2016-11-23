include("script/ai/1_Test01/main.lh");

function __lua_action_01(owner)

end

function _idle_tree()
	local result = 
	{ btatSequencer,
		{ __lua_action_01 },
	};

	return result;
end

btetAIIdle = _idle_tree();
