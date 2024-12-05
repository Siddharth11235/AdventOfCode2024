#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

void read_input (std::map<int, std::map<std::string, std::vector<int>>>  & rules, std::vector<std::vector<int>> & data, const std::string filename) {
    std::ifstream input_stream(filename);
    std::string line;
    
    bool is_data = false;

    while (std::getline(input_stream, line)) {
	if (line.empty()) {
	    is_data = true;
	}

	if (is_data && not(line.empty())) {
	    std::istringstream ss(line);
	    std::vector<int> current_line;
	    std::string num;
	
	    while (std::getline(ss, num, ',')) {
		current_line.push_back(std::stoi(num));
	    }

	    data.push_back(current_line);
	}
	else{

	    if (line.empty()) continue;
	    
	    size_t delim_pos = line.find('|');
	    if (delim_pos == std::string::npos) continue;
	    
	    int num1 = std::stoi(line.substr(0, delim_pos));
	    int num2 = std::stoi(line.substr(delim_pos + 1));
	    
	    // establish bidirectional relationship
	    rules[num1]["After"].push_back(num2);
	    rules[num2]["Before"].push_back(num1);
	} 
    }


}

int part1_soln(const std::vector<std::vector<int>> & data, std::map<int, std::map<std::string, std::vector<int>>> & rules, std::vector<std::vector<int>> & broken_updates) {
    int sum = 0;

    for (auto row: data) {
	bool is_ordered;
	bool before_case = true;
	bool after_case = true;
	
	for (unsigned long i = 0; i < row.size(); ++i) {

	    if (rules[row[i]].find("Before") == rules[row[i]].end()) {
		rules[row[i]]["Before"] = std::vector<int>{};
	    }

	    if (rules[row[i]].find("After") == rules[row[i]].end()) {
		rules[row[i]]["After"] = std::vector<int>{};
	    }
	    const auto &before_vec = rules.at(row[i]).at("Before");
            const auto &after_vec = rules.at(row[i]).at("After");

	    for (unsigned long j = 0; j < i; ++j) {
		
		if (std::find(before_vec.begin(), before_vec.end(), row[j]) == before_vec.end()) {
		    before_case = false;
		}

	    }
	    for (unsigned long j = i+1; j < row.size() ; ++j) {
		if (std::find(after_vec.begin(), after_vec.end(), row[j]) == after_vec.end()) {
		    after_case = false;
		}

	    }

	    is_ordered = before_case and after_case;
	}


	if (is_ordered) {
	    sum+=row[(row.size() -1) /2];
	}
	else {
	    broken_updates.push_back(row);
	}
    }
    
    return sum;
}


int count_occcurences(const std::vector<int> row, const std::vector<int> vec) {


    std::set<int> set_vec(vec.begin(), vec.end());
    int count = std::count_if(row.begin(), row.end(),
	[&set_vec](int x) { return set_vec.find(x) != set_vec.end(); });

    return count;
}

int part2_soln(std::vector<std::vector<int>> & broken_updates, const std::map<int, std::map<std::string, std::vector<int>>> & rules) {
    int sum = 0;
    for (auto row: broken_updates) {

	for (unsigned long i = 0; i < row.size(); ++i) {
	    const auto &before_vec = rules.at(row[i]).at("Before");
	    const auto &after_vec = rules.at(row[i]).at("After");

	    if (count_occcurences(row, before_vec) == count_occcurences(row, after_vec)) {
		sum+=row[i];
	    }	    
	}
    }


    return sum;
}


int main (int argc, char* argv[]) {

    std::vector<std::vector<int>> data;
    std::map<int, std::map<std::string, std::vector<int>>> rules;
    if (argc < 2) {  // this implicitly checks for null argv[1]
	std::cerr << "No argument provided. Please use either 'full' or 'test' as args depending on which dataset you want to use\n";
	return 1;
    }

    if (std::string(argv[1]) == "full") {

	std::cout << "Full Input is being used" << std::endl;
	read_input (rules, data, "day5/input.txt");
    }
    else if (std::string(argv[1]) == "test") {
	std::cout << "Test Input is being used" << std::endl;
	read_input (rules, data, "day5/test.txt");
    }

    else {	
	std::cout << "Incorrect input specified. Please use either full or test" << std::endl;
    }
    
    std::vector<std::vector<int>> broken_updates;
    int result_p1 = part1_soln(data, rules, broken_updates);
    std::cout << "Part 1 Solution: " << result_p1 << std::endl;

    int result_p2 = part2_soln(broken_updates, rules);
    std::cout << "Part 2 Solution: " << result_p2 << std::endl;
}
