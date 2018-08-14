#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>  /* for O_RDONLY */
#include <sys/mman.h> /* for mmap */
#include <linux/fb.h>  /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

#define FBDEVFILE "/dev/fb0"
#define COLS 480
#define ROWS 272

void usage() {
	printf("\nUsage: ./drawbmp  xxx.bmp \n");
}

// POINT: to read a bmp file and truncate the header
void read_bmp(char *filename, char **pDib, char **data, int *cols, int *rows) {
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER *bmpInfoHeader;
	unsigned int size;
	unsigned char ID[2];
	int nread;
	FILE *fp;

	fp = fopen(filename,"rb");
	if(fp == NULL) {
		printf("ERROR\n");
		return;
	}
	
	ID[0] = fgetc(fp);
	ID[1] = fgetc(fp);
	
	if(ID[0] != 'B' && ID[1] != 'M') {
		fclose(fp);
		return;
	}
	// POINT: bmp header를 제외한 사이즈를 구함
	nread = fread(&bmpHeader.bfSize,1,sizeof(BITMAPFILEHEADER),fp);
	size = bmpHeader.bfSize - sizeof(BITMAPFILEHEADER);	

	*pDib = (unsigned char *)malloc(size);	
	fread(*pDib,1,size,fp);
	
	bmpInfoHeader = (BITMAPINFOHEADER *)*pDib;

	if(24 != bmpInfoHeader->biBitCount) {
		printf("It supports only 24bit bmp!\n");
		fclose(fp);
		return;
	}
	
	*cols = bmpInfoHeader->biWidth;
	*rows = bmpInfoHeader->biHeight;
	
	*data = (char *)(*pDib + bmpHeader.bfOffBits - sizeof(bmpHeader)-2);
	fclose(fp);
}

void close_bmp(char **pDib) {
	free(*pDib);
}

int main(int argc, char *argv[]) {
	
	int cols, rows;
	char *pData,*data;
	char r,g,b;
	unsigned short bmpdata1[ROWS][COLS];
	unsigned short pixel;
	unsigned short *pfbmap;
	
	struct fb_var_screeninfo fbvar;
	int fbfd;
	int i,j,k,t;

	if(argc != 2){
		usage();
		return 0;
	}

	// POINT: to open the fb device file
	fbfd = open(FBDEVFILE, O_RDWR);
	
	if(fbfd < 0){
		perror("fbdev open");
		exit(1);
	}

	// TODO: use mmap in order to map the screen memory
	pfbmap = (unsigned short *)
		mmap(0, COLS*ROWS*2,PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);

	if((unsigned)pfbmap == (unsigned)-1)
	{
		perror("fbdev mmap");
		exit(1);
	}

	read_bmp(argv[1], &pData, &data, &cols,&rows);

	// POINT: to draw the bmp pixel data
	for(j=0;j<rows;j++){
		k = j*cols*3; 
		printf("cols = %d, rows = %d, k = %d\n", cols, rows, k);
		for(i=0;i<cols;i++) { 
			b = *(data + (k + i*3));
			g = *(data + (k + i*3+1));
			r = *(data + (k + i*3+2));
			pixel = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
			bmpdata1[(rows-1-j)+120*0][i+160*0] = pixel;
		}
	}
	// TODO: copy the bmpdata1 to the mapping memory(pfbmap)
	memcpy(pfbmap,bmpdata1, COLS*ROWS*2);
	
	munmap(pfbmap,ROWS*COLS*2);
	close_bmp(&pData);
	close(fbfd);
	return 0;
}
