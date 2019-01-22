#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pixel{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Pixel;

float pixel_med(Pixel);
void ToAscii(Pixel*, unsigned char*, int, int, int);
void MaxMin(Pixel*, int, int, int*, int*);

char AsciiBrightness[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

int main(){
  /*--- Inputing image ---*/
  FILE *image_pointer;
  Pixel* Image;
  char tmp[10];
  int height, width, deep;
  image_pointer = fopen("input/test2.ppm", "rb");
  if(image_pointer != NULL){
    fscanf(image_pointer, "%s", tmp);
	  fscanf(image_pointer, "%d %d", &width, &height);
	  Image = (Pixel*) malloc(width * height * sizeof(Pixel));
	  fscanf(image_pointer, "%d", &deep);
	  //printf("%s %d %d %d", tmp, height, width, deep);
	  for(int i = 0; i < height; ++i){
	    for(int j = 0; j < width, !feof(image_pointer); ++j){
		  fread(&((Image + i * width + j)->red), sizeof(unsigned char), 1, image_pointer);
		  fread(&((Image + i * width + j)->green), sizeof(unsigned char), 1, image_pointer);
		  fread(&((Image + i * width + j)->blue), sizeof(unsigned char), 1, image_pointer);
		  //printf("(%d,%d,%d)", (Image + i * width + j)->red, (Image + i * width + j)->green, (Image + i * width + j)->blue);
		}
	  //printf("\n");
	  }
	fclose(image_pointer);
  }
  else exit(0);
  unsigned char* ASCIIImage = (unsigned char*) malloc(width * height * sizeof(unsigned char));
  ToAscii(Image, ASCIIImage, deep, height, width);
  FILE *output_pointer;
  output_pointer = fopen("ASCIIArt.txt", "w");
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j)
		fprintf(output_pointer, "%c", AsciiBrightness[*(ASCIIImage + i * width + j)]);
	fprintf(output_pointer, "\n");
  }
  fclose(output_pointer);
  
  free(ASCIIImage);
  free(Image);
  return 0;
}
float pixel_med(Pixel p){
	return (1.0 * p.red + 1.0 * p.green + 1.0 * p.blue) / 3;
}
void ToAscii(Pixel* Image, unsigned char* ASCIIImage, int deep, int h, int w){
	int Max, Min;
	MaxMin(Image, h, w, &Max, &Min);
	for(int i = 0; i < h; ++i)
		for(int j = 0; j < w; ++j)
				*(ASCIIImage + i * w + j) =(unsigned char)((strlen(AsciiBrightness) - 1) * (pixel_med(*(Image + i * w + j)) - Min)/(Max - Min));
}
void MaxMin(Pixel* Image, int h, int w, int* max, int* min){
	int Max = 0, Min = 0x100, tmp;
	for(int i = 0; i < h; ++i)
		for(int j = 0; j < w; ++j){
			tmp = pixel_med(*(Image + i * w + j));
			if(tmp > Max)	Max = tmp;
			if(tmp < Min)	Min = tmp;
		}
	*max = Max;
	*min = Min;
}