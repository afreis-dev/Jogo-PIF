# AUGUR

Bullet Hell Roguelite · Projeto PIF 2026.1 · CESAR School

## Sobre o jogo

Em AUGUR, cada run começa com uma **Profecia** — três regras geradas proceduralmente que definem como aquele mundo funciona. Magias disparam automaticamente enquanto você esquiva de projéteis inimigos. A run é uma timeline de **15 minutos** estilo Vampire Survivors: a cada minuto cheio você escolhe cartas de upgrade, e aos 15:00 surge o chefão final. Derrotá-lo encerra a run com vitória.

Cada run tem uma **seed** visível na tela. Como a profecia é gerada de forma determinística a partir da seed, você pode compartilhar runs marcantes com amigos: mesma seed = mesma profecia = mesmo puzzle inicial. É o ponto de partida pra replays, debug e desafios entre jogadores.

> O design completo do jogo (loop principal, sistema de Profecias, Dados, magias, meta-progressão e cronograma de desenvolvimento) está no GDD: **`augur_gdd.pdf`**.

## Equipe

| Dev | Responsabilidade | Módulos |
|-----|------------------|---------|
| Arthur (Dev 1) | Engine & Core | `main.c`, `tipos.h`, `jogador`, `profecia`, `colisao`, engine de `inimigos`/`magias`/`cronograma`, `obstaculos` |
| Sofia (Dev 2) | Sistemas de Jogo | `cartas`, `dados`, `salvamento`, `hud` |
| Luísa (Dev 3) | Conteúdo | `inimigos_tipos`, `magias_tipos`, `cronograma_eventos` |

## Requisitos

- GCC 15+ via MSYS2 UCRT64
- Raylib 5.5
- GNU Make 4+

> Setup completo do ambiente (Windows e Linux), incluindo como criar um Makefile do zero, está em [docs/TUTORIAL_AMBIENTE.md](docs/TUTORIAL_AMBIENTE.md). Glossário de termos de jogos e arquitetura usados no projeto: [docs/dicionario.md](docs/dicionario.md).

## Instalação rápida do ambiente

Abra o terminal **MSYS2 UCRT64** e rode:

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-raylib
pacman -S mingw-w64-ucrt-x86_64-make
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
|   |-- core/                          <- motor: loop, contrato (tipos.h), colisão
|   |   |-- main.c                     <- game loop e máquina de estados
|   |   |-- tipos.h                    <- contrato entre devs (todas as structs)
|   |   `-- colisao.c/.h               <- detecção de colisão genérica
|   |
|   |-- entidades/                     <- coisas que vivem no mundo
|   |   |-- jogador.c/.h               <- movimento, HP e direção
|   |   |-- inimigos.c/.h              <- ENGINE de inimigos (Arthur)
|   |   |-- inimigos_tipos.c/.h        <- TABELA + IA por tipo (Luísa)
|   |   |-- magias.c/.h                <- ENGINE de projéteis (Arthur)
|   |   |-- magias_tipos.c/.h          <- TABELA por elemento + auto-fire (Luísa)
|   |   `-- obstaculos.c/.h            <- árvores e pedras (Arthur)
|   |
|   |-- sistemas/                      <- regras e lógica do jogo
|   |   |-- profecia.c/.h              <- gerador procedural de profecias
|   |   |-- cronograma.c/.h            <- ENGINE da timeline 15min (Arthur)
|   |   |-- cronograma_eventos.c/.h    <- TABELA de eventos da timeline (Luísa)
|   |   |-- cartas.c/.h                <- sistema de upgrade (Sofia)
|   |   |-- dados.c/.h                 <- sistema de dados (Sofia)
|   |   `-- salvamento.c/.h            <- save/load em arquivo (Sofia)
|   |
|   `-- interface/                     <- UI e HUD
|       `-- hud.c/.h                   <- HUD durante combate (Sofia)
|
|-- assets/                            <- sprites, sons e fontes
|-- build/                             <- arquivos .o gerados pelo make
|-- saves/                             <- progresso gerado em runtime
|-- docs/                              <- guias internos pro grupo
|   |-- TUTORIAL_AMBIENTE.md           <- setup do MSYS2/Linux + Makefile do zero
|   |-- dicionario.md                  <- glossario de termos de jogos e arquitetura
|   `-- issue-hud-cronograma.md        <- task tecnica pra Sofia (HUD VS-like)
|-- Makefile
`-- README.md
```

> **Engine vs. conteúdo.** A regra é: tudo que termina em `_tipos.c` ou `_eventos.c` é **conteúdo da Luísa** — tabelas de stats, IA específica, timeline de spawns. A engine (Arthur) consome essas tabelas. Pra balancear ou criar coisa nova, a Luísa só edita os arquivos de conteúdo.

> Os `#include` continuam sendo por nome simples (ex.: `#include "tipos.h"`) porque o Makefile adiciona cada subpasta de `src/` ao `-I` do compilador. Você não precisa escrever `#include "core/tipos.h"`.

## Como adicionar conteúdo (guia da Luísa)

Toda mudança de balanceamento ou conteúdo novo acontece em **três arquivos**:

| O que você quer | Onde editar |
|-----------------|-------------|
| Stats e IA de inimigos | `src/entidades/inimigos_tipos.c` |
| Stats e auto-fire de magias | `src/entidades/magias_tipos.c` |
| Quando spawnar o quê | `src/sistemas/cronograma_eventos.c` |

A engine (`inimigos.c`, `magias.c`, `cronograma.c`) lê dessas tabelas. Você não precisa mexer em alocação, lista encadeada, push-out ou render — tudo isso já está pronto.

### Adicionar um inimigo novo (ex.: elite à distância)

1. Em `src/core/tipos.h`, no enum `TipoInimigo`, adicione um valor novo:
   ```c
   typedef enum {
       INIMIGO_CORPO_A_CORPO,
       INIMIGO_A_DISTANCIA,
       INIMIGO_ELITE,
       INIMIGO_ELITE_DISTANCIA,   /* novo */
       INIMIGO_CHEFE
   } TipoInimigo;
   ```
2. Em `src/entidades/inimigos_tipos.c`, adicione uma linha em `PARAMETROS_INIMIGO[]` na **mesma ordem do enum**:
   ```c
   {
       .vida_base = 70, .dano = 16, .velocidade_movimento = 80.0f,
       .raio = 14.0f, .raio_visual = 14.0f,
       .cor = (Color){80, 200, 220, 255},
       .recompensa_biomassa = 30,
       .comportamento = IA_KITER,    /* reaproveita a IA do ranged base */
   },
   ```
3. Pronto. Pra fazer ele aparecer numa fase, adicione uma linha em `cronograma_eventos.c`.

### Mexer na timeline da run

Cada linha em `EVENTOS_CRONOGRAMA[]` é uma fase declarativa: "do tempo X ao tempo Y, spawnar tipo T a cada Z segundos". Tempos em segundos (`5 * 60` = minuto 5). O chefão é spawnado pela engine automaticamente aos 15:00 — não precisa cadastrar.

```c
{ 4.0f * 60.0f, 15.0f * 60.0f, INIMIGO_ELITE_DISTANCIA, 12.0f, 0.0f, false },
```

### Mexer numa magia existente

Em `magias_tipos.c`, ache a linha do elemento (ordem do enum) e ajuste dano, cooldown ou cor. Pra mudar o jeito que o jogador atira (ex.: spread em leque), edite a função `magias_tipos_processar_auto_fire`.

### Adicionar uma IA inédita

1. Em `tipos.h`, novo valor no enum `ComportamentoIA`.
2. Em `inimigos_tipos.c`, crie uma função `static void ia_minha_ia(Inimigo *i, EstadoJogo *ej)` que escreve em `i->velocidade`.
3. Adicione um `case` em `inimigos_tipos_executar_ia`.

A função recebe `EstadoJogo *ej`, então tem acesso à lista inteira de inimigos via `ej->inimigos_cabeca` — isso permite **IAs coordenadas** (cada inimigo decide olhando o que os outros estão fazendo). Exemplo já no projeto: `ia_kiter` itera a lista pra contar quantos kiters vivos existem e descobrir seu índice ordenado por ângulo polar, daí cada um ocupa um slot único num círculo orbital cercando o jogador. Outras coordenações possíveis: cargas em V, parede defendendo o boss, divisão de flancos entre dois grupos. Custo: O(N²) por frame se cada inimigo iterar a lista — aceitável até umas dezenas de inimigos.

Pra IAs **isoladas** com leve variedade entre indivíduos sem precisar de campo `id`: use `hash_pointer_para_unitario(i)` (já em `inimigos_tipos.c`) — devolve um valor estável em `[-1, 1]` derivado do endereço do nó, que serve de "personalidade" do inimigo (offset angular, fase de timer, jitter de velocidade).

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
| WASD ou setinhas | Mover jogador |
| ENTER | Confirmar / Avançar |
| ESPAÇO | Iniciar combate / Confirmar carta |
| Q | Liga/desliga tiros automáticos |
| ESC | Pausar (durante combate) / Retomar |
| F1 | Alternar modo debug |

## Conceitos obrigatórios de PIF implementados

| Conceito | Onde |
|----------|------|
| Structs | `tipos.h` — `Jogador`, `Inimigo`, `Magia`, `Profecia`, `Cronograma`, `EstadoJogo`, `DadosSalvos`, `Carta`, `Dado`, `Obstaculo`, `EventoCronograma`, `ParametrosInimigo`, `ParametrosMagia` |
| Ponteiros | `EstadoJogo *ej` em quase toda função; `proxima`/`proximo` nos nós das listas; ponteiro duplo `InimigoNo **` na remoção de mortos em `inimigos.c` |
| Alocação dinâmica | `malloc`/`free` em `MagiaNo` (`magias.c`) e `InimigoNo` (`inimigos.c`); `free` ao morrer o nó e `_liberar_tudo` ao encerrar a run |
| Listas encadeadas | `MagiaNo` (projéteis) e `InimigoNo` (inimigos) — inserção na cabeça em O(1), remoção com ponteiro duplo |
| Matrizes | `mods[3]` (profecia), `escolhas_upgrade[CARTAS_POR_ESCOLHA]`, `obstaculos[MAX_OBSTACULOS]`, `dados_ativos[MAX_DADOS_JOGADOR]`, `eventos[MAX_EVENTOS_CRONOGRAMA]` no cronograma; tabelas `PARAMETROS_INIMIGO[]`, `PARAMETROS_MAGIA[]`, `EVENTOS_CRONOGRAMA[]` |
| Arquivo | `salvamento.c` — `saves/biomassa.dat` (biomassa total, runs completadas, profecias desbloqueadas). Top scores em `scores.dat` planejados pra entrega final. |

> **Sobre a CLI-LIB.** O spec do PIF cita a [`cli-lib`](https://github.com/tgfb/cli-lib/) como biblioteca padrão sugerida (terminal-based). AUGUR optou por **Raylib** — também permitido pelo enunciado: "É permitido usar outra biblioteca para jogos, entretanto seu uso é de responsabilidade do grupo". A justificativa é técnica: bullet hell precisa renderizar dezenas de projéteis com colisão circular precisa, e câmera 2D rolando suavemente pelo mundo — coisas que ficam pesadas no terminal.
