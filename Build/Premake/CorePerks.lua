cp = {}

local printPrefix = "[CorePerks] "
local projectConfigurations = { "Debug", "Dev", "Profile", "Release" }
local precompiledFilename = "Precompiled"

function cp.Print(msg)
	print("[CorePerks] "..msg)
end

-- Load extern modules
local externModuleFiles = os.matchfiles("Extern/*.lua")
for _, file in ipairs(externModuleFiles) do
    local moduleName = path.getbasename(file)
    cp[moduleName] = dofile(file)
end

function cp.Configure(params)
	cp.rootPath = params.rootPath or path.getabsolute("..")
	cp.externPath = path.join(cp.rootPath, "Extern")
	cp.sourcePath = path.join(cp.rootPath, "Source")
	cp.buildPath = path.join(cp.rootPath, "Build")
	cp.projectsPath = path.join(cp.buildPath, "Projects")
	cp.objPath = path.join(cp.buildPath, "Obj")
	cp.targetPath = path.join(cp.buildPath, "Bin/$(Configuration)")
	cp.workspacesPath = path.join(cp.buildPath, "Workspaces")
end

function cp.Project(name)
	project(name)
	location(cp.projectsPath)
	architecture "x64"
	kind "StaticLib"
	objdir(cp.objPath)
	targetdir("$(SolutionDir)$(Platform)/$(Configuration)")
	libdirs { "$(OutDir)" }
	includedirs("..")
	includedirs(cp.externPath)

	debugdir "$(OutDir)"
	configurations(projectConfigurations)

	filter{"configurations:Debug"}
		defines {"CP_DEBUG", "CP_DEBUG_OR_DEV"}
	filter{"configurations:Dev"}
		defines {"CP_DEV", "CP_DEBUG_OR_DEV"}
	filter{"configurations:Profile"}
		defines {"CP_PROFILE"}
	filter{"configurations:Release"}
		defines {"CP_RELEASE"}
	filter {}
end

function cp.CppProject(name)
	cp.Project(name)
	rtti "Off"
	language "C++"
	exceptionhandling "Off"
	vectorextensions "AVX"
	symbols "On"
	cppdialect "C++20"
	fatalwarnings { "All"}
	flags { "MultiProcessorCompile" }
	files { "**.cpp", "**.h", "**.hpp", "**.inl", "**.natvis", "**.lua" }

	if os.isfile(precompiledFilename..".h") then
		pchheader(precompiledFilename..".h")
		filter "action:vs*"
			pchsource(precompiledFilename..".cpp")
		filter {}
	end

	filter { "action:vs*" }
		files { "*.natvis"}
		defines { "_HAS_EXCEPTIONS=0" }
		buildoptions {"/Zc:preprocessor"} -- Required for C++20's __VA_OPT__() for some reason
		--flags { "StaticRuntime" }
		--linkoptions { "/ENTRY:mainCRTStartup" } -- TODO: Not working with DLL, should avoid that case somehow.
		linkoptions {"/ignore:4221"} -- warns when .cpp are empty depending on the order of obj linking.
		
	filter { "configurations:Release" }
		optimize "On"
		omitframepointer "On"
	filter {}
end

function cp.ShaderProject(name, targetEnv)
	cp.Project("Shaders")
	kind "Utility"
	files { "**.vert", "**.frag", "**.comp", "**.tesc", "**.tese", "**.geom", "**.glsl", "**.h",
		"**.rgen", "**.rint", "**.rahit", "**.rchit", "**.rmiss", "**.rcall" }
	local shaderFilter = 'files:**.vert or **.frag or **.comp or **.tesc or **.tese or **.geom or **.rgen or **.rint or **.rahit or **.rchit or **.rmiss or **.rcall'
	local shaderOutPath = "$(OutDir)/"..name.."/%{file.name}.spv"

	filter {shaderFilter}
		buildmessage 'Compiling %{file.relpath}'
		buildoutputs { shaderOutPath }
	filter {shaderFilter, "configurations:Debug or Development"}
		buildcommands { '$(GLSLANG)/glslangValidator.exe -V -gVS --target-env '..targetEnv..' -d -o "'.. shaderOutPath ..'" %{file.relpath}' }
		--buildcommands { '$(VULKAN_SDK)/bin/glslc.exe -g --target-spv='..targetEnv..' -o "'.. shaderOutPath ..'" %{file.relpath}' }
	filter {shaderFilter, "configurations:Profile or Release"}
		buildcommands { '$(GLSLANG)/glslangValidator.exe -V -g0 --target-env '..targetEnv..' -d -o "'.. shaderOutPath ..'" %{file.relpath}' }
		--buildcommands { '$(VULKAN_SDK)/bin/glslc.exe -O --target-spv='..targetEnv..' -o "'.. shaderOutPath ..'" %{file.relpath}' }
end

function cp.Workspace(name)
	workspace(name)
	configurations(projectConfigurations)
	location "Workspaces"
	includedirs(path.join(cp.externPath, "CorePerks"))
end

function cp.AddDependency(projectName)
	filter "kind:WindowedApp"
		links { projectName }
	filter ""
		dependson { projectName }
end

function cp.AddProject()
	include(path.join(cp.externPath, "CorePerks/CorePerks"))
end
