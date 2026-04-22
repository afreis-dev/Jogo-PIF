# AUGUR

Bullet Hell Roguelite - Projeto PIF 2026.1 · CESAR School

## Sobre o jogo

Em AUGUR, cada run comeca com uma **Profecia** - tres regras geradas proceduralmente que definem como aquele mundo funciona. Magias disparam automaticamente enquanto voce esquiva de projeteis inimigos. Entre ondas, escolha cartas de upgrade e decida quando gastar seus dados limitados para tentar melhorar ou piorar um upgrade.

Cada run tem uma **seed** visivel na tela. Como a profecia e gerada de forma deterministica a partir da seed, voce pode compartilhar runs marcantes com amigos: mesma seed = mesma profecia = mesmo puzzle inicial. E o ponto de partida pra replays, debug e desafios entre jogadores.

## Equipe

| Dev | Responsabilidade | Modulos |
|-----|------------------|---------|
| Arthur (Dev 1) | Engine & Core | `main.c`, `tipos.h`, `jogador`, `profecia`, `colisao` |
| Sofia (Dev 2) | Sistemas de Jogo | `cartas`, `dados`, `salvamento`, `hud` |
| Luisa (Dev 3) | Conteudo | `magias`, `inimigos`, `onda` |

## Requisitos

- GCC 15+ via MSYS2 UCRT64
- Raylib 5.5
- GNU Make 4+

## Instalacao do ambiente

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

```text
Jogo-PIF/
|-- src/
|   |-- tipos.h         <- contrato entre devs
|   |-- main.c          <- game loop
|   |-- jogador.c/.h    <- movimento, HP e bordas
|   |-- profecia.c/.h   <- gerador procedural de profecias
|   |-- colisao.c/.h    <- deteccao de colisao generica
|   |-- magias.c/.h     <- projeteis e magias (Luisa)
|   |-- inimigos.c/.h   <- spawn e IA de inimigos (Luisa)
|   |-- onda.c/.h       <- gerador de ondas (Luisa)
|   |-- cartas.c/.h     <- sistema de upgrade (Sofia)
|   |-- dados.c/.h      <- sistema de dados (Sofia)
|   |-- salvamento.c/.h <- save/load em arquivo (Sofia)
|   `-- hud.c/.h        <- interface durante combate (Sofia)
|-- assets/             <- sprites, sons e fontes
|-- build/              <- arquivos .o gerados pelo make
|-- saves/              <- progresso gerado em runtime
|-- Makefile
`-- README.md
```

## Controles

| Tecla | Acao |
|-------|------|
| WASD | Mover jogador |
| ENTER | Confirmar / Avancar |
| ESPACO | Iniciar combate / Proxima onda |
| F1 | Alternar modo debug |
| ESC | Sair |

## Conceitos obrigatorios do PIF implementados

| Conceito | Onde |
|----------|------|
| Structs | `tipos.h` - `Jogador`, `Inimigo`, `Profecia`, `EstadoJogo` |
| Ponteiros | `EstadoJogo*` nas funcoes; `next` das listas |
| Alocacao dinamica | `malloc`/`free` em `MagiaNo` e `InimigoNo` |
| Listas encadeadas | `MagiaNo` e `InimigoNo` |
| Matrizes | `mods[3]`, `escolhas_upgrade[3]`, tabelas de nomes |
| Arquivo | `salvamento.c` - `saves/biomassa.dat` |
