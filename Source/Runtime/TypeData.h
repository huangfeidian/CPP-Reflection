/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** TypeData.h
** --------------------------------------------------------------------------*/

#pragma once

#include "MetaManager.h"

#include "Constructor.h"
#include "Destructor.h"

#include "Field.h"
#include "Global.h"

#include "Method.h"
#include "Function.h"

#include "Enum.h"
#include "EnumContainer.h"

namespace ursine
{
    namespace meta
    {
        class ReflectionDatabase;

        struct TypeData
        {
            bool isEnum : 1;
            bool isPrimitive : 1;
            bool isSigned : 1;
            bool isFloatingPoint : 1;
            bool isPointer : 1;
            bool isClass : 1;

            MetaManager meta;

            std::string name;

            // enum type

            Enum enumeration;

            // class type

            Type::Set baseClasses;
            Type::Set derivedClasses;

            Constructor arrayConstructor;

            Destructor destructor;

            std::unordered_map<
                InvokableSignature, 
                Constructor
            > constructors;

            std::unordered_map<
                InvokableSignature, 
                Constructor
            > dynamicConstructors;

            std::vector<Field> fields;
            std::vector<Global> staticFields;

            std::unordered_map<
                std::string, 
                InvokableOverloadMap<Method>
            > methods;

            std::unordered_map<
                std::string, 
                InvokableOverloadMap<Function>
            > staticMethods;

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            TypeData(void);
            TypeData(const std::string &name);

            void LoadBaseClasses(
                ReflectionDatabase &db, 
                TypeID thisType, 
                const std::initializer_list<Type> &classes
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename ClassType, bool IsDynamic, bool IsWrapped, typename ...Args>
            void AddConstructor(
                const MetaManager::Initializer &meta
            );

            template<typename ClassType>
            void SetArrayConstructor(void);

            const Constructor &GetConstructor(
                const InvokableSignature &signature
            );

            const Constructor &GetDynamicConstructor(
                const InvokableSignature &signature
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename ClassType>
            void SetDestructor(void);

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////
            
            // Method Getter, Method Setter
            template<typename ClassType, typename FieldType, typename GetterReturnType, typename SetterArgumentType>
            void AddField(
                const std::string &name,
                GetterReturnType (ClassType::*methodGetter)(void),
                void (ClassType::*methodSetter)(SetterArgumentType),
                const MetaManager::Initializer &meta
            );

            // Const Method Getter, Method Setter
            template<typename ClassType, typename FieldType, typename GetterReturnType, typename SetterArgumentType>
            void AddField(
                const std::string &name,
                GetterReturnType (ClassType::*methodGetter)(void) const,
                void (ClassType::*methodSetter)(SetterArgumentType),
                const MetaManager::Initializer &meta
            );


            // Method Getter, Field Setter
            template<typename ClassType, typename FieldType, typename GetterReturnType>
            void AddField(
                const std::string &name, 
                GetterReturnType (ClassType::*methodGetter)(void),
                typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
                const MetaManager::Initializer &meta
            );

            // Const Method Getter, Field Setter
            template<typename ClassType, typename FieldType, typename GetterReturnType>
            void AddField(
                const std::string &name, 
                GetterReturnType (ClassType::*methodGetter)(void) const,
                typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Method Setter
            template<typename ClassType, typename FieldType, typename SetterArgumentType>
            void AddField(
                const std::string &name, 
                typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
                void (ClassType::*methodSetter)(SetterArgumentType),
                const MetaManager::Initializer &meta
            );

            // Field Getter, Field Setter
            template<typename ClassType, typename FieldType>
            void AddField(
                const std::string &name, 
                typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
                typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
                const MetaManager::Initializer &meta
            );

            const Field &GetField(const std::string &name) const;

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            // Method Getter, Method Setter
            template<typename ClassType, typename FieldType, typename GetterType, typename SetterType>
            void AddStaticField(
                const std::string &name, 
                GetterType getter,
                SetterType setter,
                const MetaManager::Initializer &meta
            );

            // Method Getter, Field Setter
            template<typename ClassType, typename FieldType, typename GetterType>
            void AddStaticField(
                const std::string &name, 
                GetterType getter,
                FieldType *fieldSetter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Method Setter
            template<typename ClassType, typename FieldType, typename SetterType>
            void AddStaticField(
                const std::string &name, 
                FieldType *fieldGetter,
                SetterType setter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Field Setter
            template<typename ClassType, typename FieldType>
            void AddStaticField(
                const std::string &name, 
                FieldType *fieldGetter,
                FieldType *fieldSetter,
                const MetaManager::Initializer &meta
            );

            const Global &GetStaticField(const std::string &name) const;

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename MethodType>
            void AddMethod(
                const std::string &name, 
                MethodType method,
                const MetaManager::Initializer &meta
            );

            const Method &GetMethod(
                const std::string &name
            );

            const Method &GetMethod(
                const std::string &name, 
                const InvokableSignature &signature
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<
                typename ClassType, 
                typename FunctionType
            >
            void AddStaticMethod(
                const std::string &name, 
                FunctionType function,
                const MetaManager::Initializer &meta
            );

            const Function &GetStaticMethod(
                const std::string &name
            );

            const Function &GetStaticMethod(
                const std::string &name, 
                const InvokableSignature &signature
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename EnumType>
            void SetEnum(
                const std::string &name, 
                const typename EnumContainer<EnumType>::Initializer &initalizer
            );
        };

		template<typename ClassType, bool IsDynamic, bool IsWrapped, typename ...Args>
		void TypeData::AddConstructor(
			const MetaManager::Initializer &meta
		)
		{
			InvokableSignature signature =
				Invokable::CreateSignature<Args...>();

			Constructor ctor{
				meta_typeof(ClassType),
				signature,
				new ConstructorInvoker<ClassType, IsDynamic, IsWrapped, Args...>(),
				IsDynamic
			};

			ctor.m_meta = meta;

			if (IsDynamic)
				dynamicConstructors.emplace(signature, ctor);
			else
				constructors.emplace(signature, ctor);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType>
		void TypeData::SetArrayConstructor(void)
		{
			arrayConstructor = Constructor{
				meta_typeof(ClassType),
				{ },
				new ConstructorInvoker<Array<ClassType>, false, false>(),
				false
			};
		}

		template<typename ClassType, typename FieldType, typename GetterReturnType, typename SetterArgumentType>
		void TypeData::AddField(
			const std::string &name,
			GetterReturnType(ClassType::*methodGetter)(void),
			void (ClassType::*methodSetter)(SetterArgumentType),
			const MetaManager::Initializer &meta
		)
		{
			typedef FieldGetter<ClassType, GetterReturnType, true> GetterType;
			typedef FieldSetter<ClassType, SetterArgumentType, true> SetterType;

			static_assert(std::is_same<typename std::decay<GetterReturnType>::type, typename std::decay<FieldType>::type>::value,
				"Return type of getter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			static_assert(std::is_same<typename std::decay<SetterArgumentType>::type, typename std::decay<FieldType>::type>::value,
				"Argument type of setter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			fields.emplace_back(
				name,
				meta_typeof(FieldType),
				meta_typeof(ClassType),
				!methodGetter ? nullptr : new GetterType(methodGetter),
				!methodSetter ? nullptr : new SetterType(methodSetter)
			);

			fields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType, typename GetterReturnType, typename SetterArgumentType>
		void TypeData::AddField(
			const std::string &name,
			GetterReturnType(ClassType::*methodGetter)(void) const,
			void (ClassType::*methodSetter)(SetterArgumentType),
			const MetaManager::Initializer &meta
		)
		{
			typedef FieldGetter<ClassType, GetterReturnType, true> GetterType;
			typedef FieldSetter<ClassType, SetterArgumentType, true> SetterType;

			static_assert(std::is_same<typename std::decay<GetterReturnType>::type, typename std::decay<FieldType>::type>::value,
				"Return type of getter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			static_assert(std::is_same<typename std::decay<SetterArgumentType>::type, typename std::decay<FieldType>::type>::value,
				"Argument type of setter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			fields.emplace_back(
				name,
				meta_typeof(FieldType),
				meta_typeof(ClassType),
				!methodGetter ? nullptr : new GetterType(methodGetter),
				!methodSetter ? nullptr : new SetterType(methodSetter)
			);

			fields.back().m_meta = meta;
		}

		template<typename ClassType, typename FieldType, typename GetterReturnType>
		void TypeData::AddField(
			const std::string &name,
			GetterReturnType(ClassType::*methodGetter)(void),
			typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
			const MetaManager::Initializer &meta
		)
		{
			typedef FieldGetter<ClassType, GetterReturnType, true> GetterType;

			static_assert(std::is_same<typename std::decay<GetterReturnType>::type, typename std::decay<FieldType>::type>::value,
				"Return type of getter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			fields.emplace_back(
				name,
				meta_typeof(FieldType),
				meta_typeof(ClassType),
				!methodGetter ? nullptr : new GetterType(methodGetter),
				!fieldSetter ? nullptr : new FieldSetter<ClassType, FieldType, false>(fieldSetter)
			);

			fields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType, typename GetterReturnType>
		void TypeData::AddField(
			const std::string &name,
			GetterReturnType(ClassType::*methodGetter)(void) const,
			typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
			const MetaManager::Initializer &meta
		)
		{
			typedef FieldGetter<ClassType, GetterReturnType, true> GetterType;

			static_assert(std::is_same<typename std::decay<GetterReturnType>::type, typename std::decay<FieldType>::type>::value,
				"Return type of getter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			fields.emplace_back(
				name,
				meta_typeof(FieldType),
				meta_typeof(ClassType),
				!methodGetter ? nullptr : new GetterType(methodGetter),
				!fieldSetter ? nullptr : new FieldSetter<ClassType, FieldType, false>(fieldSetter)
			);

			fields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType, typename SetterArgumentType>
		void TypeData::AddField(
			const std::string &name,
			typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
			void (ClassType::*methodSetter)(SetterArgumentType),
			const MetaManager::Initializer &meta
		)
		{
			typedef FieldSetter<ClassType, SetterArgumentType, true> SetterType;

			static_assert(std::is_same<typename std::decay<SetterArgumentType>::type, typename std::decay<FieldType>::type>::value,
				"Argument type of setter does not match field type. "
				"This results in undefined behavior! (Even if there exists a conversion constructor between the types)"
				);

			fields.emplace_back(
				name,
				meta_typeof(FieldType),
				meta_typeof(ClassType),
				!fieldGetter ? nullptr : new FieldGetter<ClassType, FieldType, false>(fieldGetter),
				!methodSetter ? nullptr : new SetterType(methodSetter)
			);

			fields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType>
		void TypeData::AddField(
			const std::string &name,
			typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
			typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
			const MetaManager::Initializer &meta
		)
		{
			fields.emplace_back(
				name,
				meta_typeof(FieldType),
				meta_typeof(ClassType),
				!fieldGetter ? nullptr : new FieldGetter<ClassType, FieldType, false>(fieldGetter),
				!fieldSetter ? nullptr : new FieldSetter<ClassType, FieldType, false>(fieldSetter)
			);

			fields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType, typename GetterType, typename SetterType>
		void TypeData::AddStaticField(const std::string &name, GetterType getter, SetterType setter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<FieldType, true> GlobalGetterType;
			typedef GlobalSetter<FieldType, true> GlobalSetterType;

			staticFields.emplace_back(
				name,
				meta_typeof(FieldType),
				!getter ? nullptr : new GlobalGetterType(
					reinterpret_cast<typename GlobalGetterType::Signature>(getter)
				),
				!setter ? nullptr : new GlobalSetterType(
					reinterpret_cast<typename GlobalSetterType::Signature>(setter)
				),
				meta_typeof(ClassType)
			);

			staticFields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType, typename GetterType>
		void TypeData::AddStaticField(const std::string &name, GetterType getter, FieldType *fieldSetter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<FieldType, true> GlobalGetterType;
			typedef GlobalSetter<FieldType, false> GlobalSetterType;

			staticFields.emplace_back(
				name,
				meta_typeof(FieldType),
				!getter ? nullptr : new GlobalGetterType(
					reinterpret_cast<typename GlobalGetterType::Signature>(getter)
				),
				!fieldSetter ? nullptr : new GlobalSetterType(fieldSetter),
				meta_typeof(ClassType)
			);

			staticFields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType, typename SetterType>
		void TypeData::AddStaticField(const std::string &name, FieldType *fieldGetter, SetterType setter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<FieldType, false> GlobalGetterType;
			typedef GlobalSetter<FieldType, true> GlobalSetterType;

			staticFields.emplace_back(
				name,
				meta_typeof(FieldType),
				!fieldGetter ? nullptr : new GlobalGetterType(fieldGetter),
				!setter ? nullptr : new GlobalSetterType(
					reinterpret_cast<typename GlobalSetterType::Signature>(setter)
				),
				meta_typeof(ClassType)
			);

			staticFields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FieldType>
		void TypeData::AddStaticField(const std::string &name, FieldType *fieldGetter, FieldType *fieldSetter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<FieldType, false> GlobalGetterType;
			typedef GlobalSetter<FieldType, false> GlobalSetterType;

			staticFields.emplace_back(
				name,
				meta_typeof(FieldType),
				!fieldGetter ? nullptr : new GlobalGetterType(fieldGetter),
				!fieldSetter ? nullptr : new GlobalSetterType(fieldSetter),
				meta_typeof(ClassType)
			);

			staticFields.back().m_meta = meta;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType>
		void TypeData::SetDestructor(void)
		{
			destructor = {
				meta_typeof(ClassType),
				new DestructorInvoker<ClassType>()
			};
		}

		///////////////////////////////////////////////////////////////////////

		template<typename MethodType>
		void TypeData::AddMethod(
			const std::string &name,
			MethodType method,
			const MetaManager::Initializer &meta
		)
		{
			Method meth(name, method);

			meth.m_meta = meta;

			methods[name].emplace(meth.GetSignature(), meth);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename ClassType, typename FunctionType>
		void TypeData::AddStaticMethod(
			const std::string &name,
			FunctionType function,
			const MetaManager::Initializer &meta
		)
		{
			Function fn(name, function, meta_typeof(ClassType));

			fn.m_meta = meta;

			staticMethods[name].emplace(fn.GetSignature(), fn);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		void TypeData::SetEnum(
			const std::string &name,
			const typename EnumContainer<EnumType>::Initializer &initializer
		)
		{
			enumeration = new EnumContainer<EnumType>(name, initializer);
		}
    }
}