#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".
// TODO: Implement the methods of the plagiarism_checker_t class

#define REQD_LEN_EXACT 75
#define REQD_INST_DIRECT 10
#define REQD_INST_PATCH 20
#define MINLEN 15

tokenised_submission::tokenised_submission(long long timestamp, std::shared_ptr<submission_t>& sub_ptr): ptr(sub_ptr), time(timestamp), flagged(false) {
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

    for(std::shared_ptr<submission_t> submission : __submissions){
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
                // Lock is unlocked until condition becomes true, or queueCV is notified
                if (done && inputQueue.empty()) return; 
                // If the queue is empty and the done flag is set, the thread should exit
                curr_ptr = inputQueue.front();
                inputQueue.pop();
            }
            process_submission(curr_ptr);
            submissions.push_back(curr_ptr); 
            // The current submission is added to vector at the end of it checking against previous files
        }
    });
}

plagiarism_checker_t::~plagiarism_checker_t(void){
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        done = true;
    }
    queueCV.notify_one();  // Notify the worker thread that done flag has been given

    if (workerThread.joinable()) workerThread.join();
}

void plagiarism_checker_t::add_original_submission(std::shared_ptr<submission_t> __submission){
    std::shared_ptr<tokenised_submission> curr_ptr = std::make_shared<tokenised_submission>(0, __submission);
    
    curr_ptr->tokens = tokenizer_t(__submission->codefile).get_tokens();
    curr_ptr->flagged = true; 
    // So that it original submissions are never flagged in the future

    submissions.push_back(curr_ptr);
}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::shared_ptr<tokenised_submission> curr_ptr = std::make_shared<tokenised_submission>(milliseconds, __submission);

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        inputQueue.push(curr_ptr);  // Add to the processing queue
    }
    queueCV.notify_one();  // Notify the worker thread
}

void plagiarism_checker_t::process_submission(std::shared_ptr<tokenised_submission> curr_ptr){
    // Tokenising the new submission
    curr_ptr->tokens = tokenizer_t(curr_ptr->ptr->codefile).get_tokens();

    bool plag_found = false; // tracks if plagiarism has been found for the new file

    for(std::shared_ptr<tokenised_submission> sub_ptr: submissions){
        if (!plag_found){
            auto [max_matches, instances] = ExactMatchesInst(curr_ptr, sub_ptr, minLengthToMatch);

            if(max_matches>=reqd_len_exact || instances>=reqd_instances_exact){
                // Handles direct plagiarism for the new submission
                plag_found = true;
                curr_ptr->flag();                    

                // Check if the old file with direct plaigarism is within 1 sec of the new file
                if(!sub_ptr->flagged && (curr_ptr->time - sub_ptr->time <= 1000)){
                    sub_ptr->flag();
                }
            }
            else if (curr_ptr->match_indices.size()>=reqd_instances_patchwork){
                // Handles patchwork plagiarism for the new submission
                plag_found = true;
                curr_ptr->flag();
            }
        }
        // If plaigarism in new file has already been detected, check old files which havent been flagged yet within 1 sec of new file
        else if (!sub_ptr->flagged && curr_ptr->time - sub_ptr->time <= 1000){
            auto [max_matches, instances] = ExactMatchesInst(curr_ptr, sub_ptr, minLengthToMatch);

            // If the old file has direct plagiarism with the new file, flag it
            if (max_matches>=reqd_len_exact || instances>=reqd_instances_exact){
                sub_ptr->flag();
            }
        }

        // If the old file has patchwork plagiarism with the new file, flag it
        if (!sub_ptr->flagged && sub_ptr->match_indices.size()>=reqd_instances_patchwork){
            sub_ptr->flag();
        }
    }
}

std::pair<int,int> ExactMatchesInst(const std::shared_ptr<tokenised_submission> sub1, const std::shared_ptr<tokenised_submission> sub2, const int minLength) {
    // sub1 will always be the newer submission, and sub2 will be the older submission
    int len1 = sub1->tokens.size();
    int len2 = sub2->tokens.size();
    int max_exact_match = 0;
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
        if(largest_match>=minLength) {
            // Update the max_exact_match and instances of plagiarism
            max_exact_match = std::max(max_exact_match, largest_match);
            inst++;
            pos1+=largest_match-1;

            // Update the match_indices for new submission if it is not flagged yet
            if (!sub1->flagged){
                for (int i=pos1+minLength/2; i<pos1+largest_match; i+=minLength){
                    auto lb = sub1->match_indices.lower_bound(i);
                    if (lb == sub1->match_indices.end()){
                        sub1->match_indices.insert(i);
                    }
                    else if (*lb -i >= minLength*0.8 && (lb == sub1->match_indices.begin() || i - *(--lb) >= minLength*0.8)){
                        sub1->match_indices.insert(i);
                    }
                }
            }

            // Update the match_indices for old submission if it is not flagged yet and is within 1 sec of new submission
            if (!sub2->flagged && sub1->time - sub2->time <= 1000){
                for (int i=match_pos2+minLength/2; i<match_pos2+largest_match; i+=minLength){
                    auto lb = sub2->match_indices.lower_bound(i);
                    if (lb == sub2->match_indices.end()){
                        sub2->match_indices.insert(i);
                    }
                    else if (*lb -i >= minLength*0.8 && (lb == sub2->match_indices.begin() || i - *(--lb) >= minLength*0.8)){
                        sub2->match_indices.insert(i);
                    }
                }
            }
        }
    }
    return {max_exact_match, inst};
}

// End TODO