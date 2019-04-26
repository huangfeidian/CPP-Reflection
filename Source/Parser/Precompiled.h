/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Precompiled.h
** --------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <functional>
#include <filesystem>

#include <clang-c/Index.h>



#include "MetaUtils.h"
#include "MetaDataConfig.h"

#include <Mustache.h>

using MustacheTemplate = Mustache::Mustache<std::string>;
using TemplateData = Mustache::Data<std::string>;

namespace fs = std::filesystem;
