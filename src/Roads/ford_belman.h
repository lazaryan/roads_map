#ifndef __ROADS_GRAPH_FORD_BELMAN_H__
#define __ROADS_GRAPH_FORD_BELMAN_H__


#include <map>
#include <vector>
#include <iostream>

#include "./Roads.h"

namespace Graph {

const int INF = 1000000000;

/*
 * Алгоритм Форда-Бельмана
 */
Roads::t_graph_path ford_belman_search(
    Roads::t_distrcits_path_map distrcits,
    std::vector<Databases::Road*> roads,
    Roads::t_road_weights road_weights,
    size_t count_districts,
    Roads::t_district_id start_distrcit_id,
    Roads::t_district_id end_distrcit_id
 )
 {
    std::map<Roads::t_district_id, Roads::t_road_weight> distances;
    std::map<Roads::t_district_id, Roads::t_district_id> parent_distrcits;

    const size_t roads_count = roads.size();

    for (const auto& [distrcit_id, distrcits_to] : distrcits)
    {
        distances[distrcit_id] = INF;
    }

    distances[start_distrcit_id] = 0;

    for (;;)
    {
        bool any = false;
        for (int j = 0; j < roads_count; ++j)
        {
            if (distances[roads[j]->district_id_from] < INF)
            {
                if (distances[roads[j]->district_id_to] > distances[roads[j]->district_id_from] + road_weights[roads[j]->id])
                {
                    distances[roads[j]->district_id_to] = distances[roads[j]->district_id_from] + road_weights[roads[j]->id];
                    parent_distrcits[roads[j]->district_id_to] = roads[j]->district_id_from;
                    any = true;
                }
            }
        }

        if (!any)  break;
    }

    if (distances[end_distrcit_id] == INF)
    {
        std::cerr << "No path from: " << start_distrcit_id << " to: " << end_distrcit_id << std::endl;
        return Roads::t_graph_path();
    }

    Roads::t_graph_path path;
    Roads::t_district_id tmp_district = end_distrcit_id;
    while (tmp_district != start_distrcit_id)
    {
        Roads::t_district_id distrcit_from = parent_distrcits[tmp_district];
        Roads::t_district_id distrcit_to = tmp_district;

        Roads::t_road_id road_id = distrcits[distrcit_from][distrcit_to];

        path.push_back(road_id);
        tmp_district = distrcit_from;
    }

    std::reverse(std::begin(path), std::end(path));
    return path;
 }

}

#endif // !__ROADS_GRAPH_FORD_BELMAN_H__
