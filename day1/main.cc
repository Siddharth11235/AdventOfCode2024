#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <set>


void read_input(std::vector<int> & first, std::vector<int> & second, std::string filename) {
    std::ifstream input_stream(filename);
    std::string line;

    while (std::getline(input_stream, line)) {
	std::istringstream ss(line);
	int num1, num2;
	ss >> num1 >> num2;
	first.push_back(num1);
	second.push_back(num2);
    }
}

int part1_soln (std::vector<int> first, std::vector<int> second) {
    int result = 0;
    
    std::sort (first.begin(), first.end());
    std::sort (second.begin(), second.end() );
    
    for (int i = 0; i < first.size(); i++) {
	result+= abs(second[i] - first[i]);
    }

    return result;
}

int part2_soln (std::vector<int> first, std::vector<int> second) {
    
    int result = 0;
    std::set<int> first_vec_set;
    std::map <int, int> element_freq_map ;
    for (int i : first) {
	first_vec_set.insert(i);
    }
    
    for (int i: first_vec_set) {
	int element_freq = std::count(second.begin(), second.end(), i);
	element_freq_map[i] = element_freq;	
    }

    for (int i: first) {
	result+= (i*element_freq_map[i]);
    }

    return result;
} 

int main (int argc, char* argv[]) {

    std::vector<int> first;
    std::vector<int> second;


    if (argc < 2) {  // this implicitly checks for null argv[1]
	std::cerr << "No argument provided. Please use either 'full' or 'test' as args depending on which dataset you want to use\n";
	return 1;
    }

    if (std::string(argv[1]) == "full") {

	std::cout << "Full Input is being used" << std::endl;
	read_input (first, second, "day1/input.txt");
    }
    else if (std::string(argv[1]) == "test") {
	std::cout << "Test Input is being used" << std::endl;
	read_input (first, second, "day1/test.txt");
    }

    else {	
	std::cout << "Incorrect input specified. Please use either full or test" << std::endl;
    }

    
    //Part 1
    int result_p1 = part1_soln(first, second);
    std::cout << "Part 1 Solution: " << result_p1 << std::endl;

    //Part 2

    int result_p2 = part2_soln(first, second);
    std::cout << "Part 2 Solution: " << result_p2 << std::endl;
}

