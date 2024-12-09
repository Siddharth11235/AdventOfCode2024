#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <cmath>
std::vector<std::vector<char>> read_input(const std::string filename) {
    std::ifstream input_stream(filename);
    std::vector<std::vector<char>> input_data;

    if (!input_stream) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;

    while (std::getline(input_stream, line)) {
	std::vector<char> line_vec(line.begin(), line.end());
	input_data.push_back(line_vec);
    }

    return input_data;
}

std::map<char, std::vector<std::pair<long, long>>> map_antennas(const std::vector<std::vector<char>> & input_data) {
     
    std::map<char, std::vector<std::pair<long, long>>> antenna_map;
    for (unsigned long i = 0; i < input_data.size(); ++i) {
	auto row = input_data[i];
	for (unsigned long j = 0; j < row.size(); ++j) {
	    char antenna = row[j];
	    if (antenna != '.') {
		antenna_map[antenna].push_back(std::make_pair(i,j));
	    }
	}
    }
    return antenna_map;
}

std::pair<long, long> get_distance(std::pair<long, long> first_point, std::pair<long, long> second_point) {
    std::pair<long, long> dist;
    dist.first = std::abs(second_point.first - first_point.first);
    dist.second =std::abs(first_point.second- second_point.second);

    return dist;
}


std::pair<std::pair<long, long>, std::pair<long, long>> get_antinodes(std::pair<long, long> first_point, std::pair<long, long> second_point) {
    auto dist = get_distance(first_point, second_point);
    std::pair<long, long> first_antinode;
    std::pair<long, long> second_antinode;

    if (first_point.first > second_point.first) {
	first_antinode.first = first_point.first + dist.first;
	second_antinode.first = second_point.first - dist.first;
    }

    else {
	first_antinode.first = first_point.first - dist.first;
	second_antinode.first = second_point.first + dist.first;
    }


    if (first_point.second > second_point.second) {
	first_antinode.second = first_point.second + dist.second;
	second_antinode.second = second_point.second - dist.second;
    }

    else {
	first_antinode.second = first_point.second - dist.second;
	second_antinode.second = second_point.second + dist.second;
    }

    return std::make_pair(first_antinode, second_antinode);
}

bool is_within_bounds(std::pair<long, long> antinode, long num_rows, long num_cols) {
    if ((antinode.first >= 0) and (antinode.first < num_rows) and (antinode.second >= 0) and (antinode.second < num_cols)){
	return true;
    }

    return false;
}

std::set<std::pair<int, int>> get_all_antinodes(const std::map<char, std::vector<std::pair<long, long>>> & antenna_map, const unsigned long num_rows, const unsigned long num_cols) {
   
    std::set<std::pair<int, int>> all_antinodes;
    for (const auto & [key, positions]: antenna_map) {
	for (size_t i = 0; i < positions.size(); ++i) {
	    for (size_t j = i + 1; j < positions.size(); ++j) {
		auto antinodes = get_antinodes(positions[i], positions[j]);
		if (is_within_bounds(antinodes.first, num_rows, num_cols)) {
		    all_antinodes.insert(antinodes.first);
		}
		if (is_within_bounds(antinodes.second, num_rows, num_cols)) {
		    all_antinodes.insert(antinodes.second);
		}

	    }
	}
    }
    return all_antinodes;
}

std::set<std::pair<long, long>> get_resonant_antinodes(std::pair<long, long> first_point, std::pair<long, long> second_point, unsigned long num_rows, unsigned long num_cols) {
    auto dist = get_distance(first_point, second_point);
    bool is_pos_slope = first_point.second > second_point.second;
    int up_range;
    int down_range;
    if (dist.first > 0) {
	up_range = static_cast<int>(std::floor(first_point.first/dist.first));
	down_range = static_cast<int>(std::floor((num_rows -1 - second_point.first)/dist.first));
    }
    else {
	up_range = num_rows;
	down_range = num_cols;
    }

    if (dist.second > 0) {
	if (is_pos_slope) {

	    double floor_up_value = std::floor((num_cols -1 - first_point.second) / dist.second);

	    double floor_down_value = std::floor(second_point.second / dist.second);

	    up_range = std::min(static_cast<int>(floor_up_value), up_range);
	    down_range = std::min(static_cast<int>(floor_down_value), down_range);
	}
	else {
	    double floor_up_value = std::floor(first_point.second / dist.second);

	    double floor_down_value = std::floor((num_cols - 1 - second_point.second)/ dist.second);

	    up_range = std::min(static_cast<int>(floor_up_value), up_range);
	    down_range = std::min(static_cast<int>(floor_down_value), down_range);
	}
    }

    std::set<std::pair<long, long>> resonant_antinodes;
    for (int i = 0; i <= up_range; ++i) {
	std::pair<long, long> antinode;
	antinode.first = first_point.first - (i*dist.first);
	if (is_pos_slope) {
	    antinode.second = first_point.second + (i*dist.second);
	}
	else {
	    antinode.second = first_point.second - (i*dist.second);
	}

	resonant_antinodes.insert(antinode);
    } 
    
    for (int i = 0; i <= down_range; ++i) {
	std::pair<long, long> antinode;
	antinode.first = second_point.first + (i*dist.first);
	if (is_pos_slope) {
	    antinode.second = second_point.second - (i*dist.second);
	}
	else {
	    antinode.second = second_point.second + (i*dist.second);
	}

	resonant_antinodes.insert(antinode);
    }

    return resonant_antinodes;
}


std::set<std::pair<long, long>> get_all_resonant_antinodes(const std::map<char, std::vector<std::pair<long, long>>> & antenna_map, const unsigned long num_rows, const unsigned long num_cols) {
   
    std::set<std::pair<long, long>> all_resonant_antinodes;
    for (const auto & [key, positions]: antenna_map) {
	for (size_t i = 0; i < positions.size(); ++i) {
	    for (size_t j = i + 1; j < positions.size(); ++j) {
		auto resonant_antinodes = get_resonant_antinodes(positions[i], positions[j], num_rows, num_cols);
		all_resonant_antinodes.insert(resonant_antinodes.begin(), resonant_antinodes.end());
	    }
	}
    }
    return all_resonant_antinodes;
}
long part1_soln(const std::map<char, std::vector<std::pair<long, long>>> & antenna_map, const std::vector<std::vector<char>> & input_data) {

    auto num_rows = input_data.size();
    auto num_cols = input_data[0].size();

    auto all_antinodes = get_all_antinodes(antenna_map,num_rows,num_cols);
    return all_antinodes.size();
} 

long part2_soln(const std::map<char, std::vector<std::pair<long, long>>> & antenna_map, const std::vector<std::vector<char>> & input_data) {

    auto num_rows = input_data.size();
    auto num_cols = input_data[0].size();

    auto all_resonant_antinodes = get_all_resonant_antinodes(antenna_map,num_rows,num_cols);
    return all_resonant_antinodes.size();

}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "no argument provided. use 'full' or 'test'\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string filename;
    if (mode == "full") {
        filename = "day8/input.txt";
    } else if (mode == "test") {
        filename = "day8/test.txt";
    } else {
        std::cerr << "incorrect input specified. use 'full' or 'test'\n";
        return 1;
    }

    auto input_data = read_input(filename);
    auto antenna_map = map_antennas(input_data);
    
    long result_p1 = part1_soln(antenna_map, input_data);
    std::cout << "Part 1 Solution: " << result_p1 << std::endl;

    long result_p2 = part2_soln(antenna_map, input_data);
    std::cout << "Part 2 Solution: " << result_p2 << std::endl;
}
