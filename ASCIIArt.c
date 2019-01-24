#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME_SIZE 20
#define MAX_DIR_SIZE 50

/*-- a struct for defining each pixel of picture --*/
typedef struct pixel{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Pixel;

void GetFileName(const char*, char*);
float pixel_med(Pixel);
void ToAscii(Pixel*, unsigned char*, int, int, int);
void MaxMin(Pixel*, int, int, int*, int*);

//An array of characters which is sorted from most black one to white. 
char AsciiBrightness[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

int main(){
  FILE *image_pointer;
  Pixel* Image;
  
  /*--- Inputing image ---*/
  char dir[MAX_DIR_SIZE], File_Name[MAX_FILE_NAME_SIZE];
  printf("Enter direction of your .ppm file : ");
  scanf("%s", dir);
  image_pointer = fopen(dir, "rb");
  
  GetFileName(dir, File_Name);
  
  int height, width, deep;
  if (image_pointer != NULL){
	
	//First header of file that can be P1 to P6.
	fscanf(image_pointer, "%s", dir);
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
  output_pointer = fopen(strcat(File_Name, ".txt"), "w");
  
  //Writing characters in ASCIIArt.txt file.
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j)
		fprintf(output_pointer, "%c", AsciiBrightness[*(ASCIIImage + i * width + j)]);
	fprintf(output_pointer, "\n");
  }
  fclose(output_pointer);
  
  printf("Your ASCIIArt is now ready in ./%s", File_Name);
  free(ASCIIImage);
  free(Image);
  return 0;
}

//Function which is fetching out file's name from dir.
void GetFileName(const char* dir, char* filename){
  int i = strlen(dir) - 1;//Index of last array's box.
  int j = 0;
  while (dir[i] != '/' && dir[i] != '\\' && i != 0){
	filename[j++] = dir[i];  
    i--;
  }  
  filename[j] = 0;
  strrev(filename);
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
	  *(ASCIIImage + i * w + j) = (unsigned char)((strlen(AsciiBrightness) - 1) * (pixel_med(*(Image + i * w + j)) - Min)/(Max - Min));
}

//Function returning Max and Min brightness value of whole picture.
void MaxMin(Pixel* Image, int h, int w, int* max, int* min){
  int Max = 0, Min = 0x100, dir;
  for(int i = 0; i < h; ++i)
    for(int j = 0; j < w; ++j){
	  dir = pixel_med(*(Image + i * w + j));
	  if(dir > Max)	Max = dir;
	  if(dir < Min)	Min = dir;
	}
	*max = Max;
	*min = Min;
}
