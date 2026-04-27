/* ============================================================================
 * hud.h - INTERFACE DO HUD (HEADS-UP DISPLAY)
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 2
 *
 * O HUD e a camada de interface que fica sobre o jogo durante o combate:
 *   - Barra de vida do jogador
 *   - Numero da onda atual
 *   - Contador de biomassa
 *   - Icones dos dados disponiveis
 *
 * Fica como modulo separado pra nao poluir main.c com codigo de UI.
 * ========================================================================== */

#ifndef HUD_H
#define HUD_H

#include "tipos.h"

/* Desenha todos os elementos de HUD sobre o jogo. Chamada no final do
 * jogo_desenhar(), pra ficar por cima de tudo. */
void hud_desenhar_barra_vida(const EstadoJogo *ej);
void hud_desenhar_onda(const EstadoJogo *ej);
void hud_desenhar_biomassa(const EstadoJogo *ej);


#endif /* HUD_H */
