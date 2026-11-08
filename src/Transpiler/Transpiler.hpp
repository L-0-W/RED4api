#pragma once
#include "AST.hpp"
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


namespace RED4api {

  class REDPiler final {
    private:
      std::unique_ptr<Node> root;

      static void LoopNodes(const std::unique_ptr<Node>& node);
    public:
      REDPiler() : root(std::make_unique<Node>(Node{.value = Root{}})) {}
      void CreateFileMod(std::uint64_t mod_id, const std::string &mod_name,
                                const std::string &mod_authors_name,
                                const std::string &mod_version);
      void RegisterEvent();
  };
} // namespace RED4api
