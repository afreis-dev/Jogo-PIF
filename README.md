# AUGUR

Base do projeto do PIF em C com Raylib.

## Visão geral

Este repositório guarda a espinha dorsal do jogo `AUGUR`.
Nesta etapa, o foco é deixar a base do Dev 1 pronta para integração:

- janela gráfica com Raylib;
- organização modular do projeto;
- contratos públicos compartilhados;
- sistema de cenas;
- regras base do jogo;
- pontos de integração para os outros devs.

## Controles da base atual

- `Enter` ou `Espaço`: confirma a ação principal da cena atual
- `Esc`: volta ou encerra a run, dependendo da cena
- `WASD` ou setas: movem o jogador no combate
- `F3`: alterna a camada de debug

## Estrutura de pastas

```text
include/augur  -> headers públicos compartilhados
src/core       -> loop principal, cenas, janela, entrada e integração
src/regras     -> colisão, profecia, tipos base e helpers puros
src/stubs      -> pontos temporários para cartas, dados, HUD, inimigos, magias e waves
assets         -> arquivos visuais e sons do projeto
docs           -> documentação interna do grupo
```

## Build

### Windows com MSYS2 UCRT64

Pré-requisitos:

- `gcc`
- `pkg-config`
- `raylib`
- `mingw32-make`

Comando:

```powershell
mingw32-make
```

Para executar:

```powershell
.\build\augur.exe
```

### Linux ou WSL

Pré-requisitos:

- `gcc`
- `make`
- `pkg-config`
- `raylib`

Comando:

```bash
make
```

Para executar:

```bash
./build/augur
```

## Convenções do projeto

- Todo código próprio deve usar identificadores em português.
- Comentários e documentação devem ficar em português.
- Arquivos de código e documentação devem ser salvos em UTF-8.
- Nomes da Raylib e da linguagem C permanecem como definidos pelas bibliotecas.

## Situação atual da base

O projeto já abre janela, troca de cenas, move o jogador, gera uma profecia determinística por seed
e expõe stubs comentados para os outros devs encaixarem seus sistemas sem mexer na arquitetura.

## Fluxo de integração do grupo

- Dev 1: core, regras, profecia, build e integração final
- Dev 2: cartas, dados, HUD, save e score
- Dev 3: inimigos, magias, projéteis, waves e efeitos visuais

O documento [docs/integracao.md](docs/integracao.md) resume os contratos compartilhados,
a ordem de chamada no loop e os pontos onde cada dev deve entrar.
