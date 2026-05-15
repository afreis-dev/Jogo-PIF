/* ============================================================================
 * colisao.c - IMPLEMENTAÇÃO DO SISTEMA DE COLISÃO
 * ============================================================================
 *
 * Colisão em jogos 2D se resume a: "essas duas formas estão se sobrepondo?".
 *
 * TRUQUE DE PERFORMANCE IMPORTANTE:
 *   Pra saber se dois círculos se tocam, o cálculo direto seria
 *     distancia = sqrt((ax-bx)^2 + (ay-by)^2);
 *     se distancia <= ra + rb, tocam.
 *   Mas sqrt é caro. Podemos comparar o QUADRADO da distância com o QUADRADO
 *   dos raios somados — dá o mesmo resultado sem o sqrt.
 *
 * COMPLEXIDADE:
 *   colisao_verificar_tudo é O(N×M) onde N=magias, M=inimigos. Pra ~100
 *   inimigos × 200 projéteis = 20000 checagens por frame. A 60fps isso é
 *   ~1.2M/s, ainda tranquilo pra CPU moderna. Se virar gargalo, no futuro
 *   dava pra usar grid espacial, mas por ora KISS.
 * ========================================================================== */

#include "colisao.h"
#include "jogador.h"
#include <math.h>


/* ----- PRIMITIVA: CÍRCULO × CÍRCULO ----- */
bool colisao_circulo_circulo(Vector2 a, float ra, Vector2 b, float rb) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dist2 = dx * dx + dy * dy;     /* distância ao quadrado */
    float soma_raios = ra + rb;
    return dist2 <= soma_raios * soma_raios;
}


/* ----- PRIMITIVA: CAIXA × CAIXA (AABB - Axis-Aligned Bounding Box) ----- */
bool colisao_caixa_caixa(Rectangle a, Rectangle b) {
    /* Se uma caixa termina antes da outra começar em qualquer eixo,
     * não há colisão. Todos os 4 testes precisam passar. */
    return a.x < b.x + b.width  &&
           a.x + a.width  > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}


/* ----- PRIMITIVA: CÍRCULO × CAIXA -----
 * Acha o ponto da caixa mais próximo do círculo (com clamp) e checa
 * distância daquele ponto até o centro do círculo.
 * fminf/fmaxf são da math.h — versões em float de min/max. */
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
 * VERIFICAÇÃO DE ALTO NÍVEL
 * --------------------------------------------------------------------------
 * Percorre as LISTAS ENCADEADAS do estado do jogo e resolve colisões.
 *
 * Caminhar em lista encadeada: começa em "cabeça" e segue "próximo" até NULL.
 *   for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo)
 *
 * Não removemos nós aqui — só marcamos "vivo = false". A remoção efetiva
 * (com free) fica com quem cuida das listas: Dev 3 nos módulos magias.c e
 * inimigos.c. Separar essas responsabilidades evita bugs de "uso após free".
 * ========================================================================== */
void colisao_verificar_tudo(EstadoJogo *ej) {
    /* ---- 1. Jogador vs Inimigos ---- */
    for (InimigoNo *ino = ej->inimigos_cabeca; ino != NULL; ino = ino->proximo) {
        if (!ino->dados.vivo) continue;   /* pula inimigos já mortos */

        /* Vetor do inimigo para o jogador. Distância ao quadrado pra evitar
         * sqrt no caso de "não tocou". */
        float dx = ej->jogador.posicao.x - ino->dados.posicao.x;
        float dy = ej->jogador.posicao.y - ino->dados.posicao.y;
        float distancia2 = dx * dx + dy * dy;
        float soma_raios = ej->jogador.raio + ino->dados.raio;

        if (distancia2 <= soma_raios * soma_raios) {
            /* Dano por contato. Por enquanto puro — no jogo final, seria
             * legal aplicar knockback ou cooldown de hit aqui. */
            jogador_sofrer_dano(&ej->jogador, (int)ino->dados.dano);

            /* COLISÃO FÍSICA: empurra o jogador pra fora do inimigo pela
             * quantidade do "overlap" (quanto eles estão se sobrepondo).
             * Resultado: o inimigo bloqueia o jogador em vez de ser
             * atravessado.
             *
             * Caso degenerado (centros exatamente sobrepostos → distância=0):
             * empurra pra direita por uma quantidade fixa, só pra evitar a
             * divisão por zero. */
            if (distancia2 > 0.0001f) {
                float distancia = sqrtf(distancia2);
                float overlap   = soma_raios - distancia;
                ej->jogador.posicao.x += (dx / distancia) * overlap;
                ej->jogador.posicao.y += (dy / distancia) * overlap;
            } else {
                ej->jogador.posicao.x += soma_raios;
            }
        }
    }

    /* ---- 2. Magias (projéteis) vs Inimigos ---- */
    for (MagiaNo *mno = ej->magias_cabeca; mno != NULL; mno = mno->proxima) {
        if (!mno->dados.viva) continue;

        for (InimigoNo *ino = ej->inimigos_cabeca;
             ino != NULL;
             ino = ino->proximo) {

            if (!ino->dados.vivo) continue;

            /* Raio do projétil vem da struct Magia, preenchido pela engine
             * (magias.c) a partir de PARAMETROS_MAGIA[elemento].raio_projetil. */
            if (colisao_circulo_circulo(mno->dados.posicao, mno->dados.raio,
                                         ino->dados.posicao, ino->dados.raio)) {
                ino->dados.vida -= (int)mno->dados.dano;
                mno->dados.viva = false;    /* projétil some no hit */

                if (ino->dados.vida <= 0) {
                    ino->dados.vivo = false;
                    ej->jogador.biomassa += ino->dados.recompensa_biomassa;
                }
                /* break opcional aqui se projétil só pode atingir 1 inimigo.
                 * Omitido pra deixar brecha pra piercing shots. */
            }
        }
    }
}
