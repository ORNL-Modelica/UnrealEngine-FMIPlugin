// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class FMIKit : ModuleRules
{
	private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }

	private string FMIKitPath
	{
		get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "fmikit/")); }
	}

	private string LibZipPath
    {
        get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "libzip/")); }
    }
	
	    private string ZLibPath
    {
        get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "zlib/")); }
    }
	
	public FMIKit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				Path.Combine(FMIKitPath, "Include"),
				Path.Combine(LibZipPath, "Include"),
				Path.Combine(ZLibPath, "Include"),
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"Slate",
				"SlateCore",
				"XmlParser",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
			
		LoadLib(Target);
	}
	
	public bool LoadLib(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;


			// FMIKit
			PublicIncludePaths.Add(Path.Combine(FMIKitPath, "Include"));

			// LibZip
			string LibrariesPath = Path.Combine(LibZipPath, "lib");
            string DLLPath = Path.Combine(LibZipPath, "dll");
			
			PublicIncludePaths.Add(Path.Combine(LibZipPath, "Include"));

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "zip.lib"));

            PublicDelayLoadDLLs.Add("zip.dll");
            RuntimeDependencies.Add(Path.Combine(DLLPath, "zip.dll"));
			
			// ZLibPath
			LibrariesPath = Path.Combine(ZLibPath, "lib");
            DLLPath = Path.Combine(ZLibPath, "dll");
			
			PublicIncludePaths.Add(Path.Combine(ZLibPath, "Include"));

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "zlib.lib"));

            PublicDelayLoadDLLs.Add("zlib.dll");
            RuntimeDependencies.Add(Path.Combine(DLLPath, "zlib.dll"));
        }

        return isLibrarySupported;
    }
}
