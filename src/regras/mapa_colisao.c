/*
 * Arquivo: mapa_colisao.c
 * Propósito: preparar uma matriz base de colisão para a arena inicial.
 * Quem depende dele: core e futuros módulos de wave, inimigos e obstáculos.
 */

#include "augur/regras.h"

void preencher_mapa_colisao_base(int mapa[ALTURA_MAPA_COLISAO][LARGURA_MAPA_COLISAO])
{
    for (int linha = 0; linha < ALTURA_MAPA_COLISAO; ++linha) {
        for (int coluna = 0; coluna < LARGURA_MAPA_COLISAO; ++coluna) {
            const bool esta_na_borda =
                linha == 0 || linha == ALTURA_MAPA_COLISAO - 1 ||
                coluna == 0 || coluna == LARGURA_MAPA_COLISAO - 1;

            /*
             * Nesta base, a matriz representa apenas as bordas da arena.
             * Obstáculos internos entram depois, quando wave e inimigos crescerem.
             */
            mapa[linha][coluna] = esta_na_borda ? 1 : 0;
        }
    }
}
