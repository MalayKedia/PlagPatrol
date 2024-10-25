#include <array>
#include<cmath>
#include <iostream>
#include <span>
#include <vector>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
// #include <stdlib.h>
//  #include <string.h>
//  #include <limits.h>
 
//  const char *bitap_fuzzy_bitwise_search(const char *text, const char *pattern, int k)
//  {
//      const char *result = NULL;
//      int m = strlen(pattern);
//      unsigned long *R;
//      unsigned long pattern_mask[CHAR_MAX+1];
//      int i, d;
 
//      if (pattern[0] == '\0') return text;
//      if (m > 31) return "The pattern is too long!";
 
//      /* Initialize the bit array R */
//      R = malloc((k+1) * sizeof *R);
//      for (i=0; i <= k; ++i)
//          R[i] = ~1;
 
//      /* Initialize the pattern bitmasks */
//      for (i=0; i <= CHAR_MAX; ++i)
//          pattern_mask[i] = ~0;
//      for (i=0; i < m; ++i)
//          pattern_mask[pattern[i]] &= ~(1UL << i);
 
//      for (i=0; text[i] != '\0'; ++i) {
//          /* Update the bit arrays */
//          unsigned long old_Rd1 = R[0];
 
//          R[0] |= pattern_mask[text[i]];
//          R[0] <<= 1;
 
//          for (d=1; d <= k; ++d) {
//              unsigned long tmp = R[d];
//              /* Substitution is all we care about */
//              R[d] = (old_Rd1 & (R[d] | pattern_mask[text[i]])) << 1;
//              old_Rd1 = tmp;
//          }
 
//          if (0 == (R[k] & (1UL << m))) {
//              result = (text+i - m) + 1;
//              break;
//          }
//      }
 
//      free(R);
//      return result;
//  }


// int fuzzy_matches(const std::vector<int>& vec1, const std::vector<int>& vec2, int minLength, int err){
//     int len1 = vec1.size();
//     int len2 = vec2.size();
//     int count = 0;

//     for(int i=0; i<len1; i++){
//         for(int j=0; j<len2; j++){
//             for(int k=30; k<32; k++){
//                 if(bitap_fuzzy_bitwise_search())
//             }
//         }
//     }
// }


// Function to find matching subsequences
int LengthOfMatchingSubsequences(const std::vector<int>& vec1, const std::vector<int>& vec2, int minLength) {
    int len1 = vec1.size();
    int len2 = vec2.size();
    int count = 0;

    // Loop through all possible lengths between minLength and maxLength
    for(int i=0; i<len1; i++){
        int match = 0;
        for(int j=0; j<len2; j++){
            if(i>=1 && j>=1 && vec1[i-1]==vec2[j-1]) continue;
            int k;
            for(k=0; j+k<len2 && i+k<len1; k++){
                if(vec1[i+k]!=vec2[j+k]) break;
            }
            if(k>match){
                match = k;
            }
        }
        if(match>=minLength && match>count) count = match;      // use the maximum length of exact match
        // if(match>=minLength) count += match;         // use the total length of exact matches
    }

    return count;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};

    result[1] = LengthOfMatchingSubsequences(submission1, submission2, 10);
    // result[2] = bitap_fuzzy_bitwise_search()
    // result[0] = (result[1]>0);


    return result; // dummy return
    // End TODO
}