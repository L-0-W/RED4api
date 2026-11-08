#pragma  once

#include <string>
#include <variant>
#include <vector>
#include <memory>

namespace RED4api {
    
    struct Root {};
    struct Module 
    {
        std::string name;
    };
    struct Metadada 
    {
        std::string name;
        std::string author;
        std::string version;
    };
    
    using Expr = std::variant<Module, Metadada, Root>;
    
    struct Node {
        Expr value;
        std::vector<std::unique_ptr<Node>> children;
        
        RED4api::Node& InsertExpr(std::unique_ptr<Node> node);
    };
    
}