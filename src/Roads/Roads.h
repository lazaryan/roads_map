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
	 
	std::vector<Databases::Road> generate_path(
		t_district_id district_from,
		t_district_id district_to
	);
private:
	std::map<t_district_id, Databases::District> districts_map;
	std::map<t_road_id, Databases::Road> roads_map;

	std::map<t_district_id, RoadNode> nodes;

	std::map<t_district_id, RoadNode> generate_tree(
		std::map<t_district_id, Databases::District> districts_map,
		std::map<t_road_id, Databases::Road> roads_map
	);

	void set_settings(mINI::INIMap<std::string> settings);

	int traffic_light_weight = 500;
};

}

#endif // !__ROADS_STRUCT_H__
