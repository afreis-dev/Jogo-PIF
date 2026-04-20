/*
 * Arquivo: colisao.c
 * Propósito: oferecer helpers puros de colisão e limitação espacial.
 * Quem depende dele: core, combate e módulos futuros de projéteis e inimigos.
 */

#include "augur/regras.h"

bool colide_circulos(float centro_ax, float centro_ay, float raio_a,
                     float centro_bx, float centro_by, float raio_b)
{
    const float distancia_x = centro_ax - centro_bx;
    const float distancia_y = centro_ay - centro_by;
    const float soma_raios = raio_a + raio_b;

    return (distancia_x * distancia_x) + (distancia_y * distancia_y) <= (soma_raios * soma_raios);
}

bool colide_retangulos(float retangulo_ax, float retangulo_ay, float largura_a, float altura_a,
                       float retangulo_bx, float retangulo_by, float largura_b, float altura_b)
{
    const bool separacao_horizontal =
        (retangulo_ax + largura_a) < retangulo_bx || (retangulo_bx + largura_b) < retangulo_ax;
    const bool separacao_vertical =
        (retangulo_ay + altura_a) < retangulo_by || (retangulo_by + altura_b) < retangulo_ay;

    return !separacao_horizontal && !separacao_vertical;
}

void limitar_jogador_na_tela(Jogador* jogador, float origem_x, float origem_y,
                             float largura_area, float altura_area)
{
    const float limite_esquerdo = origem_x + jogador->raio_colisao;
    const float limite_direito = origem_x + largura_area - jogador->raio_colisao;
    const float limite_superior = origem_y + jogador->raio_colisao;
    const float limite_inferior = origem_y + altura_area - jogador->raio_colisao;

    if (jogador->posicao_x < limite_esquerdo) {
        jogador->posicao_x = limite_esquerdo;
    }

    if (jogador->posicao_x > limite_direito) {
        jogador->posicao_x = limite_direito;
    }

    if (jogador->posicao_y < limite_superior) {
        jogador->posicao_y = limite_superior;
    }

    if (jogador->posicao_y > limite_inferior) {
        jogador->posicao_y = limite_inferior;
    }
}
