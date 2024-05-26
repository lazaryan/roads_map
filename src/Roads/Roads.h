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

typedef std::map<t_district_id, Databases::District> t_district_map;
typedef std::map<t_road_id, Databases::Road> t_road_map;

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
	 
	std::vector<Databases::Road> generate_paths(
		t_district_id district_from_id,
		t_district_id district_to_id
	);
private:
	t_district_map districts_map;
	t_road_map roads_map;

	std::map<t_district_id, RoadNode> nodes;

	std::map<t_district_id, RoadNode> generate_tree(
		t_district_map districts_map,
		t_road_map roads_map
	);

	Settings settings;

	void set_settings(mINI::INIMap<std::string> settings);

	static inline Settings DEFAULT_SETTINGS = {
		100,
		100,
		100,
		100,
	}; 
};

}

#endif // !__ROADS_STRUCT_H__
