// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GUAO_EveryThing : ModuleRules
{
	public GUAO_EveryThing(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PrivateIncludePaths.Add("GUAO_EveryThing/Public");

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });


        PrivateDependencyModuleNames.AddRange(new string[] { "MoviePlayer"});

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" });

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
