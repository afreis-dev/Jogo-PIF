/* ============================================================================
 * cartas.c - STUB DO SISTEMA DE CARTAS
 * ============================================================================
 *
 * TODO: Dev 2 (Sofia) implementar este arquivo.
 *
 * Sugestão de implementação de cartas_gerar_escolhas:
 *   for (int i = 0; i < CARTAS_POR_ESCOLHA; i++) {
 *       ej->escolhas_upgrade[i].tipo    = rand() % CARTA_TOTAL;
 *       ej->escolhas_upgrade[i].valor   = 10 + rand() % 20;
 *       ej->escolhas_upgrade[i].raridade = rand() % 3;
 *   }
 *
 * Sugestão de implementação de cartas_aplicar:
 *   Carta c = ej->escolhas_upgrade[indice_escolhido];
 *   switch (c.tipo) {
 *       case CARTA_DANO_UP:       ... aumentar dano das magias ...   break;
 *       case CARTA_VIDA_UP:       ej->jogador.vida_maxima += c.valor; break;
 *       case CARTA_VELOCIDADE_UP: ej->jogador.velocidade_movimento += c.valor; break;
 *       ...
 *   }
 * ========================================================================== */

#include "cartas.h"

void cartas_gerar_escolhas(EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 2 — sortear 3 cartas em ej->escolhas_upgrade */
}

void cartas_aplicar(EstadoJogo *ej, int indice_escolhido) {
    (void)ej;
    (void)indice_escolhido; /* TODO: Dev 2 — aplicar efeito da carta no jogador */
}

void cartas_desenhar_ui(const EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 2 — desenhar 3 caixas com nome e descrição de cada carta */
}
