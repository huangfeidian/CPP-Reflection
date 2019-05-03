/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** MetaUtils.h
** --------------------------------------------------------------------------*/

#pragma once

#include <spdlog/logger.h>
#include "Cursor.h"
#include "Namespace.h"

#include "Mustache.h"

namespace utils
{
	std::string join(const std::vector<std::string>& param, const std::string& sep);
    void ToString(const CXString &str, std::string &output);

    Mustache::Data<std::string>::Type TemplateBool(bool value);

    std::string GetQualifiedName(const CursorType &type);

    std::string GetQualifiedName(
        const std::string &displayName, 
        const Namespace &currentNamespace
    );

    std::string GetQualifiedName(
        const Cursor &cursor, 
        const Namespace &currentNamespace
    );

    void LoadText(const std::string &filename, std::string &output);
    void WriteText(const std::string &filename, const std::string &text);

    std::filesystem::path MakeRelativePath(
        const std::filesystem::path &from, 
        const std::filesystem::path &to
    );

    void FatalError(const std::string &error);

    template<typename A, typename B>
    bool RangeEqual(A startA, A endA, B startB, B endB);
	spdlog::logger& get_logger();
}

#include "MetaUtils.hpp"