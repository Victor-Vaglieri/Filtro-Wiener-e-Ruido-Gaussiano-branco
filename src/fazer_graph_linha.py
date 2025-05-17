import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

# Função para extrair valores de uma linha da imagem
def extrair_valores_linha(imagem, linha):
    img = np.array(imagem)
    return img[linha, :, 0], img[linha, :, 1], img[linha, :, 2]  # R, G, B

# Carregar as imagens
img_original = Image.open("imagens/passaro.png")
img_ruido = Image.open("imagens/teste_gaussiano.png")
img_filtrada = Image.open("imagens/teste_wiener.png")

# Definir a linha a ser analisada (por exemplo, a linha central)
linha = img_original.height // 2

# Extrair os valores RGB para a linha escolhida
r_original, g_original, b_original = extrair_valores_linha(img_original, linha)
r_ruido, g_ruido, b_ruido = extrair_valores_linha(img_ruido, linha)
r_filtrada, g_filtrada, b_filtrada = extrair_valores_linha(img_filtrada, linha)

# Plotando os gráficos de ondas para as três imagens
fig, axs = plt.subplots(3, 1, figsize=(10, 6))

# Imagem Original
axs[0].plot(r_original, color='r', label='Vermelho')
axs[0].plot(g_original, color='g', label='Verde')
axs[0].plot(b_original, color='b', label='Azul')
axs[0].set_title("Imagem Original - Intensidade dos Pixels")
axs[0].set_xlabel("Posição dos Pixels em uma Linha")
axs[0].set_ylabel("Intensidade de Cor")
axs[0].legend()

# Imagem com Ruído
axs[1].plot(r_ruido, color='r', label='Vermelho')
axs[1].plot(g_ruido, color='g', label='Verde')
axs[1].plot(b_ruido, color='b', label='Azul')
axs[1].set_title("Imagem com Ruído - Intensidade dos Pixels")
axs[1].set_xlabel("Posição dos Pixels em uma Linha")
axs[1].set_ylabel("Intensidade de Cor")
axs[1].legend()

# Imagem Filtrada
axs[2].plot(r_filtrada, color='r', label='Vermelho')
axs[2].plot(g_filtrada, color='g', label='Verde')
axs[2].plot(b_filtrada, color='b', label='Azul')
axs[2].set_title("Imagem Filtrada - Intensidade dos Pixels")
axs[2].set_xlabel("Posição dos Pixels em uma Linha")
axs[2].set_ylabel("Intensidade de Cor")
axs[2].legend()

# Ajuste para a visualização dos gráficos
plt.tight_layout()
plt.show()
