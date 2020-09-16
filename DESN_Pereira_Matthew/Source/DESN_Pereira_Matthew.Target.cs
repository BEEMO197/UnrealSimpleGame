// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DESN_Pereira_MatthewTarget : TargetRules
{
	public DESN_Pereira_MatthewTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "DESN_Pereira_Matthew" } );
	}
}
