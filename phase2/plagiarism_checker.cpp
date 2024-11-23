#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".
// TODO: Implement the methods of the plagiarism_checker_t class
#include<functional>
int count = 0;

tokenised_submission::tokenised_submission(double& timestamp, std::shared_ptr<submission_t>& sub_ptr){
    tokenizer_t file_added(sub_ptr->codefile);
    tokens = file_added.get_tokens();
    time = timestamp;
    ptr = sub_ptr;
}

tokenised_submission::~tokenised_submission(){
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
    }
    reqd_matches = 75;
    reqd_instances = 10;
    std::cerr<<"plagiarism_checker_t constructor finished\n";
}

plagiarism_checker_t::~plagiarism_checker_t(void){
    submissions.clear();
}

std::pair<int,int> plagiarism_checker_t::ExactMatchesInst(const std::shared_ptr<tokenised_submission> sub1, const std::shared_ptr<tokenised_submission> sub2, const int& minLength) {
    int len1 = sub1->tokens.size();
    int len2 = sub2->tokens.size();
    int max_exact_matches = 0;
    int inst = 0;

    // iterate over all possible starting positions in vec1 and vec2
    for(int pos1=0; pos1<len1; pos1++){
        int match_pos1 = 0;
        for(int pos2=0; pos2<len2; pos2++){
            int curr_match = 0;
            for(; pos1+curr_match<len1 && pos2+curr_match<len2; curr_match++){
                if(sub1->tokens[pos1+curr_match]!=sub2->tokens[pos2+curr_match]) break;
            }
            if(curr_match>match_pos1){
                match_pos1 = curr_match;
            }
        }
        // std::cerr<<"match_pos1: "<<match_pos1<<std::endl;
        // if a match of length minLength or more is found, add it to the sum and skip to the end of the match
        if(match_pos1>=minLength) {
            // max_exact_matches += match_pos1;
            max_exact_matches = std::max(max_exact_matches, match_pos1);
            inst++;
            pos1+=match_pos1-1;
        }
    }
    std::cerr<<"Done exact matches\n";
    return std::make_pair(max_exact_matches, inst);
}


void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
   
    std::cerr<<"Adding submission: "<<count<<std::endl; count++;
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


    std::shared_ptr<tokenised_submission> curr_ptr = std::make_shared<tokenised_submission>(milliseconds, __submission);
    submissions.push_back(curr_ptr);

    bool plag_found = false;
    for(std::shared_ptr<tokenised_submission> sub_ptr: submissions){
        if(sub_ptr == curr_ptr) continue;

        if (!plag_found){
            auto [max_matches, instances] = ExactMatchesInst(curr_ptr, sub_ptr, 20);

            if(max_matches>=reqd_matches || instances>=reqd_instances){
                std::cerr<<"Plagiarism detected for new file\n";
                std::cerr<<"max_matches: "<<max_matches<<" instances: "<<instances<<std::endl;
                plag_found = true;
                
                __submission->student->flag_student(__submission);
                __submission->professor->flag_professor(__submission);
                flagged_files.insert(curr_ptr->ptr);

                if(sub_ptr->time - curr_ptr->time < 1000 && flagged_files.find(sub_ptr->ptr)==flagged_files.end()){
                    std::cerr<<"Plaiarism detected for old file\n";
                    std::cerr<<"max_matches: "<<max_matches<<" instances: "<<instances<<std::endl;
                    sub_ptr->ptr->student->flag_student(sub_ptr->ptr);
                    sub_ptr->ptr->professor->flag_professor(sub_ptr->ptr);

                    flagged_files.insert(sub_ptr->ptr);
                }
            }

        }
        else if (plag_found && sub_ptr->time - curr_ptr->time < 1000 && flagged_files.find(sub_ptr->ptr)==flagged_files.end()){
            auto [max_matches, instances] = ExactMatchesInst(curr_ptr, sub_ptr, 20);

            if (max_matches>=reqd_matches || instances>=reqd_instances){
                std::cerr<<"Plagiarism detected for old file\n";
                std::cerr<<"max_matches: "<<max_matches<<" instances: "<<instances<<std::endl;
                sub_ptr->ptr->student->flag_student(sub_ptr->ptr);
                sub_ptr->ptr->professor->flag_professor(sub_ptr->ptr);

                flagged_files.insert(sub_ptr->ptr);
            }
        }
    }
}

// End TODO