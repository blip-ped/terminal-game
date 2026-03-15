#include <iostream>
 #include <random>
#include <termios.h>
#include <unistd.h>

char getch() {          //for key input
    char buf = 0;
    struct termios old{};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON; 
    old.c_lflag &= ~ECHO;   
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;  
    old.c_lflag |= ECHO;    
    if(tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}

int main(){

    std::random_device rd;                      //random number generation
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9);
    
    int playerX{0};
    int playerY{0};
    int score{0};
    int treasureX{7};
    int treasureY{7};

    while(true){
          
        std::cout << "\033[2J\033[H"; //clear terminal
       
        std::cout << "\n=== :3  ===\n";
        std::cout << "Score: " << score << "\n";

        for (int y = 0; y < 10; y++) {      //map
            for (int x = 0; x < 10; x++) {
                if (x == playerX && y == playerY) {
                    std::cout << "@ ";
                }else if (x == treasureX && y == treasureY) { 
                    std::cout << "$ "; 
                }else {
                    std::cout << ". ";
                }
            }
            std::cout << '\n';
        }  
         std::cout << "=================\n";
        


        std::cout << "Press WASD to move (Press Q to Quit): ";
        char keyInput{getch()};         //take input
       

        
        switch(keyInput){
            case 'W':       
            case 'w':      
                playerY--; 
                break;

            case 'S':       
            case 's':      
                playerY++; 
                break;

            case 'A':       
            case 'a':      
                playerX--; 
                break;

            case 'D':       
            case 'd':      
                playerX++; 
                break;

            case 'Q':       
            case 'q':  
                return 0;     
            }
    
        if(playerX<0){          //map borders
            playerX=0;
        }
        if(playerX>9){
            playerX=9;
        }
        if(playerY<0){
            playerY=0;
        }
        if(playerY>9){
            playerY=9;
        }        

        //Treasure detected :o
        if ((playerX == treasureX) && (playerY == treasureY)) {
            score++;
            treasureX = distrib(gen); //randomly generate next position
            treasureY = distrib(gen); 
        }
        
    }
    return 0;
}
