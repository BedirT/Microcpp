// C++ micrograd engine implementation based on Andrej Karpathy's
// python implementation.

#include "engine.h"

// Constructor & Destructor
Value::Value(float data, std::vector<Value> children, std::string op, std::string label)
    : data(data), children(children), op(op), label(label) { grad = 0.0; }
Value::Value(float data, std::string label) : data(data), label(label) {
    children = std::vector<Value>();
    op = "";
    grad = 0.0;
}

Value::~Value() {
    // std::cout << "Value destructor called" << std::endl;
}

// Operator overloads
Value Value::operator+(const Value& other) const
{
    // Sum of two values
    return Value(data + other.data, { *this, other }, "+");
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

// Overloaded operators for float
Value Value::operator+(float other) const 
{ 
    Value other_value = Value(other);
    return *this + other_value;
}
Value Value::operator-(float other) const 
{ 
    Value other_value = Value(other);
    return *this - other_value;
}
Value Value::operator*(float other) const 
{ 
    Value other_value = Value(other);
    return *this * other_value;
}
Value Value::operator/(float other) const 
{ 
    Value other_value = Value(other);
    return *this / other_value;
}
Value& Value::operator+=(float other)
{
    data += other;
    children = { *this };
    return *this;
}
Value& Value::operator-=(float other)
{
    data -= other;
    children = { *this };
    return *this;
}
Value& Value::operator*=(float other)
{
    data *= other;
    children = { *this };
    return *this;
}
Value& Value::operator/=(float other)
{
    data /= other;
    children = { *this };
    return *this;
}

// Overloaded operators for float and int if the float or int is on the left
// Value operator+(float other, const Value& value) { return Value(other + value.get_data(), { value }); }
// Value operator-(float other, const Value& value) { return Value(other - value.get_data(), { value }); }
// Value operator*(float other, const Value& value) { return Value(other * value.get_data(), { value }); }
// Value operator/(float other, const Value& value) { return Value(other / value.get_data(), { value }); }

// Comparison operators
bool Value::operator==(const Value& other) const
{
    // Check if two values are equal
    return this->data == other.data;
}
bool Value::operator!=(const Value& other) const
{
    // Check if two values are not equal
    return this->data != other.data;
}
bool Value::operator<(const Value& other) const
{
    // Check if a value is less than another value
    return this->data < other.data;
}
bool Value::operator<=(const Value& other) const
{
    // Check if a value is less than or equal to another value
    return this->data <= other.data;
}
bool Value::operator>(const Value& other) const
{
    // Check if a value is greater than another value
    return this->data > other.data;
}
bool Value::operator>=(const Value& other) const
{
    // Check if a value is greater than or equal to another value
    return this->data >= other.data;
}

// Math functions
Value Value::pow(float exponent) const
{
    // Power function
    return Value(std::pow(this->data, exponent), { *this }, "pow");
}

Value Value::exp() const
{
    // Exponential function (e^x)
    return Value(std::exp(this->data), { *this }, "exp");
}

Value Value::tanh() const
{
    // Hyperbolic tangent function
    return Value(std::tanh(this->data), { *this }, "tanh");
}

// Gradient
void Value::build_topo(std::vector<Value*>& sorted, std::unordered_map<Value*, bool>& visited, Value* node)
{
    // Build the topological sort of the graph
    if (visited[node]) {
        return;
    }
    visited[node] = true;
    for (auto& child : node->children) {
        build_topo(sorted, visited, &child);
    }
    sorted.push_back(node);
}

void Value::backward_single()
{
    // Backpropagate the gradient, updateding children's gradients
    if (op == "") {} // Leaf node
    else if (op == "+") {
        children[0].update_grad(grad);
        children[1].update_grad(grad); 
    } else if (op == "-") {
        children[0].update_grad(grad);
        children[1].update_grad(-grad);
    } else if (op == "*") {
        children[0].update_grad(grad * children[1].get_data());
        children[1].update_grad(grad * children[0].get_data());
    } else if (op == "/") {
        children[0].update_grad(grad / children[1].get_data());
        children[1].update_grad(-grad * children[0].get_data() / (children[1].get_data() * children[1].get_data()));
        // Check this one
    } else if (op == "pow") {
        children[0].update_grad(grad * children[1].get_data() * std::pow(children[0].get_data(), children[1].get_data() - 1));
    } else if (op == "exp") {
        children[0].update_grad(grad * std::exp(children[0].get_data()));
        
    } else if (op == "tanh") {
        children[0].update_grad(grad * (1 - std::pow(std::tanh(children[0].get_data()), 2)));
    } else {
        throw std::runtime_error("Unknown op " + op);
    }
}

void Value::backward()
{
    // Topsort the graph and backpropagate the gradient
    // Note: Using pointers to avoid copying the graph
    std::vector<Value*> sorted;
    std::unordered_map<Value*, bool> visited;

    // Build the topological sort
    build_topo(sorted, visited, this);
    
    // Backpropagate the gradient
    sorted.back()->set_grad(1.0);
    for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
        (*it)->backward_single();
    }
}


