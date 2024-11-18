#include "structures.hpp"
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::vector<std::pair<std::vector<int>, std::chrono::system_clock::time_point>> submissions;
    // std::vector<std::chrono::system_clock::time_point> timestamps;
    std::pair<int,int> ExactMatchesInst(const std::vector<int>&, const std::vector<int>&, const int&);
    void processChunk(std::shared_ptr<submission_t>, std::vector<int>, int, int);
    int reqd_matches;
    int reqd_instances;
    int minLengthToMatch;
    // End TODO
};
