#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>

bool validateInput(int num) {
    if (num < 0) {
        std::cout << "Warning: Negative value " << num << " will be ignored." << std::endl;
        return false;
    }
    return true;
}

void getInput(std::vector<int>& numbers, int count) {
    std::cout << "Please provide " << count << " values separated by spaces: ";
    int num;
    for (int i = 0; i < count; i++) {
        std::cin >> num;
        if (validateInput(num)) {
            numbers.push_back(num);
        }
    }
}

int calculateUniqueSum(const std::vector<int>& elements) {
    std::unordered_map<int, bool> seen;
    int uniqueSum = 0;

    for (int val : elements) {
        if (seen.count(val) == 0) {
            seen[val] = true;
            uniqueSum += val;
        }
    }
    return uniqueSum;
}

void displayResult(int testCase, int result) {
    std::cout << "Test Case #" << testCase << ": Sum of unique positive values = " << result << std::endl;
}

void executeTestCases(int testCount) {
    for (int j = 1; j <= testCount; j++) {
        int size;
        std::cout << "For Test Case #" << j << ", how many numbers would you like to enter? ";
        std::cin >> size;

        std::vector<int> elements;
        getInput(elements, size);
        
        int sumResult = calculateUniqueSum(elements);
        displayResult(j, sumResult);
    }
}

void run() {
    int testCases;
    std::cout << "Enter number of test cases: ";
    std::cin >> testCases;

    if (testCases <= 0) {
        throw std::invalid_argument("Error: Test case count must be a positive integer.");
    }

    executeTestCases(testCases);
}

int main() {
    std::cout << "Starting Unique Sum Calculator Program\n=====================================\n";
    try {
        run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Thank you for using the Unique Sum Calculator." << std::endl;
    return 0;
}
