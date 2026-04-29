/* ============================================================================
 * profecia.h - INTERFACE DO GERADOR PROCEDURAL DE PROFECIAS
 * ============================================================================
 *
 * Uma profecia e composta por 3 modificadores do tipo
 *   [Elemento] + [Condicao] + [Efeito].
 *
 * Ex.: "Fogo | Ao matar -> Explosao em area"
 *
 * O gerador recebe uma SEED (numero unsigned int) e produz SEMPRE a mesma
 * profecia pra aquela seed - isso e chamado de "determinismo procedural".
 * Permite debug reprodutivel e compartilhar runs com amigos ("joga a seed
 * 4821, a profecia e absurda").
 * ========================================================================== */

#ifndef PROFECIA_H
#define PROFECIA_H

#include "tipos.h"

/* Gera uma profecia completa a partir da seed.
 * Escreve os 3 modificadores em p->mods e guarda a seed em p->seed. */
void profecia_gerar(Profecia *p, unsigned int seed);

/* Desenha a profecia na tela (usado no estado REVELACAO_PROFECIA). */
void profecia_desenhar(const Profecia *p);

/* Funcoes de lookup: converte enum em texto legivel.
 * Uteis pra outros modulos que queiram mostrar nomes (HUD, debug). */
const char *elemento_nome(Elemento e);
const char *condicao_nome(Condicao c);
const char *efeito_nome(Efeito e);

#endif /* PROFECIA_H */
