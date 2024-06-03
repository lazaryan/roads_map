#include <map>
#include <vector>
#include <deque>

#include "./Roads.h"

namespace Graph
{
const int inf = 1000*1000*1000;

void levit_search(
	std::map<Roads::t_district_id, Roads::RoadNode> graph,
	Roads::t_road_weights weights,
	Roads::t_district_id start_id,
	Roads::t_district_id end_id
)
{
	size_t n = graph.size();

	std::vector<Roads::t_district_id> d(n, inf);
	d[start_id] = 0;

	std::vector<Roads::t_district_id> id(n);

	std::deque<Roads::t_district_id> q;
	q.push_back(start_id);

	std::vector<Roads::t_district_id> p(n, -1);

	while (!q.empty())
	{
		Roads::t_district_id v = q.front();
		q.pop_front();

		id[v] = 1;

		std::vector<Roads::RoadPath> paths = graph[v].paths;

		for (size_t i = 0; i < paths.size(); ++i)
		{
			/*int to = paths[i].first,
				len = paths[i].second;*/

			Roads::t_district_id to = paths[i].district_to.id;
			Roads::t_road_weight weight = weights[paths[i].road.id];


			if (d[to] > d[v] + weight)
			{
				d[to] = d[v] + weight;
				if (id[to] == 0)
					q.push_back(to);
				else if (id[to] == 1)
					q.push_front(to);
				p[to] = v;
				id[to] = 1;
			}
		}
	}

	int a = 0;
}

}

