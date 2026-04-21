/* ============================================================================
 * colisao.c - IMPLEMENTACAO DO SISTEMA DE COLISAO
 * ============================================================================
 *
 * Colisao em jogos 2D se resume a: "essas duas formas estao se sobrepondo?".
 *
 * TRUQUE DE PERFORMANCE IMPORTANTE:
 *   Pra saber se dois circulos se tocam, o calculo direto seria
 *     distancia = sqrt((ax-bx)^2 + (ay-by)^2);
 *     se distancia <= ra + rb, tocam.
 *   Mas sqrt e caro. Podemos comparar o QUADRADO da distancia com o QUADRADO
 *   dos raios somados - da o mesmo resultado sem o sqrt.
 *
 * COMPLEXIDADE:
 *   colisao_verificar_tudo e O(N*M) onde N=magias, M=inimigos. Pra ~100
 *   inimigos x 200 projeteis = 20000 checagens por frame. A 60fps isso e
 *   ~1.2M/s, ainda tranquilo pra CPU moderna. Se virar gargalo, no futuro
 *   dava pra usar grid espacial, mas por ora KISS.
 * ========================================================================== */

#include "colisao.h"
#include "jogador.h"
#include <math.h>


/* ----- PRIMITIVA: CIRCULO x CIRCULO ----- */
bool colisao_circulo_circulo(Vector2 a, float ra, Vector2 b, float rb) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dist2 = dx * dx + dy * dy;     /* distancia ao quadrado */
    float soma_raios = ra + rb;
    return dist2 <= soma_raios * soma_raios;
}


/* ----- PRIMITIVA: CAIXA x CAIXA (AABB - Axis-Aligned Bounding Box) ----- */
bool colisao_caixa_caixa(Rectangle a, Rectangle b) {
    /* Se uma caixa termina antes da outra comecar em qualquer eixo,
     * nao ha colisao. Todos os 4 testes precisam passar. */
    return a.x < b.x + b.width  &&
           a.x + a.width  > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}


/* ----- PRIMITIVA: CIRCULO x CAIXA -----
 * Acha o ponto da caixa mais proximo do circulo (com clamp) e checa
 * distancia daquele ponto ate o centro do circulo.
 * fminf/fmaxf sao da math.h - versoes em float de min/max. */
bool colisao_circulo_caixa(Vector2 c, float r, Rectangle caixa) {
    float mais_proximo_x = fmaxf(caixa.x,
                                  fminf(c.x, caixa.x + caixa.width));
    float mais_proximo_y = fmaxf(caixa.y,
                                  fminf(c.y, caixa.y + caixa.height));
    float dx = c.x - mais_proximo_x;
    float dy = c.y - mais_proximo_y;
    return (dx * dx + dy * dy) <= r * r;
}


/* ============================================================================
 * VERIFICACAO DE ALTO NIVEL
 * --------------------------------------------------------------------------
 * Percorre as LISTAS ENCADEADAS do estado do jogo e resolve colisoes.
 *
 * Caminhar em lista encadeada: comeca em "cabeca" e segue "proximo" ate NULL.
 *   for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo)
 *
 * Nao removemos nos aqui - so marcamos "vivo = false". A remocao efetiva
 * (com free) fica com quem cuida das listas: Dev 3 nos modulos magias.c e
 * inimigos.c. Separar essas responsabilidades evita bugs de "uso apos free".
 * ========================================================================== */
void colisao_verificar_tudo(EstadoJogo *ej) {
    /* ---- 1. Jogador vs Inimigos ---- */
    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;   /* pula inimigos ja mortos */

        if (colisao_circulo_circulo(ej->jogador.posicao, ej->jogador.raio,
                                     ino->dados.posicao, ino->dados.raio)) {
            jogador_sofrer_dano(&ej->jogador, (int)ino->dados.dano);
            /* Nota: no jogo final, seria legal aplicar knockback ou
             * cooldown de hit aqui. Por ora, dano puro. */
        }
    }

    /* ---- 2. Magias (projeteis) vs Inimigos ---- */
    for (MagiaNo *mno = ej->magias_cabeca; mno != NULL; mno = mno->proxima) {
        if (!mno->dados.viva) continue;

        for (InimigoNo *ino = ej->inimigos_cabeca;
             ino != NULL;
             ino = ino->proximo) {

            if (!ino->dados.vivo) continue;

            /* Raio do projetil e fixo em 6 pixels por enquanto.
             * Dev 3 pode mudar isso adicionando "raio" na struct Magia. */
            if (colisao_circulo_circulo(mno->dados.posicao, 6.0f,
                                         ino->dados.posicao, ino->dados.raio)) {
                ino->dados.vida -= (int)mno->dados.dano;
                mno->dados.viva = false;    /* projetil some no hit */

                if (ino->dados.vida <= 0) {
                    ino->dados.vivo = false;
                    ej->jogador.biomassa += ino->dados.recompensa_biomassa;
                }
                /* break opcional aqui se projetil so pode atingir 1 inimigo.
                 * Omitido pra deixar brecha pra piercing shots. */
            }
        }
    }
}
