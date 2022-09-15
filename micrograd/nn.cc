// Implementations from nn.h
//

#include "nn.h"

#include <iostream>
#include <vector>
#include <random>

// Constructor & Destructor
Neuron::Neuron(int n_inputs)
{
    // std::cout << "Neuron constructor called" << std::endl;
    Value weight(1.0, "weight");
    // Initialize the weights and bias with random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    for (int i = 0; i < n_inputs; i++) {
        weights.push_back(Value(dis(gen), "w" + std::to_string(i)));
    }
    bias = Value(dis(gen), "b");
    this->n_inputs = n_inputs;
}

Neuron::~Neuron()
{
    // std::cout << "Neuron destructor called" << std::endl;
}

// Forward pass
Value Neuron::forward(const std::vector<Value>& inputs)
{
    // Compute the weighted sum of the inputs
    Value weighted_sum = Value(0.0);
    for (int i = 0; i < inputs.size(); i++) {
        weighted_sum += inputs[i] * weights[i];
    }

    // Add the bias
    weighted_sum += bias;

    // Return the result of the activation function
    return weighted_sum.tanh();
}

// Get the parameters of the neuron
std::vector<Value*> Neuron::get_parameters()
{
    std::vector<Value*> parameters;
    for (int i = 0; i < n_inputs; i++) {
        parameters.push_back(&weights[i]);
    }
    parameters.push_back(&bias);
    return parameters;
}

// Layer class
// Constructor & Destructor
Layer::Layer(int n_inputs, int n_neurons)
{
    // std::cout << "Layer constructor called" << std::endl;
    for (int i = 0; i < n_neurons; i++) {
        neurons.push_back(Neuron(n_inputs));
    }
    this->n_inputs = n_inputs;
    this->n_neurons = n_neurons;
}

Layer::~Layer()
{
    // std::cout << "Layer destructor called" << std::endl;
}

// Forward pass
std::vector<Value> Layer::forward(const std::vector<Value>& inputs)
{
    std::vector<Value> outputs;
    for (auto neuron : neurons) {
        outputs.push_back(neuron.forward(inputs));
    }
    return outputs;
}

// Get the parameters of the layer
std::vector<Value*> Layer::get_parameters()
{
    std::vector<Value*> parameters;
    for (auto neuron : neurons) {
        std::vector<Value*> neuron_parameters = neuron.get_parameters();
        parameters.insert(parameters.end(), neuron_parameters.begin(), neuron_parameters.end());
    }
    return parameters;
}

// MLP class
// Constructor & Destructor
MLP::MLP(int n_inputs, std::vector<int> n_neurons_per_layer)
{
    // std::cout << "MLP constructor called" << std::endl;
    for (int i = 0; i < n_neurons_per_layer.size(); i++) {
        if (i == 0) {
            layers.push_back(Layer(n_inputs, n_neurons_per_layer[i]));
        } else {
            layers.push_back(Layer(n_neurons_per_layer[i - 1], n_neurons_per_layer[i]));
        }
    }
    this->n_inputs = n_inputs;
    this->n_neurons_per_layer = n_neurons_per_layer;
}

MLP::~MLP()
{
    // std::cout << "MLP destructor called" << std::endl;
}

// Forward pass
std::vector<Value> MLP::forward(const std::vector<Value>& inputs)
{
    std::vector<Value> outputs = inputs;
    for (auto layer : layers) {
        outputs = layer.forward(outputs);
    }
    return outputs;
}

// Get the parameters of the MLP
std::vector<Value*> MLP::get_parameters()
{
    std::vector<Value*> parameters;
    for (auto layer : layers) {
        std::vector<Value*> layer_parameters = layer.get_parameters();
        parameters.insert(parameters.end(), layer_parameters.begin(), layer_parameters.end());
    }
    return parameters;
}

// Zero grad
void MLP::zero_grad()
{
    for (auto parameter : get_parameters()) {
        parameter->set_grad(0.0);
    }
}

// Loss function
Value* loss(const std::vector<Value>& outputs, const std::vector<Value>& targets)
{
    Value* loss = new Value(0.0);
    for (int i = 0; i < outputs.size(); i++) {
        *loss += (outputs[i] - targets[i]) * (outputs[i] - targets[i]);
    }
    return loss;
}

// Optimizer
Optimizer::Optimizer(std::vector<Value*> parameters, double learning_rate)
{
    // std::cout << "Optimizer constructor called" << std::endl;
    for (auto parameter : parameters) {
        parameter->set_grad(0.0);
        this->parameters.push_back(parameter);
    }
    this->learning_rate = learning_rate;
}

Optimizer::~Optimizer()
{
    // std::cout << "Optimizer destructor called" << std::endl;
}

// Update the parameters
void Optimizer::step()
{
    for (auto parameter : parameters) {
        parameter->set_data(parameter->get_data() - learning_rate * parameter->get_grad());
    }
}

// Convert a vector of floats/or vector of vector of floats to a vector of Value
// or vector of vector of Values
std::vector<Value> to_value(const std::vector<float>& inputs)
{
    std::vector<Value> outputs;
    for (auto input : inputs) {
        outputs.push_back(Value(input));
    }
    return outputs;
}

std::vector<std::vector<Value>> to_value(const std::vector<std::vector<float>>& inputs)
{
    std::vector<std::vector<Value>> outputs;
    for (auto input : inputs) {
        outputs.push_back(to_value(input));
    }
    return outputs;
}


// Main function
int main()
{
    // Create the dataset
    std::vector<std::vector<float>> inputs = {
        { 0.0, 2.0, 1.0 },
        { 3.0, 1.0, 2.0 },
        { 1.0, 2.0, 4.0 },
        { 4.0, 1.0, 4.0 }
    };
    std::vector<std::vector<float>> targets = {
        { -1.0 },
        { 1.0 },
        { 1.0 },
        { -1.0 }
    };

    // Create the model
    MLP model(4, { 4, 4, 1 });

    // Create the optimizer
    Optimizer optimizer(model.get_parameters(), 0.01);

    // Train the model
    for (int epoch = 0; epoch < 20; epoch++) {
        float epoch_loss = 0.0;
        for (int i = 0; i < inputs.size(); i++) {
            // Forward pass
            std::vector<Value> outputs = model.forward(to_value(inputs[i]));

            // Compute the loss
            Value* loss = ::loss(outputs, to_value(targets[i]));

            // Backward pass
            loss->backward();

            // Update the parameters
            optimizer.step();

            // Update the loss
            epoch_loss += loss->get_data();
        }

        // Zero the gradients
        model.zero_grad();
        std::cout << "Epoch " << epoch << " loss: " << epoch_loss << std::endl;
    }

    // Test the model
    for (int i = 0; i < inputs.size(); i++) {
        std::vector<Value> outputs = model.forward(to_value(inputs[i]));
        std::cout << "Input: " << inputs[i][0] << " " << inputs[i][1] << " " << inputs[i][2] << " Output: " << outputs[0].get_data() << std::endl;
    }

    return 0;
}