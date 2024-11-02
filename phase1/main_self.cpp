#include "match_submissions.hpp"
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

//     std::cout<<"No of tokens in submission 1: "<<submission1.size()<<"\n";
//     std::cout<<"No of tokens in submission 2: "<<submission2.size()<<"\n\n";

//     std::cerr<<"Submission 1\n";
//     for(int i=0; i<submission1.size(); i++) std::cerr<<"("<<i<<")"<<submission1[i]<<" ";
//     for(int i=0; i<submission1.size(); i++) std::cerr<<submission1[i]<<" ";
//     std::cerr<<"\nSubmission 2\n";
//     for(int i=0; i<submission2.size(); i++) std::cerr<<"("<<i<<")"<<submission2[i]<<" ";
//     for(int i=0; i<submission2.size(); i++) std::cerr<<submission2[i]<<" ";
//     std::cerr<<"\n\n";


    std::array<int, 5> output = match_submissions(submission1, submission2);

    std::ifstream in(test_dir + "/expected.txt");
    std::array<int, 5> expected;
    in >> expected[0] >> expected[1] >> expected[2] >> 
            expected[3] >> expected[4];
    in.close();

    for(int i = 0; i < 5; i++) {
        std::cout<<"Result "<<i<<":\t";
		std::cout<<"Your output: "<<std::setw(10)<<std::left<<output[i];
		std::cout<<"\tSample output: "<<expected[i]<<std::endl;
    }
	std::cout<<std::endl;
}

int main(void) {

    std::cout<<"Testcase 1: "<<std::endl;
    execute_and_verify_testcase("testcases/one");

    std::cout<<"Testcase 2: "<<std::endl;
    execute_and_verify_testcase("testcases/two");

    std::cout<<"Testcase 3: "<<std::endl;
    execute_and_verify_testcase("testcases/three");

    std::cout<<"Testcase Neeraje_one: "<<std::endl;
    execute_and_verify_testcase("testcases/neeraje_one");

    std::cout<<"Testcase Neeraje_two: "<<std::endl;
    execute_and_verify_testcase("testcases/neeraje_two");

    std::cout<<"Testcase Malay_one: "<<std::endl;
    execute_and_verify_testcase("testcases/malay_one");

    return 0;
}
