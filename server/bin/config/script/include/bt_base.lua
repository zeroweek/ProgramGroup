load_package("ai.pkg");
g_param_func_counter = 1;

function get_func_name(f)
	for k, v in pairs(_G) do
		if v == f then
			return k;
		end
	end
end

function create_node_from_table(parent, t)

	local node = nil
	local error = false;
	local node_type = btntInvalid;
	local name;
	local param;
	local node_flag = 0;
	local bit_flag = {1, 2, 4, 8};
	local children = {};

	if t.param then
		param = {};
		for i = 1, 4 do
			if type(t.param[i]) == 'string' then
				param[i] = g_param_func_counter;
				print("function ParamFunc" .. param[i] .. "(o) return " .. t.param[i] .. " end");
				loadstring("function ParamFunc" .. param[i] .. "(o) return " .. t.param[i] .. " end")();
				g_param_func_counter = g_param_func_counter + 1;
				node_flag = node_flag + bit_flag[i];
				--print(node_flag);
			elseif type(t.param[i]) == 'boolean' then
				if t.param[i] then
					param[i] = 1;
				else
					param[i] = 0;
				end
			else
				param[i] = t.param[i];
			end
		end
	else
		param = {0, 0, 0, 0};
	end

	for k, v in pairs(t) do 
		if type(v) == 'string' and string.sub(v, 1, 3) == 'on_' then
			v = _G[v];
		end

		if k == 'param' then

		elseif type(v) == 'table' then
			table.insert(children, v);
		else
			if type(v) == 'function' then
				name = get_func_name(v);
				node_type = find_bt_action_id(name);
				if not node_type then
					node_type = register_bt_action(name);
				end
			elseif type(v) == 'number' then
				node_type = v;
				name = tostring(node_type);
			elseif v == nil then

			else
				print("unknown node k=" .. tostring(k) .. ", v=" .. tostring(v));
			end
		end
	end

	if #children == 0 and (node_type == btatSequencer or node_type == btatParallel or node_type == btatSelector) then
		return nil, false;
	end

	if node_type ~= btntInvalid then
		node = create_bt_node(parent, node_type, name, param[1], param[2], param[3], param[4], node_flag);
	end

	if node then
		for k, v in ipairs(children) do
			local _, child_error = create_node_from_table(node, v);
			if child_error then
				if name then
					print("failed to create child node of node " .. name);
				else
					print("failed to create child node of unknown name node");
				end

				error = true;
				break;
			end
		end
	else
		if name then
			print("failed to create child node " .. name);
		else
			print("failed to create unknown node name");
		end
		error = true;
	end

	return node, error;
end

load_ai_data();

function load_bt_tree(tree_name)
	local node, error = create_node_from_table(nil, _G[tree_name]);
	if error then
		print("bt tree: " .. tree_name .. " error");
		if node then
			print_bt_tree(node);
			destroy_bt_tree(node);
		end
		return;
	else
		return node;
	end
end