// Testing the neural network implementation

#include "nn.cc"

#include <iostream>
#include <vector>
#include <cassert>

void test_single_neuron()
{
    // Create a neuron with 2 inputs
    Neuron neuron(2);

    // Create some inputs
    Value x1(1.0, "x1");
    Value x2(2.0, "x2");
    std::vector<Value> inputs = { x1, x2 };

    Value target = Value(0.5, "target"); target.set_label("target");
    
    // Compute the output of the neuron
    Value y = neuron.forward(inputs); y.set_label("y");

    // Print the output
    std::cout << "y = " << y.to_string() << std::endl;
    Value prev_y(y.get_data(), "prev_y"); prev_y.set_label("prev_y");

    // Imitate single training step
    Value loss0 = y - target; loss0.set_label("loss0");
    // Value loss = loss0 * loss0; loss.set_label("loss");
    loss0.backward();

    // update weights
    // std::vector<Value*> parameters = neuron.get_parameters();
    // for (int i = 0; i < parameters.size(); i++) {
    //     parameters[i]->set_data(parameters[i]->get_data() - 0.1 * parameters[i]->get_grad());
    // }

    // Print the output
    std::cout << "y = " << y.to_string() << std::endl;

    // Make sure the output is closer to the target
    // assert((y - target).get_data() < (prev_y - target).get_data());
}

// void test_layer()
// {
//     // Create a layer with 2 inputs and 3 neurons
//     Layer layer(2, 3);

//     // Create some inputs
//     Value x1(1.0, "x1");
//     Value x2(2.0, "x2");
//     std::vector<Value> inputs = { x1, x2 };

//     // Compute the output of the layer
//     std::vector<Value> outputs = layer.forward(inputs);

//     // Print the outputs
//     std::cout << "y1 = " << outputs[0].to_string() << std::endl;
//     std::cout << "y2 = " << outputs[1].to_string() << std::endl;
//     std::cout << "y3 = " << outputs[2].to_string() << std::endl;

//     // // Compute the gradient of the output with respect to the inputs
//     // outputs[0].backward();

//     // // Print the gradients
//     // std::cout << "x1.grad = " << x1.get_grad() << std::endl;
//     // std::cout << "x2.grad = " << x2.get_grad() << std::endl;
// }

// void test_MLP()
// {
//     // Create a MLP with 3 inputs and 2 layers with 4 and 4 neurons respectively
//     // and 1 output
//     MLP mlp(3, { 4, 4, 1 });

//     // Create some inputs
//     Value x1(1.0, "x1");
//     Value x2(2.0, "x2");
//     Value x3(3.0, "x3");
//     std::vector<Value> inputs = { x1, x2, x3 };

//     // Compute the output of the MLP
//     std::vector<Value> outputs = mlp.forward(inputs);

//     // Print the output
//     std::cout << "y = " << outputs[0].to_string() << std::endl;
    
//     // Build the graph
//     Graph gs;
//     gs.draw(outputs[0], "mlp");
// }

// void test_neural_network(){
//     // Create a dataset, an MLP. Train the MLP on the dataset for given target values
//     // and print the loss and the gradients of the parameters
//     // Create a dataset
//     std::vector<std::vector<float>> X = {
//         { 2.0, 3.0, -1.0 },
//         { 3.0, -1.0, 0.5 },
//         { 0.5, 1.0, 1.0 },
//         { 1.0, 1.0, -1.0}
//     }

//     // Create some target values
//     std::vector<double> target_values = { 0.0, 1.0, 1.0, 0.0 };

//     // Create a MLP with 3 inputs, 2 layers with 4 and 4 neurons respectively
//     // and 1 output
//     MLP model(3, { 4, 4, 1 });

//     // Train the MLP on the dataset for given target values
//     for (int i = 0; i < 20; i++) {
//         // Compute the output of the MLP
//         std::vector<Value> outputs = model.forward(X[i % X.size()]);

//         // Compute the loss
//         Value loss = outputs[0] - target_values[i % X.size()];

//         // Compute the gradient of the loss with respect to the parameters
//         loss.backward();

//         // Print the loss
//         std::cout << "loss = " << loss.to_string() << std::endl;

//         // Print the gradients of the parameters
//         for (auto& p : model.get_parameters()) {
//             std::cout << p->to_string() << std::endl;
//         }

//         // Update the parameters
//         for (auto& p : model.get_parameters()) {
//             p->data -= 0.01 * p->grad;
//         }
//     }
// }

int main()
{
    test_single_neuron();
    // test_layer();
    // test_MLP();
    // test_neural_network();
}