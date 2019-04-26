/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Function.cpp
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LanguageTypes/Function.h"
#include "LanguageTypes/Class.h"

#include <spdlog/fmt/fmt.h>


Function::Function(
    const Cursor &cursor, 
    const Namespace &currentNamespace, 
    Class *parent
)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
    , m_parent( parent )
    , m_name( cursor.GetSpelling( ) )
    , m_qualifiedName( utils::GetQualifiedName( cursor, currentNamespace ) )
{
        
}

bool Function::ShouldCompile(void) const
{
    return isAccessible( );
}

TemplateData Function::CompileTemplate(const ReflectionParser *context) const
{
    TemplateData data { TemplateData::Type::Object };

    data[ "name" ] = m_name;
    data[ "qualifiedName" ] = m_qualifiedName;

    if (m_parent)
        data[ "parentQualifiedName" ] = m_parent->m_qualifiedName;
    
    data[ "qualifiedSignature" ] = getQualifiedSignature( );

    m_metaData.CompileTemplateData( data, context );

    return data;
}

bool Function::isAccessible(void) const
{
    if (m_parent && m_accessModifier != CX_CXXPublic)
        return false;

    return m_enabled;
}

std::string Function::getQualifiedSignature(void) const
{

	return fmt::format("{0}(*){1}", m_returnType, utils::join(m_signature, ", "));
}