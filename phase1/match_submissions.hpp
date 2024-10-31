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
            std::cerr<<"At "<<i<<" match of len "<<match<<"\n";
        }     
    }

    return count;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};

    result[1] = LengthOfExactMatch(submission1, submission2, 10);
    // result[2] = bitap_fuzzy_bitwise_search()
    // result[0] = (result[1]>0);


    return result; // dummy return
    // End TODO
}