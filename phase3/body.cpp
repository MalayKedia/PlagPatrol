#include "../tokenizer.hpp"
#include <iomanip>

// You should NOT modify ANYTHING in this file.
extern std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2);

void execute_and_verify_testcase(std::string test_dir) {
    tokenizer_t file_one(test_dir + "/one.cpp");
    tokenizer_t file_two(test_dir + "/two.cpp");
    std::vector<int> submission1 = file_one.get_tokens();
    std::vector<int> submission2 = file_two.get_tokens();
    std::array<int, 5> output = match_submissions(submission1, submission2);

    for(int i = 0; i < 5; i++) {
		std::cout<<output[i]<<" ";
    }
	std::cout<<std::endl;
}

int main(void) {

    // std::cout<<"Testcase 1: ";
    // execute_and_verify_testcase("testcases/one");

    // std::cout<<"Testcase 2: ";
    // execute_and_verify_testcase("testcases/two");

    // std::cout<<"Testcase 3: ";
    // execute_and_verify_testcase("testcases/three");

    std::cout<<"Test 1: ";
    execute_and_verify_testcase("testcases/test1");

    std::cout<<"Test 2: ";
    execute_and_verify_testcase("testcases/test2");

    std::cout<<"Test 3: ";
    execute_and_verify_testcase("testcases/test3");
    
    std::cout<<"Test 4: ";
    execute_and_verify_testcase("testcases/test4");

    std::cout<<std::endl;
    return 0;
}
