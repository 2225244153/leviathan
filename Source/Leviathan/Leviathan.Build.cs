// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Leviathan : ModuleRules
{
	public Leviathan(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"AIModule",
			"AnimGraphRuntime", "GameplayTags","GameplayTasks","DeveloperSettings"
		});
	}
}