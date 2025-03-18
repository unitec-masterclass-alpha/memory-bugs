
#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stoi;
using std::ifstream;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    int n;
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Unable to open file " << argv[1] << endl;
        return 1;
    }
    input >> n; // read size of input

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }

    input.close();
    int sum = 0;
    for (int i = 0; i <= n; i++) {
        sum += arr[i];
        arr[i] = sum / arr[i];
    }
    cout << sum << endl;

    delete[] arr;
    return 0;

}