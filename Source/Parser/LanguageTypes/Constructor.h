/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Constructor.h
** --------------------------------------------------------------------------*/

#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Class;

class Constructor 
    : public LanguageType
    , public Invokable
{
public:
    Constructor(
        const Cursor &cursor, 
        const Namespace &currentNamespace, 
        Class *parent = nullptr
    );

    virtual ~Constructor(void) { } 

    bool ShouldCompile(void) const;
    
    TemplateData CompileTemplate(
        const ReflectionParser *context
    ) const override;

private:
    Class *m_parent;
	// 是否可见 public
    bool isAccessible(void) const;

	// 这个函数返回当前构造函数的所有描述信息 包括可见性和参数列表
    std::string getTemplateParameters(bool isDynamic) const;
};