// implementation of filer class

#include "filer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char* ppmRead(char* filename, int* width, int* height);


//<<<<<<<<<<<<<<<<<<<<<<<<<< implementation >>>>>>>>>>>>>>>>>>>>>>
unsigned char* Filer:: ppmRead(char* filename, int* width, int* height)
{
	FILE* fp;
   	int i; 
   	int w;
   	int h;
   	int d;
   	unsigned char* image;
   	char head[70];

   	fp = fopen(filename, "rb");
   	if(!fp) 
   	{
    	perror(filename);
      	return NULL;
   	}

   	// first two chars
   	fgets(head, 70, fp);
   	if(strncmp(head, "P6", 2)) 
   	{
      	fprintf(stderr, "%s: wrong file type!\n", filename);
      	return NULL;
   	}

   	// Grab the three elements in the header (width, height, maxval).
   	i = 0;
   	while(i < 3) 
   	{
      	fgets(head, 70, fp);
      	if(head[0] == '#')		// skip comments.
        {
        	continue;
        }

      	if(i == 0)
      	{
         	i += sscanf(head, "%d %d %d", &w, &h, &d);
      	}
      	else if(i == 1)
      	{
         	i += sscanf(head, "%d %d", &h, &d);
      	}
      	else if (i == 2)
      	{
         	i += sscanf(head, "%d", &d);
      	}
   	}

   	image = (unsigned char*) malloc(sizeof(unsigned char) * w * h * 3);
   	fread(image, sizeof(unsigned char), w * h * 3, fp);
   	fclose(fp);

   	*width = w;
   	*height = h;
   	return image;

}

