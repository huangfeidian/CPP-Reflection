/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** ArrayVariantContainer.h
** --------------------------------------------------------------------------*/

#pragma once

#include "VariantBase.h"

#include "ArrayWrapper.h"

namespace ursine
{
	namespace meta
	{

		template<typename T, typename StorageType>
		class ArrayVariantContainer : public VariantBase
		{
		public:
			ArrayVariantContainer(StorageType &rhs);

			Type GetType(void) const override;
			void *GetPtr(void) const override;

			int ToInt(void) const override;
			bool ToBool(void) const override;
			float ToFloat(void) const override;
			double ToDouble(void) const override;
			std::string ToString(void) const override;

			bool IsArray(void) const override;
			void GetArray(ArrayWrapper& _wrapper) const override;

			VariantBase *Clone(void) const override;

		private:
			StorageType m_array;
		};
		template<typename T, typename StorageType>
		ArrayVariantContainer<T, StorageType>::ArrayVariantContainer(StorageType &rhs)
			: m_array(rhs)
		{

		}

		template<typename T, typename StorageType>
		Type ArrayVariantContainer<T, StorageType>::GetType(void) const
		{
			return meta_typeof(Array<T>);
		}

		template<typename T, typename StorageType>
		void *ArrayVariantContainer<T, StorageType>::GetPtr(void) const
		{
			return const_cast<void*>(
				reinterpret_cast<const void*>(
					std::addressof(m_array)
					)
				);
		}

		template<typename T, typename StorageType>
		int ArrayVariantContainer<T, StorageType>::ToInt(void) const
		{
			return int();
		}

		template<typename T, typename StorageType>
		bool ArrayVariantContainer<T, StorageType>::ToBool(void) const
		{
			return bool();
		}

		template<typename T, typename StorageType>
		float ArrayVariantContainer<T, StorageType>::ToFloat(void) const
		{
			return float();
		}

		template<typename T, typename StorageType>
		double ArrayVariantContainer<T, StorageType>::ToDouble(void) const
		{
			return double();
		}

		template<typename T, typename StorageType>
		std::string ArrayVariantContainer<T, StorageType>::ToString(void) const
		{
			return std::string();
		}

		template<typename T, typename StorageType>
		bool ArrayVariantContainer<T, StorageType>::IsArray(void) const
		{
			return true;
		}

		template<typename T, typename StorageType>
		void ArrayVariantContainer<T, StorageType>::GetArray(ArrayWrapper& _wrapper) const
		{
			_wrapper = ArrayWrapper(const_cast<Array<T>&>(m_array));
			return;
		}

		template<typename T, typename StorageType>
		VariantBase *ArrayVariantContainer<T, StorageType>::Clone(void) const
		{
			return new ArrayVariantContainer<T, StorageType>(const_cast<Array<T>&>(m_array));
		}
	}
}