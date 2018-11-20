#include "FizzBuzz.hpp"

std::string FizzBuzz(int const number) {
    if(number != 0) {
        auto m3 = number % 3;
        auto m5 = number % 5;
        if(!m3 && !m5) {
            return "fizzbuzz";
        } else if(!m3) {
            return "fizz";
        } else if(!m5) {
            return "buzz";
        }
    }
    return std::to_string(number);
}