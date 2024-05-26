# Database structures

## Dictricts

| dictric_id | title  | road_ids |
| ---------- | :----- | :------- |
| number     | string | number[] |

## Roads

| road_id | name   | dictric_from | dictric_to | length   | quality_of_roads | speed_bumbs_count | traffic_lights_count | averager_coeff (skip) |
| ------- | :----- |:------------ | :--------- | :------- | :--------------- | :---------------- | :------------------- | :-------------------- |
| number  | number | number       | number     | [0...10] | [0...10]         | number            | number               | [0...1]               |
