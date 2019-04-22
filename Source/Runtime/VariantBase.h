/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** VariantBase.h
** --------------------------------------------------------------------------*/

#pragma once

#include <string>

#include "JsonConfig.h"

namespace ursine
{
    namespace meta
    {
        class Type;
        class ArrayWrapper;

        class VariantBase
        {
			// 这个是用来存储特定类型基础值的纯虚类
			// 下面有array的继承版本
        public:
            virtual ~VariantBase(void) { }

            virtual Type GetType(void) const = 0;

            virtual void *GetPtr(void) const = 0;

            virtual int ToInt(void) const = 0;
            virtual bool ToBool(void) const = 0;
            virtual float ToFloat(void) const = 0;
            virtual double ToDouble(void) const = 0;
            virtual std::string ToString(void) const = 0;

            virtual bool IsArray(void) const;
            virtual ArrayWrapper GetArray(void) const;

            virtual VariantBase *Clone(void) const = 0;

            virtual void OnSerialize(Json &output) const { }
            virtual void OnDeserialize(const Json &input) { }
        };
    }
}