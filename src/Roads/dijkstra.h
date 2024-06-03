#ifndef __ROADS_GRAPH_DEIKSTRA_H__
#define __ROADS_GRAPH_DEIKSTRA_H__


#include <map>
#include <vector>
#include <iostream>

#include "./Roads.h"

namespace Graph {

const Roads::t_road_weight MAX_WEIGHT = 100000000;

/*
 * Алгоритм Дейкстры
 */
Roads::t_graph_path dijkstra_search(
	Roads::t_distrcits_path_map graph,
	Roads::t_road_weights road_weights,
	Roads::t_district_id start_id,
	Roads::t_district_id end_id
)
{
	const size_t graph_size = graph.size();

	// минимальная дистанция
	std::map<Roads::t_district_id, Roads::t_road_weight> distances;
	// посещенные вершины
	std::map<Roads::t_district_id, bool> visited;

    Roads::t_road_weight minimalWeight;
    Roads::t_district_id minimalIndex;


	for (const auto& [district_id, district] : graph)
	{
        distances.emplace(district_id, MAX_WEIGHT);
		visited.emplace(district_id, false);
	}

    distances[start_id] = 0;

    do
    {
        minimalIndex = INT_MAX;
        minimalWeight = MAX_WEIGHT;

        for (const auto& [district_id, districts_to] : graph)
        {
            bool visited_district = visited[district_id];

            if (!visited_district && distances.count(district_id) && distances[district_id] < minimalWeight)
            {
                minimalIndex = district_id;
                minimalWeight = distances[district_id];
            }
        }

        if (minimalIndex != INT_MAX)
        {
            Roads::t_distrcit_road_map distrcits_to = graph[minimalIndex];
            
            for (const auto& [district_to_id, road_id] : distrcits_to)
            {
                Roads::t_road_weight temp = minimalWeight + road_weights[road_id];
                if (temp < distances[district_to_id])
                {
                    distances[district_to_id] = temp;
                }
            }

            visited[minimalIndex] = true;
        }
    } while (minimalIndex < INT_MAX);

    /**
     * Вычисление маршрута
     */
    Roads::t_road_weight end_weight = distances[end_id];
    Roads::t_district_id end_id_din = end_id;

    Roads::t_graph_path path;

    while (end_id_din != start_id)
    {
        for (auto& [district_id, distrcits_to] : graph)
        {
            if (distrcits_to.count(end_id_din))
            {
                Roads::t_road_id road_id = distrcits_to[end_id_din];
                Roads::t_road_weight temp = end_weight - road_weights[road_id];

                if (temp == distances[district_id])
                {
                    end_id_din = district_id;
                    end_weight = temp;
                    path.push_back(road_id);
                }
            }
        }
    }

    // Делаем маршрут start => finish
    std::reverse(std::begin(path), std::end(path));
    return path;
}

}

#endif // !__ROADS_GRAPH_DEIKSTRA_H__
