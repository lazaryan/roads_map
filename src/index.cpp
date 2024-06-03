#include <iostream>
#include <vector>
#include <string>

#include "../libs/ini.h"

#include "Databases/District.h"
#include "Databases/Road.h"

#include "Roads/Roads.h"

std::string const DEFAULT_ROADS_CSV_PATH = "./database/roads.csv";
std::string const DEFAULT_DICTRICTS_CSV_PATH = "./database/dictrict.csv";
std::string const PATH_TO_SETTINGS = "./settings.ini";

int main() {
	mINI::INIFile file_settings(PATH_TO_SETTINGS);
	mINI::INIStructure ini_settings;
	file_settings.read(ini_settings);

	mINI::INIMap paths_settings = ini_settings.get("paths");

	std::string const setting_roads_path = paths_settings.get("ROADS_CSV_DATABASE");
	std::string const roads_path = setting_roads_path.length() > 0 ? setting_roads_path : DEFAULT_ROADS_CSV_PATH;

	std::string const setting_dictrict_path = paths_settings.get("DISTRICTS_CSV_DATABASE");
	std::string const dictrict_path = setting_dictrict_path.length() > 0 ? setting_dictrict_path : DEFAULT_DICTRICTS_CSV_PATH;

	Databases::Roads* roads = new Databases::Roads(roads_path);
	Databases::Districts* districts = new Databases::Districts(dictrict_path);

	std::vector<Databases::District*> districts_list = districts->read_all();
	std::vector<Databases::Road*> roads_list = roads->read_all();

	if (roads->size() == 0) {
		std::cout << "ERROR reading: List roads is empty" << std::endl;
		return -1;
	}

	if (districts->size() == 0) {
		std::cout << "ERROR reading: List districts is empty" << std::endl;
		return -1;
	}

	mINI::INIMap search_settings = ini_settings.get("search");

	Roads::Roads* roads_map = new Roads::Roads(
		districts_list,
		roads_list,
		ini_settings.get("weights"),
		search_settings
	);

	std::string distrcit_from_str_setting = search_settings.get("SEARCH_FROM_ID");
	std::string distrcit_to_str_setting = search_settings.get("SEARCH_TO_ID");

	Roads::t_district_id distrcit_from = distrcit_from_str_setting.length() > 0 ? std::stoi(distrcit_from_str_setting) : 1;
	Roads::t_district_id distrcit_to = distrcit_to_str_setting.length() > 0 ? std::stoi(distrcit_to_str_setting) : 8;

	auto paths = roads_map->generate_paths(
		distrcit_from,
		distrcit_to
	);

	std::cout << "path from " << distrcit_from << " to " << distrcit_to << ":" << std::endl;

	for (Databases::Road road : paths)
	{
		std::cout << "{ road_id: \"" << road.id << "\"; road_name: \"" << road.name << "\"; }" << std::endl;
	}

	return 0;
}
