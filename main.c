#include "global.h"



#define PS2_BASE 0xFF200100
	


int betsBoard[49];

int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

int redNumbers[] = {32, 19, 21, 25, 34, 27, 36, 30, 23,
                    5,  16, 1,  14, 9,  18, 7,  12, 3};
int blackNumbers[] = {15, 4,  2,  17, 6,  13, 11, 8,  10,
                      24, 33, 20, 31, 22, 29, 28, 35, 26};

int betType;
int money = 100;
int betAmount = 1;
int result;

int drawChip = 0;

int coinType = 2;
int start = 1;

//FUNCTION DECLEARATIONS:
void wait_for_vsync();
void clear_screen();
void green_screen();
void plot_pixel(int x, int y, short int line_color);
void draw(int x, int y);
int spinRoulette();
int isWin(int bet, int result);
int isInArray(int arr[], int size, int val);
//void calculateResult(int betType, int betAmount, int result, int *money);

void draw_digit(int digit, int startX, int startY, short int color);
void draw_number(int number, int startX, int startY, short int color);
void draw_word(int startX, int startY, short int color);
void draw_chip(int startX, int startY, int Type);
void draw_word_balance(int startX, int startY, short int color);
void drawtextDisplay(const uint16_t text[50][100], int x_shift, int y_shift);
//void displayResult(int betType, int betAmount, int result);
int calculateTotalWinnings(int betAmounts[], int result, int totalBetTypes);
void displayTotalWinnings(int totalWinnings) ; 
void calculateMultipleBetsResult(int betAmounts[], int result, int* money, int totalBetTypes);
void drawGameover(const uint16_t text[240][320], short int color);
void drawLoad();
int main(void)
{
    //initArray(&coin, 0); //init 1 element
	//initArray(&type, 0); //init 1 element	

	for(int i = 0; i < 49; i++){
		betsBoard[i] = 0;
	}
	
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
	volatile int * PS2_ptr = (int *)PS2_BASE;
	int PS2_data, RVALID;
	int x = 0, y= 4;
	char byte1 = 0, byte2 = 0, byte3 = 0;
	*(PS2_ptr) = 0xFF; // reset
    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    
	/* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
	
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr; //content of the 2d array
	
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
	
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    int is_enter = 0;
	int delay_counter = 0;
    int totalwin;
    int gameover_condition =0;
    int gameover_counter = 0;

    while(start){
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000; // extract the RVALID field
        drawLoad();

        if (RVALID) { //if there are some value:
		/* shift the next data byte into the display */
		byte1 = byte2;
		byte2 = byte3;
		byte3 = PS2_data & 0xFF;  //take the first 8 bits (2 HEX) 
		wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
		if((byte2 == (char)0xF0) && (byte3 == (char)0x5A)){ //if enter is pressed
			start = 0;
			byte2 = 0x00;
			byte3 =0x00;
		}
	}
}
    
    
    
    while (1)
    {	

        
	PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
	RVALID = PS2_data & 0x8000; // extract the RVALID field
	if (RVALID) { //if there are some value:
	/* shift the next data byte into the display */
	byte1 = byte2;
	byte2 = byte3;
	byte3 = PS2_data & 0xFF;  //take the first 8 bits (2 HEX) 
    //check if 1 key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x16)){
		coinType = 2;
	}
		
	//check if 2 key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x1E)){
		coinType = 5;
	}			
		
	//check if 3 key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x26)){
		coinType =10;
	}			
		
	//check if W key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x1D)){
		if(y < 5){ //dont go outof bound
			if(x== 3 && y == 2){ //3rd 12
                x = 9;
                y = 3;

            }
            else if(x== 2&& y ==2){ //2nd 12
                y=3;
                x=5;
            }
            else if(x == 2 && y == 1){//even to 1st 12
                x = 1;
                y = 2;
            }

            else if(x == 3 && y == 1){//red to 2nd 12 
                x = 2;
                y = 2;
            }

            else if(x == 4 && y == 1){//black to 2nd 12
                x =2;
                y = 2;
            }
            else if(x == 5 && y == 1){//3nd 12 to odd
                x = 3;
                y = 2;
            }
             else if(x == 6 && y == 1){//3nd 12 to even
                x = 3;
                y = 2;
            }           
            else{
            y += 1;
            }
		}
	}

	//check if A key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x1C)){
		if(x > 0){ //dont go outof bound
			if(x == 1 && y <= 2){// dont access non existence
				x=x;
			}
			else{
				x-= 1;
			}
		}
	}	   
	   
	//check if S key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x1B)){
		if(y > 1){ //dont go outof bound
			if(x>= 9 && x <= 13 && y == 3){ //3rd 12
                x = 3;
                y = 2;

            }
            else if(x>= 5 && x <= 8&& y ==3){ //2nd 12
                y=2;
                x=2;

            }
            else if(x>= 1 && x <= 4 && y ==3){ //1st 12
                y=2;
                x=1;

            }
            else if(x==0 && y==3){ //0 bound
                y=y;
            }
            else if(x == 2 && y == 2){//2nd 12 to red 
                x =3;
                y = 1;
            }
            else if(x == 3 && y == 2){//3nd 12 to odd
                x = 5;
                y = 1;
            }
           else{
            y -= 1;
           } 
		}
	}	 	   
	   
	//check if D key is pressed:
	if((byte2 == (char)0xF0) && (byte3 == (char)0x23)){
		if(x < 13){ //dont go outof bound
			if((x == 12 && y <= 2)|| (y==2 && x ==3 ) || (y==1 && x==6)){// dont access non existence
				x=x;
			}
			else{
				x+= 1;
			}
		}
	}	
	

	//check if space is pressed:  AKA bet is placed
	if((byte2 == (char)0xF0) && (byte3 == (char)0x29)){
		//what betting number to return:
		if(x == 0 && y >= 3 && y <= 5){ //if its 0
		betType = 0;
		}

		if(x == 1 && y == 3){ //if its 1
		betType = 1;
		}

		if(x == 1 && y == 4){ //if its 2
		betType = 2;
		}

 		if(x == 1 && y == 5){ //if its 3
		betType = 3;
		}

		if(x == 2 && y == 3){ //if its 4
		betType = 4;
		}

		if(x == 2 && y == 4){ //if its 5
		betType = 5;
		}

 		if(x == 2 && y == 5){ //if its 6
		betType = 6;
		}

		if(x == 3 && y == 3){ //if its 7
		betType = 7;
		}

		if(x == 3 && y == 4){ //if its 8
		betType = 8;
		}

 		if(x == 3 && y == 5){ //if its 9
		betType = 9;
		}    

		if(x == 4 && y == 3){ //if its 10
		betType = 10;
		}

		if(x == 4 && y == 4){ //if its 11
		betType = 11;
		}

 		if(x == 4 && y == 5){ //if its 12
		betType = 12;
		}
		if(x == 5 && y == 3){ //if its 13
		betType = 13;
		}

		if(x == 5 && y == 4){ //if its 14
		betType = 14;
		}

 		if(x == 5 && y == 5){ //if its 15
		betType = 15;
		}
		if(x == 6 && y == 3){ //if its 16
		betType = 16;
		}

		if(x == 6 && y == 4){ //if its 17
		betType = 17;
		}

 		if(x == 6 && y == 5){ //if its 18
		betType = 18;
		}
		if(x == 7 && y == 3){ //if its 19
		betType = 19;
		}

		if(x == 7 && y == 4){ //if its 20
		betType = 20;
		}

 		if(x == 7 && y == 5){ //if its 21
		betType = 21;
		}

		if(x == 8 && y == 3){ //if its 22
		betType = 22;
		}

		if(x == 8 && y == 4){ //if its 23
		betType = 23;
		}

 		if(x == 8 && y == 5){ //if its 24
		betType = 24;
		}

		if(x == 9 && y == 3){ //if its 25
		betType = 25;
		}

		if(x == 9 && y == 4){ //if its 26
		betType = 26;
		}

 		if(x == 9 && y == 5){ //if its 27
		betType = 27;
		}

		if(x == 10 && y == 3){ //if its 28
		betType = 28;
		}

		if(x == 10 && y == 4){ //if its 29
		betType = 29;
		}

 		if(x == 10 && y == 5){ //if its 30
		betType = 30;
		}

		if(x == 11 && y == 3){ //if its 31
		betType = 31;
		}

		if(x == 11 && y == 4){ //if its 32
		betType = 32;
		}

 		if(x == 11 && y == 5){ //if its 33
		betType =  33;
		}

		if(x == 12 && y == 3){ //if its 34
		betType = 34;
		}

		if(x == 12 && y == 4){ //if its 35
		betType = 35;
		}

 		if(x == 12 && y == 5){ //if its 36
		betType = 36;
		}
		if(x == 13 && y == 3){ //if its 37
		betType = 37;
		}

		if(x == 13 && y == 4){ //if its 38
		betType = 38;
		}

 		if(x == 13 && y == 5){ //if its 39
		betType = 39;
		}

		if(x == 1 && y == 2){ //if its 40
		betType = 40;
		}

		if(x == 2 && y == 2){ //if its 41
		betType = 41;
		}

 		if(x == 3 && y == 2){ //if its 42
		betType = 42;
		}

		if(x == 1 && y == 1){ //if its 43
		betType = 43;
		}

		if(x == 2 && y == 1){ //if its 44
		betType = 44;
		}

 		if(x == 3 && y == 1){ //if its 45
		betType = 45;
		}
		if(x == 4 && y == 1){ //if its 46
		betType = 46;
		}

		if(x == 5 && y == 1){ //if its 47
		betType = 47;
		}

 		if(x == 6 && y == 1){ //if its 48
		betType = 48;
		}
        
        drawChip = 1;
		betsBoard[betType] += coinType;
	
		

	}	 	   	   	   
	

    if((byte2 == (char)0xF0) && (byte3 == (char)0x5A)){
        //if enter is pressed:
        //go to final result screen
        is_enter = 1;
		result = spinRoulette();
        //draw result
		draw_number(result, 150, 10, 0xFFFF);	
   		//calculateResult(betType, betAmount, result, &money);
        calculateMultipleBetsResult(betsBoard, result, &money, 49);
        totalwin = calculateTotalWinnings(betsBoard, result, 49);
    }




	if ((byte2 == (char)0xAA) && (byte3 == (char)0x00))
	// mouse inserted; initialize sending of data
	*(PS2_ptr) = 0xF4;
	}
        /* Erase any boxes and lines that were drawn in the last iteration */
		//draw into the back buffer:
		draw(x,y);
		draw_word_balance(240, 10, 0xFFFF); 
    	draw_number(money, 225, -10, 0xFFFF);
        if(is_enter){	
			delay_counter = 40;
			is_enter = 0;
            drawChip = 0;

		}
        if (delay_counter > 0) {
			//green background
			green_screen();
			//balance money
			//draw_word_balance(240, 10, 0xFFFF);
			//draw_number(money, 225, -10, 0xFFFF);
			//Win or Dealer Wins
			//displayResult(betType, betAmount, result);
            displayTotalWinnings(totalwin);
			//Red/Black+ Roulette Number
			if (isInArray(redNumbers, sizeof(redNumbers) / sizeof(redNumbers[0]), result)) {
				drawtextDisplay(red, 105, 50);
				draw_number(result, 135, 50, 38918);
			} else if (isInArray(blackNumbers, sizeof(blackNumbers) / sizeof(blackNumbers[0]), result)) {
				drawtextDisplay(black, 90, 52); 
				draw_number(result, 150, 50, 0);
			}
            //clears the board
            for(int i = 0; i < 49; i++){
	    	    betsBoard[i] = 0;
    	    }
        	delay_counter--; // delay counter
            if(delay_counter==0){
                gameover_counter = 40;
            }
    	}
        if(gameover_condition==1 && gameover_counter>0){
            drawGameover(gameover, 0xFFFF);
            gameover_counter--;
            if(gameover_counter==0){
                gameover_condition =0;
            }
        }
        // code for drawing the boxes and lines (not shown)
        // code for updating the locations of boxes (not shown)
	    if(drawChip){
            for(int s = 0; s < 49; s++){
				int imgChoose;
				
				if(betsBoard[s] != 0){
					if(betsBoard[s] >= 10){
						imgChoose = 2;
					}
					else if(betsBoard[s] >= 5){
						imgChoose = 1;
					}
					else{
						imgChoose = 0;
					}
					
                
					
                if(s == 0){
                draw_chip(30,99, imgChoose);
                }
                if(s == 1){
                draw_chip(64,142, imgChoose);
                }
                if(s == 2){
                draw_chip(64,97, imgChoose);
                }
                if(s == 3){
                draw_chip(64,55, imgChoose);
                }
                if(s == 4){
                draw_chip(82,141, imgChoose);
                }
                if(s == 5){
                draw_chip(82,97, imgChoose);
                }
                if(s == 6){
                draw_chip(82,55, imgChoose);
                }
                if(s == 7){
                draw_chip(99,141, imgChoose);
                }
                if(s == 8){
                draw_chip(99,97, imgChoose);
                }
                if(s == 9){
                draw_chip(99,55, imgChoose);
                }
                if(s == 10){
                draw_chip(116,141, imgChoose);
                }
                if(s == 11){
                draw_chip(116,97, imgChoose);
                }
                if(s == 12){
                draw_chip(116,55, imgChoose);
                }
                if(s == 13){
                draw_chip(133,141, imgChoose);
                }
                if(s == 14){
                draw_chip(133,97, imgChoose);
                }
                if(s== 15){
                draw_chip(133,55, imgChoose);
                }
                if(s == 16){
                draw_chip(150,141, imgChoose);
                }
                if(s == 17){
                draw_chip(150,97, imgChoose);
                }
                if(s == 18){
                draw_chip(150,55, imgChoose);
                }
                if(s == 19){
                draw_chip(167,141, imgChoose);
                }
                if(s == 20){
                draw_chip(167,97, imgChoose);
                }
                if(s == 21){
                draw_chip(167,55, imgChoose);
                }
                if(s == 22){
                draw_chip(184,141,imgChoose);
                }
                if(s == 23){
                draw_chip(184,97, imgChoose);
                }
                if(s == 24){
                draw_chip(184,55, imgChoose);
                }
                if(s == 25){
                draw_chip(201,141, imgChoose);
                }
                if(s == 26){
                draw_chip(201,97, imgChoose);
                }
                if(s == 27){
                draw_chip(201,55, imgChoose);
                }
                if(s == 28){
                draw_chip(218,141, imgChoose);
                }
                if(s == 29){
                draw_chip(218,97, imgChoose);
                }
                if(s == 30){
                draw_chip(218,55, imgChoose);
                }
                if(s == 31){
                draw_chip(235,141, imgChoose);
                }
                if(s == 32){
                draw_chip(235,97, imgChoose);
                }
                if(s == 33){
                draw_chip(235,55, imgChoose);
                }
                if(s== 34){
                draw_chip(252,141, imgChoose);
                }
                if(s == 35){
                draw_chip(252,97, imgChoose);
                }
                if(s == 36){
                draw_chip(252,55, imgChoose);
                }
                if(s == 37){
                draw_chip(275,141, imgChoose);
                }
                if(s == 38){
                draw_chip(275,97, imgChoose);
                }
                if(s == 39){
                draw_chip(275,55, imgChoose);
                }
                if(s == 40){
                draw_chip(83,178, imgChoose);
                }
                if(s == 41){
                draw_chip(157,178, imgChoose);
                }
                if(s == 42){
                draw_chip(232,178, imgChoose);
                }
                if(s == 43){
                draw_chip(70,204, imgChoose);
                }
                if(s == 44){
                draw_chip(115,204, imgChoose);
                }
                if(s == 45){
                draw_chip(148,204, imgChoose);
                }
                if(s == 46){
                draw_chip(179,204, imgChoose);
                }
                if(s == 47){
                draw_chip(210,204, imgChoose);
                }
                if(s == 48){
                draw_chip(253,204, imgChoose);
                }
				}
            }
		}
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
		if(money <= 0){
            money =100;
            gameover_condition=1;
        }
    }

}

// code for subroutines (not shown)

void plot_pixel(int x, int y, short int line_color)
{
    short int *one_pixel_address;
        
        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
        
        *one_pixel_address = line_color;
}



void wait_for_vsync()
{
volatile int * pixel_ctrl_ptr = (int *) 0xff203020; // base address
*pixel_ctrl_ptr = 1;
while ((*(pixel_ctrl_ptr+3) & 1) != 0);
}


void clear_screen()
{
        int y, x;
        for (x = 0; x < 320; x++){
                for (y = 0; y < 240; y++){
                        plot_pixel (x, y, 0);
				}
		}
}

void draw(int x, int y){
	clear_screen();
	
			if(x == 0 && y >= 3 && y <= 5){ //if its 0
            int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet0[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
	    }

		if(x == 1 && y == 3){ //if its 1
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet1[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 1 && y == 4){ //if its 2
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet2[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 1 && y == 5){ //if its 3
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet3[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 2 && y == 3){ //if its 4
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet4[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 2 && y == 4){ //if its 5
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet5[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 2 && y == 5){ //if its 6
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet6[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 3 && y == 3){ //if its 7
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet7[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 3 && y == 4){ //if its 8
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet8[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 3 && y == 5){ //if its 9
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet9[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}    

		if(x == 4 && y == 3){ //if its 10
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet10[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 4 && y == 4){ //if its 11
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet11[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 4 && y == 5){ //if its 12
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet12[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}
		if(x == 5 && y == 3){ //if its 13
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet13[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 5 && y == 4){ //if its 14
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet14[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 5 && y == 5){ //if its 15
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet15[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}
		if(x == 6 && y == 3){ //if its 16
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet16[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 6 && y == 4){ //if its 17
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet17[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 6 && y == 5){ //if its 18
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet18[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}
		if(x == 7 && y == 3){ //if its 19
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet19[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 7 && y == 4){ //if its 20
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet20[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 7 && y == 5){ //if its 21
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet21[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 8 && y == 3){ //if its 22
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet22[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 8 && y == 4){ //if its 23
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet23[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 8 && y == 5){ //if its 24
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet24[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 9 && y == 3){ //if its 25
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet25[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 9 && y == 4){ //if its 26
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet26[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 9 && y == 5){ //if its 27
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet27[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 10 && y == 3){ //if its 28
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet28[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 10 && y == 4){ //if its 29
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet29[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 10 && y == 5){ //if its 30
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet30[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 11 && y == 3){ //if its 31
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet31[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 11 && y == 4){ //if its 32
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet32[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 11 && y == 5){ //if its 33
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet33[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 12 && y == 3){ //if its 34
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet34[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 12 && y == 4){ //if its 35
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet35[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 12 && y == 5){ //if its 36
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet36[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}
		if(x == 13 && y == 3){ //if its 37
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet37[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 13 && y == 4){ //if its 38
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet38[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 13 && y == 5){ //if its 39
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet39[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 1 && y == 2){ //if its 40
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet40[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 2 && y == 2){ //if its 41
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet41[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 3 && y == 2){ //if its 42
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet42[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 1 && y == 1){ //if its 43
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet43[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 2 && y == 1){ //if its 44
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet44[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 3 && y == 1){ //if its 45
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet45[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}
		if(x == 4 && y == 1){ //if its 46
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet46[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

		if(x == 5 && y == 1){ //if its 47
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet47[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

 		if(x == 6 && y == 1){ //if its 48
           int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = bet48[j][i];
                plot_pixel (i, j, temp);			
                }  
            }
		}

}

	

void green_screen()
{
        int y, x;
        for (x = 0; x < 320; x++){
                for (y = 0; y < 240; y++){
                        plot_pixel (x, y, 6981);
				}
		}
}
int spinRoulette() {
  return rand() % 37;  // random number between 0 and 36
}

int isInArray(int arr[], int size, int val) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == val) {
      return 1;  // value found
    }
  }
  return 0;  // value not found
}

int isWin(int bet, int result) {
  // check if the bet is a winning bet
  if (bet == result) {
    return 35;  // straight up or 0 win
  }

  // special bets
  switch (bet) {
    case 37:
    case 38:
    case 39:
      if ((result % 3 == bet % 37) && result != 0) return 2;  // Columns
      break;
    case 40:
      if (result >= 1 && result <= 12) return 2;  // 1st 12
      break;
    case 41:
      if (result >= 13 && result <= 24) return 2;  // 2nd 12
      break;
    case 42:
      if (result >= 25 && result <= 36) return 2;  // 3rd 12
      break;
    case 43:
      if (result >= 1 && result <= 18) return 1;  // 1-18
      break;
    case 44:
      if (result > 0 && result % 2 == 0) return 1;  // Even
      break;
    case 45:
      if (isInArray(redNumbers, sizeof(redNumbers) / sizeof(redNumbers[0]),
                    result))
        return 1;  // Red
      break;
    case 46:
      if (isInArray(blackNumbers,
                    sizeof(blackNumbers) / sizeof(blackNumbers[0]), result))
        return 1;  // Black
      break;
    case 47:
      if (result > 0 && result % 2 != 0) return 1;  // Odd
      break;
    case 48:
      if (result >= 19 && result <= 36) return 1;  // 19-36
      break;
  }

  return 0;  // no match, bet lost
}


/*void calculateResult(int betType, int betAmount, int result, int *money) {
  int payoutRatio = isWin(betType, result);
  if (payoutRatio > 0) {
    int winnings = betAmount * payoutRatio;
    *money += winnings;
  } else {
    *money -= betAmount;
  }
}

void displayResult(int betType, int betAmount, int result) {
  int payoutRatio = isWin(betType, result);
  if (payoutRatio > 0) {
    int winnings = betAmount * payoutRatio;
	//draw "Win! $winnings"
	drawtextDisplay(win, 105, 100);
	draw_number(winnings, 155, 100, 65026);
  } else {
	//draw "Dealer Wins! -$betAmount"
	drawtextDisplay(dealerwin, 105, 100);
	draw_number(betAmount, 160, 102, 65026);
  }
}*/
void calculateMultipleBetsResult(int betAmounts[], int result, int* money, int totalBetTypes) {
  for (int betType = 0; betType < totalBetTypes; betType++) {
    int betAmount = betAmounts[betType];
    if (betAmount > 0) { // check if a bet is placed
      int payoutRatio = isWin(betType, result);
      if (payoutRatio > 0) {
        int winnings = betAmount * payoutRatio;
        *money += winnings;
      } else {
        *money -= betAmount;
      }
    }
  }
}

int calculateTotalWinnings(int betAmounts[], int result, int totalBetTypes) {
  int totalWinnings = 0; 
  for (int betType = 0; betType < totalBetTypes; betType++) {
    int betAmount = betAmounts[betType];
    if (betAmount > 0) { 
      int payoutRatio = isWin(betType, result);
      if (payoutRatio > 0) {
        totalWinnings += betAmount * payoutRatio; 
      } else {
        totalWinnings -= betAmount;
      }
    }
  }
  return totalWinnings;
}

void displayTotalWinnings(int totalWinnings) {
  if (totalWinnings > 0) {
    //draw "Win! $winnings"
    drawtextDisplay(win, 105, 100);
    draw_number(totalWinnings, 155, 100, 65026);
  } else if (totalWinnings < 0) {
    //draw "Dealer Wins! -$betAmount"
    drawtextDisplay(dealerwin, 105, 100);
    draw_number(-totalWinnings, 160, 102, 65026);
  }
}

// Draw "balance" on the screen
void draw_word_balance(int startX, int startY, short int color) {
    for (int y = 0; y < 5; y++) { // height
        for (int x = 0; x < 29; x++) { // width
            if (word_balance[y][x]) {
                plot_pixel(startX + x, startY + y, color);
            }
        }
    }
}

void draw_chip(int startX, int startY, int Type) {
	if (Type ==0){
    for (int y = 0; y < 15; y++) { // Height of the characters
        for (int x = 0; x < 15; x++) { // Width of the word "balance" plus spaces
                short int temp = chip0[y][x];
                plot_pixel(startX + x, startY + y, temp);
            }
        }
    }
if (Type ==1){
    for (int y = 0; y < 15; y++) { // Height of the characters
        for (int x = 0; x < 15; x++) { // Width of the word "balance" plus spaces
                short int temp = chip1[y][x];
                plot_pixel(startX + x, startY + y, temp);
            }
        }
    }
if (Type ==2){
    for (int y = 0; y < 15; y++) { // Height of the characters
        for (int x = 0; x < 15; x++) { // Width of the word "balance" plus spaces
                short int temp = chip2[y][x];
                plot_pixel(startX + x, startY + y, temp);
            }
        }
    }
}
void drawtextDisplay(const uint16_t text[50][100], int x_shift, int y_shift) {
    for (int y = 0; y < 50; ++y) {
        for (int x = 0; x < 100; ++x) {
			if(text[y][x]!=65535){
            plot_pixel(x+x_shift, y+y_shift, text[y][x]);
			}
        }
    }
}
void drawGameover(const uint16_t text[240][320], short int color) {
    for (int y = 0; y < 240; ++y) {
        for (int x = 0; x < 320; ++x) {
            //black draws white
			if(text[y][x]==0){
                plot_pixel(x, y, color);
			}else if(text[y][x]!=65535){
                plot_pixel(x, y, text[y][x]);
            }
        }
    }
}

void draw_digit(int digit, int startX, int startY, short int color) {
    const uint16_t (*digit_ptr)[100]; // pointer to the bitmap array
    switch(digit) {
        case 0: digit_ptr = digit_0; break;
        case 1: digit_ptr = digit_1; break;
        case 2: digit_ptr = digit_2; break;
        case 3: digit_ptr = digit_3; break;
        case 4: digit_ptr = digit_4; break;
        case 5: digit_ptr = digit_5; break;
        case 6: digit_ptr = digit_6; break;
        case 7: digit_ptr = digit_7; break;
        case 8: digit_ptr = digit_8; break;
        case 9: digit_ptr = digit_9; break;
        default: return; // do nothing
    }

    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 100; x++) {
            if (digit_ptr[y][x]!=65535) {
                plot_pixel(startX + x, startY + y, color);
            }
        }
    }
}

// draw number
void draw_number(int number, int startX, int startY, short int color) {
    // calculate the number of digits
    int digits = 0;
    int temp = number;
    do {
        digits++;
        temp /= 10;
    } while (temp > 0);

    // draw each digit of the number
    temp = number;
    for (int i = 0; i < digits; i++) {
        int digit = temp % 10;
        temp /= 10;

        // each digit takes 15 pixels
        int digitX = startX + (digits - 1 - i) * 15;

        draw_digit(digit, digitX, startY, color);
    }
}


void drawLoad(){
	clear_screen();
            int i,j;
            for (i = 0; i < 320; i++){
                for (j = 0; j < 240; j++){
                short int temp = load[j][i];
                plot_pixel (i, j, temp);			
                }  
            }

	
}