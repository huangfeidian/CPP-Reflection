/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** EnumContainer.h
** --------------------------------------------------------------------------*/

#pragma once

#include "EnumBase.h"

#include "Variant.h"

#include <unordered_map>
#include <string>

namespace ursine
{
    namespace meta
    {
        template<typename EnumType>
        class EnumContainer : public EnumBase
        {
        public:
            typedef std::initializer_list<std::pair<std::string, EnumType>> Initializer;
            typedef std::unordered_map<std::string, EnumType> Table;

            EnumContainer(const std::string &name, const Initializer &initializer);
            
            EnumContainer(
                const std::string &name, 
                const Initializer &initializer,
                TypeID owner
            );

            Type GetType(void) const override;
            Type GetUnderlyingType(void) const override;

            const std::vector<std::string> &GetKeys(void) const override;
            std::vector<Variant> GetValues(void) const override;

            std::string GetKey(const Argument &value) const override;
            Variant GetValue(const std::string &key) const override;

        private:
            Table m_keyToValue;
            std::vector<std::string> m_keys;
        };

		template<typename EnumType>
		EnumContainer<EnumType>::EnumContainer(
			const std::string &name,
			const Initializer &initializer
		)
			: EnumBase(name, InvalidTypeID)
		{
			for (auto &pair : initializer)
			{
				m_keys.emplace_back(pair.first);

				m_keyToValue.emplace(pair);
			}
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		EnumContainer<EnumType>::EnumContainer(
			const std::string &name,
			const Initializer &initializer,
			TypeID owner
		)
			: EnumBase(name, owner)
		{
			for (auto &pair : initializer)
			{
				m_keys.emplace_back(pair.first);

				m_keyToValue.emplace(pair);
			}
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		Type EnumContainer<EnumType>::GetType(void) const
		{
			return meta_typeof(EnumType);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		Type EnumContainer<EnumType>::GetUnderlyingType(void) const
		{
			return meta_typeof(typename std::underlying_type<EnumType>::type);
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		const std::vector<std::string> &EnumContainer<EnumType>::GetKeys(void) const
		{
			return m_keys;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		std::vector<Variant> EnumContainer<EnumType>::GetValues(void) const
		{
			std::vector<Variant> values;

			for (auto &entry : m_keyToValue)
				values.emplace_back(entry.second);

			return values;
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		std::string EnumContainer<EnumType>::GetKey(const Argument &value) const
		{
			typedef
				typename std::underlying_type< EnumType >::type UnderlyingType;

			auto type = value.GetType();

			// invalid type
			if (type != meta_typeof(EnumType) &&
				type != meta_typeof(UnderlyingType))
			{
				return std::string();
			}

			auto &converted = value.GetValue<EnumType>();

			for (auto &entry : m_keyToValue)
			{
				if (entry.second == converted)
					return entry.first;
			}

			return std::string();
		}

		///////////////////////////////////////////////////////////////////////

		template<typename EnumType>
		Variant EnumContainer<EnumType>::GetValue(const std::string &key) const
		{
			auto search = m_keyToValue.find(key);

			// not found
			if (search == m_keyToValue.end())
				return Variant();

			return Variant(search->second);
		}
    }
}