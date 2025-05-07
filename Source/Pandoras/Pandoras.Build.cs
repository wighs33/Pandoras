// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pandoras : ModuleRules
{
	public Pandoras(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "Pandoras" });

		// 게임 플레이 어빌리티 관련 모듈 추가
        PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MotionTrajectory" });
	}
}
