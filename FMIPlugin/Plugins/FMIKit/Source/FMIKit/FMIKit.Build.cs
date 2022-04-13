// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class FMIKit : ModuleRules
{
  private string PythonPath
  {
    get { return Environment.GetEnvironmentVariable("PYTHON_PATH"); }
  }
  
	private string ThirdPartyPath
  {
    get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
  }

	private string FMIKitPath
	{
		get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "fmikit/")); }
	}

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
				Path.Combine(PythonPath, "Include"),
				Path.Combine(FMIKitPath, "Include"),
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

        PublicAdditionalLibraries.AddRange(
          new string[] {
            Path.Combine(PythonPath, "libs", "python310.lib"),
          }
         );
        PublicIncludePaths.Add(Path.Combine(PythonPath, "Include"));

        // FMIKit
        PublicIncludePaths.Add(Path.Combine(FMIKitPath, "Include"));
      }

      return isLibrarySupported;
    }
}
