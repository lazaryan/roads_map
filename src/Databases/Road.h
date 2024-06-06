#ifndef __DATABASES_ROAD_H__
#define __DATABASES_ROAD_H__

#include <algorithm>
#include <string>
#include <iostream>

#include "../CSV/CSVReader.h"

namespace Databases {
	typedef int t_district_id;
	typedef int t_road_id;

	/**
	 * Сущность дороги (полосы) между двумя районами
	*/
	struct Road {
		t_road_id id;
		// имя ребра
		std::string name;
		// из какого района она ведет
		t_district_id district_id_from;
		// куда
		t_district_id district_id_to;
		// длина дороги в виде коэффициента от 0 (короткая) до 10 (длинная)
		int length;
		// качество дороги в виде коэффициента от 0 (плохо) до 10 (супер)
		int quality_of_roads;
		// число лежачих полицейских на участке
		int speed_bumbs_count;
		// число светофоров на участке
		int traffic_lights_count;
	};

	class Roads : public CSV::CSVReader<Road>
	{
	public:
		Roads(std::string path) : CSV::CSVReader<Road>(path) {};
		Roads(std::string path, char c) : CSV::CSVReader<Road>(path, c) {};

		Road* find_by_id(t_road_id id)
		{
			if (this->db.empty())
			{
				return nullptr;
			}

			for (size_t i = 0; i < this->db.size(); i++)
			{
				if (this->db[i]->id == id)
				{
					return this->db[i];
				}
			}

			return nullptr;
		}

	private:

		Road* line_reader(std::vector<std::string> item)
		{
			if (item.size() < 8) {
				return nullptr;
			}

			try
			{
				Road* new_item = new Road{
					this->convertUTF8StringToInt(item[0]),
					item[1],
					this->convertUTF8StringToInt(item[2]),
					this->convertUTF8StringToInt(item[3]),
					this->convertUTF8StringToInt(item[4]),
					this->convertUTF8StringToInt(item[5]),
					this->convertUTF8StringToInt(item[6]),
					this->convertUTF8StringToInt(item[7]),
				};

				return new_item;
			}
			catch (...)
			{
				std::cerr << "ERROR: fail line_reader in road" << std::endl;
				return nullptr;
			}
		}
	};
}

#endif // !__DATABASES_ROAD_H__
