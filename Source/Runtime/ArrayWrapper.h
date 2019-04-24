/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** ArrayWrapper.h
** --------------------------------------------------------------------------*/

#pragma once

#include "ArrayWrapperBase.h"
#include "ArrayWrapperContainer.h"

namespace ursine
{
    namespace meta
    {
        class Argument;
        class ArrayWrapper
        {
			// 这个类型好像没啥大用
			// 除了提供初始化 get set insert remove 五个接口之外
			// 居然还用标记位来标明是否是const的
        public:
            ArrayWrapper(void);

            template<typename T>
            ArrayWrapper(Array<T> &rhs);

            template<typename T>
            ArrayWrapper(const Array<T> &rhs);

			Variant GetValue(size_t index) const;
            void SetValue(size_t index, const Argument &value);

            void Insert(size_t index, const Argument &value);
            void Remove(size_t index);

            size_t Size(void) const;

            bool IsValid(void) const;
            bool IsConst(void) const;
        private:
            bool m_isConst;

            ArrayWrapperBase *m_base;
        };

		template<typename T>
		ArrayWrapper::ArrayWrapper(Array<T> &rhs)
			: m_isConst(false)
			, m_base(new ArrayWrapperContainer<T>(rhs))
		{

		}

		template<typename T>
		ArrayWrapper::ArrayWrapper(const Array<T> &rhs)
			: m_isConst(true)
			, m_base(new ArrayWrapperContainer<T>(const_cast<Array<T> &>(rhs)))
		{

		}
    }
}