#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-- a struct for defining each pixel of picture --*/
typedef struct pixel{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Pixel;

float pixel_med(Pixel);
void ToAscii(Pixel*, unsigned char*, int, int, int);
void MaxMin(Pixel*, int, int, int*, int*);

//An array of characters which is sorted from most black one to white. 
char AsciiBrightness[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

int main(){
  FILE *image_pointer;
  Pixel* Image;
  
  /*--- Inputing image ---*/
  char tmp[20];
  printf("Enter direction of your .ppm file : ");
  scanf("%s", tmp);
  image_pointer = fopen(tmp, "rb");
  
  int height, width, deep;
  if (image_pointer != NULL){
	
	//First header of file that can be P1 to P6.
	fscanf(image_pointer, "%s", tmp);
	//Second headers of file that is width and height of picture.
	fscanf(image_pointer, "%d %d", &width, &height);
	//Last header of file that is depth of pixels.
    fscanf(image_pointer, "%d", &deep);
	
	//Define image which is an array of pixels.
	Image = (Pixel*) malloc(width * height * sizeof(Pixel));

	for (int i = 0; i < height; ++i)
	  for (int j = 0; j < width, !feof(image_pointer); ++j){
	    //Fetching R, G, and B values of each pixel.
		fread(&((Image + i * width + j)->red), sizeof(unsigned char), 1, image_pointer);
		fread(&((Image + i * width + j)->green), sizeof(unsigned char), 1, image_pointer);
		fread(&((Image + i * width + j)->blue), sizeof(unsigned char), 1, image_pointer);
	  }
	fclose(image_pointer);
	
  }
  else{
	printf("Unable to open file!");
    exit(0);
  }
  //Define an ASCII Image which is representing any pixels brightness.
  unsigned char* ASCIIImage = (unsigned char*) malloc(width * height * sizeof(unsigned char));
  ToAscii(Image, ASCIIImage, deep, height, width);
  
  FILE *output_pointer;
  output_pointer = fopen("ASCIIArt.txt", "w");
  
  //Writing characters in ASCIIArt.txt file.
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j)
		fprintf(output_pointer, "%c", AsciiBrightness[*(ASCIIImage + i * width + j)]);
	fprintf(output_pointer, "\n");
  }
  fclose(output_pointer);
  
  printf("Your ASCIIArt is now ready in ./ASCIIArt.txt");
  free(ASCIIImage);
  free(Image);
  return 0;
}

//Function which returns the median value of RGB of a pixel
float pixel_med(Pixel p){
	return (1.0 * p.red + 1.0 * p.green + 1.0 * p.blue) / 3;
}

//Function that measures brightness of pixels and map them into AsciiBrightness array.
void ToAscii(Pixel* Image, unsigned char* ASCIIImage, int deep, int h, int w){
	int Max, Min;
	MaxMin(Image, h, w, &Max, &Min);
	for(int i = 0; i < h; ++i)
		for(int j = 0; j < w; ++j)
				*(ASCIIImage + i * w + j) =(unsigned char)((strlen(AsciiBrightness) - 1) * (pixel_med(*(Image + i * w + j)) - Min)/(Max - Min));
}

//Function returning Max and Min brightness value of whole picture.
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
