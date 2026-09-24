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

typedef struct{
    int x;
    int y;
} Cellcoordinate;

Cellcoordinate cell_list[1000];

unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char gray_px[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];


void read_bitmap(char * input_file_path,
        unsigned char output_image_array[BMP_WIDTH]
            [BMP_HEIGTH][BMP_CHANNELS]
);



void grayScale(void){
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH; y++){
           gray_px[x][y] = (image[x][y][0]+image[x][y][1]+image[x][y][2])/3;
        }
    }
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

void erosion(){
    int neighbors = 0;
    unsigned char out[BMP_WIDTH][BMP_HEIGTH];

    for (x = 0;x < BMP_WIDTH; x++){
        for (y = 0; y < BMP_HEIGTH; y++){
            if (x-1 >= 0 && gray_px[x-1][y] > 0){
                    neighbors++;
            }
            if (x + 1 < BMP_WIDTH && gray_px[x+1][y] > 0){
                    neighbors++;
            }
            if(y - 1 >= 0 && gray_px[x][y-1] > 0){
                    neighbors++;
            }
            
            if (y + 1 < BMP_HEIGTH && gray_px[x][y+1] > 0){
                    neighbors++;
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
        // Ikke < 12, men <= 12, fordi noget af cellen vil stadig være tilbage efter detect
        for(a = 1; a <= 12; a++){
            for(b = 1; b <= 12; b++){
                if(gray_px[x+a][y+b] > 0){

                    for(int n = 1; n <= 12; n++){
                        for(int m = 1; m <= 12; m++){
                            gray_px[x+n][y+m] = 0;
                        }
                    }

                    cell_list[countCells].x = x + 6;
                    cell_list[countCells].y = y + 6;

                    countCells++;
                    printf("%d ",countCells);
                    return 1;
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
            for(i = 0; i < 14; i++){
                if(gray_px[x+i][y+13] == 0){
                    countBlack++;
                }
            }
            //Left
            for(j = 1; j <= 12; j++){
                if(gray_px[x][y+j] == 0){
                    countBlack++;
                }
            }
            //Right
            // Ikke < 13 men <= 12
            for(j = 1; j <= 12; j++){
                if(gray_px[x+13][y+j] == 0){
                    countBlack++;
                }
            }
            if(countBlack == 52){
                if(detectCoconut(x,y)){
                }
                y+=12;
            }
            
            //printf("%d ",countBlack);
            countBlack = 0;
            
        }
    }
}
int drawRedCross(void){
    int rad = 6; // length of 1 arm of the cross

    for(int k = 0; k < countCells; k++){
        int cx = cell_list[k].x;
        int cy = cell_list[k].y;
        //From center draws line [-rad,rad]
        for(int offset = -rad; offset <= rad; offset++){
            if(cx + offset >= 0 && cx + offset < BMP_WIDTH){
                image[cx + offset][cy][0] = 255; // red
                image[cx + offset][cy][1] = 0; // green
                image[cx + offset][cy][2] = 0; // blue
            }
            if(cy + offset >= 0 && cy + offset < BMP_WIDTH){
                image[cx][cy + offset][0] = 255; // red
                image[cx][cy + offset][1] = 0; // green
                image[cx][cy + offset][2] = 0; // blue
            }

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
    //read_bitmap("samples/easy/1EASY.bmp", image);
   read_bitmap("samples/medium/1MEDIUM.bmp", image);

    grayScale();
    threshHold();
    convert2Dto3D();
    //write_bitmap(output_image, "samples/easy/1EASY_gray.bmp");
    write_bitmap(output_image, "samples/medium/1MEDIUM_gray.bmp");
    printf("%d ",countCells);

    while(finish){
    countBlack = 0;
    erosion();
    detectCells();
    //sleep(1);
    convert2Dto3D();
    //write_bitmap(output_image, "samples/easy/1EASY_gray2.bmp");
    write_bitmap(output_image, "samples/medium/1MEDIUM_gray2.bmp");

    // Stop requirement, stop if all pixels are black
    int whiteCount = 0;
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH; y++){
            if(gray_px[x][y] > 0){
                whiteCount ++;
            }
        }
    }
    if(whiteCount == 0){
        finish = 0;
    }
    }
    for(int k = 0; k < countCells; k++){
        printf("\nCelle %3d: x = %3d, y = %3d\n", k + 1, cell_list[k].x, cell_list[k].y);
    }

    drawRedCross();
    //write_bitmap(image, "samples/easy/1EASY_detected.bmp");
    write_bitmap(image, "samples/medium/1MEDIUM_detected.bmp");
    return 0;
}