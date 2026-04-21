/* ============================================================================
 * hud.c - STUB DO HUD
 * ============================================================================
 *
 * TODO: Dev 2 implementar este arquivo.
 *
 * Sugestao de implementacao de hud_desenhar:
 *
 *   // Barra de vida (canto superior esquerdo)
 *   DrawRectangle(10, 10, 200, 20, DARKGRAY);
 *   int largura_vida = (int)(200 * ej->jogador.vida / ej->jogador.vida_maxima);
 *   DrawRectangle(10, 10, largura_vida, 20, RED);
 *   char buf[32];
 *   snprintf(buf, 32, "HP: %d/%d", ej->jogador.vida, ej->jogador.vida_maxima);
 *   DrawText(buf, 14, 12, 16, WHITE);
 *
 *   // Numero da onda (topo centro)
 *   snprintf(buf, 32, "Onda %d", ej->onda_atual.numero);
 *   DrawText(buf, LARGURA_TELA/2 - 40, 10, 22, GOLD);
 *
 *   // Biomassa (canto superior direito)
 *   snprintf(buf, 32, "Biomassa: %d", ej->jogador.biomassa);
 *   DrawText(buf, LARGURA_TELA - 160, 10, 18, GREEN);
 * ========================================================================== */

#include "hud.h"

void hud_desenhar(const EstadoJogo *ej) {
    (void)ej; /* TODO: Dev 2 - implementar conforme sugestao acima */
}
