#include <iostream>
#include <vector>
#include <set>
#include <sstream>

void inputNumbers(std::vector<int>& nums, int count) {
    std::cout << "Enter " << count << " integers separated by spaces: ";
    for (int i = 0; i < count; i++) {
        int num;
        std::cin >> num;
        nums.push_back(num);
    }
}

void removeNegativeNumbers(std::vector<int>& nums) {
    std::vector<int> positiveNums;
    for (int num : nums) {
        if (num >= 0) {
            positiveNums.push_back(num);
        }
    }
    nums = positiveNums;
}

int sumOfUnique(const std::vector<int>& nums) {
    std::set<int> uniqueNumbers;
    int sum = 0;
    for (int num : nums) {
        if (uniqueNumbers.find(num) == uniqueNumbers.end()) {
            uniqueNumbers.insert(num);
            sum += num;
        }
    }
    return sum;
}

void handleTestCases(int t) {
    for (int i = 1; i <= t; i++) {
        int count;
        std::cout << "Test Case #" << i << ": Enter number of elements: ";
        std::cin >> count;

        std::vector<int> nums;
        inputNumbers(nums, count);
        
        removeNegativeNumbers(nums);
        int result = sumOfUnique(nums);
        
        std::cout << "Sum of unique positive elements for Test Case #" << i << ": " << result << std::endl;
    }
}

void parseTestCases() {
    int t;
    std::cout << "Enter the number of test cases: ";
    std::cin >> t;

    if (t < 1) {
        std::cout << "Error: Number of test cases must be at least 1." << std::endl;
        return;
    }

    handleTestCases(t);
}

int main() {
    std::cout << "Unique Sum Calculator\n---------------------\n";
    parseTestCases();
    std::cout << "Calculation complete. Thank you for using the program!" << std::endl;
    return 0;
}
