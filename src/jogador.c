/* ============================================================================
 * jogador.c - IMPLEMENTACAO DO MODULO JOGADOR
 * ============================================================================
 *
 * Movimentacao com WASD, colisao com as bordas da tela, HP.
 *
 * CONCEITOS IMPORTANTES USADOS AQUI:
 *   - Ponteiro pra struct: recebemos "Jogador *j" em vez de "Jogador j".
 *     Assim a funcao modifica o objeto ORIGINAL, sem copiar tudo.
 *   - Delta_tempo: multiplicamos velocidade pelo tempo decorrido desde o
 *     ultimo frame, garantindo velocidade constante mesmo com FPS variavel.
 *   - Normalizacao de vetor: ao apertar W+D (diagonal), o jogador nao pode
 *     andar mais rapido que na horizontal pura. Pra isso dividimos o vetor
 *     de direcao pelo seu comprimento.
 * ========================================================================== */

#include "jogador.h"
#include "raylib.h"
#include <math.h>    /* sqrtf para normalizar vetor */


/* ----- INICIALIZACAO ----- */
void jogador_inicializar(Jogador *j) {
    /* Posicao inicial: origem do mundo (0, 0). O mundo e infinito e a Camera2D
     * enquadra o jogador sempre no centro da tela, entao nao importa onde ele
     * comeca - importa que sua posicao seja interpretada em coordenadas de
     * mundo, nao de tela. */
    j->posicao        = (Vector2){ 0.0f, 0.0f };
    j->velocidade     = (Vector2){ 0.0f, 0.0f };
    j->raio           = 16.0f;
    j->vida_maxima    = 100;
    j->vida           = j->vida_maxima;
    j->velocidade_movimento = 250.0f;   /* 250 pixels por segundo */
    j->biomassa       = 0;
}


/* ----- ATUALIZACAO (chamada a cada frame durante combate) ----- */
void jogador_atualizar(Jogador *j, float delta_tempo) {
    /* 1. Ler input das teclas WASD.
     * IsKeyDown retorna true ENQUANTO a tecla esta pressionada (diferente
     * de IsKeyPressed, que so retorna true no frame em que foi apertada). */
    Vector2 direcao = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) direcao.y -= 1.0f;   /* Y invertido: cima = negativo */
    if (IsKeyDown(KEY_S)) direcao.y += 1.0f;
    if (IsKeyDown(KEY_A)) direcao.x -= 1.0f;
    if (IsKeyDown(KEY_D)) direcao.x += 1.0f;

    /* 2. Normalizar o vetor de direcao.
     * Se o jogador aperta W+D ao mesmo tempo, direcao = (1, -1), cujo
     * comprimento e sqrt(2) ~ 1.41. Dividindo pelo comprimento, o vetor fica
     * com comprimento 1, e a velocidade na diagonal fica igual a horizontal.
     *
     * Pulamos a divisao se comprimento = 0 (nenhuma tecla apertada) pra
     * evitar divisao por zero. */
    float comprimento = sqrtf(direcao.x * direcao.x + direcao.y * direcao.y);
    if (comprimento > 0.0f) {
        direcao.x /= comprimento;
        direcao.y /= comprimento;
    }

    /* 3. Aplicar velocidade */
    j->velocidade.x = direcao.x * j->velocidade_movimento;
    j->velocidade.y = direcao.y * j->velocidade_movimento;

    /* 4. Integrar (mover a posicao). delta_tempo mantem o movimento suave. */
    j->posicao.x += j->velocidade.x * delta_tempo;
    j->posicao.y += j->velocidade.y * delta_tempo;

    /* Sem clamp: o mundo e infinito (estilo Vampire Survivors). A sensacao de
     * arena vem dos inimigos spawnando em volta do jogador, nao de paredes. */
}


/* ----- DESENHO -----
 * Bolinha azul com contorno branco. Dev 3 pode trocar por sprite depois. */
void jogador_desenhar(const Jogador *j) {
    DrawCircleV(j->posicao, j->raio, SKYBLUE);
    DrawCircleLines((int)j->posicao.x, (int)j->posicao.y, j->raio, WHITE);
}


/* ----- SOFRER DANO -----
 * Subtrai HP, clampa em zero (nao deixa negativo pra UI nao ficar estranha).
 * Main checa "vida <= 0" pra transicionar pra GAME_OVER. */
void jogador_sofrer_dano(Jogador *j, int quantidade) {
    j->vida -= quantidade;
    if (j->vida < 0) {
        j->vida = 0;
    }
}
