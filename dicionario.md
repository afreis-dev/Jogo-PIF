# Dicionário do AUGUR

Este documento existe pra desbloquear o vocabulário de jogos que aparece o tempo todo no código, no README e nas conversas do grupo. Se você nunca jogou videogame, muitos desses termos parecem siglas aleatórias — eles têm significado bem específico em jogos. Aqui ficam todos juntos, em português claro, com o lugar do projeto onde cada um aparece.

> **Como usar:** abra esse arquivo lado a lado com o que você estiver lendo. Quando bater num termo estranho, dá `Ctrl+F` aqui pra achar a definição. Adicione termos novos conforme aparecerem.

---

## Sumário

- [Gêneros e estrutura macro do jogo](#gêneros-e-estrutura-macro-do-jogo)
- [Combate e entidades](#combate-e-entidades)
- [HUD e interface](#hud-e-interface)
- [Engine e renderização](#engine-e-renderização)
- [Procedural, física e geometria](#procedural-física-e-geometria)

---

## Gêneros e estrutura macro do jogo

### Bullet Hell
**Tradução literal:** "inferno de balas". Subgênero de jogos de ação onde a tela fica cheia de projéteis inimigos e o jogador precisa desviar deles enquanto ataca de volta. O foco é em sobreviver à *quantidade* de coisas voando, não em mirar com precisão.

**No projeto:** AUGUR é um bullet hell — o jogador anda com WASD esquivando, e as magias dele atacam automaticamente. Está descrito assim no `README.md` ("Bullet Hell Roguelite").

### Roguelite
Gênero em que cada partida (chamada **run**) começa do zero, é geralmente curta (10–25 minutos) e termina na morte do jogador. A diferença pro "roguelike" puro é que entre runs você acumula uma progressão permanente — alguma coisa fica desbloqueada mesmo perdendo.

**No projeto:** AUGUR é um roguelite. Cada run gera uma profecia diferente (ver `profecia.c`) e ao morrer você ganha biomassa que persiste entre runs (ver campo `biomassa_total` em `DadosSalvos`, em `tipos.h`).

### Run
Uma **partida individual** do jogo, do começo ao game over. Não confunda com "rodar o programa". Um jogador pode iniciar várias runs em uma mesma sessão de jogo.

**No projeto:** uma run vai do `ESTADO_MENU` (apertar ENTER) até `ESTADO_GAME_OVER`, passando por várias ondas. A função `iniciar_nova_run` (sandbox) reseta o estado pra começar uma run nova.

### Wave / Onda
Um **grupo de inimigos** que aparecem no jogo durante um período. Quando todos morrem (ou um timer acaba), a onda termina, geralmente abrindo uma tela de upgrade. A próxima onda costuma ser mais difícil.

**No projeto:** definida em `tipos.h` como `struct Onda` com `numero`, `inimigos_para_spawnar`, `inimigos_restantes`, `tempo_entre_spawns` e `completa`. O módulo `onda.c` (responsabilidade da Luísa) controla isso.

### Meta-progressão
Progresso permanente do jogador que **persiste entre runs**, mesmo quando ele morre. É o que faz o roguelite ser viciante: cada morte ainda dá algum ganho.

**No projeto:** a moeda é a **biomassa**. Acumula no `DadosSalvos.biomassa_total` (salvo em `saves/biomassa.dat`) e desbloqueia novos dados, magias e profecias.

---

## Combate e entidades

### AoE (Area of Effect)
Ataque de **área** — afeta todo mundo dentro de um círculo/região, em vez de um alvo só. Útil contra muitos inimigos juntos.

**No projeto:** o efeito `EF_EXPLOSAO` em `tipos.h` é AoE. As tabelas em `profecia.c` chamam de "Explosao em area".

### Buff / Debuff
**Buff** = efeito *positivo* temporário (mais dano, mais velocidade, escudo). **Debuff** = efeito *negativo* (lento, queimando, com menos defesa).

**No projeto:** em `tipos.h` os efeitos `EF_VELOCIDADE_MAIS` e `EF_DANO_MAIS` são buffs no jogador. `EF_LENTIDAO` e `EF_VENENO` são debuffs em inimigos.

### Chase AI / IA chase
Comportamento de inimigo mais simples possível: **andar em linha reta na direção do jogador**. Sem desvio, sem estratégia. Igual ao que você vê em Pac-Man.

**No projeto:** é o tipo de IA esperada em `inimigos.c` (`INIMIGO_CORPO_A_CORPO` é um chase puro). O comentário do `inimigos.c` mostra a fórmula: vetor jogador − inimigo, normalizar, multiplicar pela velocidade.

### Combo
Sequência de acertos/abates dentro de uma janela de tempo curta. Geralmente recompensa o jogador (mais dano, gatilho de efeito).

**No projeto:** a condição `COND_COMBO_X3` em `tipos.h` dispara o efeito da profecia depois de 3 kills seguidos.

### Cooldown
**Tempo de espera** entre dois usos de uma habilidade. Aperto a magia → ela dispara → preciso esperar X segundos pra usar de novo.

**No projeto:** as magias do AUGUR são automáticas (atacam sozinhas), mas têm cooldown interno — `tempo_de_vida` dos projéteis em `tipos.h::Magia` é parente disso. Cartas do tipo `CARTA_RECARGA_DADO` reduzem cooldown na ideia geral do GDD.

### Dash
**Investida rápida** numa direção, geralmente curta e com cooldown. Em jogos top-down funciona como esquiva ofensiva.

**No projeto:** a condição `COND_NO_DASH` em `tipos.h` está reservada pra quando essa mecânica for adicionada — ainda não tem dash implementado.

### DoT (Damage over Time)
Dano **contínuo ao longo do tempo**, não num pulso só. Ex.: queimadura que tira 5 HP por segundo durante 4 segundos.

**No projeto:** `EF_VENENO` em `tipos.h` é um DoT. O comentário no enum literalmente diz `/* DoT */`.

### Hit
Acerto. Tanto "o projétil acertou o inimigo" quanto "o inimigo me acertou" são "hits".

**No projeto:** as condições da profecia usam isso muito: `COND_AO_ACERTAR` (toda vez que magia acerta), `COND_EM_CRITICO` (quando o hit é crítico), `COND_AO_RECEBER_DANO` (quando o jogador toma um hit).

### Hitbox
Área **invisível** que define onde uma entidade pode ser atingida. Em jogos 2D simples, costuma ser um círculo ou retângulo.

**No projeto:** o `raio` em `Jogador` e `Inimigo` (em `tipos.h`) é a hitbox circular. A colisão em `colisao.c` usa esses raios pra decidir se duas coisas se tocaram.

### HP (Health Points)
**Pontos de vida.** Quando chega a zero, a entidade morre.

**No projeto:** campos `vida` e `vida_maxima` em `Jogador` e `Inimigo` (`tipos.h`). A barra colorida no canto da tela é a representação visual do HP do jogador (ver `hud.c`).

### Ignite (Igníção / Queimar)
**Status de fogo** que aplica DoT (ver acima) — o alvo "pega fogo" e perde HP por alguns segundos. Muitas vezes pode propagar ou empilhar.

**No projeto:** mencionado nos comentários do GDD como possível efeito do dado em `magias.c::sandbox` ("d6 saiu 6 = +30 dano + ignite passivo").

### Knockback
**Empurrão** que um ataque dá no alvo. Ataque com knockback pra fora afasta o inimigo; sem knockback ele "fica colado" em você.

**No projeto:** ainda não implementado. Em `colisao.c` há um comentário mencionando que seria legal adicionar knockback no contato jogador-inimigo no jogo final. Por ora, só dano puro.

### Piercing (Penetrante)
Atributo de projétil que **atravessa o primeiro alvo** em vez de sumir. Útil pra hit em fila.

**No projeto:** o comentário em `colisao.c` cita explicitamente: o `break` foi omitido na colisão magia-vs-inimigo "pra deixar brecha pra piercing shots".

### Projétil
Qualquer coisa que **voa pela tela** atacando — bala, flecha, bola de fogo. Em código, costuma ter posição, velocidade e tempo de vida.

**No projeto:** struct `Magia` em `tipos.h` é um projétil. A lista encadeada `magias_cabeca` em `EstadoJogo` guarda todos os projéteis ativos.

### Spawn / Spawnar
Verbo: **fazer aparecer** uma entidade no mundo (inimigo, projétil, item). Substantivo: o local/momento em que ela aparece.

**No projeto:** o módulo `onda.c` é responsável por **spawnar inimigos** ao longo do tempo. O comentário em `inimigos.h` mostra os 3 passos: alocar com `malloc`, preencher os campos, inserir no início da lista.

### Stun (Atordoamento)
Status que **impede o alvo de agir** por um curto período (não anda, não ataca). Bom contra inimigos perigosos.

**No projeto:** ainda não tem stun na implementação. O GDD menciona "Choque Térmico" como combo Bola de Fogo + Nova de Gelo: stun de 2s.

### Tick
Cada **iteração regular** de um efeito que tem duração. Um veneno de "5 dano por segundo durante 4s" tem 4 ticks. Também é usado como sinônimo de "frame" em alguns contextos.

**No projeto:** a condição `COND_A_CADA_5S` em `tipos.h` é um tick a cada 5 segundos.

---

## HUD e interface

### HUD (Heads-Up Display)
Camada de **informação fixa na tela**, sobreposta ao jogo: barra de vida, contador de munição, mapinha. Geralmente nos cantos.

**No projeto:** módulo `hud.c` da Sofia. Mostra barra de vida no canto superior esquerdo, número da onda no topo central, e moedinha de biomassa abaixo da barra.

### Overlay
Algo desenhado **por cima** do que já está na tela. Tipicamente semi-transparente — você ainda enxerga o jogo atrás.

**No projeto:** o overlay de pausa (na branch sandbox) escurece a tela com um retângulo preto a 70% de opacidade pra dar contraste com o menu de pausa por cima.

### Score
**Pontuação.** Em roguelites geralmente é "até onde você chegou" + abates + tempo.

**No projeto:** será implementado pela Sofia em `salvamento.c` (campo `melhor_onda` em `DadosSalvos` em `tipos.h` já existe pra registrar o recorde).

---

## Engine e renderização

### BeginMode2D / EndMode2D
Funções do Raylib que **abrem e fecham um bloco com câmera 2D ativa**. Tudo desenhado entre os dois é interpretado em **coordenadas de mundo** (a câmera aplica offset/zoom). Fora do bloco, tudo é coord de tela.

**No projeto:** em `main.c`, o bloco `BeginMode2D(ej->camera) ... EndMode2D()` engloba o desenho do grid, jogador, inimigos, magias e obstáculos. O HUD vem **depois** do `EndMode2D` porque deve ficar fixo na tela.

### Camera2D
Estrutura do Raylib que **define um ponto de vista**. Tem `target` (o que ela aponta), `offset` (onde o target aparece na tela), `zoom` e `rotation`. A câmera segue o jogador atualizando `target = jogador.posicao` a cada frame.

**No projeto:** `EstadoJogo.camera` em `tipos.h`. Inicializada em `main.c::jogo_inicializar` com `offset` no centro da tela e `target` na posição do jogador.

### Coord de mundo × Coord de tela
- **Coord de mundo:** posição "real" da entidade no universo do jogo. Não muda quando a câmera anda. Ex.: o jogador está em (1500, -200) no mundo.
- **Coord de tela:** posição em pixels da janela. (0, 0) é o canto superior esquerdo da tela. Muda conforme você vê o mundo.

A câmera converte de uma pra outra. HUD e menus usam coord de tela. Jogador, inimigos, magias e obstáculos usam coord de mundo.

**No projeto:** o comentário longo em `tipos.h::EstadoJogo.camera` explica isso.

### Delta time / `delta_tempo`
**Tempo decorrido entre o frame atual e o frame anterior**, em segundos. Geralmente um número pequeno tipo `0.0166` (=1/60 a 60fps).

Por que importa: se você fizer `posicao.x += 5` por frame, a velocidade depende do FPS. Se fizer `posicao.x += velocidade * delta_tempo`, a velocidade fica constante mesmo se o FPS oscilar.

**No projeto:** `EstadoJogo.delta_tempo` em `tipos.h`, atualizado a cada frame por `GetFrameTime()` em `main.c::jogo_atualizar`. Usado em quase toda função `_atualizar`.

### Frame
**Um único quadro/imagem** desenhado na tela. O jogo atualiza estado e renderiza um frame, depois outro, depois outro... Igual aos quadros de um filme.

**No projeto:** o loop em `main.c` é o "loop de frames". Cada volta = 1 frame. `SetTargetFPS(60)` pede ao Raylib pra rodar a 60 frames por segundo.

### FPS (Frames Per Second)
**Quantos frames são desenhados por segundo.** Padrão pra jogos é 60. Acima disso é mais fluido; abaixo de 30 é desconfortável.

**No projeto:** `FPS_ALVO = 60` em `tipos.h`. F1 liga o overlay de debug que mostra o FPS atual com `DrawFPS()`.

### Top-down
Câmera vista **de cima pra baixo**, em ângulo fixo. Você enxerga o personagem como se fosse uma vista de mapa. Vampire Survivors, Diablo (em parte), Hotline Miami são top-down.

**No projeto:** AUGUR é top-down. Daí o jogador ser desenhado como um círculo simples e a câmera nunca girar.

### Viewport
A **área visível** da tela do jogo. Em AUGUR, é uma janela de 1280×720 pixels. O viewport é o "buraco" através do qual você vê o mundo (que é maior/infinito).

**No projeto:** `LARGURA_TELA` e `ALTURA_TELA` em `tipos.h` definem o viewport. A função `desenhar_grid_mundo` em `main.c` calcula o viewport visível em coordenadas de mundo pra desenhar só as linhas que estão na tela.

---

## Procedural, física e geometria

### Determinístico
Algo que produz **sempre o mesmo resultado** dado a mesma entrada. Oposto de aleatório de verdade.

**No projeto:** a profecia é determinística a partir da seed. Mesma seed → mesmas 3 regras geradas, sempre. Permite replay e debug.

### Hitbox
Ver **Hitbox** em [Combate e entidades](#combate-e-entidades).

### Overlap
**Sobreposição.** Quando duas formas (círculos, caixas) ocupam parte do mesmo espaço. É o sinal de que houve colisão.

**No projeto:** `colisao.c` mede `overlap = soma_raios − distancia` quando o jogador encosta num inimigo. Esse valor é quanto eles estão "afundados" um no outro, e é exatamente quanto o jogador precisa ser empurrado pra fora.

### Procedural / proceduralmente
Conteúdo **gerado por algoritmo**, não desenhado à mão. Em vez de ter 100 mapas pré-feitos, o jogo escreve um mapa novo a cada partida usando regras + um número aleatório.

**No projeto:** profecia, layout de obstáculos e ondas são procedurais. Tudo a partir de uma seed.

### Push-out
Técnica de colisão: quando duas formas se sobrepõem, **empurra uma pra fora** pela quantidade exata do overlap, na direção do vetor entre os centros. Resultado: as formas ficam só se tocando, não dentro uma da outra.

**No projeto:** `colisao.c` faz push-out do jogador contra inimigos. `obstaculos.c` (quando você implementar) deve fazer push-out do jogador e dos inimigos contra árvores e pedras.

### Seed
**Número que alimenta um gerador procedural.** Mesma seed → mesma sequência de números aleatórios → mesmo conteúdo gerado. Tipo um "código de barras" da run.

**No projeto:** `unsigned int seed` em `Profecia` (`tipos.h`). O ENTER no menu sorteia uma seed nova com `rand()`, e ela é usada tanto pela `profecia_gerar` quanto pela `obstaculos_gerar`. A seed também é mostrada na tela de game over pro jogador anotar.

---

## Quando aparecer um termo novo

Adicione aqui assim que encontrar. Manter este documento atualizado é responsabilidade do grupo inteiro — se você buscou e não achou, esse termo provavelmente vai aparecer várias outras vezes.
