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
#include <unordered_set>

#include "engine.cc"

class Graph {
private:
    std::string graph;
    int node_count;
    // string to value nodes str, value
    // nodes -> node.get_key(); <node.to_string(), node.get_label(), node.get_op(), node.get_grad(), node.get_data()>
    std::unordered_map<std::string, std::tuple<std::string, std::string, std::string, float, float>> nodes;
    std::unordered_map<std::string, std::pair<std::string, std::string>> edges;

public:
    Graph() : graph("digraph G {\n rankdir=LR;\n"), node_count(0) {}

    std::string get_label_rpr(std::string node_key) {
        std::string label = std::get<1>(nodes[node_key]);
        if (label == "") {
            label = std::get<0>(nodes[node_key]);
            return label;
        } else {

            float data = std::get<4>(nodes[node_key]);
            float grad = std::get<3>(nodes[node_key]);
            // arrange precision
            std::stringstream ss;
            ss << "{ " << label << " | ";
            ss.precision(4);
            ss << std::fixed << data;
            ss << " | ";
            ss.precision(4);
            ss << std::fixed << grad;
            ss << " }";
            return ss.str();
        }
    }

    std::string get_op_str(std::string op) {
        // convert op to string;
        // + -> add / - -> sub / * -> mul / / -> div
        if (op == "+") {
            return "add";
        } else if (op == "-") {
            return "sub";
        } else if (op == "*") {
            return "mul";
        } else if (op == "/") {
            return "div";
        } else {
            return op;
        }
    }

    void add_node(std::string id, std::string label, std::string args="color=black", bool is_op=false) {
        std::stringstream ss;
        id = "node" + id;
        if (is_op) {
            ss << "\t" << id << " [label=\"" << label << "\",shape=circle," << args << "];\n";
        } 
        else {
            ss << "\t" << id << " [label=\"" << label << "\",shape=record," << args << "];\n";
        }
        graph += ss.str();
    }

    void add_edge(std::string from, std::string to) {
        std::stringstream ss;
        from = "node" + from;
        to = "node" + to;
        ss << "\t" << from << " -> " << to << ";\n";
        graph += ss.str();
    }

    void trace_nodes_and_edges(Value v) {
        if (nodes.find(v.get_key()) != nodes.end()) {
            return;
        }
        std::string node_key = v.get_key();
        nodes[node_key] = std::make_tuple(v.to_string(), v.get_label(), v.get_op(), v.get_grad(), v.get_data());
        for (Value& child : v.get_children()) {
            std::string child_key = child.get_key();
            if (edges.find(child_key + node_key) == edges.end()) {
                edges[child_key + node_key] = std::make_pair(child_key, node_key);
                trace_nodes_and_edges(child);
            }
        }
    }

    void trace_graph(Value v) {
        trace_nodes_and_edges(v);
        for (auto& node : nodes) {
            std::string node_key = node.first;
            std::string label = get_label_rpr(node_key);
            std::string op = std::get<2>(node.second);
            std::string node_op_str = get_op_str(op);
            add_node(node_key, label,
            "color=white,style=\"filled,rounded\",fillcolor=\"indianred4:lightcoral\",fontcolor=white,border=3");
            if (node_op_str != ""){
                add_node(node_key + node_op_str, op, "color=white,style=filled,fillcolor=\"webmaroon\",fontcolor=oldlace", true);
                add_edge(node_key + node_op_str, node_key);
            }
        }
        for (auto& edge : edges) {
            std::string to_op = get_op_str(std::get<2>(nodes[edge.second.second]));
            add_edge(edge.second.first, edge.second.second + to_op);
        }

        graph += "}";
    }

    void write_to_file(const std::string& filename) {
        std::ofstream file;
        file.open(filename + ".dot");
        file << graph;
        file.close();
    }

    void draw(const Value v, const std::string& filename) {
        trace_graph(v);
        write_to_file(filename);
        
        std::string cmd = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
        system(cmd.c_str());
    }

    ~Graph() {
        // std::cout << graph << std::endl;
    }
};