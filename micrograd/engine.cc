// C++ micrograd engine implementation based on Andrej Karpathy's
// python implementation.

#include "engine.h"

// Constructor & Destructor
Value::Value(float data, std::vector<Value> children, std::string op)
{
    this->data = data;
    this->children = children;
    this->op = op;
}
Value::~Value() {
    // std::cout << "Value destructor called" << std::endl;
}

// Operator overloads
Value Value::operator+(const Value& other) const
{
    // Sum of two values
    return Value(this->data + other.data, { *this, other }, "+");
}
Value Value::operator-(const Value& other) const
{
    // Difference of two values
    return Value(this->data - other.data, { *this, other }, "-");
}
Value Value::operator*(const Value& other) const
{
    // Product of two values
    return Value(this->data * other.data, { *this, other }, "*");
}
Value Value::operator/(const Value& other) const
{
    // Quotient of two values
    return Value(this->data / other.data, { *this, other }, "/");
}
Value Value::operator-() const
{
    // Negation of a value
    return Value(-this->data, { *this });
}
Value& Value::operator+=(const Value& other)
{
    // Increment a value by another value
    data += other.data;
    children = { *this, other };
    return *this;
}
Value& Value::operator-=(const Value& other)
{
    // Decrement a value by another value
    data -= other.data;
    children = { *this, other };
    return *this;
}
Value& Value::operator*=(const Value& other)
{
    // Multiply a value by another value
    data *= other.data;
    children = { *this, other };
    return *this;
}
Value& Value::operator/=(const Value& other)
{
    // Divide a value by another value
    data /= other.data;
    children = { *this, other };
    return *this;
}



