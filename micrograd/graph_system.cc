// System setup for graphviz. Builds a graph of the computation graph
// for the Value class.

// Graphs are setup left to right to make it similar to neural network
// representations. For any value in the graph, we have a rectangular
// box with the value in the middle, label on the left, and the gradient
// on the right. The gradient is only shown if it is not zero. All value
// boxes except he output value are connected to operation nodes. The
// operation nodes are circles with the operation name in the middle.


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "engine.cc"


class Graph {
private:
    std::string graph;
    int node_count;

public:
    Graph() : graph("digraph G {\n rankdir=LR;\n"), node_count(0) {}

    std::string get_label_rpr(const Value& v) {
        std::string label = v.get_label();
        if (label == "") {
            label = v.to_string();
        } else {
            std::stringstream grad_ss;
            grad_ss.precision(4);
            grad_ss << std::fixed << v.get_grad();
            label = "{ " + label + " | " + v.to_string() + " | " + grad_ss.str() + " }";
        }
        return label;
    }

    std::string add_node(std::string label, std::string args="color=black", bool is_op=false) {
        std::stringstream ss;
        ss << "node" << node_count;
        std::string node_name = ss.str();
        if (is_op) {
            graph += "\t" + node_name + " [label=\"" + label + "\", shape=circle, " + args + "];\n";
        } else {
            graph += "\t" + node_name + " [label=\"" + label + "\", shape=record, " + args + "];\n";
        }
        node_count++;
        return node_name;
    }

    void add_edge(std::string from, std::string to) {
        graph += "\t" + from + " -> " + to + ";\n";
    }

    std::string create_from_root(const Value& v) {
        std::string label = get_label_rpr(v);
        std::string node_name = add_node(label, "color=red");
        std::string connection_node = node_name;
        // if op is not empty add op node
        if (v.get_op() != "") {
            std::string op_label = v.get_op();
            std::string op_name = add_node(op_label, "color=black", true);
            add_edge(op_name, node_name);
            connection_node = op_name;
        }
        // add children
        for (const Value& child : v.get_children()) {
            std::string child_name = create_from_root(child);
            add_edge(child_name, connection_node);
        }
        return node_name;
    }

    void write_to_file(const std::string& filename) {
        graph += "}";
        std::ofstream file;
        file.open(filename + ".dot");
        file << graph;
        file.close();
    }

    void draw(const Value& v, const std::string& filename) {
        node_count = 0;
        create_from_root(v);
        write_to_file(filename);
        
        std::string cmd = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
        system(cmd.c_str());
    }

    ~Graph() {
        // std::cout << graph << std::endl;
    }
};