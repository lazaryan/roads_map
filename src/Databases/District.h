#ifndef __DATABASES_DISTRICT_H__
#define __DATABASES_DISTRICT_H__

#include <algorithm>
#include <string>
#include <iostream>

#include "../CSV/CSVReader.h"

namespace Databases {
	typedef int t_district_id;
	typedef int t_road_id;

	/**
	 *  Сущность района
	 */
	struct District {
		t_district_id id;
		std::string title;
		std::vector<t_road_id>* road_ids;
	};

	class Districts : public CSV::CSVReader<District>
	{
	public:
		Districts(std::string path) : CSV::CSVReader<District>(path) {};
		Districts(std::string path, char c) : CSV::CSVReader<District>(path, c) {};
		Districts(std::string path, char c, char s) : CSV::CSVReader<District>(path, c) {
			if (s == c) {
				std::cerr << "ERROR::District => separator for columns and items equal" << std::endl;
				std::exit(-1);
			}

			this->separator_roads = s;
		};

		District* find_by_id(t_district_id id)
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
		char separator_roads = ',';

		District* line_reader(std::vector<std::string> item)
		{
			if (item.size() != 3) {
				return nullptr;
			}

			size_t position_separator = 0;
			std::string item_cache;
			std::vector<t_road_id>* roads = new std::vector<t_road_id>;

			while ((position_separator = item[2].find(this->separator_roads)) != std::string::npos) {
				item_cache = item[2].substr(0, position_separator);
				roads->push_back(std::stoi(item_cache));
				item[2].erase(0, position_separator + 1);
			}

			if (item[2].length() > 0)
			{
				roads->push_back(std::stoi(item[2]));
			}

			try
			{
				District* new_item = new District{
					std::stoi(item[0]),
					item[1],
					roads,
				};

				return new_item;
			}
			catch (...)
			{
				std::cerr << "ERROR: fail line_reader in district" << std::endl;
				return nullptr;
			}
		}
	};
}

#endif // !__DATABASES_DISTRICT_H__
