#pragma once
#include "SemanticAnalysis.h"

namespace cse
{
	namespace intellisense
	{
		ref class IntelliSenseItem;
		ref class Script;

		interface class IIntelliSenseInterfaceView;

		interface class IIntelliSenseInterfaceModel
		{
		public:
			static enum class Operation
			{
				Default = 0,
				Call,
				Dot,
				Assign,
				Snippet,
			};

			property List<IntelliSenseItem^>^		DataStore;

			void									Bind(IIntelliSenseInterfaceView^ To);
			void									Unbind();
		};

		interface class IIntelliSenseInterfaceView
		{
		public:
			static enum	class MoveDirection
			{
				Up,
				Down
			};

			event EventHandler^					ItemSelected;

			property bool						Visible;
			property IntelliSenseItem^			Selection;

			void				Bind(IIntelliSenseInterfaceModel^ To);
			void				Unbind();

			void				ChangeSelection(MoveDirection Direction);
			void				DimOpacity();
			void				ResetOpacity();

			void				Update();			// refreshes the item list
			void				Show(Point Location, IntPtr Parent);
			void				Hide();
		};
	}
}