#include "cbmp.h"
#include <stdio.h>
#include <time.h> 
#include <unistd.h> 

int x = BMP_WIDTH;
int y = BMP_HEIGTH;

int i = BMP_WIDTH;
int j = BMP_HEIGTH;

int a = BMP_WIDTH;
int b = BMP_HEIGTH;

int countCells = 0;

//buffer1 
//buffer2

int finish = 1;
int countBlack = 0;


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
    return 0;
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
    int neighbors = 0;
    unsigned char out[BMP_WIDTH][BMP_HEIGTH];

    for (x = 0;x < BMP_WIDTH; x++){
        for (y = 0; y < BMP_HEIGTH; y++){
            if (x-1 >= 0){
                if(gray_px[x-1][y] > 0){
                    neighbors++;
                }
            }
            if (x+1 <= BMP_WIDTH){
                if(gray_px[x+1][y] > 0){
                    neighbors++;
                }
            }
            if(y - 1 >= 0){
                if(gray_px[x][y-1] > 0){
                    neighbors++;
                }
            }
            
            if (y + 1 <= BMP_HEIGTH){
                if (gray_px[x][y+1]){
                    neighbors++;
                }
            }
            if (neighbors == 4){
                out[x][y] = gray_px[x][y];
            } else {
                out[x][y] = 0;
            }
            //printf("%d",neighbors);
            neighbors = 0;
            }
            
        }
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH;y++){
            gray_px[x][y] = out[x][y];
        }
    }
}
int detectCoconut(int x, int y){
        for(a = 1; a < 13; a++){
            for(b = 1; b < 13; b++){
                if(gray_px[x+a][y+b] > 0){
                    for(int n = 1; n < 13; n++){
                        for(int m = 1; m < 13; m++){
                            gray_px[x+n][y+m] = 0;
                        }
                    }
                    countCells++;
                    printf("%d ",countCells);
                    return 0;
                }
            }
        }
        return 0;
}

int detectCells(){
    countBlack = 0;
    for(x = 0; x < BMP_WIDTH-14; x++){
        for(y = 0; y < BMP_HEIGTH-14; y++){
            // Top
            for(i = 0; i < 14; i++){
                if(gray_px[x+i][y] == 0){
                    countBlack++;
                }
            }
            // Bottom
            for(i = 0; i < 13; i++){
                if(gray_px[x+i][y+13] == 0){
                    countBlack++;
                }
            }
            //Left
            for(j = 1; j < 14; j++){
                if(gray_px[x][y+j] == 0){
                    countBlack++;
                }
            }
            //Right
            for(j = 1; j < 13; j++){
                if(gray_px[x+13][y+j] == 0){
                    countBlack++;
                }
            }
            if(countBlack == 52){
                detectCoconut(x,y);
            }
            
            //printf("%d ",countBlack);
            countBlack = 0;
            
        }
    }
}

void outImage(){

}

void write_bitmap(unsigned char input_image_array[BMP_WIDTH]
    [BMP_HEIGTH][BMP_CHANNELS],
    char * output_file_path
);

int main(void){
    read_bitmap("samples/easy/1EASY.bmp", image);
    grayScale();
    threshHold();
    convert2Dto3D();
    write_bitmap(output_image, "samples/easy/1EASY_gray.bmp");
    printf("%d ",countCells);

    while(finish){
    countBlack = 0;
    erosion();
    detectCells();
    sleep(1);
    convert2Dto3D();
    write_bitmap(output_image, "samples/easy/1EASY_gray2.bmp");
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH; y++){
            if(gray_px[x][y] == 0){
                countBlack++;
            }
            if(countBlack == BMP_WIDTH*BMP_HEIGTH){
                finish = 0;
            }
        }
    }
    }
    return 0;
}
