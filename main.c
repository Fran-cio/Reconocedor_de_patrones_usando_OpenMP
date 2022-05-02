#include "./src/PGM_in_c.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main ()
{
  const char* imagen_base= "./img/pibes.pgm";
  const char* imagen_cortada = "./img/pibes_test.pgm";

  PGMImage* str_imagen_base = malloc(sizeof(PGMImage));
  PGMImage* str_imagen_cortada = malloc(sizeof(PGMImage));

  // Process the image and print
  // its details
  if (openPGM(str_imagen_base, imagen_base))
    printImageDetails(str_imagen_base, imagen_base);

  if (openPGM(str_imagen_cortada, imagen_cortada))
    printImageDetails(str_imagen_cortada, imagen_cortada);

  unsigned char** matx_base,
           **matx_recortada;

  matx_base = str_imagen_base->matriz;
  matx_recortada = str_imagen_cortada->matriz;

  uint const ancho_base = str_imagen_base -> ancho,
       alto_base = str_imagen_base -> alto,
       ancho_cortada = str_imagen_cortada -> ancho,
       alto_cortada = str_imagen_cortada -> alto;

  unsigned char **matx_distancia_char = malloc(alto_base * sizeof(unsigned char*));

  for(uint i=0;i<alto_base;i++)
  {
    matx_distancia_char[i] = malloc(ancho_base * sizeof(unsigned char));
    for(uint j=0;j<ancho_base;j++)
      matx_distancia_char[i][j] = 0;
  }

  uint valor_max = 0
    ,valor_min = 999999999 
    ,x_min=0
    ,y_min=0;

  valor_max = 255 * alto_cortada * ancho_cortada; 

  for (uint i= 0; i <= (alto_base - alto_cortada ) ; i++) 
  {
    for (uint j=0; j <= (ancho_base - ancho_cortada ) ; j++)
    {
      uint temp = 0;
      for (uint k=0; k < alto_cortada ; k++)
      {
        uint x = i + k;
        for (uint l=0; l <  ancho_cortada; l++)
        {
          uint y = j + l;

          int distancia = (matx_base[x][y] - matx_recortada[k][l]); 

          temp += (unsigned) (distancia * distancia);
        }
      }

      if(temp == 0)
      {
        valor_min = 0;
        x_min=i;
        y_min=j;
      }
      printf("\r ---- %.2f%c ----",((float)i*100/(float)(alto_base - alto_cortada)),'%');
      matx_distancia_char[i + (alto_cortada/2)][j + (ancho_cortada/2)] = 
        (unsigned char)(temp/valor_max);
    }
  }
  printf("\n");


  if(valor_min != 0)
  {
    fprintf(stderr, "No se encontro patron\n");
    exit(EXIT_SUCCESS);
  }

  PGMImage* str_imagen_dist = malloc(sizeof(PGMImage));
  str_imagen_dist->ancho = ancho_base;
  str_imagen_dist->alto = alto_base;
  strcpy(str_imagen_dist->tipo_pgm, "P5");
  str_imagen_dist->valor_max = 255;
  str_imagen_dist->matriz = matx_distancia_char;

  crear_imagen(str_imagen_dist, "./img/imagenes_generadas/mapa_distancia.pgm");

  for (uint i=x_min; i < (x_min + alto_cortada) ; i++)
  {
    matx_base[i][y_min] = 0;
    matx_base[i][y_min + 1] = 0;
    matx_base[i][y_min + ancho_cortada] = 0;
    matx_base[i][y_min - 1 + ancho_cortada] = 0;
  }

  for(uint j=y_min; j < (y_min + ancho_cortada); j++)
  {
    matx_base[x_min][j] = 0;
    matx_base[x_min + 1][j] = 0;
    matx_base[x_min - 2 + alto_cortada][j] = 0;
    matx_base[x_min - 1 + alto_cortada][j] = 0;
  }


  PGMImage* str_imagen_marcada = malloc(sizeof(PGMImage));
  str_imagen_marcada->ancho = ancho_base;
  str_imagen_marcada->alto = alto_base;
  strcpy(str_imagen_marcada->tipo_pgm, "P5");
  str_imagen_marcada->valor_max = 255;
  str_imagen_marcada->matriz = matx_base;

  crear_imagen(str_imagen_marcada , "./img/imagenes_generadas/foto_marcada.pgm");

  // for(uint i=0;i<str_imagen_dist->alto;i++)
  // {
  //   for(uint j=0;j<str_imagen_dist->ancho;j++)
  //   {
  //     printf("|%d",str_imagen_dist ->matriz[i][j]);
  //   }
  //   printf("|\n");
  // }
  //
  //
  free(str_imagen_cortada);
  free(str_imagen_dist);
  free(str_imagen_base);
  free(str_imagen_marcada);
  free(matx_distancia_char);

  return 0;
}

