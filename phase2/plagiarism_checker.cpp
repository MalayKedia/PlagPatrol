#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".
// TODO: Implement the methods of the plagiarism_checker_t class
#include<functional>
int count = 0;

subm_token_time_ptr::subm_token_time_ptr(std::vector<int>& vec, double& d, std::shared_ptr<submission_t>& pt){
    tokens = vec;
    time = d;
    ptr = pt;
}

plagiarism_checker_t::plagiarism_checker_t(void){
    reqd_matches = 75;
    reqd_instances = 10;
}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
    for(auto submission : __submissions){
        tokenizer_t file_added(submission->codefile);
        std::vector<int> tokens = file_added.get_tokens();
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        submissions.push_back(subm_token_time_ptr(tokens, milliseconds, submission));
    }
    reqd_matches = 75;
    reqd_instances = 10;
}

plagiarism_checker_t::~plagiarism_checker_t(void){
    submissions.clear();
}

std::pair<int,int> plagiarism_checker_t::ExactMatchesInst(const std::vector<int>& vec1, const std::vector<int>& vec2, const int& minLength) {
    int len1 = vec1.size();
    int len2 = vec2.size();
    int max_exact_matches = 0;
    int inst = 0;

    // iterate over all possible starting positions in vec1 and vec2
    for(int pos1=0; pos1<len1; pos1++){
        int match_pos1 = 0;
        for(int pos2=0; pos2<len2; pos2++){
            int curr_match = 0;
            for(; pos1+curr_match<len1 && pos2+curr_match<len2; curr_match++){
                if(vec1[pos1+curr_match]!=vec2[pos2+curr_match]) break;
            }
            if(curr_match>match_pos1){
                match_pos1 = curr_match;
            }
        }
        std::cout<<"match_pos1: "<<match_pos1<<std::endl;
        // if a match of length minLength or more is found, add it to the sum and skip to the end of the match
        if(match_pos1>=minLength) {
            max_exact_matches += match_pos1;
            // max_exact_matches = std::max(max_exact_matches, match_pos1);
            // inst++;
            pos1+=match_pos1-1;
        }
    }
    return std::make_pair(max_exact_matches, inst);
}

void plagiarism_checker_t::processChunk(std::shared_ptr<submission_t> submission, std::vector<int> tokens, int start, int end, double now){
    std::cerr<<"Process Chunk "<<count<<"\n";
    std::cerr<<start<<","<<end<<std::endl;
    for (size_t i = start; i < end; ++i) {
        // Lock mutex for synchronized output
        // std::lock_guard<std::mutex> lock(mtx);
        std::cerr<<"hi"<<std::endl;
        std::pair<int,int> matches = this->ExactMatchesInst(tokens, submissions[i].tokens, minLengthToMatch);
        std::cerr<<"ANYTHING"<<std::endl;
        int max_matches = matches.first;
        int instances = matches.second;
        if(max_matches>=reqd_matches || instances>=reqd_instances){
            submission->student->flag_student(submission);
            submission->professor->flag_professor(submission);
        }
        if(submissions[i].time - now < 1){
            submissions[i].ptr->student->flag_student(submission);
            submissions[i].ptr->professor->flag_professor(submission);
        }

        std::cerr<<"ANYTHING"<<std::endl;
    }
}



void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
   
    std::cout<<"Adding submission: "<<count<<std::endl; count++;
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    tokenizer_t file_added(__submission->codefile);
    std::vector<int> tokens = file_added.get_tokens();

    submissions.push_back(subm_token_time_ptr(tokens, milliseconds, __submission));

    unsigned int num_threads = std::thread::hardware_concurrency()-5;
    size_t chunk_size = (submissions.size() + num_threads - 1) / num_threads;

    // Vector to hold threads
    std::thread thread;

    // Create threads
    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, submissions.size());

        // Create and add thread to the vector
        threads.emplace_back(
            std::bind(&plagiarism_checker_t::processChunk, this, __submission, tokens, start, end, milliseconds)
        );
        // threads.emplace_back(this->processChunk, __submission, tokens, start, end, milliseconds);
        // threads.emplace_back([this, __submission, &tokens, start, end, milliseconds]() {
        //     this->processChunk(__submission, tokens, start, end, milliseconds);
        // });
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
}

// End TODO