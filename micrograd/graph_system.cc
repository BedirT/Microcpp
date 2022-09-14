// System setup for graphviz. Builds a graph of the computation graph
// for the Value class.

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "engine.cc"


class GraphSystem {
private:
    std::string graph;

public:
    GraphSystem() : graph("digraph G {\n") {}

    std::string get_label_rpr(const Value& v) {
        std::string label = v.get_label();
        if (label == "") {
            label = v.to_string();
        } else {
            label = label + " | " + v.to_string();
        }
        return label;
    }

    void add_node(const Value& v) {
        std::string label = get_label_rpr(v);
        graph += "    " + label + " [label=\"" + label + "\"];\n";
    }

    void add_edge(const Value& v1, const Value& v2) {
        std::string label1 = get_label_rpr(v1);
        std::string label2 = get_label_rpr(v2);
        graph += "    " + label1 + " -> " + label2 + ";\n";
    }

    void create_from_root(const Value& v) {
        add_node(v);
        for (Value child : v.get_children()) {
            add_edge(v, child);
            create_from_root(child);
        }
    }

    void write_to_file(const std::string& filename) {
        graph += "}";
        std::ofstream file;
        file.open(filename);
        file << graph;
        file.close();
    }


    ~GraphSystem() {
        std::cout << graph << std::endl;
    }
};