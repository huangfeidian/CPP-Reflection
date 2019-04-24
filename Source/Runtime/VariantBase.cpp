/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** VariantBase.cpp
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VariantBase.h"

#include "ArrayWrapper.h"

namespace ursine
{
    namespace meta
    {
        bool VariantBase::IsArray(void) const
        {
            return false;
        }

        void VariantBase::GetArray(ArrayWrapper& _wrapper) const
        {
            // invalid wrapper
			return;
        }
    }
}