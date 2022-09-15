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
#include <unordered_map>

#include "engine.cc"


class Graph {
private:
    std::string graph;
    int node_count;
    std::unordered_map<std::string, bool> visited_nodes; // nodes
    std::unordered_map<std::string, bool> visited_edges; // edges

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

    void add_node(std::string id, std::string label, std::string args="color=black", bool is_op=false) {
        if (is_op) {
            graph += "\t" + id + " [label=\"" + label + "\",shape=circle," + args + "];\n";
        } else {
            graph += "\t" + id + " [label=\"" + label + "\",shape=record," + args + "];\n";
        }
    }

    void add_edge(std::string from, std::string to) {
        graph += "\t" + from + " -> " + to + ";\n";
    }

    std::string get_id(const Value& v) {
        std::stringstream ss;
        ss << "node";
        std::string label = v.get_label();
        if (label == "") {
            ss << node_count;
            node_count++;
        } else {
            ss << label;
        }
        return ss.str();
    }

    std::string create_from_root(const Value& v) {
        std::string label = get_label_rpr(v);
        std::string node_id = get_id(v);
        add_node(node_id, label, "color=white,style=\"filled,rounded\",fillcolor=\"indianred4:lightcoral\",fontcolor=white,border=3");
        std::string connection_id = node_id;
        // if op is not empty add op node
        if (v.get_op() != "") {
            std::string op_label = v.get_op();
            std::string op_id = "op" + node_id;
            add_node(op_id, op_label, "color=white,style=filled,fillcolor=\"webmaroon\",fontcolor=oldlace", true);
            if (visited_edges.find(op_id + node_id) == visited_edges.end()) {
                add_edge(op_id, node_id);
                visited_edges[op_id + node_id] = true;
            }
            connection_id = op_id;
        }
        // add children
        for (const Value& child : v.get_children()) {
            std::string child_id = create_from_root(child);
            if (visited_edges.find(child_id + connection_id) == visited_edges.end()) {
                add_edge(child_id, connection_id);
                visited_edges[child_id + connection_id] = true;
            }
        }
        return node_id;
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