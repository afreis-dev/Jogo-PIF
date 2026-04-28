/* ============================================================================
 * inimigos.h - INTERFACE DO MODULO DE INIMIGOS
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3 (Luisa)
 *
 * Cuida de tudo sobre inimigos:
 *   - Mover em direcao ao jogador (IA simples: "andar pra onde o jogador ta").
 *   - Remover inimigos marcados como "vivo = false" pela colisao.
 *   - Liberar (free) os nos ao fim do jogo.
 *
 * COMO CRIAR UM INIMIGO:
 *   1. malloc(sizeof(InimigoNo))
 *   2. Preencher: posicao (borda da tela), vida, velocidade, tipo.
 *   3. Inserir na cabeca: novo->proximo = ej->inimigos_cabeca;
 *                          ej->inimigos_cabeca = novo;
 *
 * O modulo colisao.c marca "vivo = false". Este modulo remove da lista.
 *
 * ATENCAO: nesta branch test/sandbox-integracao o modulo esta IMPLEMENTADO
 * de forma minima (chase-AI) so pra validar HUD/colisao/biomassa. Ele NAO
 * deve voltar pra main - eh trabalho da Luisa fazer a versao oficial.
 * ========================================================================== */

#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "tipos.h"

/* Move cada inimigo em direcao ao jogador, remove os mortos. */
void inimigos_atualizar(EstadoJogo *ej);

/* Desenha cada inimigo na tela. */
void inimigos_desenhar(const EstadoJogo *ej);

/* Libera todos os nos da lista. */
void inimigos_liberar_tudo(EstadoJogo *ej);

/* SANDBOX: spawna um inimigo em uma posicao especifica (coord de mundo).
 * Usado pela onda nesta branch de testes. */
void inimigos_spawnar_em(EstadoJogo *ej, Vector2 posicao);

#endif /* INIMIGOS_H */
