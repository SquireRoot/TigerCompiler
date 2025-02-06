/**
 * @file TigerTreeVisitor.h
 * @brief contains the declaration for the TigerTreeVisitor class
 * @author Evan Newman, evanmnewman1@gmail.com
 * @date February 2025
 */

#ifndef TIGERC_TIGERTREEVISITOR_H_
#define TIGERC_TIGERTREEVISITOR_H_

#include <sstream>
#include <vector>

#include <antlr4-runtime.h>
#include "TigerBaseVisitor.h"

#include "Common.h"

namespace tigerc {

namespace hidden {
inline size_t parse_tree_to_graph_viz_recursive(antlr4::tree::ParseTree* current_node,
                                          antlr4::Parser& parser,
                                          std::stringstream& graphviz_out,
                                          size_t& available_id) {
    // assign this node a unique id
    size_t node_id = available_id++;

    // write the node definition to the graphviz output
    graphviz_out << "\t" << node_id << " [label=\"" << antlr4::tree::Trees::getNodeText(current_node, &parser) << "\"];" << std::endl;

    // vector of node ids that the children make for themselves
    std::vector<size_t> child_ids;

    // iterate and recurse through all the child nodes
    for (size_t i = 0; i < current_node->children.size(); i++) {
        size_t child_id = parse_tree_to_graph_viz_recursive(current_node->children[i], parser, graphviz_out, available_id);
        child_ids.push_back(child_id);
    }

    // once this node has its id and all the children have assigned themselves ids, make the edge
    graphviz_out << "\t" << node_id << " -> {";

    for (size_t i = 0; i < child_ids.size(); i++) {
        if (i != 0) {
            graphviz_out << ", ";
        }
        graphviz_out << child_ids.at(i);
    }

    graphviz_out << "};" << std::endl;

    return node_id;
}
}

inline std::string parse_tree_to_graph_viz(antlr4::tree::ParseTree* tree, antlr4::Parser& parser) {
    if (tree == nullptr) {
        return "";
    }
    
    size_t available_id = 0;
    std::stringstream graphviz_out;

    graphviz_out << "digraph {" << std::endl;

    hidden::parse_tree_to_graph_viz_recursive(tree, parser, graphviz_out, available_id);

    graphviz_out << "}";
    return graphviz_out.str();
}

inline std::string token_stream_to_string(antlr4::CommonTokenStream& tokens, antlr4::Lexer& lexer) {
    std::stringstream token_stream;

    for (auto token : tokens.getTokens()) {
        size_t token_type_idx = token->getType() - 1;
        if (token_type_idx < lexer.getRuleNames().size()) {
            token_stream << "<" << lexer.getRuleNames().at(token_type_idx) 
                         << ", \"" << token->getText() << "\">" << std::endl;
        }
    }

    return token_stream.str();
}

}

#endif