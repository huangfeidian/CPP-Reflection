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

