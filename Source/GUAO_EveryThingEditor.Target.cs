// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GUAO_EveryThingEditorTarget : TargetRules
{
	public GUAO_EveryThingEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "GUAO_EveryThing" } );
	}
}