#pragma once

#include "..\CSECoda.h"

using namespace BGSEditorExtender::BGSEEScript;
using namespace BGSEditorExtender::BGSEEScript::Commands;

namespace ConstructionSetExtender
{
	namespace BGSEEScript
	{
		namespace Commands
		{
			namespace Reference
			{
				CodaScriptCommandRegistrarDecl;

				CodaScriptCommandPrototype(CreateRef,
					0,
					"Creates an object reference in the passed cell. The worldspace parameter is ignored for interior cells.",
					0,
					9,
					ICodaScriptDataStore::kDataType_Reference);

				CodaScriptCommandPrototype(GetRefPosition,
					0,
					"Returns the position of the reference in the given axis.",
					0,
					2,
					ICodaScriptDataStore::kDataType_Numeric);

				CodaScriptCommandPrototype(GetRefRotation,
					0,
					"Returns the rotation of the reference in the given axis.",
					0,
					2,
					ICodaScriptDataStore::kDataType_Numeric);

				CodaScriptParametricCommandPrototype(GetRefScale,
					0,
					"Returns the scale of the reference.",
					0,
					1,
					OneForm,
					ICodaScriptDataStore::kDataType_Numeric);

				CodaScriptParametricCommandPrototype(GetRefPersistent,
					0,
					"Returns the persistence of the reference.",
					0,
					1,
					OneForm,
					ICodaScriptDataStore::kDataType_Numeric);

				CodaScriptParametricCommandPrototype(GetRefDisabled,
					0,
					"Returns the disabled state of the reference.",
					0,
					1,
					OneForm,
					ICodaScriptDataStore::kDataType_Numeric);

				CodaScriptParametricCommandPrototype(GetRefVWD,
					0,
					"Returns the visible when distant state of the reference.",
					0,
					1,
					OneForm,
					ICodaScriptDataStore::kDataType_Numeric);

				CodaScriptParametricCommandPrototype(GetRefBaseForm,
					0,
					"Returns the base form of the reference.",
					0,
					1,
					OneForm,
					ICodaScriptDataStore::kDataType_Reference);

				CodaScriptCommandPrototype(SetRefPosition,
					0,
					"Sets the position of the reference in the given axis.",
					0,
					3,
					ICodaScriptDataStore::kDataType_Invalid);

				CodaScriptCommandPrototype(SetRefRotation,
					0,
					"Sets the rotation of the reference in the given axis.",
					0,
					3,
					ICodaScriptDataStore::kDataType_Invalid);

				CodaScriptParametricCommandPrototype(SetRefScale,
					0,
					"Sets the scale of the reference.",
					0,
					2,
					FormNumber,
					ICodaScriptDataStore::kDataType_Invalid);

				CodaScriptParametricCommandPrototype(SetRefPersistent,
					0,
					"Sets the persistence of the reference.",
					0,
					2,
					FormNumber,
					ICodaScriptDataStore::kDataType_Invalid);

				CodaScriptParametricCommandPrototype(SetRefDisabled,
					0,
					"Sets the disabled state of the reference.",
					0,
					2,
					FormNumber,
					ICodaScriptDataStore::kDataType_Invalid);

				CodaScriptParametricCommandPrototype(SetRefVWD,
					0,
					"Sets the visible when distant state of the reference.",
					0,
					2,
					FormNumber,
					ICodaScriptDataStore::kDataType_Invalid);

				CodaScriptCommandPrototype(GetCellObjects,
					0,
					"Returns an array of references that present in the given cell.",
					0,
					1,
					ICodaScriptDataStore::kDataType_Array);

				CodaScriptSimpleCommandPrototype(GetCurrentRenderWindowSelection,
					0,
					"Returns an array of references that are currently selected in the render window.",
					0,
					ICodaScriptDataStore::kDataType_Array);
			}
		}
	}
}