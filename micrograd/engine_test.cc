// Testing micrograd/engine.h implementation
//
#include <assert.h> 
#include <iostream>

// #include "engine.cc"
#include "graph_system.cc"

void test_value_constructor()
{
    // Test Value constructor
    Value v(1.0);
    assert(v.get() == 1.0);
}

void test_value_set()
{
    // Test Value set method
    Value v(1.0);
    v.set(2.0);
    assert(v.get() == 2.0);
}

void test_value_to_string()
{
    // Test Value to_string method
    Value v(1.0);
    assert(v.to_string() == "1.00");
}

void test_value_operations()
{
    // Test Value operations
    Value v1(5.0);
    Value v2(2.0);
    Value v3 = v1 + v2; assert(v3.get() == 7.0);
    Value v4 = v1 - v2; assert(v4.get() == 3.0);
    Value v5 = v1 * v2; assert(v5.get() == 10.0);
    Value v6 = v1 / v2; assert(v6.get() == 2.5);
    Value v7 = -v1; assert(v7.get() == -5.0);
    Value v8 = v1; v8 += v2; assert(v8.get() == 7.0);
    Value v9 = v1; v9 -= v2; assert(v9.get() == 3.0);
    Value v10 = v1; v10 *= v2; assert(v10.get() == 10.0);
    Value v11 = v1; v11 /= v2; assert(v11.get() == 2.5);
}

void test_value_set_label()
{
    // Test Value set_label method
    Value v(1.0);
    v.set_label("v");
    assert(v.get_label() == "v");
}

void test_graph_system()
{
    // Test graph system
    Value v1(5.0, "v1");
    Value v2(2.0, "v2");
    Value v3 = v1 + v2;
    v3.set_label("v3");
    Value v4 = v1 - v2;
    v4.set_label("v4");
    Value v5 = v3 * v4;
    v5.set_label("v5");

    Graph gs;
    gs.draw(v5, "graph");
}

int main()
{
    test_value_constructor();
    test_value_set();
    test_value_to_string();
    test_value_operations();
    test_value_set_label();
    test_graph_system();
}