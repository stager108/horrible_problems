#include <iostream>
#include <vector>
#include <string>

char complementary(char letter) {
    switch (letter) {
        case '(' :
            return ')';
        case '[' :
            return ']';
        case '{' :
            return '}';
        default:
            return ' ';
    }
}

int check_sequence(std::string input_string) {

    std::vector<char> sequence;
    sequence.clear();
    int size = static_cast<int>(input_string.size());
    for (int index = 0; index < size; ++index) {
        switch (input_string[index]) {
            case '(':
            case '[':
            case '{':
                sequence.push_back(complementary(input_string[index]));
                break;

            default:
                if (sequence.empty()) { return index; }
                char w = sequence[static_cast<int>(sequence.size()) - 1];
                if (w != input_string[index]) {
                    return index;
                }
                sequence.pop_back();
        }
    }
    return (sequence.empty()) ? -1 : static_cast<int>(sequence.size());
}


int main() {

    std::string sequence;
    std::cin >> sequence;
    int ans = check_sequence(sequence);
    if (ans == -1) {
        std::cout << "CORRECT";
    } else {
        std::cout << ans;
    }
    return 0;
}
