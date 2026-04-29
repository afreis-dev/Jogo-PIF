# AUGUR

Bullet Hell Roguelite · Projeto PIF 2026.1 · CESAR School

## Sobre o jogo

Em AUGUR, cada run começa com uma **Profecia** — três regras geradas proceduralmente que definem como aquele mundo funciona. Magias disparam automaticamente enquanto você esquiva de projéteis inimigos. Entre ondas, escolha cartas de upgrade e decida quando gastar seus dados limitados pra tentar melhorar (ou piorar) um upgrade.

Cada run tem uma **seed** visível na tela. Como a profecia é gerada de forma determinística a partir da seed, você pode compartilhar runs marcantes com amigos: mesma seed = mesma profecia = mesmo puzzle inicial. É o ponto de partida pra replays, debug e desafios entre jogadores.

## Equipe

| Dev | Responsabilidade | Módulos |
|-----|------------------|---------|
| Arthur (Dev 1) | Engine & Core | `main.c`, `tipos.h`, `jogador`, `profecia`, `colisao` |
| Sofia (Dev 2) | Sistemas de Jogo | `cartas`, `dados`, `salvamento`, `hud` |
| Luísa (Dev 3) | Conteúdo | `magias`, `inimigos`, `onda`, `obstaculos` |

## Requisitos

- GCC 15+ via MSYS2 UCRT64
- Raylib 5.5
- GNU Make 4+

## Instalação do ambiente

Abra o terminal **MSYS2 UCRT64** e rode:

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-raylib
pacman -S make
```

Valide:

```bash
pkg-config --cflags --libs raylib
gcc --version
mingw32-make --version
```

## Como compilar e rodar

No PowerShell:

```powershell
mingw32-make
.\augur.exe
mingw32-make clean
```

No terminal MSYS2:

```bash
make
make run
make clean
```

## Estrutura de pastas

O código é organizado em módulos por responsabilidade. Cada subpasta de `src/` agrupa arquivos com propósito relacionado, deixando claro pra cada dev onde encostar.

```text
Jogo-PIF/
|-- src/
|   |-- core/                    <- motor: loop, contrato (tipos.h), colisão
|   |   |-- main.c               <- game loop e máquina de estados
|   |   |-- tipos.h              <- contrato entre devs (todas as structs)
|   |   `-- colisao.c/.h         <- detecção de colisão genérica
|   |
|   |-- entidades/               <- coisas que vivem no mundo
|   |   |-- jogador.c/.h         <- movimento, HP e direção
|   |   |-- inimigos.c/.h        <- spawn, IA e lista encadeada (Luísa)
|   |   |-- magias.c/.h          <- projéteis e magias (Luísa)
|   |   `-- obstaculos.c/.h      <- árvores e pedras (Luísa)
|   |
|   |-- sistemas/                <- regras e lógica do jogo
|   |   |-- profecia.c/.h        <- gerador procedural de profecias
|   |   |-- onda.c/.h            <- wave generator (Luísa)
|   |   |-- cartas.c/.h          <- sistema de upgrade (Sofia)
|   |   |-- dados.c/.h           <- sistema de dados (Sofia)
|   |   `-- salvamento.c/.h      <- save/load em arquivo (Sofia)
|   |
|   `-- interface/               <- UI e HUD
|       `-- hud.c/.h             <- HUD durante combate (Sofia)
|
|-- assets/                      <- sprites, sons e fontes
|-- build/                       <- arquivos .o gerados pelo make
|-- saves/                       <- progresso gerado em runtime
|-- Makefile
`-- README.md
```

> Os `#include` continuam sendo por nome simples (ex.: `#include "tipos.h"`) porque o Makefile adiciona cada subpasta de `src/` ao `-I` do compilador. Você não precisa escrever `#include "core/tipos.h"`.

## Convenções de código C

### Header (`.h`) é o "cartão de visita" do módulo

Cada módulo tem um par `nome.c` + `nome.h`:

- **`.h`** — DECLARA o que o módulo expõe pra fora: protótipos das funções públicas, structs, enums, constantes. Nada de implementação aqui.
- **`.c`** — IMPLEMENTA o que o `.h` declarou. É onde o código de verdade vive.

Por que separar?

- Outros arquivos só precisam incluir o `.h` pra usar as funções — sem ver os detalhes da implementação.
- O compilador valida o tipo dos argumentos na hora da chamada, evitando bugs.
- Recompila mais rápido: mudar só o `.c` não obriga recompilar quem usa o módulo.

### `#include`-guard (`#ifndef`/`#define`/`#endif`)

Todo header começa com este padrão:

```c
#ifndef NOME_H
#define NOME_H

/* ... conteúdo do header ... */

#endif /* NOME_H */
```

Garante que o conteúdo do header só seja lido **uma única vez** pelo compilador, mesmo que vários arquivos o incluam (direta ou indiretamente). Sem isso, daria erro de "redefinição" de tipos.

### `EstadoJogo *ej` em todo lugar

A struct raiz `EstadoJogo` (definida em `tipos.h`) carrega TODO o estado da run: jogador, listas de inimigos e magias, profecia, onda atual, etc. Quase toda função do projeto recebe um `EstadoJogo *ej` por parâmetro. Vantagens:

- Zero variáveis globais espalhadas pelo código.
- Qualquer função vê o contexto inteiro.
- Fica óbvio quem depende de quê (basta olhar o `ej->...` no corpo da função).

## Controles

| Tecla | Ação |
|-------|------|
| WASD | Mover jogador |
| ENTER | Confirmar / Avançar |
| ESPAÇO | Iniciar combate / Próxima onda |
| F1 | Alternar modo debug |
| ESC | Sair |

## Conceitos obrigatórios de PIF implementados

| Conceito | Onde |
|----------|------|
| Structs | `tipos.h` — `Jogador`, `Inimigo`, `Profecia`, `EstadoJogo` |
| Ponteiros | `EstadoJogo *` nas funções; `next` das listas |
| Alocação dinâmica | `malloc`/`free` em `MagiaNo` e `InimigoNo` |
| Listas encadeadas | `MagiaNo` e `InimigoNo` |
| Matrizes | `mods[3]`, `escolhas_upgrade[3]`, `obstaculos[]`, tabelas de nomes |
| Arquivo | `salvamento.c` — `saves/biomassa.dat` |
