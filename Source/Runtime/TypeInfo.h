/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** TypeInfo.h
** --------------------------------------------------------------------------*/

#pragma once

#include "TypeConfig.h"

#include "../Common/Compiler.h"

#include <type_traits>

#if defined(COMPILER_CLANG) || defined(COMPILER_GNU)

#define IsTriviallyDefaultConstructible(x) std::has_trivial_default_constructor<x>::value

#else

#define IsTriviallyDefaultConstructible(x) std::is_trivially_default_constructible<x>::value

#endif

namespace ursine
{
    namespace meta
    {
        struct TypeData;

        template<typename T>
        struct TypeInfo
        {
            static bool Defined;

            static void Register(TypeID id, TypeData &data, bool beingDefined);

        private:
            template<typename U = T>
            static void addDefaultConstructor(
                TypeData &data, 
                typename std::enable_if<
                    !IsTriviallyDefaultConstructible(U)
                >::type* = nullptr
            );

            template<typename U = T>
            static void addDefaultConstructor(
                TypeData &data, 
                typename std::enable_if<
                    IsTriviallyDefaultConstructible(U)
                >::type* = nullptr
            );

            template<typename U = T>
            static void applyTrivialAttributes(TypeData &data, 
                typename std::enable_if< 
                    !std::is_trivial<U>::value 
                >::type* = nullptr
            );

            template<typename U = T>
            static void applyTrivialAttributes(TypeData &data, 
                typename std::enable_if< 
                    std::is_trivial<U>::value 
                >::type* = nullptr
            );
        };

		template<typename T>
		bool TypeInfo<T>::Defined = false;

		template<typename T>
		void TypeInfo<T>::Register(
			TypeID id,
			TypeData &data,
			bool beingDefined
		)
		{
			// 这里才是真正的注册所有类型的地方
			// already defined
			if (id == Type::Invalid().GetID())
				return;

			TypeIDs<T>::ID = id;

			typedef typename std::remove_pointer< typename std::decay<T>::type >::type Decayed;

			data.isClass = std::is_class< Decayed >::value;
			data.isEnum = std::is_enum< Decayed >::value;
			data.isPointer = std::is_pointer< T >::value;
			data.isPrimitive = std::is_arithmetic< Decayed >::value;
			data.isFloatingPoint = std::is_floating_point< Decayed >::value;
			data.isSigned = std::is_signed< Decayed >::value;

			if (beingDefined)
			{
				addDefaultConstructor(data);

				applyTrivialAttributes(data);
			}
		}

		template<typename T>
		template<typename U>
		void TypeInfo<T>::addDefaultConstructor(
			TypeData &data,
			typename std::enable_if<
			!IsTriviallyDefaultConstructible(U)
			>::type*
		)
		{
			// do nothing
		}

		template<typename T>
		template<typename U>
		void TypeInfo<T>::addDefaultConstructor(
			TypeData &data,
			typename std::enable_if<
			IsTriviallyDefaultConstructible(U)
			>::type*
		)
		{
			// add the good 'ol default constructor
			data.AddConstructor<T, false, false>({ });

			// add the good 'ol dynamic default constructor
			data.AddConstructor<T, true, false>({ });
		}

		template<typename T>
		template<typename U>
		void TypeInfo<T>::applyTrivialAttributes(
			TypeData &data,
			typename std::enable_if<
			!std::is_trivial<U>::value
			>::type*
		)
		{
			// do nothing
		}

		template<typename T>
		template<typename U>
		void TypeInfo<T>::applyTrivialAttributes(
			TypeData &data,
			typename std::enable_if<
			std::is_trivial<U>::value
			>::type*
		)
		{
			data.SetDestructor<T>();
		}
	}
}
