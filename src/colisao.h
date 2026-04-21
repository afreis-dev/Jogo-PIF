/* ============================================================================
 * colisao.h - INTERFACE DO SISTEMA DE COLISAO
 * ============================================================================
 *
 * Dois niveis de API:
 *
 * 1. Primitivas GEOMETRICAS (colisao_* com parametros de forma):
 *    Funcoes pequenas e reutilizaveis que qualquer modulo pode chamar.
 *    Ex.: "essas duas bolinhas estao se tocando?".
 *
 * 2. Verificacao ALTO NIVEL (colisao_verificar_tudo):
 *    Percorre listas de magias e inimigos do estado do jogo, aplica dano e
 *    marca entidades como mortas. Chamada uma vez por frame pelo main.
 * ========================================================================== */

#ifndef COLISAO_H
#define COLISAO_H

#include "tipos.h"

/* --- Primitivas geometricas --- */

/* Dois circulos se tocam? (centros a,b e raios ra,rb) */
bool colisao_circulo_circulo(Vector2 a, float ra, Vector2 b, float rb);

/* Duas caixas se tocam? (formato Rectangle do Raylib: x, y, width, height) */
bool colisao_caixa_caixa(Rectangle a, Rectangle b);

/* Um circulo e uma caixa se tocam? Util pra HUD clickable, botoes, etc. */
bool colisao_circulo_caixa(Vector2 c, float r, Rectangle caixa);

/* --- Alto nivel --- */

/* Percorre TODAS as listas encadeadas do jogo e resolve colisoes:
 *   - Jogador x inimigos (aplica dano no jogador)
 *   - Magias x inimigos (aplica dano no inimigo, marca magia como morta)
 * Deve ser chamada UMA vez por frame apos atualizar as posicoes. */
void colisao_verificar_tudo(EstadoJogo *ej);

#endif /* COLISAO_H */
