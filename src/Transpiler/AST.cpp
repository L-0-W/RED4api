#include "AST.hpp"

RED4api::Node& RED4api::Node::InsertExpr(std::unique_ptr<Node> node)
{
    this->children.push_back(std::move(node));

    auto node_ref_idx = this->children.size()-1;
    auto& node_ref = *this->children.at(node_ref_idx);

    return node_ref;
}