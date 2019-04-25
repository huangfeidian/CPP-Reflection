/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Invokable.h
** --------------------------------------------------------------------------*/

#pragma once
#include <vector>
#include "InvokableConfig.h"
#include "ArgumentConfig.h"

namespace ursine
{
	namespace meta
	{
		class Type;

		class Invokable
		{
		public:
			Invokable(const std::string &name = "INVALID");

			template<typename ...Types>
			static InvokableSignature CreateSignature(void);

			const InvokableSignature &GetSignature(void) const;

			const std::string &GetName(void) const;

		protected:
			std::string m_name;

			InvokableSignature m_signature;
		};
	}
}
#include "Type.h"
#include "TypeUnpacker.h"
namespace ursine
{
	namespace meta
	{
		template<typename ...Types>
		InvokableSignature Invokable::CreateSignature(void)
		{
			static InvokableSignature signature;

			static auto initial = true;

			if (initial)
			{
				TypeUnpacker<Types...>::Apply(signature);

				initial = false;
			}

			return signature;
		}
    }
}

namespace std
{
	template<>
	struct hash<ursine::meta::InvokableSignature>
	{
		size_t operator()(
			const ursine::meta::InvokableSignature &signature
			) const
		{
			hash<ursine::meta::TypeID> hasher;

			size_t seed = 0;

			// combine the hash of all type IDs in the signature
			for (auto &type : signature)
				seed ^= hasher(type.GetID()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}
	};
}
