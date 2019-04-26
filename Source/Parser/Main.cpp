/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** Main.cpp
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ReflectionOptions.h"
#include "ReflectionParser.h"

#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
void parse(const std::string& config_file_path);

int main(int argc, char *argv[])
{
    auto start = std::chrono::system_clock::now( );
	if (argc != 2)
	{
		std::cerr << "only need one argument to specify the config file path" << std::endl;
		return 0;
	}
    // parse command line
    try 
    {
        // path to the executable
        auto exeDir = fs::path( argv[ 0 ] ).parent_path( );

        // set the working directory to the executable directory
        if (!exeDir.empty( ))
            fs::current_path( exeDir );

       

        parse( argv[1]);
    }
    catch (std::exception &e)
    {
        utils::FatalError( e.what( ) );
    }
    catch (...) 
    {
        utils::FatalError( "Unhandled exception occurred!" );
    }

    auto duration = std::chrono::system_clock::now( ) - start;

    std::cout << "Completed in " 
              << std::chrono::duration_cast<std::chrono::milliseconds>( duration ).count( ) 
              << "ms" << std::endl;

    return EXIT_SUCCESS;
}

void parse(const std::string& config_file_path)
{
    ReflectionOptions options;
	if (!parse_json_config(config_file_path, options))
	{
		std::cerr << "cant parse config " << config_file_path << std::endl;
		return;
	}

    
    std::cout << std::endl;
    std::cout << "Parsing reflection data for target \"" 
              << options.targetName << "\"" 
              << std::endl;

    ReflectionParser parser( options );

    parser.Parse( );

    try
    {
        parser.GenerateFiles( );
    }
    catch (std::exception &e)
    {
        utils::FatalError( e.what( ) );
    }
}
bool parse_json_config(std::string config_file_path, ReflectionOptions& result_opt)
{
	std::ifstream config_file(config_file_path);
	std::string config_data((std::istreambuf_iterator<char>(config_file)), (std::istreambuf_iterator<char>()));
	json json_obj;
	try {
		json_obj = json::parse(config_data);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "fail to parse config " << config_file_path << std::endl;
		return false;
	}
	if (!json_obj.is_object())
	{
		std::cerr << "the data should be a map" << std::endl;
		return false;
	}
	std::vector<std::string> str_required_keys = {
		"target_name",
		"source_root",
		"in_source",
		"module_header",
		"out_source",
		"out_dir",
		"pch",
		"template_directory"

	};
	for (const auto& one_key : str_required_keys)
	{
		auto temp_value_iter = json_obj.find(one_key);
		if (temp_value_iter == json_obj.end())
		{
			std::cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
		if (!temp_value_iter->is_string())
		{
			std::cerr << "cant find string value for key " << one_key << std::endl;
			return false;
		}
		auto temp_value = temp_value_iter->get<std::string>();
		if (temp_value.empty())
		{
			std::cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
	}
	result_opt.targetName = json_obj["target_name"].get<std::string>();
	result_opt.sourceRoot = json_obj["source_root"].get<std::string>();
	result_opt.inputSourceFile = json_obj["in_source"].get<std::string>();
	result_opt.moduleHeaderFile = json_obj["module_header"].get<std::string>();
	result_opt.outputModuleSource = json_obj["out_source"].get<std::string>();
	result_opt.outputModuleFileDirectory = json_obj["out_dir"].get<std::string>();
	result_opt.precompiledHeader = json_obj["pch"].get<std::string>();
	result_opt.templateDirectory = json_obj["template_directory"].get<std::string>();

	std::vector<std::string> bool_required_keys = {
		"force_rebuild",
		"display_diagnostics",
	};
	for (const auto& one_key : bool_required_keys)
	{
		auto temp_value_iter = json_obj.find(one_key);
		if (temp_value_iter == json_obj.end())
		{
			std::cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
		if (!temp_value_iter->is_boolean())
		{
			std::cerr << "cant find bool value for key " << one_key << std::endl;
			return false;
		}
	}
	result_opt.forceRebuild = json_obj["force_rebuild"].get<bool>();
	result_opt.displayDiagnostics = json_obj["display_diagnostics"].get<bool>();

	std::vector<std::string> vector_required_keys = {
		"includes",
		"defines",
	};
	for (const auto& one_key : vector_required_keys)
	{
		auto temp_value_iter = json_obj.find(one_key);
		if (temp_value_iter == json_obj.end())
		{
			std::cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
		if (!temp_value_iter->is_array())
		{
			std::cerr << "cant find vector value for key " << one_key << std::endl;
			return false;
		}
		for (auto& one_item : *temp_value_iter)
		{
			if (!one_item.is_string())
			{
				std::cerr << "expect string for key " << one_key << " but get " << one_item << std::endl;
				return false;
			}
		}
	}
	for (const auto& one_item : json_obj["includes"].get<std::vector<std::string>>())
	{
		result_opt.arguments.emplace_back("-I" + one_item);
	}
	for (const auto& one_item : json_obj["defines"].get<std::vector<std::string>>())
	{
		result_opt.arguments.emplace_back("-D" + one_item);
	}

}