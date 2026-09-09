#include "cbmp.h"
#include <stdio.h>
int x = BMP_WIDTH;
int y = BMP_HEIGTH;



unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char gray_px[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];



void read_bitmap(char * input_file_path,
        unsigned char output_image_array[BMP_WIDTH]
            [BMP_HEIGTH][BMP_CHANNELS]
);



int grayScale(void){
    for(x = 0; x < BMP_WIDTH-1; x++){
        for(y = 0; y < BMP_HEIGTH-1; y++){
           gray_px[x][y] = (image[x][y][0]+image[x][y][1]+image[x][y][2])/3;
        }
    }
    return 0;
}

int convert2Dto3D(void){
    for (x = 0; x < BMP_WIDTH; x++) {
        for (y = 0; y < BMP_HEIGTH; y++) {
            unsigned char g = gray_px[x][y];

            output_image[x][y][0] = g;  // Red
            output_image[x][y][1] = g;  // Green
            output_image[x][y][2] = g;  // Blue
        }
    }
}

int threshHold(void){
    int max = 90;
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH;y++){
            if(gray_px[x][y] <= max){
                gray_px[x][y] = 0;
            } else if(gray_px[x][y] > max){
                gray_px[x][y] = 255;
    }
        }
    }
    
    return 0;
}

int erosion(){
    for (x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH; y++){

        }
    }
    {
        int neighbours = 0;
        
    }int neighbours(int x, int y){
       int neighbours = 0;
       if (image[x][y][0] == 0){
           return 0;
       }
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ( )
                    if (gameState[y + i][x + j] == 1) {
                        neighbours++;
                    }
            }
        }
        if (neighbours < 4){
            return 0;
        } else {
            return 1;
        }
    }
}

int detectCells(){

}

void outImage(){

}

void write_bitmap(unsigned char input_image_array[BMP_WIDTH]
    [BMP_HEIGTH][BMP_CHANNELS],
    char * output_file_path
);

int main(void){
    read_bitmap("samples/easy/1EASY.bmp", image);
    printf("%d\n", image[0][0][0]);
    grayScale();
    threshHold();
    convert2Dto3D();
    write_bitmap(output_image, "samples/easy/1EASY_gray.bmp");
    return 0;
}
