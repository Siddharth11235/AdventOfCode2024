#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <utility> 
#include <algorithm>
#include <set>

struct guard_state {
    std::pair<int, int> guard_pos;
    std::pair<int, int> guard_dir;

    bool operator<(const guard_state& other) const {
        // compare guard positions first
        if (guard_pos != other.guard_pos) return guard_pos < other.guard_pos;
        // if positions are equal, compare directions
        return guard_dir < other.guard_dir;
    }
};

struct guard_path {
    std::vector<guard_state> path;
    bool is_cyclical;
};

static bool file_exists(const std::string& filename) {
    std::ifstream test(filename);
    return test.good();
}

std::vector<std::vector<char>> read_input (const std::string filename) {
    std::vector<std::vector<char>> data;
    std::ifstream input_stream(filename);
    std::string line;
    while (std::getline(input_stream, line)) {
        std::istringstream ss(line);
        std::vector<char> current_line;
        char token;
        
        // Read all integers from the current line
        while (ss >> token) {
            current_line.push_back(token);
        }
        data.push_back(current_line);
    }
    return data;
}

std::pair<int, int> find_position(const std::vector<std::vector<char>> &matrix, char value) {
	for (size_t row = 0; row < matrix.size(); ++row) {
	    const auto &inner = matrix[row];
	    auto it = std::find(inner.begin(), inner.end(), value);
	    if (it != inner.end()) {
		size_t col = std::distance(inner.begin(), it);
		return {row, col};
	    }
	}
	throw std::logic_error("Guard position not found"); // this shouldn't happen
}


void change_direction(std::pair<int, int> & current_dir) {
    size_t tmp = current_dir.first;
    current_dir.first = current_dir.second;
    current_dir.second = -tmp;
};

bool is_within_bounds(int y, int x, int vertical_limit, int horizontal_limit) {
    return y >= 0 && y < vertical_limit && x >= 0 && x < horizontal_limit;
}


guard_path track_guard_path(const std::vector<std::vector<char>>& map) {
    int vertical_limit = map.size();
    int horizontal_limit = map[0].size();
   

    std::set<guard_state> visited_states;
    guard_path full_path;
    full_path.is_cyclical = false;

    guard_state state = {find_position(map, '^'), {-1, 0}}; // initial position and direction (up)

    while (true) {

        full_path.path.push_back(state);

        int y_next = state.guard_pos.first + state.guard_dir.first;
        int x_next = state.guard_pos.second + state.guard_dir.second;

        if (!visited_states.insert(state).second) {
            full_path.is_cyclical = true;
            break; // cyclical path detected
        }
        if (!is_within_bounds(y_next, x_next, vertical_limit, horizontal_limit)) {
            break;
        }

        if (map[y_next][x_next] == '#') {
            change_direction(state.guard_dir);
        } else {
            state.guard_pos = {y_next, x_next};
        }
    }

    return full_path;
}

int move_guard(std::vector<std::vector<char>> map) {
    auto full_path= track_guard_path(map);
    std::set<std::pair<int, int>> unique_positions;
    for (const auto& state : full_path.path) {
        unique_positions.insert(state.guard_pos);
    }

    return unique_positions.size();
}


int get_all_cyclical_paths(const std::vector<std::vector<char>> map) {
    const auto correct_path = track_guard_path(map);
    const auto guard_initial_pos = correct_path.path[0].guard_pos;    

    std::set<std::pair<int, int>>cycle_positions;

    int vertical_limit = map.size();
    int horizontal_limit = map[0].size();

    for (auto state: correct_path.path) {

	std::vector<std::vector<char>> modified_map = map;
	int next_pos_row = state.guard_pos.first + state.guard_dir.first;
	int next_pos_col = state.guard_pos.second + state.guard_dir.second;

	if (!((next_pos_row == guard_initial_pos.first) and (next_pos_col == guard_initial_pos.second)) and is_within_bounds(next_pos_row, next_pos_col, vertical_limit, horizontal_limit)){
	    modified_map[next_pos_row][next_pos_col] = '#';
	}
	auto path = track_guard_path(modified_map);

	if (path.is_cyclical) {
	    cycle_positions.insert(std::make_pair(next_pos_row, next_pos_col));
	}
    }

    return cycle_positions.size();
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "no argument provided. use 'full' or 'test'\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string filename;
    if (mode == "full") {
        filename = "day6/input.txt";
    } else if (mode == "test") {
        filename = "day6/test.txt";
    } else {
        std::cerr << "incorrect input specified. use 'full' or 'test'\n";
        return 1;
    }


    if(!file_exists(filename)) {
        std::cerr << "file not found: " << filename << "\n";
        return 1;
    }

    auto input_data = read_input(filename);


    int result_p1 = move_guard(input_data);
    std::cout << "Part 1 Solution: " << result_p1 << "\n";

    int result_p2 = get_all_cyclical_paths(input_data);
    std::cout << "Part 2 Solution: " << result_p2 << "\n";
}

