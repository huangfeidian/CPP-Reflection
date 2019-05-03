/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** MetaUtils.cpp
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "MetaUtils.h"

#include <fstream>
#include <exception>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace
{
	std::shared_ptr<spdlog::logger> create_logger()
	{
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::warn);
		console_sink->set_pattern("[meta] [%^%l%$] %v");

		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("meta.txt", true);
		file_sink->set_level(spdlog::level::trace);
		auto logger = std::make_shared<spdlog::logger>("meta", spdlog::sinks_init_list{ console_sink, file_sink });
		logger->set_level(spdlog::level::trace);
		return logger;
	}
}
namespace utils
{
	std::string join(const std::vector<std::string>& param, const std::string& sep)
	{
		std::string result = "";
		for (const auto& one_str : param)
		{
			if (!result.empty())
			{
				result += sep + one_str;
			}
			else
			{
				result = one_str;
			}
		}
		return result;
	}
    void ToString(const CXString &str, std::string &output)
    {
        auto cstr = clang_getCString( str );

        output = cstr;

        clang_disposeString( str );
    }

    TemplateData::Type TemplateBool(bool value)
    {
        return value ? TemplateData::Type::True : TemplateData::Type::False;
    }

    std::string GetQualifiedName(const CursorType &type)
    {
        if (type.GetKind( ) != CXType_Typedef)
            return type.GetDisplayName( );

        auto declaration = type.GetDeclaration( );

        auto parent = declaration.GetLexicalParent( );

        Namespace parentNamespace;

        // walk up to the root namespace
        while (parent.GetKind( ) == CXCursor_Namespace)
        {
            parentNamespace.emplace( parentNamespace.begin( ), parent.GetDisplayName( ) );

            parent = parent.GetLexicalParent( );
        }

        // add the display name as the end of the namespace
        parentNamespace.emplace_back(
            type.GetDisplayName( )
        );

        return join(parentNamespace, "::" );
    }

    std::string GetQualifiedName(
        const std::string &displayName, 
        const Namespace &currentNamespace
    )
    {
        auto name = join( currentNamespace, "::" );

        if (!currentNamespace.empty( ))
            name += "::";

        name += displayName;

        return name;
    }

    std::string GetQualifiedName(
        const Cursor &cursor, 
        const Namespace &currentNamespace
    )
    {
        return GetQualifiedName( cursor.GetSpelling( ), currentNamespace );
    }

    void LoadText(const std::string &filename, std::string &output)
    {
        std::ifstream input( filename );

        if (!input)
        {
            std::stringstream error;

            error << "Unable to open file \"" 
                  << filename 
                  << "\" for reading." << std::endl;
            error << strerror( errno );

            throw std::runtime_error( error.str( ) );
        }

        input.seekg( 0, std::ios::end );

        output.reserve(
            static_cast<std::string::size_type>( input.tellg( ) )
        );

        input.seekg( 0, std::ios::beg );

        output.assign(
            // disambiguate function declaration
            (std::istreambuf_iterator<char>( input )),
            std::istreambuf_iterator<char>( ) 
        );

        input.close( );
    }

    void WriteText(const std::string &filename, const std::string &text)
    {
        std::ofstream output( filename );

        if (!output)
        {
            std::stringstream error;

            error << "Unable to open file \"" 
                  << filename << "\" for writing." 
                  << std::endl;
            error << strerror( errno );

            throw std::runtime_error( error.str( ) );
        }

        output << text;

        output.close( );
    }

    std::filesystem::path MakeRelativePath(const std::filesystem::path &from, const std::filesystem::path &to)
    {
        // Start at the root path and while they are the same then do nothing then when they first
        // diverge take the remainder of the two path and replace the entire from path with ".."
        // segments.
        auto itFrom = from.begin( );
        auto itTo = to.begin( );

        // Loop through both
        while (itFrom != from.end( ) && itTo != to.end( ) && (*itTo) == (*itFrom))
        {
            ++itTo;
            ++itFrom;
        }

        std::filesystem::path finalPath;

        while (itFrom != from.end( ))
        {
            finalPath /= "..";

            ++itFrom;
        }

        while (itTo != to.end( ))
        {
            finalPath /= *itTo;

            ++itTo;
        }

        return finalPath;
    }

    void FatalError(const std::string &error)
    {
        std::cerr << "Error: " << error << std::endl;

        exit( EXIT_FAILURE );
    }
	spdlog::logger & get_logger()
	{
		static std::shared_ptr<spdlog::logger> _logger = create_logger();
		return *_logger;
	}
	

}