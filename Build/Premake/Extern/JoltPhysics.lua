local m = {}
function m.AddDependency()
	defines("CP_JOLT_PHYSICS")
	includedirs { path.join(cp.externPath, "JoltPhysics")}
	filter "kind:WindowedApp"
	links { "Jolt" }
	filter { "kind:WindowedApp", "configurations:Debug" }
		libdirs { path.join(cp.externPath, "JoltPhysics/Build/VS2022_CL_Double/Debug") }
	filter { "kind:WindowedApp", "configurations:Release" }
		libdirs { path.join(cp.externPath, "JoltPhysics/Build/VS2022_CL_Double/Release") }
	filter {}
end
return m
