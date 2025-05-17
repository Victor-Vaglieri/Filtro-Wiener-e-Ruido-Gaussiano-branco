# Remoção de Ruído Branco Gaussiano com Filtro de Wiener

Este repositório contém um tutorial prático e teórico sobre a aplicação de ruído branco gaussiano a imagens e sua posterior remoção utilizando o Filtro de Wiener.

---
## Ruído Branco Gaussiano
O ruído branco gaussiano aditivo (AWGN) é um modelo probabilístico amplamente utilizado em sistemas de comunicação e processamento de imagens digitais. Ele é chamado de "branco" porque apresenta distribuição espectral plana, ou seja, sua energia é distribuída igualmente entre todas as frequências (GONZALEZ; WOODS, 2010). É dito "gaussiano" porque os valores de intensidade do ruído seguem uma distribuição normal (curva de Gauss), caracterizada por uma média ($μ$) e e uma variância ($σ^2$).

### Ruído em Imagens RGB
Na prática, especialmente em imagens coloridas no formato RGB, o ruído branco gaussiano pode ser aplicado individualmente a cada canal de cor (vermelho, verde e azul). Em vez de interferir apenas na intensidade luminosa (como em imagens em tons de cinza), o ruído afeta diretamente os valores dos pixels em cada um dos três canais, gerando artefatos visuais coloridos. Isso simula situações reais em que sensores de câmeras digitais captam interferência elétrica ou térmica de maneira não uniforme, resultando em ruído perceptível em cores diferentes (LIM, 1990).

### O ruído aditivo é comum em:

+ Sistemas de aquisição de imagem, como scanners e câmeras digitais;
+ Transmissão de imagens por canais sujeitos a interferências (ex: rede sem fio);
+ Equipamentos sujeitos a ruído térmico, como sensores CCD e CMOS (PRATT, 2007).

Do ponto de vista matemático, se $I(x,y)$ representa a imagem original, $n(x,y)$ é o ruído gaussinao com distribuição $N(0,\sigma^2)$ e $n(x,y)$ representa um valor gerado aleatoriamente pelo ruído, a imagem ruidosa é dada por:

$I_r(x,y) = I(x,y) + n(x,y)$

No caso de imagens RGB, a fórmula se aplica separadamente aos canais R, G e B.



---

## Filtro de Wiener

O filtro de Wiener é uma técnica clássica de restauração de imagens ruidosas, projetado para minimizar o erro quadrático médio (MSE) entre a imagem restaurada e a imagem original. Ele parte do princípio de que a imagem degradada pode ser modelada como a soma da imagem original com um ruído aditivo, como o ruido gaussiano de média zero.
O filtro de Wiener trabalha localmente, isto é, calcula estatísticas (média e variância) dentro de uma vizinhança em torno de cada pixel para estimar o valor mais provável do sinal original naquele ponto. O valor restaurado $Î(x,y)$ é obtido por meio da seguinte equação:

$Î(x,y) = μ+((σ^2σ_n^2)/σ^2)*(I_r(x,y)-μ)$

Onde:
+ μ é a média local da vizinhança;
+ $σ^2# é a variância local;
+ $σ_n^2 é a variância estimada do ruído (normalmente fornecida como parâmetro);
+ $I_r(x,y)$ é o valor do pixel observado com ruído.

Essa fórmula permite que o filtro atue de maneira adaptativa: quando a variância local é alta, indicando detalhes ou bordas, o filtro preserva melhor o contraste local; quando a variância é baixa, indicando áreas homogêneas, o filtro atua de forma mais agressiva para suavizar o ruído (GONZALEZ; WOODS, 2010).

A principal vantagem do filtro de Wiener sobre filtros lineares simples (como a média) é sua capacidade de equilibrar a redução de ruído com a preservação de detalhes, tornando-o ideal para casos em que o ruído é do tipo gaussiano branco, como ocorre comumente em sensores digitais (JAIN, 1989; LIM, 1990).

### Filtro em Imagens RGB

No caso de imagens RGB, como é o contexto deste trabalho, o filtro é aplicado separadamente em cada canal de cor (vermelho, verde e azul). Assim, a suavização respeita a estrutura de cor da imagem, reduzindo o ruído em cada componente de forma independente.

# Referências
GONZALEZ, R. C.; WOODS, R. E. Digital Image Processing. 3. ed. Upper Saddle River: Pearson Prentice Hall, 2010.

JAIN, A. K. Fundamentals of Digital Image Processing. Englewood Cliffs: Prentice Hall, 1989.

KAY, S. M. Fundamentals of Statistical Signal Processing: Estimation Theory. Englewood Cliffs: Prentice Hall, 1993.

LIM, J. S. Two-Dimensional Signal and Image Processing. Englewood Cliffs: Prentice-Hall, 1990.

PRATT, W. K. Digital Image Processing: PIKS Scientific Inside. 4. ed. Hoboken: Wiley-Interscience, 2007.

