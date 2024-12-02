
#include <iostream>
#include <fstream>
#include <vector>	
#include <string>
#include <sstream>
#include <algorithm>

bool test_condition(int a, int b) {
    bool out = (a < b && b - a >=1 && b - a <=3);
    return out;
    
}

std::vector<bool> check_descending(std::vector<int> measurements) {
    std::vector<unsigned long> errors;
    
    for (unsigned long i = 0; i < measurements.size() -1; i++) {
	if  (not(test_condition(measurements[i+1], measurements[i]))) {
	    errors.push_back(i);
	}
    }
    
    bool descending = errors.size() == 0;
    bool dampened_error = false;
    
    if (descending) {
	dampened_error = true;
    }

    if (errors.size() == 1) {
	if (errors[0] == 0) {
	    dampened_error = true;
	}
	if (errors[0] == measurements.size() -2) {
	    dampened_error = true;
	}

	if ((errors[0] < measurements.size() -2) && (errors[0] > 0)) {
	    dampened_error = (test_condition(measurements[errors[0]+2], measurements[errors[0]]) || test_condition(measurements[errors[0]+1], measurements[errors[0]-1]));
	}
    }


    if (errors.size() == 2) {
	if (errors[1] - errors[0] == 1) {
	    dampened_error = test_condition(measurements[errors[0]+2], measurements[errors[0]]);
	}
    }
    

    std::vector<bool> out = {descending, dampened_error};
    return out;
}

std::vector<bool> check_ascending(std::vector<int> measurements) {
    std::vector<unsigned long> errors;

    for (unsigned long i = 0; i < measurements.size() -1; i++) {
	if  (not(test_condition(measurements[i], measurements[i+1]))) {
	    errors.push_back(i);
	}
    }
    
    bool ascending = errors.size() == 0;
    bool dampened_error = false;
    
    if (ascending) {
	dampened_error = true;
    }

    if (errors.size() == 1) {
	if (errors[0] == 0) {
	    dampened_error = true;
	}

	if (errors[0] == measurements.size() -2) {
	    dampened_error = true;
	}

	if ((errors[0] < measurements.size() -2) && (errors[0] > 0)) {

	    dampened_error = (test_condition(measurements[errors[0]], measurements[errors[0]+2]) || test_condition(measurements[errors[0]-1], measurements[errors[0]+1]));
	}
    }

    if (errors.size() == 2) {	
	if (errors[1] - errors[0] == 1) {
	    dampened_error = test_condition(measurements[errors[0]], measurements[errors[0]+2]);
	}
    }
    
    std::vector<bool> out = {ascending, dampened_error};
    return out;
}


void read_input(std::vector<std::vector<int>> & data, std::string filename) {
    std::ifstream input_stream(filename);
    std::string line;
    while (std::getline(input_stream, line)) {
        std::istringstream ss(line);
        std::vector<int> current_line;
        int num;
        
        // Read all integers from the current line
        while (ss >> num) {
            current_line.push_back(num);
        }

        
        // Add the vector of integers for this line to the main data vector
        data.push_back(current_line);
    }
}

int main (int argc, char* argv[]) {

    std::vector<std::vector<int>> data;

    if (argc < 2) {  // this implicitly checks for null argv[1]
	std::cerr << "No argument provided. Please use either 'full' or 'test' as args depending on which dataset you want to use\n";
	return 1;
    }

    if (std::string(argv[1]) == "full") {

	std::cout << "Full Input is being used" << std::endl;
	read_input (data, "day2/input.txt");
    }
    else if (std::string(argv[1]) == "test") {
	std::cout << "Test Input is being used" << std::endl;
	read_input (data, "day2/test.txt");
    }

    else {	
	std::cout << "Incorrect input specified. Please use either full or test" << std::endl;
    }
    
    int out_p1 = 0;
    int out_p2 = 0;

    int i = 0;
    for (auto row: data) {
	if (check_descending(row)[0] || check_ascending(row)[0]) {
	    out_p1+=1;
	}
	if (check_descending(row)[1] || check_ascending(row)[1]) {
	    out_p2+=1;
	}
	i++;
    }

    std::cout << "Part 1 Soln: " << out_p1 << std::endl;
    std::cout << "Part 2 Soln: " << out_p2 << std::endl;
}
