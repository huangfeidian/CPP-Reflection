/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** ConstructorInvoker.h
** --------------------------------------------------------------------------*/

#pragma once
#include <utility>

#include "ConstructorInvokerBase.h"

#include "Logging.h"

namespace ursine
{
	namespace meta
	{
		class Variant;
		class Argument;

		template<typename ClassType, bool IsDynamic, bool IsObjectWrapped, typename ...ArgTypes>
		class ConstructorInvoker : public ConstructorInvokerBase
		{
		public:
			static_assert(THIS_ARG_COUNT <= MaxArgumentCount,
				"Constructor has too many arguments. It's time to generate more overloads."
				);

			Variant Invoke(const ArgumentList &arguments) override;
		};
	}
}
#include "Variant.h"
#include "Argument.h"
#include "ArgumentConfig.h"
namespace ursine
{
	namespace meta
	{
		template<typename ClassType, bool IsObjectWrapped, typename ...ArgTypes>
		class ConstructorInvoker<ClassType, false, IsObjectWrapped, ArgTypes...> : public ConstructorInvokerBase
		{
		public:
			static_assert(THIS_ARG_COUNT <= MaxArgumentCount,
				"Constructor has too many arguments. It's time to generate more overloads."
				);

			Variant Invoke(const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid constructor arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				return invoke<void, ArgTypes...>(arguments);
			}

		private:
			template<typename _, typename... args>
			Variant invoke(const ArgumentList &arguments) const
			{
				return invoke_impl<_, args...>(arguments, std::index_sequence_for<args...>{});
			}
			template<typename _, typename... args, size_t... arg_idxes>
			Variant invoke_impl(const ArgumentList &arguments, std::index_sequence<arg_idxes...>) const
			{
				return ClassType(arguments[arg_idxes].GetValue<args>()...);
			}
		};

		///////////////////////////////////////////////////////////////////////
		// Dynamic, Non Wrapped
		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename ...ArgTypes>
		class ConstructorInvoker<ClassType, true, false, ArgTypes...> : public ConstructorInvokerBase
		{
		public:

			static_assert(THIS_ARG_COUNT <= MaxArgumentCount,
				"Constructor has too many arguments. It's time to generate more overloads."
				);

			Variant Invoke(const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid constructor arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				return invoke<void, ArgTypes...>(arguments);
			}

		private:
			template<typename _, typename... args>
			Variant invoke(const ArgumentList &arguments) const
			{
				return invoke_impl<_, args...>(arguments, std::index_sequence_for<args...>{});
			}
			template<typename _, typename... args, size_t... arg_idxes>
			Variant invoke_impl(const ArgumentList &arguments, std::index_sequence<arg_idxes...>) const
			{
				return ClassType(arguments[arg_idxes].GetValue<args>()...);
			}

			
		};

		///////////////////////////////////////////////////////////////////////
		// Dynamic, Wrapped
		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename ...ArgTypes>
		class ConstructorInvoker<ClassType, true, true, ArgTypes...> : public ConstructorInvokerBase
		{
		public:
			static_assert(THIS_ARG_COUNT <= MaxArgumentCount,
				"Constructor has too many arguments. It's time to generate more overloads."
				);

			Variant Invoke(const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid constructor arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				return invoke<void, ArgTypes...>(arguments);
			}

		private:
			template<typename _, typename... args>
			Variant invoke(const ArgumentList &arguments) const
			{
				return invoke_impl<_, args>(arguments, std::index_sequence_for<args...>{});
			}
			template<typename _, typename... args, size_t... arg_idxes>
			Variant invoke_impl(const ArgumentList &arguments, std::index_sequence<arg_idxes...>) const
			{
				return ObjectVariant(new ClassType(arguments[arg_idxes].GetValue<args>()...));
			}
		};
    }
}