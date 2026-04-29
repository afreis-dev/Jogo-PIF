/* ============================================================================
 * cartas.c - STUB DO SISTEMA DE CARTAS
 * ============================================================================
 *
 * TODO: Dev 2 implementar este arquivo.
 *
 * Sugestao de implementacao de cartas_gerar_escolhas:
 *   for (int i = 0; i < CARTAS_POR_ESCOLHA; i++) {
 *       ej->escolhas_upgrade[i].tipo    = rand() % CARTA_TOTAL;
 *       ej->escolhas_upgrade[i].valor   = 10 + rand() % 20;
 *       ej->escolhas_upgrade[i].raridade = rand() % 3;
 *   }
 *
 * Sugestao de implementacao de cartas_aplicar:
 *   Carta c = ej->escolhas_upgrade[indice_escolhido];
 *   switch (c.tipo) {
 *       case CARTA_DANO_UP:       ... aumentar dano das magias ...   break;
 *       case CARTA_VIDA_UP:       ej->jogador.vida_maxima += c.valor; break;
 *       case CARTA_VELOCIDADE_UP: ej->jogador.velocidade_movimento += c.valor; break;
 *       ...
 *   }
 * ========================================================================== */

#include "cartas.h"

#define INICIO_CARTA_X  160
#define INICIO_CARTA_Y  200
#define LARGURA_CARTA   240
#define ALTURA_CARTA    320
#define GAP_CARTAS      360



void cartas_gerar_escolhas(EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 2 - sortear 3 cartas em ej->escolhas_upgrade */
}

void cartas_aplicar(EstadoJogo *ej, int indice_escolhido) {
    (void)ej;
    (void)indice_escolhido; /* TODO: Dev 2 - aplicar efeito da carta no jogador */
}

void cartas_desenhar_ui(const EstadoJogo *ej) {

    DrawRectangle(INICIO_CARTA_X - 2 , INICIO_CARTA_Y - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    DrawRectangle(INICIO_CARTA_X , INICIO_CARTA_Y , LARGURA_CARTA, ALTURA_CARTA, BLACK);
    

    DrawRectangle(INICIO_CARTA_X + GAP_CARTAS - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    DrawRectangle(INICIO_CARTA_X + GAP_CARTAS , (INICIO_CARTA_Y), LARGURA_CARTA, ALTURA_CARTA, BLACK);

    DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) - 2 , (INICIO_CARTA_Y) - 2, LARGURA_CARTA + 4, ALTURA_CARTA + 4, BLUE);
    DrawRectangle(INICIO_CARTA_X + (GAP_CARTAS * 2) , (INICIO_CARTA_Y), LARGURA_CARTA, ALTURA_CARTA, BLACK);

    (void)ej; 
}
