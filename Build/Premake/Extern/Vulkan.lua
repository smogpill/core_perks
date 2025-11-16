local m = {}
function m.AddDependency()
	defines("CP_VULKAN")
	includedirs "%VULKAN_SDK%/Include"
	filter "kind:WindowedApp"
		libdirs "%VULKAN_SDK%/Lib"
		links "vulkan-1"
	filter {}
end
return m