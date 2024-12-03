#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <fstream>


std::string read_input(const std::string& filename) {
    std::ifstream input_stream(filename);

    if (!input_stream) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::ostringstream buffer;
    buffer << input_stream.rdbuf();
    return buffer.str();
}

long evaluate_mul_expression(std::string mul_string) {
    
    mul_string = mul_string.substr(4, mul_string.size() - 5); // remove "mul(" and ")"

    // replace the comma with a space to separate the numbers
    std::replace(mul_string.begin(), mul_string.end(), ',', ' ');

    // parse the integers
    std::istringstream ss(mul_string);
    int num1, num2;
    ss >> num1 >> num2;
    
    int out = num1*num2;
    return out;
}

long regex_sum_per_string(std::string operation) {
    std::regex const e{"mul\\(\\d+,\\d+\\)", std::regex_constants::ECMAScript};
    long sum = 0;
    
    auto words_begin = std::sregex_iterator(operation.begin(), operation.end(), e);
    auto words_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
	std::smatch match = *i;
        std::string matched_string = match.str();
	sum+=evaluate_mul_expression(matched_string);
    }
    return sum;
}

long split_by_do_dont(std::string operation){

    long sum = 0;

    std::regex re{"(don't|do)"};
    std::sregex_iterator begin(operation.begin(), operation.end(), re);
    std::sregex_iterator end;

    // keep track of the current position for extracting contexts
    size_t current_pos = 0;
    
    std::vector<std::string> matches;
    std::vector<std::string> delimiters;

    for (; begin != end; ++begin) {
        std::smatch match = *begin;

        // extract the context up to the current match
        std::string context = operation.substr(current_pos, match.position() - current_pos);
        std::string delimiter = match.str();
        matches.push_back(context);

        // extract and print the matching delimiter
	
        delimiters.push_back(delimiter);
        // update the position to continue processing
        current_pos = match.position() + match.length();
    }

    // print remaining context after the last match
    if (current_pos < operation.length()) {
        std::string remaining_context = operation.substr(current_pos);
        matches.push_back(remaining_context);
    }

    sum+=regex_sum_per_string(matches[0]);
    for (unsigned long i = 0; i < delimiters.size(); ++i) {
	if (delimiters[i] == "do") {
	    sum+=regex_sum_per_string(matches[i+1]);
	}
    }

    return sum;

}

std::vector<long> soln (std::string operations) {
    long p1_sum = 0;
    long p2_sum = 0;
	
    p1_sum+=regex_sum_per_string(operations);
    p2_sum+=split_by_do_dont(operations);
    

    std::vector<long> out = {p1_sum, p2_sum};

    return out;

}


int main(int argc, char* argv[]) {
    
    std::string operations;
    if (argc < 2) {  // this implicitly checks for null argv[1]
	std::cerr << "No argument provided. Please use either 'full' or 'test' as args depending on which dataset you want to use\n";
	return 1;
    }

    if (std::string(argv[1]) == "full") {
	std::cout << "Full Input is being used" << std::endl;
	operations = read_input ("day3/input.txt");
    }
    else if (std::string(argv[1]) == "test") {
	std::cout << "Test Input is being used" << std::endl;
	operations = read_input ("day3/test.txt");
    }

    else {	
	std::cout << "Incorrect input specified. Please use either full or test" << std::endl;
    }

    auto result = soln(operations);
    std::cout << "Part 1 Solution: " << result[0] << std::endl;
    std::cout << "Part 2 Solution: " << result[1] << std::endl;

}
