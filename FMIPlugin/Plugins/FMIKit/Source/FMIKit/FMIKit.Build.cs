// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Runtime.Intrinsics.X86;
using UnrealBuildTool;
//using static System.Net.Mime.MediaTypeNames;

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
    //private string FMILIBPath
    //{
    //    get { return Path.GetFullPath("C:\\Users\\green\\Documents\\FMI\\fmi-library\\build-fmil\\test"); }
    //}
    public FMIKit(ReadOnlyTargetRules Target) : base(Target)
    {
        bEnableExceptions = true;

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);

        PrivateIncludePaths.AddRange(
			new string[] {
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

        LoadFMIKIT(Target);
        //LoadFMILIB(Target);
    }
	
	public bool LoadFMIKIT(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            isLibrarySupported = true;

			PublicIncludePaths.Add(Path.Combine(FMIKitPath, "Include"));
        }

        return isLibrarySupported;
    }

    //public bool LoadFMILIB(ReadOnlyTargetRules Target)
    //{
    //    bool isLibrarySupported = false;

    //    if ((Target.Platform == UnrealTargetPlatform.Win64))
    //    {
    //        isLibrarySupported = true;

    //        PublicDelayLoadDLLs.Add("fmilib_shared.dll");
    //        PublicAdditionalLibraries.Add(Path.Combine(FMILIBPath, "lib/fmilib_shared.lib"));
    //        PublicIncludePaths.Add(Path.Combine(FMILIBPath, "include"));
    //    }

    //    return isLibrarySupported;
    //}
}
