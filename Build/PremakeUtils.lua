function cp_Init()
	cp_rootDir = path.getabsolute("..")
	cp_externDir = path.join(cp_rootDir, "Extern")
	cp_sourceDir = path.join(cp_rootDir, "Source")
	cp_buildDir = path.join(cp_rootDir, "Build")
	cp_projectsDir = path.join(cp_buildDir, "Projects")
	cp_objDir = path.join(cp_buildDir, "Obj")
	cp_targetDir = path.join(cp_buildDir, "Bin/$(Configuration)")
	cp_workspacesDir = path.join(cp_buildDir, "Workspaces")
	cp_configurations = { "Debug", "Dev", "Profile", "Release" }
	cp_pchName = "Precompiled"
end

function cp_SetProjectDefaults(name)
	project(name)
	location(cp_projectsDir)
	architecture "x64"
	kind "StaticLib"
	objdir(cp_objDir)
	targetdir("$(SolutionDir)$(Platform)/$(Configuration)")
	libdirs { "$(OutDir)" }
	includedirs("..")
	includedirs(cp_externDir)

	debugdir "$(OutDir)"
	configurations(cp_configurations)

	filter{"configurations:Debug"}
		defines {"AB_DEBUG", "AB_DEBUG_OR_DEV"}
	filter{"configurations:Development"}
		defines {"AB_DEV", "AB_DEBUG_OR_DEV"}
	filter{"configurations:Profile"}
		defines {"AB_PROFILE"}
	filter{"configurations:Release"}
		defines {"AB_RELEASE"}
	filter {}
end

function cp_SetCppProjectDefaults(name)
	cp_SetProjectDefaults(name)
	rtti "Off"
	language "C++"
	exceptionhandling "Off"
	vectorextensions "AVX"
	symbols "On"
	cppdialect "C++20"
	fatalwarnings { "All"}
	flags { "MultiProcessorCompile" }
	files { "**.cpp", "**.h", "**.hpp", "**.inl", "**.natvis", "**.lua" }

	if os.isfile(cp_pchName..".h") then
		pchheader(cp_pchName..".h")
		pchsource(cp_pchName..".cpp")
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

function cp_SetShaderProjectDefaults(name, targetEnv)
	SetProjectDefaults("Shaders")
	kind "Utility"
	files { "**.vert", "**.frag", "**.comp", "**.tesc", "**.tese", "**.geom", "**.glsl", "**.h",
		"**.rgen", "**.rint", "**.rahit", "**.rchit", "**.rmiss", "**.rcall" }
	shaderFilter = 'files:**.vert or **.frag or **.comp or **.tesc or **.tese or **.geom or **.rgen or **.rint or **.rahit or **.rchit or **.rmiss or **.rcall'
	shaderOutPath = "$(OutDir)/"..name.."/%{file.name}.spv"

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

function cp_SetWorkspaceDefaults(name)
	workspace(name)
	configurations(cp_configurations)
	location "Workspaces"
end

function cp_SDL3_ExternalProject()
	externalproject "SDL"
   	location(cp_externDir.."/SDL3/VisualC/SDL")
   	uuid "81CE8DAF-EBB2-4761-8E45-B71ABCCA8C68"
   	kind "SharedLib"
   	language "C++"
   	architecture "x64"
   	objdir(cp_objDir)
	targetdir(cp_targetDir)
end
