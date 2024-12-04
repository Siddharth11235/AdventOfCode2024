#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


void read_input (std::vector<std::string> & data, const std::string filename) {
    std::ifstream input_stream(filename);
    std::string line;

    while (std::getline(input_stream, line)) {
	data.push_back(line);
    }
}

bool check_sequence_validity (char x, char y, char z, char w) {
    return ((x == 'X'&& y == 'M' && z == 'A' && w=='S') or (x=='S' && y=='A' && z == 'M' && w=='X'));
}


bool check_mas_seq_validity(const std::vector<std::string> data, const unsigned long j, const unsigned long i) {
    
    char x1 = data[j-1][i-1];
    char z1 = data[j+1][i+1];

    char x2 = data[j-1][i+1];
    char z2 = data[j+1][i-1];

    bool cond1 ((x1 == 'M'&& z1 == 'S') or (x1=='S' && z1 == 'M'));
    bool cond2 ((x2 == 'M' && z2 == 'S') or (x2=='S' && z2 == 'M'));

    return (cond1 and cond2);
}

unsigned long count_sequences(const std::vector<std::string>& data, int dx, int dy) {
    unsigned long count = 0;

    int height = data.size();
    int width = data[0].length();

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            // ensure the 4-step sequence stays within bounds
            if (j + 3 * dy < height && j + 3 * dy >= 0 &&
                i + 3 * dx < width && i + 3 * dx >= 0) {
                if (check_sequence_validity(data[j][i],
                                            data[j + dy][i + dx],
                                            data[j + 2 * dy][i + 2 * dx],
                                            data[j + 3 * dy][i + 3 * dx])) {
                    count++;
                }
            }
        }
    }
    return count;
}
unsigned long get_x_mas_count(const std::vector<std::string> & data) {
    
    unsigned long count = 0;

    for (unsigned long j = 1; j < data.size()-1; ++j) {
	for (unsigned long i = 1; i < data[j].length() -1; ++i) {
	    if (data[j][i] == 'A'){
		if (check_mas_seq_validity(data, j, i)) {
		    count++;
		}	
	    }
	}
    }
    return count;
}

    
int main(int argc, char* argv[]) {
    std::vector<std::string> data;
    if (argc < 2) {  // this implicitly checks for null argv[1]
	std::cerr << "No argument provided. Please use either 'full' or 'test' as args depending on which dataset you want to use\n";
	return 1;
    }

    if (std::string(argv[1]) == "full") {
	std::cout << "Full Input is being used" << std::endl;
	read_input (data, "day4/input.txt");
    }
    else if (std::string(argv[1]) == "test") {
	std::cout << "Test Input is being used" << std::endl;
	read_input (data, "day4/test.txt");
    }

    else {	
	std::cout << "Incorrect input specified. Please use either full or test" << std::endl;
    }

    auto result_p1 = count_sequences(data, 1, 0) + count_sequences(data, 0, 1) + count_sequences(data, 1, 1) + count_sequences(data, -1, 1);
    auto result_p2 = get_x_mas_count(data);
    std::cout << "Part 1 Solution: " << result_p1 << std::endl;
    std::cout << "Part 2 Solution: " << result_p2 << std::endl;
}
