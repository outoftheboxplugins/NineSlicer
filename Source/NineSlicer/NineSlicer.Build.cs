// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

using UnrealBuildTool;

public class NineSlicer : ModuleRules
{
	public NineSlicer(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"DeveloperSettings",
			"Slate",
			"SlateCore",
			"UMG",
			"UMGEditor",
			"UnrealEd"
		});
	}
}