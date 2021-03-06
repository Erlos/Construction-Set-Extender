#pragma once

#include "TESObject.h"

//	EditorAPI: TESObjectAPPA class.

/*
	...
*/

// C8
class TESObjectAPPA : public TESBoundObject, public TESFullName, public TESModel, public TESIcon, public TESScriptableForm, public TESValueForm, public TESWeightForm, public TESQualityForm
{
public:
	enum ApparatusType
	{
		kApparatusType_MortarPestle			= 0x0,
		kApparatusType_Alembic,
		kApparatusType_Calcinator,
		kApparatusType_Retort,
		kApparatusType__MAX
	};

	// members
	//     /*00*/ TESBoundObject
	//     /*58*/ TESFullName
	//     /*64*/ TESModel
	//	   /*88*/ TESIcon
	//	   /*A0*/ TESScriptableForm
	//	   /*AC*/ TESValueForm
	//	   /*B4*/ TESWeightForm
	//	   /*BC*/ TESQualityForm
	/*C4*/ UInt8				apparatusType;
	/*C5*/ UInt8				padC5[3];
};