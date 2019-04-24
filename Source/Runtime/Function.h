/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Function.h
** --------------------------------------------------------------------------*/

#pragma once

#include "MetaContainer.h"
#include "Invokable.h"

#include "Variant.h"
#include "Argument.h"

#include "FunctionInvoker.h"

#include <functional>

namespace ursine
{
    namespace meta
    {
        class Function
            : public MetaContainer
            , public Invokable
        {
        public:
            typedef std::function<Variant(ArgumentList&)> Invoker;

            Function(void);

            template<typename ReturnType, typename ...ArgTypes>
            Function(
                const std::string &name, 
                ReturnType (*function)(ArgTypes...),
                Type parentType = Type::Invalid( )
            );

            static const Function &Invalid(void);

            Type GetParentType(void) const;

            bool IsValid(void) const;

            Variant InvokeVariadic(ArgumentList &arguments) const;

            template<typename ...Args>
            Variant Invoke(Args &&...args) const;

        private:
            Type m_parentType;

            std::shared_ptr<FunctionInvokerBase> m_invoker;
        };

		template<typename ReturnType, typename ...ArgTypes>
		Function::Function(
			const std::string &name,
			ReturnType(*function)(ArgTypes...),
			Type parentType
		)
			: Invokable(name)
			, m_parentType(parentType)
			, m_invoker(new FunctionInvoker<ReturnType, ArgTypes...>(function))
		{
			TypeUnpacker<ArgTypes...>::Apply(m_signature);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ...Args>
		Variant Function::Invoke(Args &&...args) const
		{
			ArgumentList arguments{ std::forward<Args>(args)... };

			return InvokeVariadic(arguments);
		}
    }
}