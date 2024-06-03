#include <map>
#include <algorithm>

#include "./Roads.h"
#include "./levit.h"
#include "./dijkstra.h"
#include "./ford_belman.h"

namespace Roads
{

Roads::Roads(
	std::vector<Databases::District*> districts,
	std::vector<Databases::Road*> roads,
	mINI::INIMap<std::string> settings_weights,
	mINI::INIMap<std::string> settings_search
)
{
	this->set_settings(settings_weights);
	this->settings_search = SearchSettings{
		settings_search.get("SEARCH_ALGORITHM")
	};

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
	this->roads_vector = roads;

	this->distrcits_map_paths = this->generater_districts_map(districts_map, roads_map);

	this->road_weights = this->get_road_weights(this->roads_map, this->settings_weights);
}

Roads::~Roads()
{
	this->districts_map.clear();
	this->roads_map.clear();
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

		this->settings_weights = settings;
	}
	catch (...) {
		std::cerr << "ERROR: fail get settings" << std::endl;
		std::exit(-1);
	}
}

t_distrcits_path_map Roads::generater_districts_map(
	t_district_map districts_map,
	t_road_map roads_map
)
{
	t_distrcits_path_map districts_path;

	for (const auto& [district_id, district] : districts_map)
	{
		t_distrcit_road_map district_roads_map;

		for (const Databases::t_road_id road_id : *district.road_ids)
		{
			Databases::Road road = roads_map[road_id];

			district_roads_map.emplace(
				road.district_id_to,
				road_id
			);
		}

		districts_path.emplace(
			district_id,
			district_roads_map
		);
	}

	return districts_path;
}

t_road_path Roads::generate_paths(
	t_district_id district_from_id,
	t_district_id district_to_id
)
{
	std::cout << "Generate tree path from " << district_from_id << " to " << district_to_id << std::endl;
	
	t_distrcits_path_map::const_iterator district_from_iter = this->distrcits_map_paths.find(district_from_id);
	t_distrcits_path_map::const_iterator district_to_iter = this->distrcits_map_paths.find(district_to_id);

	if (district_from_iter == this->distrcits_map_paths.end()) {
		std::cout << "District from " << district_from_id << " not found" << std::endl;
		return std::vector<Databases::Road>();
	}

	if (district_to_iter == this->distrcits_map_paths.end()) {
		std::cout << "District to " << district_to_id << " not found" << std::endl;
		return std::vector<Databases::Road>();
	}

	t_graph_path path;

	if (this->settings_search.alg == "ford_belman")
	{
		std::cout << "Usign alghoritm: " << "Ford-Belman" << std::endl;

		path = Graph::ford_belman_search(
			this->distrcits_map_paths,
			this->roads_vector,
			this->road_weights,
			this->distrcits_map_paths.size(),
			district_from_id,
			district_to_id
		);
	}
	else
	{
		std::cout << "Usign alghoritm: " << "Deikstra" << std::endl;

		path = Graph::dijkstra_search(
			this->distrcits_map_paths,
			this->road_weights,
			district_from_id,
			district_to_id
		);
	}

	t_road_path result_path;

	for (const t_road_id road_id : path)
	{
		result_path.push_back(this->roads_map[road_id]);
	}

	return result_path;
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
	);

	if (weight < 0) return 0;

	return weight;
}

}
