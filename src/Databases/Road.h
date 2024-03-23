#ifndef __DATABASES_ROAD_H__
#define __DATABASES_ROAD_H__

#include <algorithm>
#include <string>
#include <iostream>

#include "../CSV/CSVReader.h"

namespace Databases {
	typedef int t_district_id;
	typedef int t_road_id;

	struct Road {
		t_road_id id;
		t_district_id district_id_from;
		t_district_id district_id_to;
		int weight;
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
			if (item.size() != 4) {
				return nullptr;
			}

			try
			{
				Road* new_item = new Road{
					std::stoi(item[0]),
					std::stoi(item[1]),
					std::stoi(item[2]),
					std::stoi(item[3]),
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
