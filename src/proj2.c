
// integrantes

// João Pedro Souza - 10400720
// David Pessoa Varão - 10402647
// Pedro Nomura Picchioni - 10401616
// Victor Vaglieri de Oliveira - 10400787

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// ----------------------------------------------------------------------------------------------------------------------------------------------
static const char *CAMINHO_IMAGEM = "imagens/passaro.png";
static const char *CAMINHO_WIENER = "imagens/teste_wiener.png";
static const char *CAMINHO_GAUSSIANO = "imagens/teste_gaussiano.png";
static const float RUIDO = 20.0f; // desvio padrao do ruido gaussiano
static const float MEDIA = 0.0f; // media do ruido gaussiano
static const float FILTRO = RUIDO*RUIDO; // variancia do filtro de wiener
static const int AREA_FILTRO = 5; // area do filtro de mediana (deve ser impar)
// ----------------------------------------------------------------------------------------------------------------------------------------------

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Surface *surface = NULL;
static SDL_Texture *texture = NULL;
static SDL_FRect textureRect = {.x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f};

// Gera número com distribuição normal padrão (Box-Muller)
float gerarRuidoGaussiano(float media, float desvio) {
    float u1 = (rand() + 1.0f) / (RAND_MAX + 1.0f);
    float u2 = (rand() + 1.0f) / (RAND_MAX + 1.0f);
    float z = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * M_PI * u2);
    return z * desvio + media;
}

// Aplica ruído gaussiano branco à imagem
void applyGaussianNoise(SDL_Surface *surface, float media, float desvio) {
    int largura = surface->w;
    int altura = surface->h;

    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            Uint32 pixel = *((Uint32 *)surface->pixels + y * largura + x);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, format, NULL, &r, &g, &b);

            float novoR = r + gerarRuidoGaussiano(media, desvio);
            float novoG = g + gerarRuidoGaussiano(media, desvio);
            float novoB = b + gerarRuidoGaussiano(media, desvio);

            // Clamping
            r = (Uint8)(novoR < 0 ? 0 : (novoR > 255 ? 255 : novoR));
            g = (Uint8)(novoG < 0 ? 0 : (novoG > 255 ? 255 : novoG));
            b = (Uint8)(novoB < 0 ? 0 : (novoB > 255 ? 255 : novoB));

            *((Uint32 *)surface->pixels + y * largura + x) = SDL_MapRGB(format,NULL, r, g, b);
        }
    }
}

void applyWienerFilterRGB(SDL_Surface *surface, float noiseVariance) {
    int largura = surface->w;
    int altura = surface->h;

    SDL_Surface *copySurface = SDL_ConvertSurface(surface, surface->format);
    if (!copySurface){
        SDL_Log("Erro ao criar superfície de cópia: %s\n", SDL_GetError());
        return;
    }

    int meioFiltro = AREA_FILTRO / 2;
    int totalPixels = AREA_FILTRO * AREA_FILTRO;
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {

            float somaR = 0, somaG = 0, somaB = 0;
            float somaR2 = 0, somaG2 = 0, somaB2 = 0;

            for (int dy = -meioFiltro; dy <= meioFiltro; dy++) {
                for (int dx = -meioFiltro; dx <= meioFiltro; dx++) {
                    int rx = x + dx;
                    int ry = y + dy;

                    // Reflexão nas bordas
                    if (rx < 0) rx = -rx;
                    if (ry < 0) ry = -ry;
                    if (rx >= largura) rx = 2 * largura - rx - 2;
                    if (ry >= altura) ry = 2 * altura - ry - 2;

                    Uint32 pixel = *((Uint32 *)surface->pixels + ry * largura + rx);
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, format,NULL, &r, &g, &b);

                    somaR += r;
                    somaG += g;
                    somaB += b;

                    somaR2 += r * r;
                    somaG2 += g * g;
                    somaB2 += b * b;
                }
            }

            float mediaR = somaR / totalPixels;
            float mediaG = somaG / totalPixels;
            float mediaB = somaB / totalPixels;

            float varR = (somaR2 / totalPixels) - (mediaR * mediaR);
            float varG = (somaG2 / totalPixels) - (mediaG * mediaG);
            float varB = (somaB2 / totalPixels) - (mediaB * mediaB);
            Uint32 pixelOrig = *((Uint32 *)copySurface->pixels + y * largura + x);
            //Uint32 pixelOrig = *((Uint32 *)surface->pixels + y * largura + x);
            Uint8 rOrig, gOrig, bOrig;
            SDL_GetRGB(pixelOrig,format, NULL,&rOrig, &gOrig, &bOrig);

            // Aplica o filtro de Wiener por canal
            float fatorR = (varR > noiseVariance) ? (varR - noiseVariance) / varR : 0;
            float fatorG = (varG > noiseVariance) ? (varG - noiseVariance) / varG : 0;
            float fatorB = (varB > noiseVariance) ? (varB - noiseVariance) / varB : 0;

            float rFinal = mediaR + fatorR * (rOrig - mediaR);
            float gFinal = mediaG + fatorG * (gOrig - mediaG);
            float bFinal = mediaB + fatorB * (bOrig - mediaB);

            // Clamping
            Uint8 r = (Uint8)(rFinal < 0 ? 0 : (rFinal > 255 ? 255 : rFinal));
            Uint8 g = (Uint8)(gFinal < 0 ? 0 : (gFinal > 255 ? 255 : gFinal));
            Uint8 b = (Uint8)(bFinal < 0 ? 0 : (bFinal > 255 ? 255 : bFinal));

            Uint32 pixelNovo = SDL_MapRGB(format,NULL, r, g, b);
            *((Uint32 *)copySurface->pixels + y * largura + x) = pixelNovo;
        }
    }

    SDL_BlitSurface(copySurface, NULL, surface, NULL); // ele troca a imagem original pela imagem com o filtro de Wiener
    SDL_DestroySurface(copySurface);
}


const char *WINDOW_TITLE = "(1) imagem original | (2) Ruido Gaussiano | (3) Filtro de Wiener";
enum constants
{
    WINDOW_WIDTH = 600,
    WINDOW_HEIGHT = 200,
    WINDOW_TITLE_MAX_LENGTH = 64,
};

// função que libera a memória alocada e encerra o SDL
static void shutdown(void)
{
    SDL_Log("Destruindo renderizador...\n");
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_Log("Destruindo janela...\n");
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Log("Encerrando SDL...\n");
    SDL_Quit();
}

// função principal do programa
// inicializa o SDL, cria a janela e o renderizador, carrega as imagens e inicia o loop de eventos
int main(int argc, char *argv[])
{
    atexit(shutdown);
    if (AREA_FILTRO % 2 == 0)
    {
        SDL_Log("O tamanho do filtro deve ser ímpar\n");
        return SDL_APP_FAILURE;
    }
    srand(time(NULL));
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Erro ao iniciar a SDL: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Erro ao criar a janela e/ou renderizador: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Texture *texBmp = IMG_LoadTexture(renderer, CAMINHO_IMAGEM); // Carrega a imagem original
    texture = texBmp;
    if (!texBmp)
    {
        SDL_Log("Erro ao carregar a imagem: %s\n", SDL_GetError());
    }
    SDL_GetTextureSize(texture, &textureRect.w, &textureRect.h);


    // Carrega a imagem original
    SDL_Surface *surfaceBmp = IMG_Load(CAMINHO_IMAGEM);
    surfaceBmp = SDL_ConvertSurface(surfaceBmp, SDL_PIXELFORMAT_RGBA32);

    applyGaussianNoise(surfaceBmp, MEDIA, RUIDO); // Aplica o ruido gaussiano na imagem original
    SDL_SaveBMP(surfaceBmp, CAMINHO_GAUSSIANO); // Salva a imagem com ruido
    applyWienerFilterRGB(surfaceBmp, FILTRO); // Aplica o filtro de Wiener na imagem com ruido e salva a imagem
    SDL_SaveBMP(surfaceBmp, CAMINHO_WIENER); // Salva a imagem com o filtro de Wiener

    surfaceBmp = IMG_Load(CAMINHO_IMAGEM);
    surfaceBmp = SDL_ConvertSurface(surfaceBmp, SDL_PIXELFORMAT_RGBA32);
    // ------------------------------------------------------------------------------------------------------------------------------------------------

    if (textureRect.w > WINDOW_WIDTH || textureRect.h > WINDOW_HEIGHT)
    {
        SDL_SetWindowSize(window, (int)textureRect.w, (int)textureRect.h);
        SDL_SyncWindow(window);
    }

    SDL_Event event;
    bool isRunning = true;
    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT: // Fechar a janela
                isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN: 
                if (event.key.key == SDLK_2 && !event.key.repeat) // Tecla 2 pressionada para aplicar o ruido gaussiano
                {
                    applyGaussianNoise(surfaceBmp, MEDIA, RUIDO); // Aplica o ruido gaussiano na imagem original
                    SDL_DestroyTexture(texture);
                    texture = SDL_CreateTextureFromSurface(renderer, surfaceBmp); // Cria a textura a partir da superficie
                }
                if (event.key.key == SDLK_3 && !event.key.repeat) // Tecla 3 pressionada para aplicar o filtro de Wiener
                {
                    applyWienerFilterRGB(surfaceBmp, FILTRO); // Aplica o filtro de Wiener na imagem com ruido
                    SDL_DestroyTexture(texture);
                    texture = SDL_CreateTextureFromSurface(renderer, surfaceBmp); // Cria a textura a partir da superficie
                }
                if (event.key.key == SDLK_1 && !event.key.repeat) // Tecla 1 pressionada para carregar a imagem original
                {
                    SDL_DestroyTexture(texture);
                    surfaceBmp = IMG_Load(CAMINHO_IMAGEM);
                    surfaceBmp = SDL_ConvertSurface(surfaceBmp, SDL_PIXELFORMAT_RGBA32);
                    texture = IMG_LoadTexture(renderer, CAMINHO_IMAGEM);
                    
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, &textureRect, &textureRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    return 0;
}
