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
#include <cxxopts.hpp>




using json = nlohmann::json;
void parse(int argc, char* argv[]);

bool parse_by_command_line(int argc, char* argv[], ReflectionOptions& result_opt);
bool parse_json_config(std::string config_file_path, ReflectionOptions& result_opt);
using namespace std;

int main(int argc, char *argv[])
{
	
    auto start = std::chrono::system_clock::now( );
    // parse command line
    try 
    {
        // path to the executable
        auto exeDir = fs::path( argv[ 0 ] ).parent_path( );

        // set the working directory to the executable directory
        if (!exeDir.empty( ))
            fs::current_path( exeDir );

       

        parse(argc, argv);
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

    cout << "Completed in " 
              << std::chrono::duration_cast<std::chrono::milliseconds>( duration ).count( ) 
              << "ms" << std::endl;

    return EXIT_SUCCESS;
}

void parse(int argc, char* argv[])
{
    ReflectionOptions options;
	if (!parse_by_command_line(argc, argv, options))
	{
		cerr << "cant parse config " << std::endl;
		return;
	}

    
    cout << std::endl;
    cout << "Parsing reflection data for target \"" 
              << options.targetName << "\"" 
              << std::endl;
	auto& logger = utils::get_logger();
	logger.info("targetName: {}", options.targetName);
	logger.info("sourceRoot: {}", options.sourceRoot);
	logger.info("inputSourceFile: {}", options.inputSourceFile);
	logger.info("moduleHeaderFile: {}", options.moduleHeaderFile);
	logger.info("outputModuleSource: {}", options.outputModuleSource);
	logger.info("outputModuleFileDirectory: {}", options.outputModuleFileDirectory);
	logger.info("templateDirectory: {}", options.templateDirectory);
	logger.info("precompiledHeader: {}", options.precompiledHeader);
	logger.info("forceRebuild: {}", options.forceRebuild);
	logger.info("displayDiagnostics: {}", options.displayDiagnostics);
	for (const auto& one_arg : options.arguments)
	{
		logger.info("arguments: {}", one_arg);
	}
    ReflectionParser parser(options);

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
bool parse_by_command_line(int argc, char* argv[], ReflectionOptions& result_opt)
{
	cxxopts::Options options("MetaParser", "A program to do script work on cpp with the help of libclang");
	options.add_options()
		("o, display_diagnostics", "Whether or not to display diagnostics from clang.", cxxopts::value<bool>())
		("t, target_name", "Input target project name.", cxxopts::value<std::string>())
		("r, source_root", "Root source directory that is shared by all header files.", cxxopts::value<std::string>())
		("i, in_source", "Source file (header) to compile reflection data from.", cxxopts::value<std::string>())
		("m, module_header", "Header file that declares this reflection module.", cxxopts::value<std::string>())
		("s, out_source", "Output generated C++ module source file.", cxxopts::value<std::string>())
		("c, out_dir", "Output directory for generated C++ module file, header / source files.", cxxopts::value<std::string>())
		("d, temp_directory", "Directory that contains the mustache templates.", cxxopts::value<std::string>()->default_value("Templates/"))
		("p, pch", "Optional name of the precompiled header file for the project.", cxxopts::value<std::string>())
		("f, includes", "Optional file that includes the include directories for this target.", cxxopts::value<std::string>())
		("x, defines", "Optional list of definitions to include for the compiler.", cxxopts::value<std::vector<std::string>>())
		("e, force_rebuild", "Whether or not to ignore cache and write the header source files.", cxxopts::value<bool>());
	try
	{
		auto result = options.parse(argc, argv);
		result_opt.targetName = result["target_name"].as<std::string>();
		result_opt.sourceRoot = result["source_root"].as<std::string>();
		result_opt.inputSourceFile = result["in_source"].as<std::string>();
		result_opt.moduleHeaderFile = result["module_header"].as<std::string>();
		result_opt.outputModuleSource = result["out_source"].as<std::string>();
		result_opt.outputModuleFileDirectory = result["out_dir"].as<std::string>();
		result_opt.templateDirectory = result["temp_directory"].as<std::string>();
		if (result.count("pch"))
		{
			result_opt.precompiledHeader = result["pch"].as<std::string>();
		}
		result_opt.forceRebuild = result["force_rebuild"].as<bool>();
		result_opt.displayDiagnostics = result["display_diagnostics"].as<bool>();
		result_opt.arguments = { {
			"-x",
			"c++",
			"-std=c++17",
			"-D__REFLECTION_PARSER__"
		} };
		if (result.count("includes"))
		{
			auto includes = result["includes"].as<std::string >();
			std::ifstream includesFile(includes);

			std::string include;

			while (std::getline(includesFile, include))
				result_opt.arguments.emplace_back("-I" + include);
		}
		if (result.count("defines"))
		{
			for (const auto& one_define : result["defines"].as<std::vector<std::string>>())
			{
				result_opt.arguments.emplace_back("-D" + one_define);
			}
		}
		
		return true;
		
	}
	catch (std::exception& e)
	{
		cerr << "fail to parse the args: " << e.what() << std::endl;
		return false;
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
		cerr << e.what() << std::endl;
		cerr << "fail to parse config " << config_file_path << std::endl;
		return false;
	}
	if (!json_obj.is_object())
	{
		cerr << "the data should be a map" << std::endl;
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
			cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
		if (!temp_value_iter->is_string())
		{
			cerr << "cant find string value for key " << one_key << std::endl;
			return false;
		}
		auto temp_value = temp_value_iter->get<std::string>();
		if (temp_value.empty())
		{
			cerr << "cant find value for key " << one_key << std::endl;
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
			cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
		if (!temp_value_iter->is_boolean())
		{
			cerr << "cant find bool value for key " << one_key << std::endl;
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
			cerr << "cant find value for key " << one_key << std::endl;
			return false;
		}
		if (!temp_value_iter->is_array())
		{
			cerr << "cant find vector value for key " << one_key << std::endl;
			return false;
		}
		for (auto& one_item : *temp_value_iter)
		{
			if (!one_item.is_string())
			{
				cerr << "expect string for key " << one_key << " but get " << one_item << std::endl;
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
	return true;
}