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
	this->road_weights = this->get_road_weights(this->roads_map, this->settings);
}

Roads::~Roads()
{
	this->districts_map.clear();
	this->roads_map.clear();

	this->nodes.clear();
}

void Roads::set_settings(mINI::INIMap<std::string> settings_map)
{
	std::string road_lengths_weight = settings_map.get("WEIGHT_ROAD_LENGTH");
	std::string quality_of_roads_weight = settings_map.get("WEIGHT_QUALITY_OF_ROADS");
	std::string speed_bumbs_weight = settings_map.get("WEIGHT_SPEED_BUMBS");
	std::string traffic_light_weight = settings_map.get("WEIGHT_TRAFFIC_LIGHT");

	try
	{
		Settings settings = Settings {
			road_lengths_weight.length() > 0 ? std::stoi(road_lengths_weight) : Roads::DEFAULT_SETTINGS.road_lengths_weight,
			quality_of_roads_weight.length() > 0 ? std::stoi(quality_of_roads_weight) : Roads::DEFAULT_SETTINGS.quality_of_roads_weight,
			speed_bumbs_weight.length() > 0 ? std::stoi(speed_bumbs_weight) : Roads::DEFAULT_SETTINGS.speed_bumbs_weight,
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
	t_district_map districts_map,
	t_road_map roads_map
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
					<< road_id << " for district "
					<< district_id << ", because road not found" << std::endl;
				continue;
			};

			std::map<t_district_id, Databases::District>::const_iterator district = districts_map.find(road->second.district_id_to);
			if (district == districts_map.end()) {
				std::cout << "Skip add district_to "
					<< road->second.district_id_to << " for district "
					<< district_id << ", because district not found" << std::endl;
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

std::vector<Databases::Road> Roads::generate_paths(
	t_district_id district_from_id,
	t_district_id district_to_id
)
{
	std::cout << "Generate tree path from " << district_from_id << " to " << district_to_id << std::endl;
	
	std::map<t_district_id, RoadNode>::const_iterator district_from_iter = this->nodes.find(district_from_id);
	std::map<t_district_id, RoadNode>::const_iterator district_to_iter = this->nodes.find(district_to_id);

	if (district_from_iter == this->nodes.end()) {
		std::cout << "District from " << district_from_id << " not found" << std::endl;
		return std::vector<Databases::Road>();
	}

	if (district_to_iter == this->nodes.end()) {
		std::cout << "District to " << district_to_id << " not found" << std::endl;
		return std::vector<Databases::Road>();
	}

	return std::vector<Databases::Road>();
}

t_road_weights Roads::get_road_weights(t_road_map roads_map, Settings settings)
{
	t_road_weights weights = t_road_weights();

	for (const auto& [road_id, road] : roads_map)
	{
		t_road_weight weight = this->get_road_weight(road, settings);
		weights.emplace(road_id, weight);
	}

	return weights;
}

t_road_weight Roads::get_road_weight(Databases::Road road, Settings settings)
{
	t_road_weight weight = (t_road_weight)(
		road.length * settings.road_lengths_weight
		+ (MAX_VALUE_QALITY_WEIGHT - road.quality_of_roads) * settings.quality_of_roads_weight
		+ road.speed_bumbs_count * settings.speed_bumbs_weight
		+ road.traffic_lights_count * settings.traffic_light_weight
		) / (MAX_SETTING_WEIGHT * 4);

	if (weight < 0) return 0;
	if (weight > 1) return 1;

	return weight;
}

}
