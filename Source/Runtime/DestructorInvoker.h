/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** DestructorInvoker.h
** --------------------------------------------------------------------------*/

#pragma once

#include "DestructorInvokerBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType>
        class DestructorInvoker : public DestructorInvokerBase
        {
        public:
            void Invoke(const Variant &obj) override;
        };
		template<typename ClassType>
		void DestructorInvoker<ClassType>::Invoke(const Variant& obj)
		{
			auto &instance = obj.GetValue<ClassType>();

			instance.~ClassType();
		}
    }
}