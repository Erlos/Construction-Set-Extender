#pragma once

#include "[Common]\HandShakeStructs.h"
#include "SemanticAnalysis.h"
#include "ScriptTextEditorInterface.h"

namespace cse
{
	namespace intellisense
	{
		ref class IntelliSenseInterface;
		ref class CodeSnippet;

		ref class IntelliSenseItem
		{
			static array<String^>^ IntelliSenseItemTypeID =
			{
				"Unknown Object",
				"Command",
				"Local Variable",
				"Remote Variable",
				"Global Variable",
				"Quest",
				"User Defined Function",
				"Game Setting",
				"Object",
				"Code Snippet"
			};
		public:
			static enum class IntelliSenseItemType
			{
				Invalid = 0,
				Command,
				LocalVar,
				RemoteVar,
				GlobalVar,
				Quest,
				UserFunction,
				GMST,
				Form,
				Snippet,
			};

			IntelliSenseItem();
			IntelliSenseItem(String^ Desc, IntelliSenseItemType Type);

			virtual String^										Describe();
			virtual void										Insert(textEditors::IScriptTextEditor^ Editor);

																// returns true if the item can be enumerated in the interface
			virtual bool										GetShouldEnumerate(String^ Token, bool SubstringSearch);

																// returns true if the item allows a quick view tooltip and the token matches
			virtual bool										GetIsQuickViewable(String^ Token);

																// identifier for display in the interface
			virtual String^										GetIdentifier() abstract;

																// string to be inserted into the code
			virtual String^										GetSubstitution() abstract;

			IntelliSenseItemType								GetItemType();
			String^												GetItemTypeID();
		protected:
			String^												Description;
			IntelliSenseItemType								Type;
		};

		ref class IntelliSenseItemScriptCommand : public IntelliSenseItem
		{
		public:
			static enum class IntelliSenseCommandItemSourceType
			{
				Vanilla = 0,
				OBSE
			};
		private:
			static array<String^>^ IntelliSenseItemCommandReturnTypeID =
			{
				"Numeric",
				"Form",
				"String",
				"Array",
				"Array [Reference]",
				"Ambiguous"
			};
			static enum class IntelliSenseItemCommandReturnType
			{
				Default = 0,
				Form,
				String,
				Array,
				ArrayIndex,
				Ambiguous
			};

			String^												Name;
			String^												CmdDescription;
			String^												Shorthand;
			UInt16												ParamCount;
			bool												RequiresParent;
			UInt16												ReturnType;
			IntelliSenseCommandItemSourceType					Source;
		public:
			IntelliSenseItemScriptCommand(String^ Name,
										  String^ Desc,
										  String^ Shorthand,
										  UInt16 NoOfParams,
										  bool RequiresParent,
										  UInt16 ReturnType,
										  IntelliSenseCommandItemSourceType Source,
										  String^ Params);

			virtual bool										GetShouldEnumerate(String^ Token, bool SubstringSearch) override;
			virtual String^										GetIdentifier() override;
			virtual String^										GetSubstitution() override;
			virtual bool										GetIsQuickViewable(String^ Token) override;
			bool												GetRequiresParent();
			IntelliSenseCommandItemSourceType					GetSource();
			String^												GetShorthand();
		};

		ref class IntelliSenseItemVariable : public IntelliSenseItem
		{
			String^												Name;
			obScriptParsing::Variable::DataType					DataType;
			String^												Comment;
		public:
			IntelliSenseItemVariable(String^ Name, String^ Comment, obScriptParsing::Variable::DataType Type, IntelliSenseItemType Scope);

			virtual String^										GetIdentifier() override;
			virtual String^										GetSubstitution() override;
			String^												GetComment();
			obScriptParsing::Variable::DataType					GetDataType();
			String^												GetDataTypeID();
		};

		ref class IntelliSenseItemQuest : public IntelliSenseItem
		{
		protected:
			String^												Name;
			String^												ScriptName;
		public:
			IntelliSenseItemQuest(String^ EditorID, String^ Desc, String^ ScrName);

			virtual String^										GetIdentifier() override;
			virtual String^										GetSubstitution() override;
		};

		ref class Script
		{
		public:
			typedef List<IntelliSenseItemVariable^>				VarListT;
		protected:
			Script();
			Script(String^ ScriptText, String^ Name);

			VarListT^											VarList;
			String^												Name;
			String^												CommentDescription;
		public:
			static Script^										NullScript = gcnew Script(gcnew String("scn nullscript"));

			Script(String^ ScriptText);

			virtual String^										Describe();
			virtual String^										GetIdentifier();

			List<IntelliSenseItemVariable^>::Enumerator^		GetVariableListEnumerator();
		};

		ref class UserFunction : public Script
		{
		protected:
			Array^												Parameters;			// indices of the parameters in VarList
			int													ReturnVar;			// index of the return var. -9 for ambiguous retn values
		public:
			UserFunction(String^ ScriptText);

			virtual String^										Describe() override;
		};

		ref class IntelliSenseItemUserFunction : public IntelliSenseItem
		{
			UserFunction^										Parent;
		public:
			IntelliSenseItemUserFunction(UserFunction^ Parent);

			virtual String^										GetIdentifier() override;
			virtual String^										GetSubstitution() override;
		};

		ref class IntelliSenseItemEditorIDForm : public IntelliSenseItem
		{
		protected:
			static enum class FormFlags
			{
				FromMaster           = /*00*/ 0x00000001,
				FromActiveFile       = /*01*/ 0x00000002,
				Deleted              = /*05*/ 0x00000020,
				TurnOffFire          = /*07*/ 0x00000080,
				QuestItem            = /*0A*/ 0x00000400,
				Disabled             = /*0B*/ 0x00000800,
				Ignored              = /*0C*/ 0x00001000,
				Temporary            = /*0E*/ 0x00004000,
				VisibleWhenDistant   = /*0F*/ 0x00008000,
			};

			String^												Name;
			UInt32												TypeID;
			UInt32												FormID;
			UInt32												Flags;

			String^												GetFormTypeIdentifier();
		public:
			IntelliSenseItemEditorIDForm(componentDLLInterface::FormData* Data);

			virtual String^										GetIdentifier() override;
			virtual String^										GetSubstitution() override;
		};

		ref class IntelliSenseItemCodeSnippet : public IntelliSenseItem
		{
		protected:
			CodeSnippet^										Parent;
		public:
			IntelliSenseItemCodeSnippet(CodeSnippet^ Source);

			virtual void										Insert(textEditors::IScriptTextEditor^ Editor) override;
			virtual bool										GetShouldEnumerate(String^ Token, bool SubstringSearch) override;
			virtual bool										GetIsQuickViewable(String^ Token) override;
			virtual String^										GetIdentifier() override;
			virtual String^										GetSubstitution() override;
		};

		ref class IntelliSenseItemSorter : public System::Collections::Generic::IComparer<IntelliSenseItem^>
		{
		protected:
			SortOrder	Order;
		public:
			IntelliSenseItemSorter(SortOrder Order) : Order(Order) {}

			virtual int Compare(IntelliSenseItem^ X, IntelliSenseItem^ Y)
			{
				int Result = X->GetItemType() < Y->GetItemType();
				if (X->GetItemType() == Y->GetItemType());
					Result = String::Compare(X->GetIdentifier(), Y->GetIdentifier(), true);

				if (Order == SortOrder::Descending)
					Result *= -1;

				return Result;
			}
		};
	}
}