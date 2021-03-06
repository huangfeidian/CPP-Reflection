/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** ReflectionDatabase.cpp
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ReflectionDatabase.h"

#include "Type.h"

// 下面这个宏用来注册所有的内置类型
#define REGISTER_NATIVE_TYPE(type)                    \
    {                                                 \
        auto id = AllocateType( #type );              \
        auto &handle = types[ id ];                   \
                                                      \
        TypeInfo<type>::Register( id, handle, true ); \
    }                                                 \

// 自动注册指针 和const 指针
#define REGISTER_NATIVE_TYPE_VARIANTS(type) \
    REGISTER_NATIVE_TYPE( type )            \
    REGISTER_NATIVE_TYPE( type* )           \
    REGISTER_NATIVE_TYPE( const type* )     \

#define REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY(type)         \
    REGISTER_NATIVE_TYPE_VARIANTS( type )                   \
    types[ meta_typeidof( type ) ].SetArrayConstructor<type>( ); \

namespace ursine
{
    namespace meta
    {
        ReflectionDatabase::ReflectionDatabase(void)
            : types( 1 )
            , m_nextID( 1 )
        {
            types[ InvalidTypeID ].name = "UNKNOWN";

            // register all of the native type variants explicity, before
            // anything else
            REGISTER_NATIVE_TYPE_VARIANTS( void );
            REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY( int );
            REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY( unsigned int );
            REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY( bool );
            REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY( float );
            REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY( double );
            REGISTER_NATIVE_TYPE_VARIANTS_W_ARRAY( std::string );

            auto &stringType = types[ meta_typeidof( std::string ) ];

            // explicitly add default constructors for string

            stringType.AddConstructor<std::string, false, false>( { } );
            stringType.AddConstructor<std::string, false, true>( { } );
        }

        ///////////////////////////////////////////////////////////////////////

        ReflectionDatabase::~ReflectionDatabase(void) { }

        ///////////////////////////////////////////////////////////////////////

        ReflectionDatabase &ReflectionDatabase::Instance(void)
        {
            static ReflectionDatabase instance;

            return instance;
        }

        ///////////////////////////////////////////////////////////////////////

        TypeID ReflectionDatabase::AllocateType(const std::string &name)
        {
            auto search = ids.find( name );

            // already defined
            if (search != ids.end( ))
                return InvalidTypeID;

            types.emplace_back( name );

            auto id = m_nextID++;

            ids[ name ] = id;

            return id;
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &ReflectionDatabase::GetGlobalFunction(
            const std::string &name
        )
        {
            auto &base = globalFunctions[ name ];

            if (!base.size( ))
                return Function::Invalid( );

            return base.begin( )->second;
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &ReflectionDatabase::GetGlobalFunction(
            const std::string &name, 
            const InvokableSignature &signature
        )
        {
            auto &base = globalFunctions[ name ];

            auto search = base.find( signature );

            if (search == base.end( ))
                return Function::Invalid( );

            return search->second;
        }
    }
}
