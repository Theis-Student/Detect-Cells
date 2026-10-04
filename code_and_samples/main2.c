#include "cbmp.h"
#include <stdio.h>
#include <time.h> 
#include <unistd.h> 
#include <string.h>
clock_t start, end;
double cpu_time_used;

int x = BMP_WIDTH;
int y = BMP_HEIGTH;

int countCells = 0;
int finish = 1;
int countBlack = 0;

int exclusionSize = 14;
int bordermargin = 0;
int patternSize = 1;
int neighborLimit = 7;

typedef struct{
    int x;
    int y;
} Cellcoordinate;

Cellcoordinate cell_list[400];

unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

//makes sure that the HEIGTH is divisible by 8
#define BIT_HEIGHT ((BMP_HEIGTH + 7) / 8)
// Defining that 1 byte contains 8 pixels
unsigned char gray_bit_px[BMP_WIDTH][BIT_HEIGHT];  



void bit_px(int x, int y, int val){    
    if (x < 0 || x >= BMP_WIDTH || y < 0 || y >= BMP_HEIGTH) return;    
    if(val){        
        gray_bit_px[x][y/8] |= (1 << (y % 8));  
        //turns only that one bit on and does not change any other 
    }else{        
        gray_bit_px[x][y/8] &= ~(1 <<(y % 8));
        //turns only that one bit off
    }
} 

int get_bit_px(int x, int y){    
    if (x < 0 || x >= BMP_WIDTH || y < 0 || y >= BMP_HEIGTH) return 0;    
    return(gray_bit_px[x][y/8] >> (y % 8)) & 1;}


void read_bitmap(char * input_file_path,
        unsigned char output_image_array[BMP_WIDTH]
            [BMP_HEIGTH][BMP_CHANNELS]
);

void write_bitmap(unsigned char input_image_array[BMP_WIDTH]
    [BMP_HEIGTH][BMP_CHANNELS],
    char * output_file_path
);

int convertBitTo3D(void){    
    for(int x = 0; x < BMP_WIDTH; x++){
        for(int y = 0; y < BMP_HEIGTH;y++){
            unsigned char val = get_bit_px(x, y) ? 255 : 0;            
            output_image[x][y][0] = val;  // Red            
            output_image[x][y][1] = val;  // Green            
            output_image[x][y][2] = val;  // Blue        
            }    
        }   
    return 0;
}

int threshHold(void){
    int max = 90;
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH;y++){
            int avg = (image[x][y][0] + image[x][y][1] + image[x][y][2])/3;
            bit_px(x,y, avg > max);
        }
    }
    return 0;
}

void erosion(){

    unsigned char bitOut[BMP_WIDTH][BIT_HEIGHT] = {0};

    for (x = 0;x < BMP_WIDTH; x++){
        for (y = 0; y < BMP_HEIGTH; y++){

            if (get_bit_px(x,y) == 1){
                int neighbors = 0;
                for(int i = -patternSize; i <= patternSize; i++){
                    for(int j = -patternSize; j <= patternSize; j++){

                    if(i == 0 && j == 0) continue;
                    if (x + i >= 0 && x + i < BMP_WIDTH &&
                        y + j >= 0 && y + j < BMP_HEIGTH) {
                            neighbors += get_bit_px(x + i,y + j);
                }
            } 
        }             
            if (neighbors >= neighborLimit){
                bitOut[x][y/8] |= (1 << (y % 8));
            }
            }
        }
    }
        memcpy(gray_bit_px, bitOut, sizeof(gray_bit_px));
}

int detectCoconut(int x, int y){
        // Ikke < 12, men <= 12, fordi noget af cellen vil stadig være tilbage efter detect
        for(int a = 1; a <= exclusionSize-2; a++){
            for(int b = 1; b <= exclusionSize-2; b++){
                if(get_bit_px(x+a,y+b) == 1){
                    
                    for(int n = 0; n <= exclusionSize-1; n++){
                        for(int m = 0; m <= exclusionSize-1; m++){
                            bit_px(x+n,y+m,0);
                        }
                    }
                    if(countCells < 400){
                    cell_list[countCells].x = x + ((exclusionSize-2)/2);
                    cell_list[countCells].y = y + ((exclusionSize-2)/2);
                    countCells++;
                    }
                    return 1;
                }
            }
        }
        return 0;
}

int detectCells(){
    countBlack = 0;
    int totalborder = exclusionSize * 4 - 4;
    for(x = 0; x < BMP_WIDTH-exclusionSize; x++){
        for(y = 0; y < BMP_HEIGTH-exclusionSize; y++){
            // Top
            for(int i = 0; i < exclusionSize; i++){
                if(get_bit_px(x+i,y) == 0){
                    countBlack++;
                }
            }
            // Bottom
            for(int i = 0; i < exclusionSize; i++){
                if(get_bit_px(x+i,y+(exclusionSize-1)) == 0){
                    countBlack++;
                }
            }
            //Left
            for(int j = 1; j <= exclusionSize-2; j++){
                if(get_bit_px(x,y+j) == 0){
                    countBlack++;
                }
            }
            //Right
            // Ikke < 13 men <= 12
            for(int j = 1; j <= exclusionSize-2; j++){
                if(get_bit_px(x+(exclusionSize-1),y+j) == 0){
                    countBlack++;
                }
            }

            if(countBlack == totalborder-bordermargin){
                if(detectCoconut(x,y)){
                }
                y+=exclusionSize-2;
            }
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


int main(void){
    start = clock();
    read_bitmap("samples/easy/1EASY.bmp", image);
    //read_bitmap("samples/easy/2EASY.bmp", image);
    //read_bitmap("samples/medium/1MEDIUM.bmp", image);
    //read_bitmap("samples/impossible/1IMPOSSIBLE.bmp", image);
    //read_bitmap("samples/hard/1HARD.bmp", image);
    threshHold(); 
    convertBitTo3D(); 
    
    //write_bitmap(output_image, "samples/easy/1EASY_gray.bmp");
    //write_bitmap(output_image, "samples/easy/2EASY_gray.bmp");
    //write_bitmap(output_image, "samples/medium/1MEDIUM_gray.bmp");
    //write_bitmap(output_image, "samples/hard/1HARD_gray.bmp");

    //write_bitmap(output_image, "samples/impossible/1IMPOSSIBLE_gray.bmp");

    //printf("%d ",countCells);

    while(finish){
    countBlack = 0;
    erosion();
    detectCells();
    convertBitTo3D();
    //write_bitmap(output_image, "samples/easy/1EASY_gray2.bmp");
    //write_bitmap(output_image, "samples/easy/2EASY_gray2.bmp");
    //write_bitmap(output_image, "samples/medium/1MEDIUM_gray2.bmp");
    //write_bitmap(output_image, "samples/hard/1HARD_gray2.bmp");
    //write_bitmap(output_image, "samples/impossible/1IMPOSSIBLE_gray2.bmp");
    // Stop requirement, stop if all pixels are black
    int whiteCount = 0;
    for(x = 0; x < BMP_WIDTH; x++){
        for(y = 0; y < BMP_HEIGTH; y++){
            if(get_bit_px(x,y) == 1){
                whiteCount ++;
            }
        }
    }
    if(whiteCount == 0){
        finish = 0;
    }
    }
    /*
    for(int k = 0; k < countCells; k++){
        printf("\nCelle %3d: x = %3d, y = %3d\n", k + 1, cell_list[k].x, cell_list[k].y);
    }
    */

    drawRedCross();
    write_bitmap(image, "samples/easy/1EASY_detected.bmp");
    //write_bitmap(image, "samples/easy/2EASY_detected.bmp");
    //write_bitmap(image, "samples/medium/1MEDIUM_detected.bmp");
    //write_bitmap(image, "samples/hard/1HARD_detected.bmp");
    //write_bitmap(image, "samples/impossible/1IMPOSSIBLE_detected.bmp");
    end = clock(); 
    cpu_time_used = end - start;
    printf("Total time: %f ms\n", cpu_time_used * 1000.0 /
    CLOCKS_PER_SEC);
    return 0;
}
