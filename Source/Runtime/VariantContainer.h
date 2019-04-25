/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** VariantContainer.h
** --------------------------------------------------------------------------*/

#pragma once

#include "VariantBase.h"

#include "MetaTraits.h"

#include "Object.h"
#include "Type.h"


#define DEFAULT_TYPE_HANDLER(typeName)                                                           \
    template<typename U = T>                                                                     \
    typeName get##typeName(                                                                      \
        typename std::enable_if<                                                                 \
            !std::is_convertible<typename meta_traits::TypeOrEnumType<U>::type, typeName>::value \
        >::type* = nullptr                                                                       \
    ) const;                                                                                     \
    template<typename U = T>                                                                     \
    typeName get##typeName(                                                                      \
        typename std::enable_if<                                                                 \
            std::is_convertible<typename meta_traits::TypeOrEnumType<U>::type, typeName>::value  \
        >::type* = nullptr                                                                       \
    ) const;                                                                                     \

#define DEFAULT_TYPE_HANDLER_IMPL(typeName)                                                      \
    template<typename T>                                                                         \
    template<typename U>                                                                         \
    typeName VariantContainer<T>::get##typeName(                                                 \
        typename std::enable_if<                                                                 \
            !std::is_convertible<typename meta_traits::TypeOrEnumType<U>::type, typeName>::value \
        >::type*                                                                                 \
        ) const                                                                                  \
    {                                                                                            \
        return typeName( );                                                                      \
    }                                                                                            \
    template<typename T>                                                                         \
    template<typename U>                                                                         \
    typeName VariantContainer<T>::get##typeName(                                                 \
        typename std::enable_if<                                                                 \
            std::is_convertible<typename meta_traits::TypeOrEnumType<U>::type, typeName>::value  \
        >::type*                                                                                 \
    ) const                                                                                      \
    {                                                                                            \
        return static_cast<typeName>( m_value );                                                 \
    }                                                                                            \


#define DECLARE_STANDARD_VARIANT(type)                                     \
    template<>                                                             \
    class VariantContainer<type> : public VariantBase                      \
    {                                                                      \
    public:                                                                \
        VariantContainer(const type &value);                               \
        VariantContainer(const type &&value);                              \
                                                                           \
        Type GetType(void) const override;                                 \
        void *GetPtr(void) const override;                                 \
                                                                           \
        int ToInt(void) const override;                                    \
        bool ToBool(void) const override;                                  \
        float ToFloat(void) const override;                                \
        double ToDouble(void) const override;                              \
        std::string ToString(void) const override;                         \
                                                                           \
        VariantBase *Clone(void) const override;                           \
                                                                           \
    private:                                                               \
        friend class Variant;                                              \
                                                                           \
        VariantContainer &operator=(const VariantContainer &rhs) = delete; \
                                                                           \
        type m_value;                                                      \
    };                                                                     \

namespace ursine
{
	namespace meta
	{
		template<typename T>
		class VariantContainer : public VariantBase
		{
		public:
			typedef typename std::remove_reference<T>::type NonRefType;

			VariantContainer(const NonRefType &value);
			VariantContainer(const NonRefType &&value);

			Type GetType(void) const override;

			void *GetPtr(void) const override;

			int ToInt(void) const override;
			bool ToBool(void) const override;
			float ToFloat(void) const override;
			double ToDouble(void) const override;
			std::string ToString(void) const override;

			VariantBase *Clone(void) const override;

			void OnSerialize(Json &output) const override;
			void OnDeserialize(const Json &input) override;
		private:
			friend class Variant;

			T m_value;

			VariantContainer &operator=(const VariantContainer &rhs) = delete;

			DEFAULT_TYPE_HANDLER(int);
			DEFAULT_TYPE_HANDLER(bool);
			DEFAULT_TYPE_HANDLER(float);
			DEFAULT_TYPE_HANDLER(double);

			template<typename U = T>
			std::string getString(
				typename std::enable_if<
				!std::is_arithmetic<U>::value
				>::type* = nullptr
			) const;

			template<typename U = T>
			std::string getString(
				typename std::enable_if<
				std::is_arithmetic<U>::value
				>::type* = nullptr
			) const;

			template<typename U = T>
			void onSerialize(
				Json &output,
				typename std::enable_if<
				!std::is_pointer<U>::value && std::is_base_of<Object, U>::value
				>::type* = nullptr
			) const;

			template<typename U = T>
			void onSerialize(
				Json &output,
				typename std::enable_if<
				std::is_pointer<U>::value || !std::is_base_of<Object, U>::value
				>::type* = nullptr
			) const;

			template<typename U = T>
			void onDeserialize(
				const Json &input,
				typename std::enable_if<
				!std::is_pointer<U>::value && std::is_base_of<Object, U>::value
				>::type* = nullptr
			);

			template<typename U = T>
			void onDeserialize(
				const Json &input,
				typename std::enable_if<
				std::is_pointer<U>::value || !std::is_base_of<Object, U>::value
				>::type* = nullptr
			);
		};
	}
}
#include "Type.h"
#include "../Common/Compiler.h"
namespace ursine
{
	namespace meta
	{
		template<typename T>
		VariantContainer<T>::VariantContainer(const NonRefType &value)
			: m_value(const_cast<NonRefType&>(value))
		{

		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		VariantContainer<T>::VariantContainer(const NonRefType &&value)
			: m_value(std::move(const_cast<NonRefType&&>(value)))
		{

		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		Type VariantContainer<T>::GetType(void) const
		{
			return meta_typeof(T);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		int VariantContainer<T>::ToInt(void) const
		{
			return getint();
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		bool VariantContainer<T>::ToBool(void) const
		{
			return getbool();
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		float VariantContainer<T>::ToFloat(void) const
		{
			return getfloat();
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		double VariantContainer<T>::ToDouble(void) const
		{
			return getdouble();
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		std::string VariantContainer<T>::ToString(void) const
		{
			return getString();
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		void *VariantContainer<T>::GetPtr(void) const
		{
			return const_cast<void*>(
				reinterpret_cast<const void*>(
					std::addressof(m_value)
					)
				);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename T>
		VariantBase *VariantContainer<T>::Clone(void) const
		{
			// use the non reference type to ensure a copy is made
			return new VariantContainer<NonRefType>(m_value);
		}

		template<typename T>
		void VariantContainer<T>::OnSerialize(Json &output) const
		{
			onSerialize(output);
		}

		template<typename T>
		void VariantContainer<T>::OnDeserialize(const Json &input)
		{
			onDeserialize(input);
		}

		///////////////////////////////////////////////////////////////////////

		DEFAULT_TYPE_HANDLER_IMPL(int);
		DEFAULT_TYPE_HANDLER_IMPL(bool);
		DEFAULT_TYPE_HANDLER_IMPL(float);
		DEFAULT_TYPE_HANDLER_IMPL(double);

		template<typename T>
		template<typename U>
		std::string VariantContainer<T>::getString(
			typename std::enable_if<
			!std::is_arithmetic<U>::value
			>::type*
		) const
		{
			return std::string();
		}

		template<typename T>
		template<typename U>
		std::string VariantContainer<T>::getString(
			typename std::enable_if<
			std::is_arithmetic<U>::value
			>::type*
		) const
		{
			return std::to_string(m_value);
		}

		template<typename T>
		template<typename U>
		void VariantContainer<T>::onSerialize(
			Json &output,
			typename std::enable_if<
			!std::is_pointer<U>::value && std::is_base_of<Object, U>::value
			>::type*
		) const
		{
			m_value.OnSerialize(output);
		}

		template<typename T>
		template<typename U>
		void VariantContainer<T>::onSerialize(
			Json &output,
			typename std::enable_if<
			std::is_pointer<U>::value || !std::is_base_of<Object, U>::value
			>::type*
		) const
		{
			// do nothing
		}

		template<typename T>
		template<typename U>
		void VariantContainer<T>::onDeserialize(
			const Json &input,
			typename std::enable_if<
			!std::is_pointer<U>::value && std::is_base_of<Object, U>::value
			>::type*
		)
		{
			m_value.OnDeserialize(input);
		}

		template<typename T>
		template<typename U>
		void VariantContainer<T>::onDeserialize(
			const Json &input,
			typename std::enable_if<
			std::is_pointer<U>::value || !std::is_base_of<Object, U>::value
			>::type*
		)
		{
			// do nothing
		}

		// void is a tricky 'ol fella, and must be defined manually
		template<>
		class VariantContainer<void> : public VariantBase
		{
		public:
			VariantContainer(void);

			Type GetType(void) const override;
			void *GetPtr(void) const override;

			int ToInt(void) const override;
			bool ToBool(void) const override;
			float ToFloat(void) const override;
			double ToDouble(void) const override;
			std::string ToString(void) const override;

			VariantBase *Clone(void) const override;

		private:
			friend class Variant;

			VariantContainer &operator=(const VariantContainer &rhs) = delete;
		};

		DECLARE_STANDARD_VARIANT(int);
		DECLARE_STANDARD_VARIANT(unsigned int);
		DECLARE_STANDARD_VARIANT(bool);
		DECLARE_STANDARD_VARIANT(float);
		DECLARE_STANDARD_VARIANT(double);
		DECLARE_STANDARD_VARIANT(std::string);
	}
}

#undef DEFAULT_TYPE_HANDLER
#undef DEFAULT_TYPE_HANDLER_IMPL
#undef DECLARE_STANDARD_TYPE
