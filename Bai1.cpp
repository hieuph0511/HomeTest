#include <iostream>
#include <vector>

// Hàm để hợp nhất hai mảng đã được sắp xếp
std::vector<int> merge_array(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> result;
    int i = 0, j = 0;
    
    while (i < arr1.size() && j < arr2.size()) {
        if (arr1[i] < arr2[j]) {
            result.push_back(arr1[i]);
            i++;
        } else {
            result.push_back(arr2[j]);
            j++;
        }
    }
    
    while (i < arr1.size()) {
        result.push_back(arr1[i]);
        i++;
    }
    
    while (j < arr2.size()) {
        result.push_back(arr2[j]);
        j++;
    }

    return result;
}

int main() {
    std::vector<int> arr1 = {1, 3, 5};
    std::vector<int> arr2 = {2, 4, 6};
    std::vector<int> merged = merge_array(arr1, arr2);

    for (int i = 0; i < merged.size(); i++) {
        std::cout << merged[i] << " ";
    }

    return 0;
}
