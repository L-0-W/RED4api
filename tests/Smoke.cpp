#include "RED4api/Transpiler/Events/InputEvents.hpp"
#include <RED4api/RED4api.hpp>

int main() {


  const RED4api::Mod mod{"smoke-test", "RED4api", "1.0.0"};
  auto result =
      RED4api::InputEvents::Keyboard(mod, RED4api::InputAction::Press,
                               RED4api::InputKey::KEY_E, [] { return true; })
          ? 0
          : 1;

  return result;
}

// TODO -- InputEvents::Keyboard -> mod.RegisterEvent() -> Transpiler.RegisterEvent
// O Codigo final vai ser no trapiler, que vai chamar a AST e criar a estrutura para registrar esse evento.
// Também, verificar uma forma de ter "enum Event" que deve conter os KeyBoard, etc..
// Conter em Mod, uma vector de Eventos Registrado para aquele mod: Evento, ATIVAÇÂO_DO_EVENTO(ex: Keyboard = KEY_B seria a ativação), variavel contendo o callback
 