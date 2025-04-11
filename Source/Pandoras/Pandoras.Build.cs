// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pandoras : ModuleRules
{
	public Pandoras(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "Pandoras" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
