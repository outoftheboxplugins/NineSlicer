// Copyright Out-of-the-Box Plugins 2018-2023. All Rights Reserved.

using UnrealBuildTool;

public class NineSlicer : ModuleRules
{
	public NineSlicer(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new [] {
			"Core",
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"InputCore",
			"Kismet",
			"Slate",
			"SlateCore",
			"ToolMenus",
			"UMG",
			"UMGEditor",
			"UnrealEd", 
			"ModelViewViewModelEditor",
		});
	}
}
