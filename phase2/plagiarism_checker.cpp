#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".
// TODO: Implement the methods of the plagiarism_checker_t class

#define REQD_LEN_EXACT 75
#define REQD_INST_DIRECT 10
#define REQD_INST_PATCH 20
#define MINLEN 15

tokenised_submission::tokenised_submission(int timestamp, std::shared_ptr<submission_t>& sub_ptr): ptr(sub_ptr), time(timestamp), flagged(false) {
    // Tokenising in the worker thread
}

tokenised_submission::~tokenised_submission(){
}

void tokenised_submission::flag(void){
    ptr->student->flag_student(ptr);
    ptr->professor->flag_professor(ptr);
    flagged = true;
}

plagiarism_checker_t::plagiarism_checker_t(void): done(false), reqd_len_exact(REQD_LEN_EXACT), reqd_instances_exact(REQD_INST_DIRECT), reqd_instances_patchwork(REQD_INST_PATCH), minLengthToMatch(MINLEN) {
    start_worker_thread();
}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions): done(false), reqd_len_exact(REQD_LEN_EXACT), reqd_instances_exact(REQD_INST_DIRECT), reqd_instances_patchwork(REQD_INST_PATCH), minLengthToMatch(MINLEN) {

    for(auto submission : __submissions){
        add_original_submission(submission);
    }

    start_worker_thread();
}

void plagiarism_checker_t::start_worker_thread(void){
    workerThread = std::thread([this] {
        while (true) {
            std::shared_ptr<tokenised_submission> curr_ptr;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCV.wait(lock, [this] { return !inputQueue.empty() || done; });
                if (done && inputQueue.empty()) return;
                curr_ptr = inputQueue.front();
                inputQueue.pop();
            }
            process_submission(curr_ptr);
        }
    });
}

plagiarism_checker_t::~plagiarism_checker_t(void){
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        done = true;
    }
    queueCV.notify_all();  // Notify the worker thread to stop

    if (workerThread.joinable()) workerThread.join();

    // std::cerr<<"times\n";
    // double start_time = 0;
    // for (auto sub : submissions){
    //     if (sub->time == 0) continue;
    //     if (start_time == 0) start_time = sub->time;
    //     std::cerr<<sub->time - start_time<<std::endl;
    // }

    // std::cerr<<"Indexes\n";
    // for (auto sub: submissions){
    //     std::cerr<<"size "<<sub->tokens.size()<<std::endl;
    //     for (auto indices: sub->match_indices){
    //         std::cerr<<indices<<" ";
    //     }
    //     std::cerr<<std::endl;
    // }
}

void plagiarism_checker_t::add_original_submission(std::shared_ptr<submission_t> __submission){
    std::shared_ptr<tokenised_submission> curr_ptr = std::make_shared<tokenised_submission>(0, __submission);
    
    curr_ptr->tokens = tokenizer_t(__submission->codefile).get_tokens();
    curr_ptr->flagged = true; // So that it is never flagged in the future

    submissions.push_back(curr_ptr);
}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::shared_ptr<tokenised_submission> curr_ptr = std::make_shared<tokenised_submission>(milliseconds, __submission);

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        inputQueue.push(curr_ptr);  // Add to the processing queue
    }
    queueCV.notify_one();  // Notify the worker thread
}

void plagiarism_checker_t::process_submission(std::shared_ptr<tokenised_submission> curr_ptr){
    curr_ptr->tokens = tokenizer_t(curr_ptr->ptr->codefile).get_tokens();

    bool plag_found = false; // tracks if plagiarism has been found for the new file
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        for(std::shared_ptr<tokenised_submission> sub_ptr: submissions){

            if (!plag_found){
                auto [max_matches, instances] = ExactMatchesInst(curr_ptr, sub_ptr, minLengthToMatch);

                if(max_matches>=reqd_len_exact || instances>=reqd_instances_exact){
                    // std::cerr<<"Plagiarism detected for new file\n";
                    // std::cerr<<"max_matches: "<<max_matches<<" instances: "<<instances<<std::endl;
                    plag_found = true;
                    curr_ptr->flag();                    

                    if(!sub_ptr->flagged && (curr_ptr->time - sub_ptr->time <= 1000 || sub_ptr->match_indices.size()>=reqd_instances_patchwork)){
                        // std::cerr<<"Plagiarism detected for old file with time diff: "<<sub_ptr->time - curr_ptr->time<<std::endl;
                        // std::cerr<<"max_matches: "<<max_matches<<" instances: "<<instances<<std::endl;
                        sub_ptr->flag();
                    }
                }

            }
            // If plaigarism in new file has already been detected, check old files within 1 sec 
            else if (curr_ptr->time - sub_ptr->time <= 1000 && !sub_ptr->flagged){
                auto [max_matches, instances] = ExactMatchesInst(curr_ptr, sub_ptr, minLengthToMatch);

                if (max_matches>=reqd_len_exact || instances>=reqd_instances_exact || sub_ptr->match_indices.size()>=reqd_instances_patchwork){
                    // std::cerr<<"Plagiarism detected for old file with time diff: "<<sub_ptr->time - curr_ptr->time<<std::endl;
                    // std::cerr<<"max_matches: "<<max_matches<<" instances: "<<instances<<std::endl;
                    // std::cerr<<"Patchwork size "<<sub_ptr->match_indices.size()<<std::endl;
                    sub_ptr->flag();
                }
            }
        }

        submissions.push_back(curr_ptr); // The current submission is added to vector at the end of it checking against previous files
    }
}

std::pair<int,int> ExactMatchesInst(const std::shared_ptr<tokenised_submission> sub1, const std::shared_ptr<tokenised_submission> sub2, const int minLength) {
    int len1 = sub1->tokens.size();
    int len2 = sub2->tokens.size();
    int max_exact_matches = 0;
    int inst = 0;

    // iterate over all possible starting positions in vec1 and vec2
    for(int pos1=0; pos1<len1; pos1++){
        int largest_match = 0, match_pos2 = 0;
        for(int pos2=0; pos2<len2; pos2++){
            int curr_match = 0;
            for(; pos1+curr_match<len1 && pos2+curr_match<len2; curr_match++){
                if(sub1->tokens[pos1+curr_match]!=sub2->tokens[pos2+curr_match]) break;
            }
            if(curr_match>largest_match){
                largest_match = curr_match;
                match_pos2 = pos2;
            }
        }
        // if a match of length minLength or more is found, add it to the sum and skip to the end of the match
        if(largest_match>=minLength) {
            // max_exact_matches += match_pos1;
            max_exact_matches = std::max(max_exact_matches, largest_match);
            inst++;
            pos1+=largest_match-1;

            for (int i=pos1+minLength/2; i<pos1+largest_match; i+=minLength){
                auto ub = sub1->match_indices.upper_bound(i);
                if (ub == sub1->match_indices.end()){
                    sub1->match_indices.insert(i);
                }
                else if (*(sub1->match_indices.lower_bound(i)) -i >= minLength*0.8 && (ub == sub1->match_indices.begin() || i - *(--ub) >= minLength*0.8)){
                    sub1->match_indices.insert(i);
                }
            }
            if (!sub2->flagged){
                for (int i=match_pos2+minLength/2; i<match_pos2+largest_match; i+=minLength){
                    auto ub = sub2->match_indices.upper_bound(i);
                    if (ub == sub2->match_indices.end()){
                        sub2->match_indices.insert(i);
                    }
                    else if (*(sub2->match_indices.lower_bound(i)) -i >= minLength*0.8 && (ub == sub2->match_indices.begin() || i - *(--ub) >= minLength*0.8)){
                        sub2->match_indices.insert(i);
                    }
                }
            }
        }
    }
    return {max_exact_matches, inst};
}

// End TODO