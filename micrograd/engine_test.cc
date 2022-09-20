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
    assert(v.get_data() == 1.0);
}

void test_value_set()
{
    // Test Value set method
    Value v(1.0);
    v.set_data(2.0);
    assert(v.get_data() == 2.0);
}

void test_value_to_string()
{
    // Test Value to_string method
    Value v(1.0);
    assert(v.get_data() == 1.0);
}

void test_value_operations()
{
    // Test Value operations
    Value v1(5.0);
    Value v2(2.0);
    Value v3 = v1 + v2; assert(v3.get_data() == 7.0);
    Value v4 = v1 - v2; assert(v4.get_data() == 3.0);
    Value v5 = v1 * v2; assert(v5.get_data() == 10.0);
    Value v7 = -v1; assert(v7.get_data() == -5.0);
    Value v8 = v1; v8 += v2; assert(v8.get_data() == 7.0);
    Value v9 = v1; v9 -= v2; assert(v9.get_data() == 3.0);
    Value v10 = v1; v10 *= v2; assert(v10.get_data() == 10.0);
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
    // Todo: assert that graph was drawn
}

void test_tanh_function()
{
    // Test tanh function
    Value v1(5.0, "v1");
    Value v2 = v1.tanh();
    v2.set_label("v2");

    Graph gs;
    gs.draw(v2, "graph_tanh");
    // Todo: assert that graph was drawn
}

void test_gradient_calculation_single_neuron()
{
    // Test gradient calculation
    // variables
    Value x1 = Value(2.0, "x1");
    Value x2 = Value(0.0, "x2");
    // weights
    Value w1 = Value(-3.0, "w1");
    Value w2 = Value(1.0, "w2");
    // bias
    Value b = Value(6.8813735870195432, "b");
    // neuron (x1*w1 + x2*w2 + b)
    Value x1w1 = x1 * w1; x1w1.set_label("x1w1");
    Value x2w2 = x2 * w2; x2w2.set_label("x2w2");
    Value x1w1_x2w2 = x1w1 + x2w2; x1w1_x2w2.set_label("x1w1_x2w2");
    Value n = x1w1_x2w2 + b; n.set_label("n");
    // output w tanh
    Value o = n.tanh(); o.set_label("o");

    o.backward();
    Graph gs;
    gs.draw(o, "graph_single_neuron");
}

void test_integer_ops()
{
    Value v1(5.0, "v1");
    int i = 2;
    Value v2 = v1 + i;
    assert(v2.get_data() == 7.0);
}

void test_float_ops()
{
    Value v1(5.0, "v1");
    float f = 2.0;
    Value v2 = v1 + f;
    assert(v2.get_data() == 7.0);
}

void test_imitation_of_training()
{
    // Test imitation of training
    float x1 = 2.0;
    float x2 = 0.0;

    // weights
    float w1 = -3.0;
    float w2 = 1.0;
    float b = 6.8813735870195432;

    // y target
    float y = 0.0;

    // backpropagation by converting all to values
    Value v1(x1, "x1");
    Value v2(x2, "x2");
    Value v3(w1, "w1");
    Value v4(w2, "w2");
    Value v5(b, "b");

    // neuron (x1*w1 + x2*w2 + b)
    Value v6 = v1 * v3; v6.set_label("x1w1");
    Value v7 = v2 * v4; v7.set_label("x2w2");
    Value v8 = v6 + v7; v8.set_label("x1w1_x2w2");
    Value v9 = v8 + v5; v9.set_label("n");

    // output w tanh
    Value v10 = v9.tanh(); v10.set_label("o");

    // calculate gradient
    v10.backward();

    // update weight of w1
    v3.set_data(v3.get_data() - 0.1 * v3.get_grad());

    // check if the prediction is closer to the target
    // imitate forward pass
    Value v11 = v1 * v3; v11.set_label("x1w1");
    Value v12 = v2 * v4; v12.set_label("x2w2");
    Value v13 = v11 + v12; v13.set_label("x1w1_x2w2");
    Value v14 = v13 + v5; v14.set_label("n");
    Value v15 = v14.tanh(); v15.set_label("o");
    
    assert((v15.get_data() - y) < (v10.get_data() - y));
}

int main()
{
    // test_value_constructor();
    // test_value_set();
    // test_value_to_string();
    // test_value_operations();
    // test_value_set_label();
    // test_graph_system();
    // test_tanh_function();
    // test_gradient_calculation_single_neuron();
    // test_integer_ops();
    // test_float_ops();
    test_imitation_of_training();
}