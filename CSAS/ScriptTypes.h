#pragma once

#ifndef CSE
	namespace CSAutomationScript
	{
	// use the CSEScriptInterface to initialize the following global fn ptrs
		#pragma region Internal Use Only
			typedef Array*							(* _CSASArrayInterface_AllocateNewArray)(CSASDataElement* Element, UInt32 Size);
			typedef Array*							(* _CSASArrayInterface_CopyNewArray)(Array* Source);
			typedef void							(* _CSASArrayInterface_ReleaseArray)(Array* Source);

			extern _CSASArrayInterface_AllocateNewArray	CSASArrayInterface_AllocateNewArray;
			extern _CSASArrayInterface_CopyNewArray		CSASArrayInterface_CopyNewArray;
			extern _CSASArrayInterface_ReleaseArray		CSASArrayInterface_ReleaseArray;
		#pragma endregion
	}
#endif

class TESForm;

namespace CSAutomationScript
{
	class Array;

	class CSASDataElement
	{
#ifdef CSE
		static UInt32					GlobalInstanceCount;
#endif
	public:
		enum
		{
			kParamType_Invalid = 0,
			kParamType_Numeric,
			kParamType_Reference,
			kParamType_String,
			kParamType_Array							// return type only
		};
	protected:
		union
		{
			TESForm*					RefData;
			double						NumericData;
			char*						StringData;
			Array*						ArrayData;		// every Array* points to an unique instance
		};

		UInt32							Type;

		virtual void					Reset();

		CSASDataElement(const CSASDataElement& rhs);
		CSASDataElement& operator=(const CSASDataElement& rhs);
	public:
		UInt8							GetType() const { return Type; }
		TESForm*						GetForm() const { ASSERT(Type == kParamType_Reference); return RefData; }
		double							GetNumber() const { ASSERT(Type == kParamType_Numeric); return NumericData; }
		const char*						GetString() const { ASSERT(Type == kParamType_String); return StringData; }
		Array*							GetArray() const { ASSERT(Type == kParamType_Array); return ArrayData; }

		void							SetForm(TESForm* Data);
		void							SetNumber(double Data);
		void							SetString(const char* Data);
		void							SetArray(Array* Data);
		void							SetArray(CSASDataElement* Elements, UInt32 Size);

		CSASDataElement() : Type(kParamType_Invalid) {}
		CSASDataElement(CSASDataElement* Data);
		CSASDataElement(double Num) : NumericData(Num), Type(kParamType_Numeric) {}
		CSASDataElement(const char* Str) { SetString(Str); }
		CSASDataElement(TESForm* Form) : RefData(Form), Type(kParamType_Reference) {}
		CSASDataElement(Array* Array) { SetArray(Array); }
		CSASDataElement(CSASDataElement* Elements, UInt32 Size) { SetArray(Elements, Size); }

		virtual ~CSASDataElement() { Reset(); }

#ifdef CSE
		static UInt32					GetGlobalInstanceCount() { return GlobalInstanceCount; }
#endif
	};
}
