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

No início do projeto, existe apenas uma estrutura mínima para compilar e organizar as próximas etapas.
As funcionalidades jogáveis são adicionadas em commits separados para manter o histórico claro.
