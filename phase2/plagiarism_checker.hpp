#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <set>
#include <queue>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class tokenised_submission {
    public:
        std::shared_ptr<submission_t> ptr;
        int time;
        std::vector<int> tokens;

        std::set<int> match_indices;
        bool flagged;

        tokenised_submission(int, std::shared_ptr<submission_t>&);
        ~tokenised_submission();

        void flag(void);
};


class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    void start_worker_thread(void);
    void add_original_submission(std::shared_ptr<submission_t>);
    void process_submission(std::shared_ptr<tokenised_submission>);

    std::queue<std::shared_ptr<tokenised_submission>> inputQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    bool done = false;
    std::thread workerThread; 

    std::vector<std::shared_ptr<tokenised_submission>> submissions;
    int reqd_len_exact;
    int reqd_instances_exact;
    int reqd_instances_patchwork;
    int minLengthToMatch;
    // End TODO
};

std::pair<int,int> ExactMatchesInst(std::shared_ptr<tokenised_submission>, std::shared_ptr<tokenised_submission>, const int);