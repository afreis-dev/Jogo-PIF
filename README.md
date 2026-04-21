# AUGUR

Bullet Hell Roguelite - Projeto PIF 2026.1 · CESAR School

## Sobre o jogo

Em AUGUR, cada run comeca com uma **Profecia** - tres regras geradas proceduralmente que definem como aquele mundo funciona. Magias disparam automaticamente enquanto voce esquiva de projeteis inimigos. Entre ondas, escolha cartas de upgrade e decida quando gastar seus dados limitados para tentar melhorar ou piorar um upgrade.

## Equipe

| Dev | Responsabilidade | Modulos |
|-----|------------------|---------|
| Arthur (Dev 1) | Engine & Core | `main.c`, `tipos.h`, `jogador`, `profecia`, `colisao` |
| Dev 2 | Sistemas de Jogo | `cartas`, `dados`, `salvamento`, `hud` |
| Dev 3 | Conteudo | `magias`, `inimigos`, `onda` |

## Requisitos

- GCC 15+ via MSYS2 UCRT64
- Raylib 5.5
- GNU Make 4+

## Build no Windows com MSYS2 UCRT64

Instale o ambiente:

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

Compile:

```powershell
mingw32-make
```

Rode:

```powershell
.\augur.exe
```

Limpe:

```powershell
mingw32-make clean
```

## Estrutura do projeto

```text
src/      -> codigo fonte em modulos por responsabilidade
assets/   -> sprites, sons e fontes
saves/    -> arquivos de progresso gerados em runtime
build/    -> objetos gerados pelo make
```

## Convencoes

- Todo identificador proprio do projeto fica em portugues.
- Comentarios e documentacao ficam em portugues.
- O contrato compartilhado entre os devs fica centralizado em `src/tipos.h`.
- O `main.c` orquestra o loop principal e chama os modulos de jogo.

## Objetivo desta base

Esta base foi reorganizada para deixar o projeto mais facil de manter, compilar e evoluir em grupo. A ideia e que cada dev consiga trabalhar no seu modulo sem precisar refatorar a estrutura do jogo toda vez que uma nova funcionalidade entrar.
