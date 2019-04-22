/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** TokenizerResult.h
** --------------------------------------------------------------------------*/

#pragma once

template<typename TokenType>
class TokenizerResult
{
public:
    typedef std::vector<TokenType> TokenList;

    TokenizerResult(const TokenList &tokens);

    const TokenList &GetTokens(void) const;

	// 找到从start开始的第一个type 在types里的token索引
	// 找不到的话返回最后一个
    template<typename... TokenTypeList>
    size_t FindNext(size_t start, TokenTypeList &&...types);
	
	// 找到从start开始的上一个type在types里的token索引 偏移
	// 找不到的话返回0
    template<typename... TokenTypeList>
    size_t FindPrevious(size_t start, TokenTypeList &&...types);

	// 删除所有的type在types列表的token
    template<typename... TokenTypeList>
    void RemoveAll(TokenTypeList &&...types);

	// 这个是对start 到end范围内的tokens进行值的累加
    typename TokenType::InputValueType ConsumeRange(size_t start, size_t end);

	// 找出从start反向开始的类型在types里面的token 然后按照从左到右顺序累加
    template<typename... TokenTypeList>
    typename TokenType::InputValueType ConsumeAllPrevious(size_t start, TokenTypeList &&...types);

	// 找出从start开始的类型在types里的token 然后按照从左到右的顺序累加
    template<typename... TokenTypeList>
    typename TokenType::InputValueType ConsumeAllNext(size_t start, TokenTypeList &&...types);
private:
    TokenList m_tokens;
};

#include "Impl/TokenizerResult.hpp"