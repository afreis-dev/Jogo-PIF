/* ============================================================================
 * jogador.h - INTERFACE DO MODULO JOGADOR
 * ============================================================================
 *
 * O que e um header (.h)?
 *   E o "cartao de visita" de um modulo. Ele DECLARA as funcoes publicas
 *   (protótipos), mas NAO implementa. A implementacao fica no .c.
 *
 * Por que separar?
 *   - Outros arquivos so precisam incluir o .h pra usar as funcoes.
 *   - O compilador checa o tipo dos argumentos na hora da chamada.
 *   - Recompila mais rapido (mudar .c nao obriga recompilar quem usa).
 * ========================================================================== */

#ifndef JOGADOR_H
#define JOGADOR_H

/* #include-guard acima (#ifndef/#define/#endif):
 * Garante que o header so seja lido UMA vez, mesmo se for incluido por
 * varios arquivos. Sem isso, daria erro de "redefinicao" de tipos. */

#include "tipos.h"

/* --- Protótipos publicos --- */

/* Prepara o jogador com valores padrao (posicao, HP, velocidade). */
void jogador_inicializar(Jogador *j);

/* Le WASD e move o jogador. Recebe delta_tempo pra movimento suave. */
void jogador_atualizar(Jogador *j, float delta_tempo);

/* Desenha o jogador na tela. "const" = funcao nao modifica o jogador. */
void jogador_desenhar(const Jogador *j);

/* Aplica dano ao jogador. Main chama isso quando colisao detecta hit. */
void jogador_sofrer_dano(Jogador *j, int quantidade);

#endif /* JOGADOR_H */
