#include "cbmp.h"
#include <stdio.h>
int x = BMP_WIDTH;
int y = BMP_HEIGTH;



void main(){
    int image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
    read_bitmap("code_and_samples/code_and_samples/samples/easy/1EASY.bmp", image);
    printf(image);
    return 0;
}


void read_bitmap(char * input_file_path,
        unsigned char output_image_array[BMP_WIDTH]
            [BMP_HEIGTH][BMP_CHANNELS]
);


/*
int grayScale(){
    for(x = 0; x < BMP_WIDTH-1; x++){
        for(y = 0; y < BMP_HEIGTH-1; y++){
            int gray_px[x][y] = (image[x][y][0]+image[x][y][1]+image[x][y][2])/3;
        }
    }
}
*/


int threshhold(){

}

int erosion(){

}

int detectCells(){

}

void outImage(){

}

void write_bitmap(unsigned char input_image_array[BMP_WIDTH]
    [BMP_HEIGTH][BMP_CHANNELS],
    char * output_file_path
);
