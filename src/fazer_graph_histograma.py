import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

# Carregar as imagens
img_original = Image.open("imagens/passaro.png")
img_ruido = Image.open("imagens/teste_gaussiano.png")
img_filtrada = Image.open("imagens/teste_wiener.png")

# Função para extrair os valores RGB da imagem
def extrair_valores_rgb(imagem):
    img = np.array(imagem)
    r = img[:, :, 0].flatten()
    g = img[:, :, 1].flatten()
    b = img[:, :, 2].flatten()
    return r, g, b

# Extrair os valores RGB das imagens
r_original, g_original, b_original = extrair_valores_rgb(img_original)
r_ruido, g_ruido, b_ruido = extrair_valores_rgb(img_ruido)
r_filtrada, g_filtrada, b_filtrada = extrair_valores_rgb(img_filtrada)

# Plotando os histogramas
fig, axs = plt.subplots(3, 3, figsize=(15, 10))

# Imagem Original
axs[0, 0].hist(r_original, bins=256, color='r', alpha=0.7)
axs[0, 0].set_title("Histograma R - Original")
axs[0, 0].set_xlabel("Intensidade de Cor")
axs[0, 0].set_ylabel("Quantidade")

axs[1, 0].hist(g_original, bins=256, color='g', alpha=0.7)
axs[1, 0].set_title("Histograma G - Original")
axs[1, 0].set_xlabel("Intensidade de Cor")
axs[1, 0].set_ylabel("Quantidade")

axs[2, 0].hist(b_original, bins=256, color='b', alpha=0.7)
axs[2, 0].set_title("Histograma B - Original")
axs[2, 0].set_xlabel("Intensidade de Cor")
axs[2, 0].set_ylabel("Quantidade")

# Imagem com Ruído
axs[0, 1].hist(r_ruido, bins=256, color='r', alpha=0.7)
axs[0, 1].set_title("Histograma R - Com Ruído")
axs[0, 1].set_xlabel("Intensidade de Cor")
axs[0, 1].set_ylabel("Quantidade")

axs[1, 1].hist(g_ruido, bins=256, color='g', alpha=0.7)
axs[1, 1].set_title("Histograma G - Com Ruído")
axs[1, 1].set_xlabel("Intensidade de Cor")
axs[1, 1].set_ylabel("Quantidade")

axs[2, 1].hist(b_ruido, bins=256, color='b', alpha=0.7)
axs[2, 1].set_title("Histograma B - Com Ruído")
axs[2, 1].set_xlabel("Intensidade de Cor")
axs[2, 1].set_ylabel("Quantidade")

# Imagem Filtrada
axs[0, 2].hist(r_filtrada, bins=256, color='r', alpha=0.7)
axs[0, 2].set_title("Histograma R - Filtrada")
axs[0, 2].set_xlabel("Intensidade de Cor")
axs[0, 2].set_ylabel("Quantidade")

axs[1, 2].hist(g_filtrada, bins=256, color='g', alpha=0.7)
axs[1, 2].set_title("Histograma G - Filtrada")
axs[1, 2].set_xlabel("Intensidade de Cor")
axs[1, 2].set_ylabel("Quantidade")

axs[2, 2].hist(b_filtrada, bins=256, color='b', alpha=0.7)
axs[2, 2].set_title("Histograma B - Filtrada")
axs[2, 2].set_xlabel("Intensidade de Cor")
axs[2, 2].set_ylabel("Quantidade")

# Ajuste para a visualização dos gráficos
plt.tight_layout()
plt.show()
