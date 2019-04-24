/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** MethodInvoker.h
** --------------------------------------------------------------------------*/

#pragma once

#include "MethodInvokerBase.h"

#include <utility>

namespace ursine
{
    namespace meta
    {
        template<typename ClassType, typename ReturnType, typename ...ArgTypes>
        class MethodInvoker : public MethodInvokerBase
        {
        public:
            typedef ReturnType (ClassType::*Signature)(ArgTypes...);
            typedef ReturnType (ClassType::*ConstSignature)(ArgTypes...) const;

            //static_assert( THIS_ARG_COUNT <= MaxArgumentCount,
            //    "Method has too many arguments. It's time to generate more overloads." 
            //);

			MethodInvoker(Signature method)
				:m_method(reinterpret_cast<ConstSignature>(method))
			{

			}
			MethodInvoker(ConstSignature method)
				:m_method(method)
			{

			}

			Variant Invoke(Variant &obj, const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid method arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				return invoke<void, ArgTypes...>(obj, arguments);
			}

        private:
			template<typename _, typename... args, size_t... arg_idxes>
			Variant invoke_impl(Variant &obj, const ArgumentList &arguments, std::index_sequence<arg_idxes...>)
			{
				auto &instance = obj.GetValue<ClassType>();

				return (instance.*m_method)(
					argument[arg_idxes].GetValue<args>()...
					);
			}
			template<typename _, typename... args>
			Variant invoke(Variant &obj, const ArgumentList &arguments)
			{
				return invoke_impl<_, args...>(obj, Argument, std::index_sequence_for<args...>{})
			}
			

            ConstSignature m_method;
        };

		// 下面这个类是用来给void返回值特化的
		template<typename ClassType, typename ...ArgTypes>
		class MethodInvoker<ClassType, void, ArgTypes...> : public MethodInvokerBase
		{
		public:
			typedef void (ClassType::*Signature)(ArgTypes...);
			typedef void (ClassType::*ConstSignature)(ArgTypes...) const;

			//static_assert(THIS_ARG_COUNT <= MaxArgumentCount,
			//	"Method has too many arguments. It's time to generate more overloads."
			//	);

			MethodInvoker(Signature method)
				: m_method(reinterpret_cast<ConstSignature>(method)) { }

			MethodInvoker(ConstSignature method)
				: m_method(method) { }

			Variant Invoke(Variant &obj, const ArgumentList &arguments) override
			{
				UAssert(arguments.size() == THIS_ARG_COUNT,
					"Invalid method arguments.\nExpected %i args but got %i.",
					THIS_ARG_COUNT,
					arguments.size()
				);

				invoke<void, ArgTypes...>(obj, arguments);

				return { };
			}

		private:
			template<typename X, typename... args>
			void invoke(Variant &obj, const ArgumentList &arguments)
			{
				return invoke_impl<X, args...>(obj, arguments, std::index_sequence_for<args...>{})
			}

			template<typename X, typename... args, size_t... arg_idxes>
			void invoke_impl(Variant &obj, const ArgumentList &arguments, std::index_sequence<arg_idxes...>)
			{
				auto &instance = obj.GetValue<ClassType>();

				(instance.*m_method)(
					arguments[arg_idxes].GetValue<args>()...
					);
			}
			ConstSignature m_method;
		};
    }
}
