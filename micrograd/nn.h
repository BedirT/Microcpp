// Using the Value implementation from engine here we implement the neural network structure
// piece by piece. We start with Neuron, which is a single node in the network. It has a
// number of inputs and a number of outputs. It also has a weight and a bias. The weight
// and bias are the parameters that we want to learn. The forward function computes the
// output of the neuron given the inputs. The backward function computes the gradient of
// the neuron with respect to the inputs, weights, and biases. The backward function is
// called by the backward function of the Layer class.

#include "graph_system.cc"

#include <iostream>
#include <vector>
#include <unordered_map>


class Neuron {
public:
    Neuron(int n_inputs);
    ~Neuron();

    int n_inputs;

    Value forward(const std::vector<Value>& inputs);

    // get_parameters() returns a vector of pointers to the parameters of the neuron
    std::vector<Value*> get_parameters();

private:
    std::vector<Value> weights;
    Value bias = Value(0.0, "b");
};


// Layer; a collection of neurons 
class Layer {
public:
    Layer(int n_inputs, int n_neurons);
    ~Layer();

    int n_inputs;
    int n_neurons;

    std::vector<Value> forward(const std::vector<Value>& inputs);

    std::vector<Neuron> get_neurons() const { return neurons; }
    std::vector<Value*> get_parameters();
    
private:
    std::vector<Neuron> neurons;
};


// MLP; Multi-Layer Perceptron; a collection of layers
class MLP {
public:
    MLP(int n_inputs, std::vector<int> n_neurons_per_layer);
    ~MLP();

    int n_inputs;
    std::vector<int> n_neurons_per_layer;

    std::vector<Value> forward(const std::vector<Value>& inputs);

    std::vector<Layer> get_layers() const { return layers; }
    std::vector<Value*> get_parameters();

    void zero_grad();

private:
    std::vector<Layer> layers;
};


// Loss function, returns a Value pointer to the loss
Value* loss(const std::vector<Value>& outputs, const std::vector<Value>& targets);


// Optimizer
class Optimizer {
public:
    Optimizer(std::vector<Value*> parameters, double learning_rate);
    ~Optimizer();

    void step();

private:
    std::vector<Value*> parameters;
    double learning_rate;
};