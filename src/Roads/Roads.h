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
typedef double t_road_weight;

typedef std::map<t_district_id, Databases::District> t_district_map;
typedef std::map<t_road_id, Databases::Road> t_road_map;
typedef std::map<t_road_id, double> t_road_weights;

const int MAX_SETTING_WEIGHT = 1000;
const int MAX_VALUE_QALITY_WEIGHT = 10;

/*
* ��������� ��� �������� �����
* �������� �� 0 �� 1000
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
	t_road_weights road_weights;

	std::map<t_district_id, RoadNode> nodes;

	Settings settings;

	void set_settings(mINI::INIMap<std::string> settings);

	std::map<t_district_id, RoadNode> generate_tree(
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
