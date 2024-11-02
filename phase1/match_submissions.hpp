#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

// Function to find matching subsequences
int LengthOfExactMatch(const std::vector<int>& vec1, const std::vector<int>& vec2, int minLength) {
    int len1 = vec1.size();
    int len2 = vec2.size();
    int sum_exact_matches = 0;

    // Loop through all possible lengths between minLength and maxLength
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

        if(match_pos1>=minLength) {
            sum_exact_matches += match_pos1;
            pos1+=match_pos1-1;
        }
    }
    return sum_exact_matches;
}

std::pair<int, int> traversal(int t){
    int x = std::sqrt(t);

    int diff = t - x*x;
    if(diff<x) return {x+1, diff+1};
    else return {diff-x+1, x+1};
}

int approximate_match(const std::vector<int>& doc1, const std::vector<int>& doc2, int start1, int start2, double mismatch_threshold) {
    int m = doc1.size() - start1;
    int n = doc2.size() - start2;

    int min_mn = std::min(m,n);
    std::vector<std::vector<int>> dp(min_mn + 1, std::vector<int>(min_mn + 1, 0));
    for (int i = 0; i <= min_mn; ++i) {
        dp[i][0] = i;
        dp[0][i] = i;
    }

    int longest_len = 0;
    int continuous_mismatch = 0;

    for (int t = 0; t < min_mn*min_mn; ++t) {
        auto [i, j] = traversal(t);

        if (doc1[start1 + i - 1] == doc2[start2 + j - 1]) {
            dp[i][j] = dp[i - 1][j - 1];
        } else {
            dp[i][j] = 1 + std::min(dp[i - 1][j], std::min(dp[i][j - 1], dp[i - 1][j - 1]));
        }

        if (dp[i][j] < mismatch_threshold * (i + j)/2) {
            longest_len = std::max(longest_len, (i + j) / 2);
            continuous_mismatch = 0;
        }
        else {
            continuous_mismatch ++;
        }

        if (std::max(i,j)>=10 && continuous_mismatch == 2*std::max(i,j)+1) break;
    }

    if (longest_len == min_mn) {
        int dist = dp[min_mn][min_mn];

        int extra_len = 0;
        while (extra_len<abs(m-n) && dist < mismatch_threshold * (min_mn + extra_len/2)) {
            longest_len++;
            extra_len++;
            dist ++;
        }
    }
    return longest_len;
}

#include <future>
#include <mutex>

std::tuple<int, int, int> longest_approximate_match(const std::vector<int>& doc1, const std::vector<int>& doc2, double mismatch_threshold) {
    int len1 = doc1.size();
    int len2 = doc2.size();

    std::mutex mtx;
    int longest_len = 0;
    int best_pos1 = -1, best_pos2 = -1;

    // Vector to store futures
    std::vector<std::future<void>> futures;

    for (int i = 0; i < len1; i += 10) {
        for (int j = 0; j < len2; j += 10) {
            if ((len1 - i + len2 - j) / 2 < longest_len + 10) continue; // early stopping

            // Launch each approximate_match in a separate thread
            futures.emplace_back(std::async(std::launch::async, [&, i, j] {
                int match_len = approximate_match(doc1, doc2, i, j, mismatch_threshold);

                std::lock_guard<std::mutex> lock(mtx);
                if (match_len > longest_len) {
                    longest_len = match_len;
                    best_pos1 = i;
                    best_pos2 = j;
                }
            }));
        }
    }

    // Wait for all tasks to complete
    for (auto& f : futures) {
        f.get();
    }

    if (longest_len < 30) return {0, -1, -1};
    return {longest_len, best_pos1, best_pos2};
}

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

std::tuple<int, int, int> longestApproximateSubsequence3DP(
    const std::vector<int>& vector1, 
    const std::vector<int>& vector2, 
    int max_mismatches) 
{
    int n = vector1.size();
    int m = vector2.size();

    // DP table where dp[i][j] keeps track of longest subsequence length ending at vector1[i-1] and vector2[j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    int max_len = 0;
    int best_start1 = -1;
    int best_start2 = -1;

    // Track mismatch counts separately to avoid exceeding max_mismatches
    std::vector<std::vector<int>> mismatches(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            // Reset for any new starting points, only extend for matches within mismatch tolerance
            if (vector1[i - 1] == vector2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                mismatches[i][j] = mismatches[i - 1][j - 1];
            } else if (mismatches[i - 1][j - 1] < max_mismatches) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                mismatches[i][j] = mismatches[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = 0;
                mismatches[i][j] = 0;
            }

            // Update maximum length and track start indices if a new max is found
            if (dp[i][j] > max_len) {
                max_len = dp[i][j];
                best_start1 = i - dp[i][j];
                best_start2 = j - dp[i][j];
            }
        }
    }

    return {max_len, best_start1, best_start2};
}


std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    int len1 = submission1.size(), len2 = submission2.size();

    std::array<int, 5> result = {0, 0, 0, 0, 0};

    result[1] = LengthOfExactMatch(submission1, submission2, 10);

    std::tuple<int, int, int> match = longestApproximateSubsequence3DP(submission1, submission2, 0.05*(submission1.size()+submission2.size()));
    result[2] = std::get<0>(match);
    result[3] = std::get<1>(match);
    result[4] = std::get<2>(match);

    // result[2] = longestApproximateSubsequence_3D_DP(submission1, submission2, 0.025*(submission1.size()+submission2.size()), result[3], result[4]); // 10% mismatch threshold
    // std::tuple<int, int, int> match = longest_approximate_match(submission1, submission2, 0.10); // 10% mismatch threshold
    // result[2] = std::get<0>(match);
    // result[3] = std::get<1>(match);
    // result[4] = std::get<2>(match);

    if (result[1] > 0.5*(len1+len2)/2 && result[2] > 0.2* (len1+len2)/2) result[0]=1;
    // plagged if 20% of continuous code approx matches, and almost 50% of code is exact copied

    return result;
}