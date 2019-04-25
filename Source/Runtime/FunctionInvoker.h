/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** FunctionInvoker.h
** --------------------------------------------------------------------------*/

#pragma once

#include "FunctionInvokerBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ReturnType, typename ...ArgTypes>
        class FunctionInvoker : public FunctionInvokerBase
        {
        public:
            typedef ReturnType (*Signature)(ArgTypes...);

            static_assert( THIS_ARG_COUNT <= MaxArgumentCount,
                "Function has too many arguments. It's time to generate more overloads." 
            );

			FunctionInvoker(Signature function)
				:m_function(function)
			{

			}

			Variant Invoke(const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid function arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				return invoke<void, ArgTypes...>(arguments);
			}

        private:
            template<typename _, typename... args>
			Variant invoke(const ArgumentList &arguments)
			{
				return invoke_impl<_, args...>(arguments);
			}

            template<typename _, typename... args, size_t... arg_idxes>
			Variant invoke_impl(const ArgumentList &arguments, std::index_sequence<arg_idxes...>)
			{
				return m_function(
					arguments[arg_idxes].GetValue<args>()...
				);
			}

			

            Signature m_function;
        };


		

		template<typename ...ArgTypes>
		class FunctionInvoker<void, ArgTypes...> : public FunctionInvokerBase
		{
		public:
			typedef void(*Signature)(ArgTypes...);

			static_assert(THIS_ARG_COUNT <= MaxArgumentCount,
				"Function has too many arguments. It's time to generate more overloads."
				);

			FunctionInvoker(Signature function)
				: m_function(function) { }

			Variant Invoke(const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid function arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				invoke<void, ArgTypes...>(arguments);

				return { };
			}

		private:
			template<typename _, typename... args>
			void invoke(const ArgumentList &arguments)
			{
				invoke_impl<_, args...>(arguments);
			}

			template<typename _, typename... args, size_t... arg_idxes>
			void invoke_impl(const ArgumentList &arguments, std::index_sequence<arg_idxes...>)
			{
				m_function(
					arguments[arg_idxes].GetValue<args>()...
				);
			}
			Signature m_function;
		};
    }
}