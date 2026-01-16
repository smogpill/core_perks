local m = {}
m.define_suffix = "GLM"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "glm") }
end
return m
