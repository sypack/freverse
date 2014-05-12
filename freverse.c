#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

typedef struct node {
   char *extension;
   struct node *next;
} node;

char *strnrev(char *str, unsigned long length)
{

    if (! str || ! *str)
        return str;

    char *p1, *p2;


    for (p1 = str, p2 = str + length - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

int is_correct_extension(char *dot, node *extentions)
{
    if(!dot)
        return 0;

    node *curr = extentions;

    while(curr)
    {
      if(strcasecmp(curr->extension, dot) == 0)
        return 1;
      curr = curr->next ;
   }

    return 0;
}

char *reverse_filename(char *filename, node *extentions)
{
    char *reversed_filename = strdup(filename);
    char *dot = strrchr(reversed_filename, '.');

    if(is_correct_extension(dot, extentions))
        strnrev(reversed_filename, strlen(reversed_filename) - (strlen(reversed_filename) - ( dot - reversed_filename )));

    return reversed_filename;
}

int main ( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        printf( "usage: %s extension1 extenstion2 extension3 ...\nexample: %s .mp4 .mkv .nfo .srt\n\n", basename(argv[0]), basename(argv[0]) );
        return 0;
    }

    node *curr, *head = NULL;

    int i;
    for(i = 1; i < argc ; i++)
    {
      curr = (node *)malloc(sizeof(node));
      curr->extension = strdup(argv[i]);
      curr->next  = head;
      head = curr;
   }

    DIR           *d;
    struct dirent *dir;
    char *reversed_filename;
    d = opendir(".");

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            reversed_filename = reverse_filename(dir->d_name, head);

            if(strcmp(reversed_filename, dir->d_name) != 0)
            {
                if( rename(dir->d_name, reversed_filename) == 0)
                    printf("%s --> %s\n", dir->d_name, reversed_filename);
   				else printf("Error: unable to reverse file %s\n", dir->d_name);
            }
            free(reversed_filename);

    }
    closedir(d);

    // free linked list
    node *curr = head;
    node *prev;
    while(curr)
    {
      if(curr->extension)
        free(curr->extension);

      prev = curr;
      curr = curr->next ;
      free(prev);
   }
  }

  return 0;
}


