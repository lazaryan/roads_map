#ifndef __ROADS_GRAPH_LEVIT_H__
#define __ROADS_GRAPH_LEVIT_H__

#include <map>
#include <vector>
#include <deque>

#include "./Roads.h"

namespace Graph
{

/*
 * Алгоритм Левит
 */
Roads::t_graph_path levit_search(
	Roads::t_distrcits_path_map graph,
	Roads::t_road_weights road_weights,
	Roads::t_district_id start_id,
	Roads::t_district_id end_id
)
{
	const int INF = 1000 * 1000 * 1000;

	std::map<Roads::t_district_id, Roads::t_road_weight> distances;
	std::map<Roads::t_district_id, Roads::t_district_id> parents;
	std::map<Roads::t_district_id, Roads::t_road_weight> p;
	std::map<Roads::t_district_id, Roads::t_road_weight> id_visited;

	std::deque<Roads::t_district_id> queue;

	queue.push_back(start_id);

	for (const auto& [distrcit_id, distrcits_to] : graph)
	{
		distances[distrcit_id] = INF;
		p[distrcit_id] = -1;
	}
	distances[start_id] = 0;

	while (!queue.empty())
	{
		Roads::t_district_id queue_district = queue.front();
		queue.pop_front();

		id_visited[queue_district] = 1;

		for (const auto& [distrcit_to_id, road_id] : graph[queue_district])
		{
			if (distances[distrcit_to_id] > distances[queue_district] + road_weights[road_id])
			{
				distances[distrcit_to_id] = distances[queue_district] + road_weights[road_id];
				parents[distrcit_to_id] = queue_district;

				if (id_visited[distrcit_to_id] == 0)
				{
					queue.push_back(distrcit_to_id);
				}
				else if (id_visited[distrcit_to_id] == 1)
				{
					queue.push_front(distrcit_to_id);
				}

				p[distrcit_to_id] = queue_district;
				id_visited[distrcit_to_id] = 1;
			}
		}
	}

	if (distances[end_id] == INF)
	{
		std::cerr << "No path from: " << start_id << " to: " << end_id << std::endl;
		return Roads::t_graph_path();
	}

	Roads::t_graph_path path;
	Roads::t_district_id tmp_district = end_id;
	while (tmp_district != start_id)
	{
		Roads::t_district_id distrcit_from = parents[tmp_district];
		Roads::t_district_id distrcit_to = tmp_district;

		Roads::t_road_id road_id = graph[distrcit_from][distrcit_to];

		path.push_back(road_id);
		tmp_district = distrcit_from;
	}

	std::reverse(std::begin(path), std::end(path));
	return path;
}

}

#endif // !__ROADS_GRAPH_LEVIT_H__
