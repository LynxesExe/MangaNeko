#pragma once
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <string>
#include <fstream>

namespace MangaNeko {
	class GlobalConfiguration
	{
	public:
		GlobalConfiguration(const std::string& p_configFilePath)
		{
			configFilePath = p_configFilePath;

			std::ifstream ifs(configFilePath);
			std::string configFileRaw((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));

			rapidjson::Document parsedJson;
			parsedJson.Parse(configFileRaw.c_str());

			const rapidjson::Value& MangaNekoConfigs = parsedJson["MangaNeko"];
			if (!MangaNekoConfigs.IsObject())
			{
				throw std::runtime_error("Error: config file doesn not contain MangaNeko object!");
			}
			if (!MangaNekoConfigs["entrypoint"].IsString())
			{
				throw std::runtime_error("Error: your configs do not specified a required entrypoint!");
			}

			entryPointPath = MangaNekoConfigs["entrypoint"].GetString();
		}

	public:
		std::string configFilePath;
		std::string entryPointPath;
	};

	static const GlobalConfiguration globalConfiguration = GlobalConfiguration("../config/config.json");
}