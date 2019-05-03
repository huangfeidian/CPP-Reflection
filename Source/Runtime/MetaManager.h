/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** MetaManager.h
** --------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

#include "MetaProperty.h"

#include "JsonConfig.h"



namespace ursine
{
	namespace meta
	{
		class Type;
		class Variant;

		class MetaManager
		{
			// 这里的metamanager是记录一个符号的所有meta信息的容器
			// meta信息有两种形式 
			// 一种是xxx, 代表bool型变量，有这个字段代表此符号的xxx属性为true， 例如Enabled, Registered
			// 另外一种是xxx(vargs...) xxx代表类型描述符，里面的参数就是构造这个类型描述符的构造参数，例如 Rpc(RPCTYPE::CLIENT_ONLY, RPCARG::INT, RPCARG::FLOAT)
			// 这两种情况的xxx都需要继承自MetaProperty，是个类型名字
		public:
			typedef std::initializer_list<
				std::pair<Type, const MetaProperty *>
			> Initializer;

			typedef std::vector<Variant> PropertyList;

			MetaManager(void);
			MetaManager(const MetaManager &rhs);
			MetaManager(const MetaManager &&rhs);

			const MetaManager &operator=(const MetaManager &rhs);

			MetaManager(const Initializer &properties);

			~MetaManager(void);

			Variant GetProperty(Type type) const;

			template<typename PropertyType>
			const PropertyType *GetProperty(void) const;

			// Sets the given property
			void SetProperty(Type type, const MetaProperty *value);

			// Gets all properties
			PropertyList GetProperties(void) const;

			Json SerializeJson(void) const;

		private:
			void copy(const MetaManager &rhs);

			std::map<Type, const MetaProperty *> m_properties;
		};
	}
}
#include "Type.h"
namespace ursine
{
	namespace meta
	{
		template<typename PropertyType>
		const PropertyType *MetaManager::GetProperty(void) const
		{
			static_assert(std::is_base_of<MetaProperty, PropertyType>::value,
				"Type must be a MetaProperty."
				);

			static const auto type = meta_typeof(PropertyType);

			auto search = m_properties.find(type);

			// doesn't exist
			if (search == m_properties.end())
				return nullptr;

			return static_cast<const PropertyType*>(search->second);
		}
    }
}

