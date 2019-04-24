/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** TokenizerState.h
** --------------------------------------------------------------------------*/

#pragma once

#include <memory>
#include <unordered_map>

template<typename TokenEnumType, typename InputType>
class Tokenizer;

template<typename TokenEnumType, typename InputType>
class TokenizerState
{
public:
    typedef TokenizerState* Handle;

    explicit TokenizerState(TokenEnumType acceptingType);

    void SetAcceptingType(TokenEnumType acceptingType);

    void AddEdge(Handle to, char character);

    template<typename... CharacterList>
    void AddEdge(Handle to, char character, CharacterList &&...list);

    template<typename... CharacterList>
    void SetLooping(CharacterList &&...list);

    void SetDefaultEdge(Handle defaultEdge);

private:
    friend class Tokenizer<TokenEnumType, InputType>;

    TokenEnumType m_acceptingType;

    std::unordered_map<char, Handle> m_edges;
    Handle m_defaultEdge;
};

template<typename TokenEnumType, typename InputType>
TokenizerState<TokenEnumType, InputType>::TokenizerState(TokenEnumType acceptingType)
    : m_acceptingType( acceptingType )
    , m_defaultEdge( nullptr ) { }

template<typename TokenEnumType, typename InputType>
void TokenizerState<TokenEnumType, InputType>::SetAcceptingType(TokenEnumType acceptingType)
{
    m_acceptingType = acceptingType;
}

template<typename TokenEnumType, typename InputType>
void TokenizerState<TokenEnumType, InputType>::AddEdge(Handle to, char character)
{
    m_edges[ character ] = to;
}

template<typename TokenEnumType, typename InputType>
template<typename ... CharacterList>
void TokenizerState<TokenEnumType, InputType>::AddEdge(Handle to, char character, CharacterList &&...list)
{
    AddEdge( to, character );
    AddEdge( to, list... );
}

template<typename TokenEnumType, typename InputType>
template<typename ... CharacterList>
void TokenizerState<TokenEnumType, InputType>::SetLooping(CharacterList &&...list)
{
    AddEdge( Handle( this ), list... );
}

template<typename TokenEnumType, typename InputType>
void TokenizerState<TokenEnumType, InputType>::SetDefaultEdge(Handle defaultEdge)
{
    m_defaultEdge = defaultEdge;
}
