/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Token.h
** --------------------------------------------------------------------------*/

#pragma once

template<typename TokenEnumType, typename InputType>
struct Token
{
    typedef TokenEnumType EnumType;
    typedef InputType InputValueType;

    TokenEnumType type;
    InputType value;

    Token(void);
    Token(TokenEnumType type, const InputType &value);
};

template<typename TokenEnumType, typename InputType>
Token<TokenEnumType, InputType>::Token(void)
    : type( TokenEnumType::Invalid ) { }

template<typename TokenEnumType, typename InputType>
Token<TokenEnumType, InputType>::Token(TokenEnumType type, const InputType &value)
    : type( type )
    , value( value ) { }
