// Fuzzy.cpp
#include "Fuzzy.h"
#include <algorithm>  // For std::min and std::max
#include <stdexcept>  // For std::out_of_range

// Constructor to initialize the fuzzy value
Fuzzy::Fuzzy(double v) {
    if (v < 0.0 || v > 1.0) {
        throw std::out_of_range("Fuzzy value must be between 0 and 1");
    }
    value = v;
}

// Overload the AND operator (min operation)
Fuzzy Fuzzy::operator&(const Fuzzy& other) const {
    return Fuzzy(std::min(this->value, other.value));
}

// Overload the OR operator (max operation)
Fuzzy Fuzzy::operator|(const Fuzzy& other) const {
    return Fuzzy(std::max(this->value, other.value));
}

// Overload the NOT operator (1 - value)
Fuzzy Fuzzy::operator!() const {
    return Fuzzy(1.0 - this->value);
}

// Getter for the value
double Fuzzy::getValue() const {
    return value;
}

// Overload the << operator for easy printing
std::ostream& operator<<(std::ostream& os, const Fuzzy& f) {
    os << f.value;
    return os;
}
