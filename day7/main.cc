#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


struct InputData {
    long long target;
    std::vector<long long> values;
};


std::vector <InputData> read_input(const std::string filename) {
    std::vector<InputData> input_data_vec;
    std::ifstream input_stream(filename);
    if(!input_stream) {
        std::cerr << "could not open file: " << filename << std::endl;
        return input_data_vec;
    }

    std::string line;

    while (std::getline(input_stream, line)) {

	InputData input_data;
	size_t delim_pos = line.find(':');
	if (delim_pos == std::string::npos) continue;

	input_data.target = std::stoll(line.substr(0, delim_pos));
	std::string value_str = line.substr(delim_pos + 1);
	std::stringstream ss(value_str);
	int number;
	
	while (ss >> number) {
	    input_data.values.push_back(number);
	}


	input_data_vec.push_back(input_data);
    }

    return input_data_vec;
}


long long pipe_op(long long first_val, long long second_val) {

    std::string first_val_str = std::to_string(first_val);
    std::string second_val_str = std::to_string(second_val);

    std::string concatenated = first_val_str + second_val_str;

    return std::stoll(concatenated);
}

bool backtrack(unsigned long index, const long long target, long long current_val, const std::vector<long long> values, bool has_pipe_op) {

    if (index == values.size()) {
	return current_val == target;
    }
    
    std::vector<char> operators = {'+', '*', '|'};
    for (auto op: operators) {
	if (op == '+') {
	    if (backtrack(index+1, target, current_val+values[index], values, has_pipe_op)) {
		return true;
	    }
	}
	else if (op == '*') {
	    if  (backtrack(index+1, target, current_val*values[index], values, has_pipe_op)) {
		return true;
	    }
	}

	else if ((op == '|') and has_pipe_op) {
	    if (backtrack(index+1, target, pipe_op(current_val, values[index]), values, has_pipe_op)){
		return true;
	    }
	}
    }

    return false;
}

long long part1_soln(std::vector<InputData> input_data_vec) {
    long long sum = 0;

    for (auto input_data_row: input_data_vec) {
	if (backtrack(1, input_data_row.target, input_data_row.values[0], input_data_row.values, false)) {
	    sum+=input_data_row.target;
	}
    }

    return sum;
}

long long part2_soln(std::vector<InputData> input_data_vec) {

    long long sum = 0;

    for (auto input_data_row: input_data_vec) {
	if (backtrack(1, input_data_row.target, input_data_row.values[0], input_data_row.values, true)) {
	    sum+=input_data_row.target;
	}
    }

    return sum;

}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "no argument provided. use 'full' or 'test'\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string filename;
    if (mode == "full") {
        filename = "day7/input.txt";
    } else if (mode == "test") {
        filename = "day7/test.txt";
    } else {
        std::cerr << "incorrect input specified. use 'full' or 'test'\n";
        return 1;
    }

    auto input_data = read_input(filename);
    
    long result_p1 = part1_soln(input_data);
    std::cout << "Part 1 Solution: " << result_p1 << std::endl;
    long result_p2 = part2_soln(input_data);
    std::cout << "Part 2 Solution: " << result_p2 << std::endl;
}

