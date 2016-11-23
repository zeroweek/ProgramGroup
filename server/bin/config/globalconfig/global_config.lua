include("config/globalconfig/common_config.lua");
include("config/globalconfig/gameserver_config.lua");
include("config/globalconfig/gamelogic_config.lua");
include("config/globalconfig/gateserver_config.lua");
include("config/globalconfig/guardserver_config.lua");


table_stack = {};

function get_table_stack()
	local s = '';

	for i, tn in ipairs(table_stack) do
		s = s .. tn .. '.';
	end

	return s;
end

function load_traberse_table(d, t)
	for k, v in pairs(t) do
		
--		print("type(d) is " .. type(d));
--		print("type(t) is " .. type(t));
--		print("k: " .. k .. ", type(k) is " .. type(k));
--		print("type(v) is " .. type(v));
--		print("type(d[k]) is " .. type(d[k]));
--		print("type(t[k]) is " .. type(t[k]));

		if d[k] == nil then
			print("variable " .. get_table_stack() .. k .. " is nil")
		else
			if type(v) == "table" then
				table.insert(table_stack, k);
				load_traberse_table(d[k], v);
				table.remove(table_stack);
			else
				--print("d[k]=" .. d[k]);
				--print("t[k]=" .. t[k]);
				d[k] = t[k];
				--print("d[k]=" .. d[k]);
				--print("t[k]=" .. t[k]);
			end
		end
	end
end

function load_file(config, config_name)
	print("loading " .. config_name);
	load_traberse_table(config, _G[config_name]);
end
