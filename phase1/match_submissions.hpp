#include <array>
#include <iostream>
#include <span>
#include <vector>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

// Function to compare subsequences of specific length in two vectors
bool compareSubsequence(const std::vector<int>& vec1, int start1, const std::vector<int>& vec2, int start2, int length) {
    for (int i = 0; i < length; ++i) {
        if (vec1[start1 + i] != vec2[start2 + i]) {
            return false;
        }
    }
    return true;
}

// Function to find matching subsequences
int findMatchingSubsequences(const std::vector<int>& vec1, const std::vector<int>& vec2, int minLength, int maxLength) {
    int len1 = vec1.size();
    int len2 = vec2.size();
    int count = 0;

    // Loop through all possible lengths between minLength and maxLength
    for (int length = minLength; length <= maxLength; ++length) {
        // Check subsequences of 'length' in vec1 and vec2
        for (int i = 0; i <= len1 - length; ++i) { // subsequences from vec1
            for (int j = 0; j <= len2 - length; ++j) { // subsequences from vec2
                if (compareSubsequence(vec1, i, vec2, j, length)) {
                    count += length; 
                }
            }
        }
    }

    return count;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};

    result[1] = findMatchingSubsequences(submission1, submission2, 10, 20);
    result[0] = (result[1]>0);


    return result; // dummy return
    // End TODO
}