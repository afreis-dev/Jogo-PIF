/* ============================================================================
 * main.c - PONTO DE ENTRADA E GAME LOOP
 * ============================================================================
 *
 * Responsabilidade: inicializar a janela do Raylib, rodar o loop principal
 * e chamar os modulos certos dependendo do estado atual do jogo.
 *
 * COMO FUNCIONA UM GAME LOOP:
 *   while (janela aberta) {
 *       1. Calcular delta_tempo (tempo desde o ultimo frame)
 *       2. Atualizar estado (mover coisas, checar colisao, etc.)
 *       3. Desenhar tudo na tela
 *   }
 * A cada iteracao = 1 frame. Com SetTargetFPS(60), o Raylib garante que o loop
 * roda ~60 vezes por segundo.
 *
 * IMPORTANTE - MAQUINA DE ESTADOS:
 *   O jogo tem varias "telas" (menu, combate, upgrade, game over). Em vez de
 *   ter um loop gigante com 20 if/else, usamos um ENUM (EstadoAtual) e um
 *   SWITCH que chama a funcao certa. Facil de adicionar novos estados depois.
 * ========================================================================== */

#include "raylib.h"
#include "tipos.h"

/* Modulos implementados pelo Dev 1 (Arthur) */
#include "jogador.h"
#include "profecia.h"
#include "colisao.h"

/* Modulos que sao STUBS por enquanto - Dev 2 e Dev 3 vao preencher.
 * O codigo compila e roda mesmo com stubs vazios porque os headers
 * declaram as funcoes e os .c implementam versoes que nao fazem nada. */
#include "magias.h"
#include "inimigos.h"
#include "onda.h"
#include "cartas.h"
#include "dados.h"
#include "salvamento.h"
#include "hud.h"

#include <stdlib.h>   /* srand, rand */
#include <stdio.h>    /* snprintf */
#include <time.h>     /* time() pra seed inicial */
#include <math.h>     /* floorf, ceilf pra desenhar o grid infinito */


/* ============================================================================
 * PROTOTIPOS (declaracoes) de funcoes LOCAIS deste arquivo.
 * Ficam como "static" porque so sao usadas dentro de main.c.
 * ========================================================================== */
static void jogo_inicializar(EstadoJogo *ej);
static void jogo_atualizar(EstadoJogo *ej);
static void jogo_desenhar(const EstadoJogo *ej);
static void jogo_finalizar(EstadoJogo *ej);

static void desenhar_grid_mundo(const Camera2D *camera);
static void desenhar_mundo_combate(const EstadoJogo *ej);

static void atualizar_menu(EstadoJogo *ej);
static void atualizar_revelacao_profecia(EstadoJogo *ej);
static void atualizar_combate(EstadoJogo *ej);
static void atualizar_cartas_upgrade(EstadoJogo *ej);
static void atualizar_game_over(EstadoJogo *ej);


/* ============================================================================
 * MAIN - PONTO DE ENTRADA
 * ========================================================================== */
int main(void) {
    /* 1. Inicializacao do Raylib (abre a janela, prepara contexto grafico) */
    InitWindow(LARGURA_TELA, ALTURA_TELA, "AUGUR - Projeto PIF CESAR");
    SetTargetFPS(FPS_ALVO);

    /* Inicializa o gerador aleatorio do sistema com o horario atual.
     * Sem isso, toda vez que o jogo abre, as seeds seriam iguais. */
    srand((unsigned int)time(NULL));

    /* 2. Prepara o estado do jogo.
     * A sintaxe " = {0}" zera TODOS os campos da struct (ponteiros = NULL,
     * numeros = 0, bools = false). E um idioma classico em C. */
    EstadoJogo ej = {0};
    jogo_inicializar(&ej);

    /* 3. LOOP PRINCIPAL.
     * WindowShouldClose() = true quando o usuario clica no X ou aperta ESC.
     * ESTADO_SAIR permite que a gente encerre pelo codigo tambem. */
    while (!WindowShouldClose() && ej.estado_atual != ESTADO_SAIR) {
        jogo_atualizar(&ej);

        /* Bloco de desenho - tudo entre BeginDrawing e EndDrawing vai pra tela. */
        BeginDrawing();
            ClearBackground(BLACK);   /* limpa o frame anterior */
            jogo_desenhar(&ej);
        EndDrawing();
    }

    /* 4. Limpeza - libera memoria alocada dinamicamente e fecha a janela. */
    jogo_finalizar(&ej);
    CloseWindow();
    return 0;
}


/* ============================================================================
 * INICIALIZACAO
 * --------------------------------------------------------------------------
 * Chamada UMA VEZ no inicio. Prepara valores default das structs.
 * ========================================================================== */
static void jogo_inicializar(EstadoJogo *ej) {
    ej->estado_atual   = ESTADO_MENU;
    ej->proximo_estado = ESTADO_MENU;

    jogador_inicializar(&ej->jogador);
    salvamento_carregar(&ej->salvamento);   /* stub por enquanto */

    /* Listas comecam vazias (cabeca = NULL) */
    ej->magias_cabeca   = NULL;
    ej->inimigos_cabeca = NULL;

    /* Camera 2D: offset no centro da tela + target na posicao do jogador.
     * Resultado: o jogador aparece sempre centralizado e o mundo "rola"
     * conforme ele anda. Zoom 1.0 = sem zoom. */
    ej->camera.offset   = (Vector2){ LARGURA_TELA / 2.0f, ALTURA_TELA / 2.0f };
    ej->camera.target   = ej->jogador.posicao;
    ej->camera.rotation = 0.0f;
    ej->camera.zoom     = 1.0f;

    ej->modo_debug = false;
}


/* ============================================================================
 * ATUALIZACAO (LOGICA DO FRAME)
 * --------------------------------------------------------------------------
 * Chamada UMA VEZ POR FRAME. Decide o que acontece baseado no estado atual.
 * ========================================================================== */
static void jogo_atualizar(EstadoJogo *ej) {
    /* GetFrameTime() retorna quantos segundos passaram desde o ultimo frame.
     * Usamos pra movimento suave: "posicao += velocidade * delta_tempo"
     * garante velocidade constante mesmo se o FPS variar. */
    ej->delta_tempo = GetFrameTime();
    ej->tempo_total += ej->delta_tempo;
    ej->contador_frames++;

    /* F1 alterna modo debug (mostra FPS, etc.) */
    if (IsKeyPressed(KEY_F1)) {
        ej->modo_debug = !ej->modo_debug;
    }

    /* DISPATCHER DA MAQUINA DE ESTADOS.
     * Cada estado tem sua propria funcao de atualizacao. */
    switch (ej->estado_atual) {
        case ESTADO_MENU:               atualizar_menu(ej);               break;
        case ESTADO_REVELACAO_PROFECIA: atualizar_revelacao_profecia(ej); break;
        case ESTADO_COMBATE:            atualizar_combate(ej);            break;
        case ESTADO_CARTAS_UPGRADE:     atualizar_cartas_upgrade(ej);     break;
        case ESTADO_GAME_OVER:          atualizar_game_over(ej);          break;
        default: break;
    }

    /* Transicao limpa entre estados.
     * Se algum modulo mudou "proximo_estado", a troca efetiva acontece aqui,
     * ao final do frame. Isso evita bugs tipo "troquei de estado no meio do
     * update e a funcao X ainda rodou com o estado errado". */
    if (ej->proximo_estado != ej->estado_atual) {
        ej->estado_atual = ej->proximo_estado;
    }
}


/* --- Estado: MENU ---------------------------------------------------------
 * Tela inicial. ENTER comeca uma nova run: gera a profecia e vai pra tela
 * de revelacao. */
static void atualizar_menu(EstadoJogo *ej) {
    if (IsKeyPressed(KEY_ENTER)) {
        /* Gera profecia com uma seed aleatoria.
         * rand() retorna int, convertemos pra unsigned. */
        unsigned int seed = (unsigned int)rand();
        profecia_gerar(&ej->profecia, seed);
        ej->proximo_estado = ESTADO_REVELACAO_PROFECIA;
    }
}

/* --- Estado: REVELACAO_PROFECIA -------------------------------------------
 * Mostra os 3 modificadores sorteados. Jogador le e aperta ESPACO pra
 * comecar o combate. */
static void atualizar_revelacao_profecia(EstadoJogo *ej) {
    if (IsKeyPressed(KEY_SPACE)) {
        /* Prepara onda 1 (stub do Dev 3) */
        onda_inicializar(&ej->onda_atual, 1);
        ej->proximo_estado = ESTADO_COMBATE;
    }
}

/* --- Estado: COMBATE ------------------------------------------------------
 * O coracao do jogo. Atualiza jogador, magias, inimigos, onda, colisoes.
 * Se o jogador morrer, vai pra GAME_OVER. Se a onda acabar, vai pra
 * CARTAS_UPGRADE. */
static void atualizar_combate(EstadoJogo *ej) {
    jogador_atualizar(&ej->jogador, ej->delta_tempo);

    /* A camera segue o jogador em tempo real. Como o offset e o centro da
     * tela, isso mantem o player sempre centralizado e o mundo rola em volta. */
    ej->camera.target = ej->jogador.posicao;

    magias_atualizar(ej);               /* stub Dev 3 */
    inimigos_atualizar(ej);             /* stub Dev 3 */
    onda_atualizar(&ej->onda_atual, ej); /* stub Dev 3 */

    colisao_verificar_tudo(ej);         /* Dev 1 - implementado */

    if (ej->jogador.vida <= 0) {
        ej->proximo_estado = ESTADO_GAME_OVER;
    } else if (ej->onda_atual.completa) {
        /* Onda acabou - Dev 2 sorteia cartas, vai pra tela de escolha */
        cartas_gerar_escolhas(ej);
        ej->proximo_estado = ESTADO_CARTAS_UPGRADE;
    }
}

/* --- Estado: CARTAS_UPGRADE ------------------------------------------------
 * Tela de escolha entre ondas. Dev 2 processa input de 1/2/3 pra escolher
 * carta. Depois vai de volta pro combate (proxima onda). */
static void atualizar_cartas_upgrade(EstadoJogo *ej) {
    /* Placeholder: pula pra proxima onda apertando ESPACO.
     * Dev 2 substitui por input real das cartas. */
    if (IsKeyPressed(KEY_SPACE)) {
        onda_inicializar(&ej->onda_atual, ej->onda_atual.numero + 1);
        ej->proximo_estado = ESTADO_COMBATE;
    }
}

/* --- Estado: GAME_OVER ----------------------------------------------------
 * Mostra score, seed e espera input. */
static void atualizar_game_over(EstadoJogo *ej) {
    if (IsKeyPressed(KEY_ENTER)) {
        /* Volta pro menu. Poderia tambem salvar o score aqui (Dev 2). */
        ej->proximo_estado = ESTADO_MENU;
    }
}


/* ============================================================================
 * DESENHO (RENDERIZACAO DO FRAME)
 * --------------------------------------------------------------------------
 * Chamada UMA VEZ POR FRAME, depois de atualizar. So desenha - NAO modifica
 * estado (por isso o ponteiro e "const"). Cada estado desenha uma coisa
 * diferente.
 * ========================================================================== */
static void jogo_desenhar(const EstadoJogo *ej) {
    switch (ej->estado_atual) {
        case ESTADO_MENU:
            DrawText("AUGUR", LARGURA_TELA/2 - 140, 180, 100, GOLD);
            DrawText("Bullet Hell Roguelite",
                     LARGURA_TELA/2 - 140, 290, 24, GRAY);
            DrawText("Pressione ENTER para iniciar",
                     LARGURA_TELA/2 - 180, 440, 24, WHITE);
            DrawText("PIF 2026.1 - CESAR School",
                     LARGURA_TELA/2 - 130, ALTURA_TELA - 40, 16, DARKGRAY);
            break;

        case ESTADO_REVELACAO_PROFECIA:
            profecia_desenhar(&ej->profecia);
            DrawText("Pressione ESPACO para comecar a onda",
                     LARGURA_TELA/2 - 260, ALTURA_TELA - 80, 22, GRAY);
            break;

        case ESTADO_COMBATE:
            desenhar_mundo_combate(ej);
            break;

        case ESTADO_CARTAS_UPGRADE:
            /* Mundo congelado ao fundo: como atualizar_cartas_upgrade nao roda
             * a logica do combate, jogador, inimigos e projeteis ficam parados
             * exatamente onde estavam quando a onda terminou. Visualmente parece
             * que o tempo travou. */
            desenhar_mundo_combate(ej);

            /* Overlay escuro semi-transparente pra dar contraste com a UI. */
            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, (Color){ 0, 0, 0, 160 });

            DrawText("ESCOLHA UM UPGRADE",
                     LARGURA_TELA/2 - 180, 80, 32, GOLD);
            cartas_desenhar_ui(ej); /* stub */
            DrawText("(ESPACO pra continuar)",
                     LARGURA_TELA/2 - 150, ALTURA_TELA - 50, 18, GRAY);
            break;

        case ESTADO_GAME_OVER: {
            DrawText("GAME OVER", LARGURA_TELA/2 - 140, 200, 60, RED);
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "Seed da run: %u",
                     ej->profecia.seed);
            DrawText(buffer, LARGURA_TELA/2 - 100, 320, 22, WHITE);
            DrawText("Pressione ENTER para voltar ao menu",
                     LARGURA_TELA/2 - 230, 440, 20, GRAY);
            break;
        }
        default: break;
    }

    /* Overlay de debug - por cima de tudo. */
    if (ej->modo_debug) {
        DrawFPS(10, 10);
        DrawText("DEBUG [F1]", 10, 30, 14, LIME);
    }
}


/* ============================================================================
 * LIMPEZA FINAL
 * --------------------------------------------------------------------------
 * Chamada UMA VEZ no fim. Libera memoria e salva progresso.
 * ========================================================================== */
static void jogo_finalizar(EstadoJogo *ej) {
    magias_liberar_tudo(ej);      /* stub - libera lista encadeada */
    inimigos_liberar_tudo(ej);    /* stub - libera lista encadeada */
    salvamento_salvar(&ej->salvamento);  /* stub - grava arquivo */
}


/* ============================================================================
 * DESENHO DO MUNDO DE COMBATE
 * --------------------------------------------------------------------------
 * Junta tudo que aparece em tela durante o combate: grid + jogador + magias
 * + inimigos (em coord de mundo, dentro do BeginMode2D) e o HUD por cima
 * (em coord de tela). Vira um helper pra que o estado CARTAS_UPGRADE possa
 * mostrar o mesmo cenario "congelado" como pano de fundo da tela de upgrade.
 * ========================================================================== */
static void desenhar_mundo_combate(const EstadoJogo *ej) {
    BeginMode2D(ej->camera);
        desenhar_grid_mundo(&ej->camera);
        jogador_desenhar(&ej->jogador);
        magias_desenhar(ej);
        inimigos_desenhar(ej);
    EndMode2D();
    desenhar_hud(ej);
}


/* ============================================================================
 * GRID DE REFERENCIA DO MUNDO
 * --------------------------------------------------------------------------
 * Desenha linhas finas em intervalos fixos, mas SO NA AREA VISIVEL da camera.
 * Isso da sensacao de "mundo infinito" (as linhas aparecem rolando conforme o
 * jogador anda) sem precisar renderizar milhoes de linhas - so as que cabem
 * na tela a cada frame.
 *
 * Como deve ser chamada dentro de BeginMode2D, usamos coord de mundo: a area
 * visivel em mundo vai de (target - tela/2/zoom) ate (target + tela/2/zoom).
 * ========================================================================== */
static void desenhar_grid_mundo(const Camera2D *camera) {
    const float ESPACAMENTO = 128.0f;     /* distancia entre linhas, em pixels de mundo */
    const Color COR_GRID    = (Color){ 30, 30, 45, 255 };  /* azul bem escuro, discreto */

    /* Limites visiveis em coord de mundo. zoom divide porque zoom 2 mostra
     * metade do mundo, zoom 0.5 mostra o dobro. */
    float meia_largura = (LARGURA_TELA / 2.0f) / camera->zoom;
    float meia_altura  = (ALTURA_TELA  / 2.0f) / camera->zoom;

    float esquerda = camera->target.x - meia_largura;
    float direita  = camera->target.x + meia_largura;
    float topo     = camera->target.y - meia_altura;
    float baixo    = camera->target.y + meia_altura;

    /* Alinha o inicio pra cair num multiplo de ESPACAMENTO (efeito de grid
     * "fixo no mundo", nao colado na camera). */
    float primeiro_x = floorf(esquerda / ESPACAMENTO) * ESPACAMENTO;
    float primeiro_y = floorf(topo     / ESPACAMENTO) * ESPACAMENTO;

    /* Verticais */
    for (float x = primeiro_x; x <= direita; x += ESPACAMENTO) {
        DrawLineV((Vector2){ x, topo }, (Vector2){ x, baixo }, COR_GRID);
    }
    /* Horizontais */
    for (float y = primeiro_y; y <= baixo; y += ESPACAMENTO) {
        DrawLineV((Vector2){ esquerda, y }, (Vector2){ direita, y }, COR_GRID);
    }
}
