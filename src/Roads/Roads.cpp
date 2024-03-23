#include <map>
#include <algorithm>

#include "./Roads.h"

namespace Roads
{

Roads::Roads(
	std::vector<Databases::District*> districts,
	std::vector<Databases::Road*> roads,
	mINI::INIMap<std::string> settings
)
{
	this->set_settings(settings);

	// convert vectors to maps
	std::map<int, Databases::District> districts_map;
	std::map<int, Databases::Road> roads_map;

	std::transform(
		districts.begin(),
		districts.end(),
		std::inserter(districts_map, districts_map.end()),
		[](const Databases::District* item) { return std::make_pair(item->id, *item); }
	);

	std::transform(
		roads.begin(),
		roads.end(),
		std::inserter(roads_map, roads_map.end()),
		[](const Databases::Road* item) { return std::make_pair(item->id, *item); }
	);

	this->districts_map = districts_map;
	this->roads_map = roads_map;

	this->nodes = this->generate_tree(districts_map, roads_map);
}

Roads::~Roads()
{
	this->districts_map.clear();
	this->roads_map.clear();

	this->nodes.clear();
}

void Roads::set_settings(mINI::INIMap<std::string> settings_map)
{
	std::string traffic_light_weight = settings_map.get("WEIGHT_TRAFFIC_LIGHT");

	try
	{
		Settings settings = Settings {
			traffic_light_weight.length() > 0 ? std::stoi(traffic_light_weight) : Roads::DEFAULT_SETTINGS.traffic_light_weight,
		};

		this->settings = settings;
	}
	catch (...) {
		std::cerr << "ERROR: fail get settings" << std::endl;
		std::exit(-1);
	}
}

std::map<t_district_id, RoadNode> Roads::generate_tree(
	std::map<t_district_id, Databases::District> districts_map,
	std::map<t_road_id, Databases::Road> roads_map
)
{
	std::map<t_district_id, RoadNode> nodes;

	for (const auto& [district_id, district] : this->districts_map)
	{
		std::vector<RoadPath> paths;

		for (const auto road_id : *district.road_ids)
		{
			std::map<t_road_id, Databases::Road>::const_iterator road = roads_map.find(road_id);
			if (road == roads_map.end()) {
				std::cout << "Skip add road "
					<< road_id << "For district "
					<< district_id << "Because road not found" << std::endl;
				continue;
			};

			std::map<t_district_id, Databases::District>::const_iterator district = districts_map.find(road->second.district_id_to);
			if (district == districts_map.end()) {
				std::cout << "Skip add district_to "
					<< road->second.district_id_to << "For district "
					<< district_id << "Because district not found" << std::endl;
				continue;
			};

			RoadPath path = RoadPath{
				road->second,
				district->second
			};

			paths.push_back(path);
		}

		RoadNode node = RoadNode{
			district,
			paths
		};

		nodes.emplace(district_id, node);
	}

	return nodes;
}

std::vector<Databases::Road> Roads::generate_path(
	t_district_id district_from,
	t_district_id district_to
)
{
	std::vector<Databases::Road> path;
	// todo

	return path;
}

}
