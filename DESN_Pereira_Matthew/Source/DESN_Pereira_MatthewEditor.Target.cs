// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DESN_Pereira_MatthewEditorTarget : TargetRules
{
	public DESN_Pereira_MatthewEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "DESN_Pereira_Matthew" } );
	}
}
