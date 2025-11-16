local m = {}
function m.AddDependency()
	defines("CP_VMA")
	includedirs { path.join(cp.externPath, "VulkanMemoryAllocator/include") }
end
return m