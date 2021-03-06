#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../imagelib/image.h"
#include "../structs/hash_container/hash_container.h"

#define BLACK 0
#define WHITE 127
#define GREY 64


int main(int argc, char** argv)
{  
  if (argc != 2)
  {
    printf("Modo de uso: %s INPUT \n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE* input_file = fopen(argv[1], "r");

  /* Leemos la ruta de la imagen principal */
  char filename[32];
  fscanf(input_file, "%s", filename);
  /* Abrimos la imagen principal */
  Image* image = img_png_read_from_file(filename);

  /* Aca puedes crear la tabla de hash */
  HashContainer* hashContainer = init_hash_container(image->height);
  populate_hash_container(hashContainer, image);
  // print_hash_container(hashContainer);
  // printf("%i\n", image->height);

  /* Leemos cada una de las consultas */
  int Q;
  char query_in[32];
  char query_out[32];
  fscanf(input_file, "%d", &Q);


  for (int q = 0; q < Q; q++)
  {
    /* Leemos las rutas de las imagenes */
    fscanf(input_file, "%s", query_in);
    fscanf(input_file, "%s", query_out);

    /* Abrimos la imagen de consulta de input */
    Image* query_image = img_png_read_from_file(query_in);



    /* Creamos una nueva imagen en blanco con las mismas dimensiones que la original */
    Image *out_image = calloc(1, sizeof(Image));
    out_image->height = image->height;
    out_image->width = image->width;
    out_image->pixel_count = image->pixel_count;
    out_image->pixels = calloc(image->pixel_count, sizeof(int));

    for (int location = 0; location < out_image->pixel_count; location++)
    {
      out_image->pixels[location] = image->pixels[location];
    }
    


    HashedImg* hashedImgs = hash_query(hashContainer, query_image);
    // printf("\n");
    while (hashedImgs)
    {
      // printf("Pos: %i, ", hashedImgs->position);
      // printf("Valor: %i\n", compare_subimg_and_query(hashedImgs->position, query_image, image));
      if (compare_subimg_and_query(hashedImgs->position, query_image, image))
      {
        modify_output(hashedImgs->position, query_image, out_image);
      }
      hashedImgs = hashedImgs->next;
    }
    // printf("\n");


    /* Escribimos la imagen de output*/
    img_png_write_to_file(out_image, query_out);


    /* Liberamos la memoria de las imagenes*/
    img_png_destroy(query_image);
    img_png_destroy(out_image);
  
  }

  /* Liberamos la memoria de la imagen principal */
  destroy_hash_container(hashContainer);
  img_png_destroy(image);
  /* Cerramos el archivo de input */
  fclose(input_file);

  return 0;
}
