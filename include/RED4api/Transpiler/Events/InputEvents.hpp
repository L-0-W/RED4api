#pragma once

#include <concepts>

#include <RED4api/Mod.hpp>
#include <RED4api/Types/Enums.hpp>

namespace RED4api 
{
  struct InputEvents {
    template <typename Func>
      requires requires(Func &callback) {
        { callback() } -> std::same_as<bool>;
      }
    static bool Keyboard(const Mod & mod, InputAction ic, InputKey ip, Func callback) {
      // mod.RegisterEvcent()
      return true;
    }

    bool Mouse();
    bool Microfone();
  };
} // namespace RED4api
