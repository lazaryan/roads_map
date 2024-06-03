#ifndef __ROADS_STRUCT_H__
#define __ROADS_STRUCT_H__

#include <vector>
#include <string>
#include <map>

#include "../../libs/ini.h"

#include "../Databases/District.h"
#include "../Databases/Road.h"

namespace Roads
{

typedef int t_district_id;
typedef int t_road_id;
typedef long t_road_weight;

typedef std::map<t_district_id, Databases::District> t_district_map;
typedef std::map<t_road_id, Databases::Road> t_road_map;
typedef std::map<t_road_id, t_road_weight> t_road_weights;

typedef std::map<t_district_id, t_road_id> t_distrcit_road_map;
// distrcit_from_id => distrcit_to_id => road_id
typedef std::map<t_district_id, t_distrcit_road_map> t_distrcits_path_map;

typedef std::vector<t_road_id> t_graph_path;
typedef std::vector<Databases::Road> t_road_path;

const int MAX_SETTING_WEIGHT = 1000;
const int MAX_VALUE_QALITY_WEIGHT = 10;

/*
* Настройки для подсчета весов
* значения от 0 до 1000
*/
struct Settings
{
	int road_lengths_weight;
	int quality_of_roads_weight;
	int speed_bumbs_weight;
	int traffic_light_weight;
};

struct RoadPath
{
	Databases::Road road;
	Databases::District district_to;
};

struct RoadNode
{
	Databases::District district;
	std::vector<RoadPath> paths;
};

class Roads
{
public:
	Roads(
		std::vector<Databases::District*> districts,
		std::vector<Databases::Road*> roads,
		mINI::INIMap<std::string> settings
	);
	~Roads();
	 
	t_road_path generate_paths(
		t_district_id district_from_id,
		t_district_id district_to_id
	);
private:
	t_district_map districts_map;
	t_road_map roads_map;
	t_road_weights road_weights;

	std::map<t_district_id, RoadNode> nodes;

	t_distrcits_path_map distrcits_map_paths;

	Settings settings;

	void set_settings(mINI::INIMap<std::string> settings);

	std::map<t_district_id, RoadNode> generate_tree(
		t_district_map districts_map,
		t_road_map roads_map
	);

	t_distrcits_path_map generater_districts_map(
		t_district_map districts_map,
		t_road_map roads_map
	);

	t_road_weights get_road_weights(t_road_map roads_map, Settings settings);
	t_road_weight get_road_weight(Databases::Road road, Settings settings);

	static inline Settings DEFAULT_SETTINGS = {
		100,
		100,
		100,
		100,
	};
};

}

#endif // !__ROADS_STRUCT_H__
