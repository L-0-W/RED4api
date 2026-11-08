# RED4api

Biblioteca em **C++** voltada a **aprendizado**, para construção de mods de *Cyberpunk 2077*.

Ela integra o [RED4ext.SDK](https://github.com/WopsS/RED4ext.SDK) com um **transpilador** que gera scripts `.reds`, permitindo escrever a lógica do mod em C++ e, no futuro, expor tanto APIs de script quanto funções nativas de forma unificada.

## Propósito e escopo

RED4api **não** oferece vantagem prática sobre o fluxo tradicional (scripts `.reds` + RED4ext quando for preciso nativo): não é mais rápida, nem mais completa, nem um substituto “melhor” dessas ferramentas.

O foco é **aprendizado de C++** aplicado a modding: modelar mods, eventos, AST e geração de código, com uma API confortável para quem quer focar em C++ e ainda usar o ecossistema de scripts do jogo.

Com a biblioteca (quando madura) você poderá:

- usar funcionalidades já disponíveis nos scripts `.reds`;
- criar funções nativas via RED4ext;
- manter uma integração fluida entre as duas camadas.

> **Estado atual:** o projeto está **incompleto**. O fluxo documentado abaixo cobre o uso mínimo do SDK, igual ao smoke test em `tests/Smoke.cpp`.

---

## Arquitetura (visão geral)

```
include/RED4api/     → API pública (headers)
src/Core/            → dados do mod (Mod)
src/Transpiler/      → AST + geração/registro (REDPiler)
src/DLL/             → suporte estático a plugins RED4ext
src/Utils/           → utilitários internos
tools/CLI/           → executável `red4api`
tests/               → smoke / testes
cmake/               → helpers CMake
```

Targets CMake principais:

| Target | Tipo | Função |
|--------|------|--------|
| `RED4apiCore` | static lib | Modelo do mod (`Mod`) |
| `RED4apiTranspiler` | static lib | Transpilador / AST (`REDPiler`) |
| `RED4apiDLL` | static lib | Ponte com RED4ext.SDK (headers) |
| `RED4apiCLI` (`red4api`) | executable | CLI experimental |
| `RED4apiSmokeTest` | executable + CTest | Teste mínimo de uso da API |

Dependência externa: **RED4ext.SDK** (baixado via `FetchContent` no `CMakeLists.txt`).

---

## O que cada arquivo faz

### Raiz / build

| Arquivo | Função |
|---------|--------|
| `CMakeLists.txt` | Define o projeto C++23, opções `RED4API_BUILD_CLI` / `RED4API_BUILD_TESTS`, fetch do RED4ext.SDK e os targets acima |
| `cmake/CompilerOptions.cmake` | Reservado a flags de compilação (ainda vazio) |
| `cmake/Dependencies.cmake` | Reservado a dependências extras (ainda vazio) |
| `cmake/RED4apiConfig.cmake.in` | Template de pacote CMake para consumo externo (install/find_package) |
| `CMakeSettings.json` / `.vscode/*` | Configuração local de IDE (configure/build/debug) |
| `LICENSE` | Licença do projeto |

### API pública (`include/RED4api/`)

| Arquivo | Função |
|---------|--------|
| `RED4api.hpp` | Header “guarda-chuva”: inclui `Mod` e `InputEvents` |
| `Mod.hpp` | Struct `Mod`: nome, autor, versão, id; setters; `RegisterEvent(...)` |
| `Types/Enums.hpp` | Enums de input (`InputKey`, `InputAction`) usados pela API de eventos |
| `Transpiler/Events/InputEvents.hpp` | API estática de eventos de input (`Keyboard`, stubs de `Mouse` / `Microfone`) |

### Core (`src/Core/`)

| Arquivo | Função |
|---------|--------|
| `Mod.cpp` | Implementação de `Mod`: gera id (`IDGenerator`), cria estrutura inicial no `REDPiler` e encaminha `RegisterEvent` ao transpilador |

### Transpilador (`src/Transpiler/`)

| Arquivo | Função |
|---------|--------|
| `Transpiler.hpp` / `Transpiler.cpp` | Classe `REDPiler`: monta árvore a partir de metadados do mod, percorre nós (`LoopNodes`), stubs de `RegisterEvent` e (comentado) escrita de `.reds` |
| `AST.hpp` / `AST.cpp` | AST simples (`Root`, `Module`, `Metadada`/`Metadata`, `Node`, `InsertExpr`) |
| `Files.hpp` | Mapa `mod_id → path` dos arquivos gerados (uso futuro na emissão de `.reds`) |
| `Events/InputEvents.cpp` | Implementações stub de `Mouse` / `Microfone` |

### DLL / utils / CLI / testes

| Arquivo | Função |
|---------|--------|
| `src/DLL/Dll.cpp` | Mantém o target `RED4apiDLL` compilável; entry points do plugin ficam no mod consumidor |
| `src/Utils/IDGenerator.hpp` | Gerador sequencial de ids para instâncias de `Mod` |
| `tools/CLI/main.cpp` | Entrada da CLI `red4api` (argumentos mínimos) |
| `tools/CLI/Commands/Commands.hpp` | Parse simples de comandos (`init` / default) |
| `tests/CMakeLists.txt` | Define `RED4apiSmokeTest` e registra no CTest |
| `tests/Smoke.cpp` | Exemplo mínimo de uso da API (ver seção abaixo) |

---

## Requisitos

- CMake **≥ 3.24**
- Compilador com **C++23** (MSVC / clang-cl, etc.)
- Gerador **Ninja** (recomendado; já usado nas tasks do VS Code)
- Git (para o `FetchContent` do RED4ext.SDK)
- Rede na primeira configuração (download do SDK)

---

## Build

Na raiz do repositório:

```bash
cmake -S . -B build-clangd -G Ninja ^
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DRED4API_BUILD_CLI=ON ^
  -DRED4API_BUILD_TESTS=ON

cmake --build build-clangd
```

No VS Code / Cursor, as tasks equivalentes são:

- `cmake-configure-clangd`
- `cmake-build-clangd`
- `cmake-reconfigure-and-build-clangd`

Artefatos úteis após o build:

- `build-clangd/tests/RED4apiSmokeTest.exe` — smoke test
- `build-clangd/red4api.exe` — CLI
- bibliotecas estáticas: `RED4apiCore`, `RED4apiTranspiler`, `RED4apiDLL`

Rodar o teste:

```bash
ctest --test-dir build-clangd -R RED4apiSmokeTest --output-on-failure
# ou diretamente:
build-clangd\tests\RED4apiSmokeTest.exe
```

---

## Usar o SDK (teste simples = Smoke)

Hoje o caminho mínimo é o mesmo de `tests/Smoke.cpp`:

1. Incluir a API pública.
2. Criar um `Mod` com nome, autor e versão.
3. Registrar um evento de teclado via `InputEvents::Keyboard`.
4. Passar um callback que retorna `bool`.

```cpp
#include <RED4api/RED4api.hpp>
// ou, de forma equivalente no smoke:
// #include "RED4api/Transpiler/Events/InputEvents.hpp"
// #include <RED4api/RED4api.hpp>

int main()
{
  const RED4api::Mod mod{"smoke-test", "RED4api", "1.0.0"};

  const bool ok = RED4api::InputEvents::Keyboard(
      mod,
      RED4api::InputAction::Press,
      RED4api::InputKey::KEY_E,
      [] { return true; });

  return ok ? 0 : 1;
}
```

### Linkagem (CMake)

Para um executável de teste como o Smoke:

```cmake
add_executable(MeuSmoke MeuSmoke.cpp)
target_link_libraries(MeuSmoke PRIVATE RED4api::Core RED4api::Transpiler)
```

- `RED4api::Core` — tipos e ciclo de vida do `Mod`
- `RED4api::Transpiler` — camada que o Core já usa ao construir o mod / registrar eventos
- `RED4api::DLL` — só quando for ligar código nativo RED4ext num plugin real

### O que acontece hoje (importante)

No estado atual:

- `Mod` cria um id e chama `REDPiler::CreateFileMod` (monta nós de metadados/módulo na AST e imprime tipos; **escrita de `.reds` ainda comentada**).
- `InputEvents::Keyboard` **ainda não chama** `mod.RegisterEvent` — retorna `true` (stub).
- `RegisterEvent` / `REDPiler::RegisterEvent` também são stubs (`TODO`).

Ou seja: o Smoke valida **compilar, linkar e exercitar a superfície da API**, não um mod completo no jogo.

Direção prevista (comentários no próprio `Smoke.cpp`):  
`InputEvents::Keyboard` → `Mod::RegisterEvent` → `REDPiler::RegisterEvent` → AST → emissão `.reds`, com lista de eventos registrados no `Mod`.

---

## Opções CMake

| Opção | Default | Descrição |
|-------|---------|-----------|
| `RED4API_BUILD_CLI` | `ON` | Compila o executável `red4api` |
| `RED4API_BUILD_TESTS` | `ON` | Compila e registra os testes |

---

## Resumo

RED4api é um **SDK educacional em C++** para mods de Cyberpunk 2077, unindo ideia de scripts `.reds` e nativos RED4ext. Use o Smoke como modelo de consumo; a geração real de script e o registro completo de eventos ainda estão em construção.
```
