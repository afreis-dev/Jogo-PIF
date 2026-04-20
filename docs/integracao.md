# Integração do Grupo

## Visão geral

Esta base foi organizada para permitir trabalho paralelo sem quebrar o core.
O Dev 1 mantém o ciclo principal do jogo e expõe contratos estáveis para os outros módulos.

## Contratos compartilhados

Os tipos públicos ficam em `include/augur`:

- `IdCena`
- `DadosSalvos`
- `ModificadorProfecia`
- `Profecia`
- `NoInimigo`
- `NoMagia`
- `NoProjetil`
- `Jogador`
- `EstadoRun`
- `ContextoJogo`

As APIs públicas principais são:

- `iniciar_contexto`
- `atualizar_contexto`
- `desenhar_contexto`
- `finalizar_contexto`
- `trocar_cena`
- `gerar_profecia`
- `colide_circulos`
- `colide_retangulos`
- `limitar_jogador_na_tela`
- `preencher_mapa_colisao_base`

## Divisão de responsabilidade

### Dev 1

- loop principal
- janela, tempo e entrada
- sistema de cenas
- profecia
- colisão e helpers puros
- integração final

### Dev 2

- cartas de upgrade
- dados
- HUD
- save
- score

### Dev 3

- inimigos
- waves
- magias
- projéteis
- efeitos visuais

## Ordem de chamada no core

### Combate

1. core atualiza movimento do jogador
2. `atualizar_stub_waves`
3. `atualizar_stub_inimigos`
4. `atualizar_stub_magias`
5. `atualizar_stub_hud`
6. core resolve transições de cena

Na renderização do combate:

1. arena e grade base
2. `desenhar_stub_waves`
3. `desenhar_stub_inimigos`
4. `desenhar_stub_magias`
5. jogador
6. `desenhar_stub_hud`

### Upgrade

1. `atualizar_stub_cartas`
2. `atualizar_stub_dados`
3. core lê confirmação ou cancelamento

Na renderização do upgrade:

1. `desenhar_stub_cartas`
2. `desenhar_stub_dados`

## Campos que os outros devs devem observar

- `contexto->profecia`: leitura livre para adaptar efeitos à run
- `contexto->jogador`: pode receber upgrades, dano, cura e biomassa da run
- `contexto->estado_run`: pode controlar wave atual, listas ativas e flags de transição
- `contexto->dados_salvos`: deve ser tratado com cuidado quando save e score forem entrar

## Observações práticas

- Os stubs atuais já compilam e já são chamados pelo core.
- Sempre que possível, preservar os contratos públicos em `include/augur`.
- Se um módulo precisar de dado novo no contexto, o ideal é discutir primeiro o header público e só depois espalhar uso pelo projeto.
