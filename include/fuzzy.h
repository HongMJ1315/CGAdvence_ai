// Fuzzy.h
#ifndef FUZZY_H
#define FUZZY_H

#include <iostream>

class Fuzzy {
private:
    double value;  // The degree of membership, should be between 0 and 1

public:
    // Constructor to initialize the fuzzy value
    Fuzzy(double v);

    // Overload the AND operator (min operation)
    Fuzzy operator&(const Fuzzy& other) const;

    // Overload the OR operator (max operation)
    Fuzzy operator|(const Fuzzy& other) const;

    // Overload the NOT operator (1 - value)
    Fuzzy operator!() const;

    // Getter for the value
    double getValue() const;

    // Overload the << operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Fuzzy& f);
};

#endif // FUZZY_H
