/* ============================================================================
 * obstaculos.h - INTERFACE DO MODULO DE OBSTACULOS
 * ============================================================================
 *
 * RESPONSABILIDADE: Dev 3 (Luisa)
 *
 * Cuida dos objetos fixos do mapa - arvores, pedras, e o que mais a gente
 * decidir adicionar (toco, arbusto, runa). Eles BLOQUEIAM o jogador e os
 * inimigos (push-out fisico) mas nao causam dano.
 *
 * IDEIA:
 *   - O layout do mapa eh gerado UMA UNICA VEZ no inicio da run, a partir
 *     da seed da profecia. Mesma seed = mesmo mapa, sempre. Isso ajuda no
 *     debug e permite jogadores compartilharem mapas legais.
 *   - Como sao FIXOS durante o combate (nao spawnam nem somem), nao precisa
 *     de lista encadeada: usamos o array obstaculos[] em EstadoJogo.
 *
 * CONTRATO COM A COLISAO:
 *   - colisao.c (Dev 1) ja resolve jogador-vs-inimigos e magia-vs-inimigos.
 *   - As funcoes obstaculos_resolver_* sao chamadas APOS colisao_verificar_tudo,
 *     pra garantir que o jogador/inimigo termine o frame fora de qualquer
 *     obstaculo, mesmo que outra colisao tenha tentado empurrar pra dentro.
 *
 * REGRA: Se voce mudar a geometria (raios, distribuicao, area do mapa),
 * teste com seeds variadas pra confirmar que o jogador nunca nasce dentro
 * de um obstaculo.
 * ========================================================================== */

#ifndef OBSTACULOS_H
#define OBSTACULOS_H

#include "tipos.h"

/* Gera o layout aleatorio de obstaculos a partir da seed.
 * Substitui qualquer layout anterior. Chamada quando uma run comeca. */
void obstaculos_gerar(EstadoJogo *ej, unsigned int seed);

/* Desenha todos os obstaculos. DEVE ser chamado DENTRO de BeginMode2D
 * (eles vivem em coordenadas de mundo, nao de tela). */
void obstaculos_desenhar(const EstadoJogo *ej);

/* Empurra o jogador pra fora de qualquer obstaculo que ele esteja sobrepondo.
 * Chamada apos colisao_verificar_tudo, no fim do update do combate. */
void obstaculos_resolver_jogador(EstadoJogo *ej);

/* Mesmo padrao, mas pra cada inimigo da lista encadeada. */
void obstaculos_resolver_inimigos(EstadoJogo *ej);

#endif /* OBSTACULOS_H */
