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
	 * �������� ������ (������) ����� ����� ��������
	*/
	struct Road {
		t_road_id id;
		// ��� �����
		std::string name;
		// �� ������ ������ ��� �����
		t_district_id district_id_from;
		// ����
		t_district_id district_id_to;
		// ����� ������ � ���� ������������ �� 0 (��������) �� 10 (�������)
		int length;
		// �������� ������ � ���� ������������ �� 0 (�����) �� 10 (�����)
		int quality_of_roads;
		// ����� ������� ����������� �� �������
		int speed_bumbs_count;
		// ����� ���������� �� �������
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
					std::stoi(item[0]),
					item[1],
					std::stoi(item[2]),
					std::stoi(item[3]),
					std::stoi(item[4]),
					std::stoi(item[5]),
					std::stoi(item[6]),
					std::stoi(item[7]),
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
