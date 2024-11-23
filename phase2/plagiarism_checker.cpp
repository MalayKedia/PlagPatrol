#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".
// TODO: Implement the methods of the plagiarism_checker_t class
#include<functional>
int count = 0;

subm_token_time_ptr::subm_token_time_ptr(double& d, std::shared_ptr<submission_t>& pt){
    tokenizer_t file_added(pt->codefile);
    tokens = file_added.get_tokens();
    time = d;
    ptr = pt;
}

subm_token_time_ptr::~subm_token_time_ptr(){
    // delete ptr;
}

plagiarism_checker_t::plagiarism_checker_t(void){
    reqd_matches = 75;
    reqd_instances = 10;
}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
    std::cerr<<"plagiarism_checker_t constructor called"<<std::endl;
    for(auto submission : __submissions){
        std::cerr<<"Adding an original submission\n";
        add_submission(submission);
        // auto now = std::chrono::system_clock::now();
        // auto duration = now.time_since_epoch();
        // double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        // submissions.push_back(subm_token_time_ptr(milliseconds, submission));
    }
    reqd_matches = 75;
    reqd_instances = 10;
    std::cerr<<"plagiarism_checker_t constructor finished\n";
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
        std::cerr<<"match_pos1: "<<match_pos1<<std::endl;
        // if a match of length minLength or more is found, add it to the sum and skip to the end of the match
        if(match_pos1>=minLength) {
            max_exact_matches += match_pos1;
            // max_exact_matches = std::max(max_exact_matches, match_pos1);
            // inst++;
            pos1+=match_pos1-1;
        }
    }
    std::cerr<<"Done exact matches\n";
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
   
    std::cerr<<"Adding submission: "<<count<<std::endl; count++;
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    subm_token_time_ptr curr(milliseconds, __submission);
    submissions.push_back(curr);

    // thread for the call
    std::thread thread;
    // thread.detach();

    for(auto subm: submissions){
        std::pair<int,int> matches = ExactMatchesInst(curr.tokens, subm.tokens, 10);
        int max_matches = matches.first;
        int instances = matches.second;
        if(max_matches>=reqd_matches || instances>=reqd_instances){
            __submission->student->flag_student(__submission);
            __submission->professor->flag_professor(__submission);
            if(subm.time - curr.time < 1){
                subm.ptr->student->flag_student(subm.ptr);
                subm.ptr->professor->flag_professor(subm.ptr);
            }
        }
    }
    std::cerr<<"thread detached here\n";
    // thread.detach();

}

// End TODO