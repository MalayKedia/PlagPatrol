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
    int count = 0;

    // Loop through all possible lengths between minLength and maxLength
    for(int i=0; i<len1; i++){
        int match = 0;
        for(int j=0; j<len2; j++){
            // if(i>=1 && j>=1 && vec1[i-1]==vec2[j-1]) break;
            int k;
            for(k=0; j+k<len2 && i+k<len1; k++){
                if(vec1[i+k]!=vec2[j+k]) break;
            }
            if(k>match){
                match = k;
            }
        }

        if(match>=minLength) {
            count += match;
            i+=match-1;
            // std::cerr<<"At "<<i<<" match of len "<<match<<"\n";
        }     
    }

    return count;
}

#include <unordered_map>
#include <cassert>
#include <span>

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
    for (int t = 0; t < min_mn*min_mn; ++t) {
        auto [i, j] = traversal(t);

        if (doc1[start1 + i - 1] == doc2[start2 + j - 1]) {
            dp[i][j] = dp[i - 1][j - 1];
        } else {
            dp[i][j] = 1+std::min( dp[i - 1][j], std::min(dp[i][j - 1], dp[i - 1][j - 1]));
        }

        int continuous_mismatch = 0;

        if (dp[i][j] < mismatch_threshold * std::min(i, j)) {
            longest_len = std::max(longest_len, (i + j) / 2);
            continuous_mismatch = 0;
        }
        else {
            continuous_mismatch ++;
        }

        if (continuous_mismatch == std::max(i,j)) break;
    }
    return longest_len;
}

std::tuple<int, int, int> longest_approximate_match(const std::vector<int>& doc1, const std::vector<int>& doc2, double mismatch_threshold) {
    int len1 = doc1.size();
    int len2 = doc2.size();

    int longest_len = 0;
    int best_pos1 = -1, best_pos2 = -1;
    for (int i = 0; i < len1; i+=10) {
        for (int j = 0; j < len2; j+=10) {
            if ((len1-i+len2-j)/2 < longest_len+10) continue; // early stopping

            int match_len = approximate_match(doc1, doc2, i, j, mismatch_threshold);
            if (match_len > longest_len) {
                longest_len = match_len;
                best_pos1 = i;
                best_pos2 = j;
            }
        }
    }
    return {longest_len, best_pos1, best_pos2};
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    int len1 = submission1.size(), len2 = submission2.size();

    std::array<int, 5> result = {0, 0, 0, 0, 0};

    result[1] = LengthOfExactMatch(submission1, submission2, 10);

    std::tuple<int, int, int> match = longest_approximate_match(submission1, submission2, 0.05); // 5% mismatch threshold
    result[2] = std::get<0>(match);
    result[3] = std::get<1>(match);
    result[4] = std::get<2>(match);

    if (result[1] > 0.5*(len1+len2)/2 && result[2] > 0.2* (len1+len2)/2) result[0]=1;
    // plagged if 20% of continuous code approx matches, and almost 50% of code is exact copied

    return result;
}