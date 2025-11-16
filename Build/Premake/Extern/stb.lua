local m = {}
function m.AddDependency()
	defines("CP_STB")
	includedirs { path.join(cp.externPath, "stb") }
end
return m