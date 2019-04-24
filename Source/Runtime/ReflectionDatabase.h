﻿/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** ReflectionDatabase.h
** --------------------------------------------------------------------------*/

#pragma once

#include "TypeData.h"
#include "TypeInfo.h"

#include <vector>
#include <unordered_map>

namespace ursine
{
    namespace meta
    {
        class ReflectionDatabase
        {
        public:
            ReflectionDatabase(void);
            ~ReflectionDatabase(void);

            std::vector<TypeData> types;

            std::unordered_map<std::string, TypeID> ids;

            std::unordered_map<std::string, Global> globals;
            
            std::unordered_map<
                std::string, 
                InvokableOverloadMap<Function>
            > globalFunctions;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            static ReflectionDatabase &Instance(void);

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            TypeID AllocateType(const std::string &name);

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Function Getter, Function Setter
            template<typename GlobalType, typename GetterType, typename SetterType>
            void AddGlobal(
                const std::string &name,
                GetterType getter,
                SetterType setter,
                const MetaManager::Initializer &meta
            );

            // Function Getter, Raw Setter
            template<typename GlobalType, typename GetterType>
            void AddGlobal(
                const std::string &name,
                GetterType getter,
                GlobalType *globalSetter,
                const MetaManager::Initializer &meta
            );

            // Raw Getter, Function Setter
            template<typename GlobalType, typename SetterType>
            void AddGlobal(
                const std::string &name,
                GlobalType *globalGetter,
                SetterType setter,
                const MetaManager::Initializer &meta
            );

            // Raw Getter, Raw Setter
            template<typename GlobalType>
            void AddGlobal(
                const std::string &name,
                GlobalType *globalGetter,
                GlobalType *globalSetter,
                const MetaManager::Initializer &meta
            );

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            template<typename FunctionType>
            void AddGlobalFunction(
                const std::string &name, 
                FunctionType globalFunction,
                const MetaManager::Initializer &meta
            );

            const Function &GetGlobalFunction(const std::string &name);

            const Function &GetGlobalFunction(
                const std::string &name, 
                const InvokableSignature &signature
            );

        private:
            TypeID m_nextID;
        };

		template<typename GlobalType, typename GetterType, typename SetterType>
		void ReflectionDatabase::AddGlobal(const std::string &name, GetterType getter, SetterType setter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<GlobalType, true> GlobalGetterType;
			typedef GlobalSetter<GlobalType, true> GlobalSetterType;

			Global global{
				name,
				meta_typeof(GlobalType),
				!getter ? nullptr : new GlobalGetterType(
					reinterpret_cast<typename GlobalGetterType::Signature>(getter)
				),
				!setter ? nullptr : new GlobalSetterType(
					reinterpret_cast<typename GlobalSetterType::Signature>(setter)
				)
			};

			global.m_meta = meta;

			globals.emplace(name, global);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename GlobalType, typename GetterType>
		void ReflectionDatabase::AddGlobal(const std::string &name, GetterType getter, GlobalType *globalSetter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<GlobalType, true> GlobalGetterType;
			typedef GlobalSetter<GlobalType, false> GlobalSetterType;

			Global global{
				name,
				meta_typeof(GlobalType),
				!getter ? nullptr : new GlobalGetterType(
					reinterpret_cast<typename GlobalGetterType::Signature>(getter)
				),
				!globalSetter ? nullptr : new GlobalSetterType(globalSetter)
			};

			global.m_meta = meta;

			globals.emplace(name, global);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename GlobalType, typename SetterType>
		void ReflectionDatabase::AddGlobal(const std::string &name, GlobalType *globalGetter, SetterType setter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<GlobalType, false> GlobalGetterType;
			typedef GlobalSetter<GlobalType, true> GlobalSetterType;

			Global global{
				name,
				meta_typeof(GlobalType),
				!globalGetter ? nullptr : new GlobalGetterType(globalGetter),
				!setter ? nullptr : new GlobalSetterType(
					reinterpret_cast<typename GlobalSetterType::Signature>(setter)
				)
			};

			global.m_meta = meta;

			globals.emplace(name, global);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename GlobalType>
		void ReflectionDatabase::AddGlobal(const std::string &name, GlobalType *globalGetter, GlobalType *globalSetter, const MetaManager::Initializer &meta)
		{
			typedef GlobalGetter<GlobalType, false> GlobalGetterType;
			typedef GlobalSetter<GlobalType, false> GlobalSetterType;

			Global global{
				name,
				meta_typeof(GlobalType),
				!globalGetter ? nullptr : new GlobalGetterType(globalGetter),
				!globalSetter ? nullptr : new GlobalSetterType(globalSetter)
			};

			global.m_meta = meta;

			globals.emplace(name, global);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename FunctionType>
		void ReflectionDatabase::AddGlobalFunction(
			const std::string &name,
			FunctionType globalFunction,
			const MetaManager::Initializer &meta
		)
		{
			Function function(name, globalFunction);

			function.m_meta = meta;

			globalFunctions[name].emplace(
				function.GetSignature(),
				function
			);
		}
    }
}