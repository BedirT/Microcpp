#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>

class Value
{
private:
    float data;
    std::vector<Value> children;
    std::string op;
    std::string label;
    float grad;

public:
    Value(float data, 
          std::vector<Value> children = std::vector<Value>(),
          std::string op = "",
          std::string label = "");
    Value(float data, std::string label);
    ~Value();

    void set(float data) { this->data = data; }
    void set_label(std::string label) { this->label = label; }
    void set_grad(float grad) { this->grad = grad; }

    float get() const { return data; }
    float get_grad() const { return grad; }
    std::string get_label() const { return label; }
    std::vector<Value> get_children() const { return children; }
    std::string get_op() const { return op; }

    std::string to_string() const {
        std::stringstream ss;
        ss.precision(2);
        ss << std::fixed << data;
        return ss.str();
    }
    
    // define operator overloads
    Value operator+(const Value& other) const;
    Value operator-(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator/(const Value& other) const;
    Value operator-() const;
    Value& operator+=(const Value& other);
    Value& operator-=(const Value& other);
    Value& operator*=(const Value& other);
    Value& operator/=(const Value& other);

    // define comparison operators
    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;
    bool operator<(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator>(const Value& other) const;
    bool operator>=(const Value& other) const;

    // define math functions
    Value pow(float exponent) const;
    Value exp(float base = M_E) const;
    Value tanh() const;

    // gradient
    void backward();
    void backward_single();
    void build_topo(std::vector<Value*>& sorted, std::unordered_map<Value*, bool>& visited, Value* node);
};