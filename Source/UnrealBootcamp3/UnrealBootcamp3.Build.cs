// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealBootcamp3 : ModuleRules
{
	public UnrealBootcamp3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
