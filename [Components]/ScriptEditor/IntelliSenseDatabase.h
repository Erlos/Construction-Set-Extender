#pragma once

namespace cse
{
	namespace intellisense
	{
		ref class Script;
		ref class IntelliSenseItem;
		ref class UserFunction;
		ref class IntelliSenseInterface;
		ref struct IntelliSenseParseScriptData;
		ref class CodeSnippetCollection;
		ref class IntelliSenseItemScriptCommand;
		ref class IntelliSenseItemVariable;

		ref class IntelliSenseDatabase
		{
		protected:
			static IntelliSenseDatabase^						Singleton = nullptr;

			IntelliSenseDatabase();

			virtual void										DatabaseUpdateTimer_Tick(Object^ Sender, EventArgs^ E);

			Timer^												DatabaseUpdateTimer;
			LinkedList<UserFunction^>^							UserFunctionList;
			Dictionary<String^, String^>^						DeveloperURLMap;
			Dictionary<String^, Script^>^						RemoteScripts;				// key = baseEditorID
			CodeSnippetCollection^								CodeSnippets;
			UInt32												UpdateTimerInterval;		// in minutes
			LinkedList<IntelliSenseItemScriptCommand^>^			ScriptCommands;
			LinkedList<IntelliSenseItemVariable^>^				GameSettings;
			List<IntelliSenseItem^>^							Enumerables;

			void												UpdateDatabase();
		public:
			~IntelliSenseDatabase();

			static IntelliSenseDatabase^						GetSingleton();

			property List<IntelliSenseItem^>^					ItemRegistry
			{
				List<IntelliSenseItem^>^ get() { return Enumerables; }
			}

			virtual UInt32										InitializeCommandTableDatabase(componentDLLInterface::CommandTableData* Data);
			virtual void										InitializeGMSTDatabase(componentDLLInterface::IntelliSenseUpdateData* GMSTCollection);

			void												RegisterDeveloperURL(String^ CmdName, String^ URL);
			String^												LookupDeveloperURLByCommand(String^ CmdName);

			String^												SanitizeIdentifier(String^ Name);

			Script^												CacheRemoteScript(String^ BaseEditorID, String^ ScriptText);	// returns the cached script
			IntelliSenseItem^									LookupRemoteScriptVariable(String^ BaseEditorID, String^ Variable);

			bool												GetIsIdentifierUserFunction(String^ Name);
			bool												GetIsIdentifierScriptCommand(String^ Name);
			bool												GetIsIdentifierScriptableForm(String^ Name);
			bool												GetIsIdentifierScriptableForm(String^ Name,
																							  componentDLLInterface::ScriptData** OutScriptData);	// caller takes ownership of pointer
			bool												GetIsIdentifierForm(String^ Name);

			void												ForceUpdateDatabase();
			void												ShowCodeSnippetManager();
		};

#define ISDB											IntelliSenseDatabase::GetSingleton()
	}
}