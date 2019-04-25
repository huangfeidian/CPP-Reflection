/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Argument.h
** --------------------------------------------------------------------------*/

#pragma once
#include <vector>

#include "TypeConfig.h"

#include "Variant.h"



namespace ursine
{
    namespace meta
    {
        class Type;
        class Variant;

        class Argument
        {
        public:
            Argument(void);
            Argument(const Argument &rhs);
            Argument(Variant &obj);
            Argument(const Variant &obj);

            template<typename T>
            Argument(const T &data);

            template<typename T>
            Argument(T &data);

            Argument &operator=(const Argument &rhs);

            Type GetType(void) const;

            void *GetPtr(void) const;

            template<typename T>
            T &GetValue(void) const;

        private:
            const TypeID m_typeID;
            const bool m_isArray;

            const void *m_data;
        };
		template<typename T>
		Argument::Argument(const T &data)
			: m_typeID(typeidof(T))
			, m_isArray(meta_traits::IsArray<T>::value)
			, m_data(reinterpret_cast<const void*>(std::addressof(data)))
		{
			static_assert(!std::is_same< Argument, T >::value,
				"Cannot use Argument as an argument"
				);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		Argument::Argument(T &data)
			: m_typeID(typeidof(T))
			, m_isArray(meta_traits::IsArray<T>::value)
			, m_data(reinterpret_cast<const void*>(std::addressof(data)))
		{
			static_assert(!std::is_same< Argument, T >::value,
				"Cannot use Argument as an argument"
				);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		T &Argument::GetValue(void) const
		{
			return *reinterpret_cast<
				typename std::remove_reference< T >::type*
			>(
				const_cast<void *>(m_data)
				);
		}
    }

}