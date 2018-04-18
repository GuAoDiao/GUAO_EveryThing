// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GUAO_EveryThingTarget : TargetRules
{
	public GUAO_EveryThingTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "GUAO_EveryThing" } );
	}
}
