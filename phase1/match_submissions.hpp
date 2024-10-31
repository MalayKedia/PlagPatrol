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

void hash_doc(const std::vector<int> &doc, std::vector<long long int> &hashes, int ngram_size){
    long long int p = 331;
    long long int m = 1e9 + 9;
    long long int p_pow = 1;

    for(int i=0; i<ngram_size; i++){
        p_pow = (p_pow*p)%m;
    }
    
    int hash = 0;
    for(int i=0; i<doc.size(); i++){
        hash = (hash*p + doc[i])%m;
        if(i>=ngram_size-1){
            hashes.push_back(hash);
            hash = (hash - doc[i-ngram_size+1]*p_pow)%m;
            if(hash<0) hash += m;
        }
    }
}

void fingerprint_doc(const std::vector<int>& doc, std::vector<std::pair<long long int, int>>& fingerprints, int guarantee_threshold, int noise_threshold){
    // Size of n gram is noise_threshold
    int window_size = guarantee_threshold - noise_threshold + 1;

    std::vector<long long int> hashes;
    hash_doc(doc, hashes, noise_threshold);

    assert(fingerprints.size() == 0);

    int last_fp_posn = -1;
    for (int i = 0; i <= hashes.size() - window_size; i++) {
        long long min_hash = hashes[i];
        int min_pos = i;
        for (int j = i + 1; j < i + window_size; j++) {
            if (hashes[j] < min_hash) {
                min_hash = hashes[j];
                min_pos = j;
            }
        }
        if (last_fp_posn!=min_pos) {
            fingerprints.push_back({min_hash, min_pos});
            last_fp_posn = min_pos;
        }
    }
    
    // for (int i=0; i<hashes.size(); i++) {
        // fingerprints.push_back({hashes[i], i});
    // }
}

int extend_match(const std::vector<std::pair<long long, int>> fingerprints1, const std::vector<std::pair<long long, int>> fingerprints2, int pos1, int pos2, int mismatch_threshold = 4) {
    int n = fingerprints1.size() - pos1;
    int m = fingerprints2.size() - pos2;
    assert(n>0 && m>0);

    // DP table to store the longest match lengths
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    int longest_len = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (fingerprints1[pos1+ i - 1].first == fingerprints2[pos2+ j - 1].first) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1+ std::min( dp[i - 1][j], std::min(dp[i][j - 1] , dp[i - 1][j - 1]));
            }

            // Update longest match if this is the largest match length found
            if (dp[i][j] < mismatch_threshold) {
                int len_new1 = fingerprints1[pos1 + i - 1].second - fingerprints1[pos1].second + 1;
                int len_new2 = fingerprints2[pos2 + j - 1].second - fingerprints2[pos2].second + 1;

                longest_len = std::max(longest_len, (len_new1 + len_new2) / 2);
            }
        }
    }
    
    return longest_len;
}

std::tuple<int, int, int> longest_approximate_match(const std::vector<int>& doc1, const std::vector<int>& doc2, int noise_threshold, int guarantee_threshold, int mismatch_threshold) {
    std::vector<std::pair<long long, int>> fingerprints1, fingerprints2;
    fingerprint_doc(doc1, fingerprints1, guarantee_threshold, noise_threshold);
    fingerprint_doc(doc2, fingerprints2, guarantee_threshold, noise_threshold);

    // std::cout<<"fingerprint1 size: "<<fingerprints1.size()<<"\n";
    // std::cout<<"\nfingerprint2 size: "<<fingerprints2.size()<<"\n";

    // for (int i=0; i<fingerprints1.size() && i<fingerprints2.size(); i++) {
    //     std::cout<<fingerprints1[i].first<<": "<<fingerprints1[i].second<<"  "<<fingerprints2[i].first<<": "<<fingerprints2[i].second<<"\n";
    // }


    std::unordered_map<long long, std::vector<int>> position_map;
    for (int i = 0; i < fingerprints1.size(); i++) {
        auto fp= fingerprints1[i];
        position_map[fp.first].push_back(i);
    }

    int longest_len = 0, best_pos1 = -1, best_pos2 = -1;
    for (int i = 0; i < fingerprints2.size(); i++) {
        const auto& fp = fingerprints2[i];

        if (position_map.find(fp.first) != position_map.end()) {
            for (int pos1 : position_map[fp.first]) {
                // std::cout<<"pos1: "<<pos1<<" pos2: "<<fp.second<<"\n";
                int match_len = extend_match(fingerprints1, fingerprints2, pos1, i, mismatch_threshold);
                if (match_len > longest_len) {
                    longest_len = match_len;
                    best_pos1 = pos1;
                    best_pos2 = fp.second;
                }
            }
        }
    }
    
    return {longest_len, best_pos1, best_pos2};
}



std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};

    result[1] = LengthOfExactMatch(submission1, submission2, 10);

    std::tuple<int, int, int> match = longest_approximate_match(submission1, submission2, 5, 15, 5);
    result[2] = std::get<0>(match);
    result[3] = std::get<1>(match);
    result[4] = std::get<2>(match);

    return result;
}