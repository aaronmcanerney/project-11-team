/** @file rushhour.cpp
 @author Aaron Mcanerney
@version Revision 1.0
@breif solves the rush hour game using DFS
@details Uses DFS to solve the rush hour puzzle game. uses a struct to hold vehicles
and does everything else with pass by reference methods. 
@date 10/3/2017
**/


#include<iostream>
#include <unistd.h>

using namespace std;

struct Vehicle{
    int length;
    char orientation;
    int row;
    int column;
};

//consts for array size, car and truck size, and horizontal check
const int CAR = 2;
const int TRUCK = 3;
const char HORIZONTAL = 'H';
const int MAX_ARR = 6;
const int MAX_VEHICLE = 10;

void read(int board[][MAX_ARR], int& numCars, Vehicle cars[]);
void setBoard(int board[][MAX_ARR], const Vehicle& v, const int car);
bool isCar(const Vehicle& v);
void print(const int board[][MAX_ARR]);
void fillArray(int board[][MAX_ARR]);
bool moveForward(Vehicle& v, int board[][MAX_ARR]);
bool moveBackward(Vehicle& v, int board[][MAX_ARR]);
bool isComplete(const Vehicle& v, const int board[][MAX_ARR]);
bool isHorizontal(const Vehicle& v);
void solve(int numMoves, Vehicle cars[], int board[][MAX_ARR], int& best,const int& numCars, bool& result);
bool isCollisionForward(const Vehicle& v, const int board[][MAX_ARR]);
bool isCollisionBackward(const Vehicle& v, const int board[][MAX_ARR]);

/**
* Main method
*
*@return int indicating success
*
*@pre unsolved rush hour
*
*@post solved rush hour
*
*
**/
int main(){
    int board[MAX_ARR][MAX_ARR];
    Vehicle cars[MAX_VEHICLE];
    int numCars;

    fillArray(board);
    read(board, numCars, cars);
    int moves = 0;
    int best = 11;
    bool result = false;

    
    solve(moves, cars,board,best, numCars, result);
    if(result){
        cout << "Scenario 1" << " requires " << best << " moves"<<endl;
    }
    else{
        cout << "Cant be solved "<< endl;
    }
    return 1;
}

/**
* read  method that  populates the board and vehicles array.
*uses helper function set board.
*
*@return void
*
*@param board board that the game is played on
*
*@param v a vehicle
*
*@param numCars the number of cars on the board
*
*@pre unfilled board and car array
*
*@post filled board and car array
*
**/

void read(int board[][MAX_ARR], int& numCars, Vehicle cars[]){
    cin >> numCars;
    for(int i = 0; i < numCars; i++){
        Vehicle v;
        cin >> v.length >> v.orientation >> v.row >> v.column;
        cars[i] = v;
        setBoard(board, v, i+1);
    }
}


/**
* Set board  method that populates the two dimensional array with cars
*
*@return void
*
*@param v a vehicle
*
*@param board board that the game is played on
*
*@pre array and vehile with car number to be displayed
*
*@post a board with a new car in postion x,y
*
**/
void setBoard(int board[][MAX_ARR], const Vehicle& v, const int car){
    for(int i = 0 ; i < v.length; i++){
        if(isHorizontal(v)){
            board[v.row][v.column + i] = car; 
        }
        else{
            board[v.row + i][v.column] = car;
        }
    }
}

/**
* IsCar  method that indcates whether a vehicle is a car or a truck
*
*@return bool is a a car?
*
*@param v a vehicle
*
*@pre vehicle v
*
*@post whether or not they vehicle is a car
*
**/
bool isCar(const Vehicle& v){
    return v.length == CAR;
}


/**
* IsHorizontal method that indcates whether a vehicle is horizontal
*
*@return bool is a horizontal?
*
*@param v a vehicle
*
*@pre vehicle v
*
*@post whether or not they vehicle is horizontal
*
**/
bool isHorizontal(const Vehicle& v){
    return v.orientation == HORIZONTAL;
}

/**
* print  method that prints the board
*
*@return void
*
*@param board board that the game is played on
*
*@pre a const board
*
*@post a printed 2d array
*
**/
void print(const int board[][MAX_ARR]){
    for(int i = 0; i <  6; i ++){
        for(int j = 0; j < 6; j ++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
* fillArray  method that populates the board with 0's
*
*@return void
*
*@param board board that the game is played on
*
*@pre and empty board 
*
*@post a 2d array filled with zeros
*
**/
void fillArray(int board[][MAX_ARR]){
    for(int i = 0; i <  MAX_ARR; i ++){
        for(int j = 0; j < MAX_ARR; j ++){
            board[i][j] = 0;
        }
    }
}


/**
* isCollissionForward  method that indcates whether or not moving a vehicle forward results in a collision
*
*@return bool indicating collision course
*
*@param board board that the game is played on
*
*@param v a vehicle
*
*@pre vehicle v, 2d board
*
*@post a boolean value indicating collision
*
**/
bool isCollisionForward(const Vehicle& v, const int board[][MAX_ARR]){
    if(isHorizontal(v)){
        if(isCar(v)){
            if(board[v.row][v.column + CAR] != 0){
                return true;
            }
        }
        else{
            if(board[v.row][v.column + TRUCK] != 0){
                return true;
            }
        }
    }
    else{
        if(isCar(v)){
            if(board[v.row + CAR][v.column] != 0){
                return true;
            }
        }
        else{
            if(board[v.row + TRUCK][v.column] != 0){
                return true;
            }
        }
    }
    return false;
}
/**
* isCollissionForward  method that indcates whether or not moving a vehicle backwards results in a collision
*
*@return bool indicating collision course
*
*@param board board that the game is played on
*
*@param v a vehicle
*
*@pre vehicle v, 2d board
*
*@post a boolean value indicating collision
*
**/
bool isCollisionBackward(const Vehicle& v, const int board[][MAX_ARR]){
    if(isHorizontal(v)){
        if(isCar(v)){
            if(board[v.row][v.column - 1] != 0){
                return true;
            }
        }
        else{
            if(board[v.row][v.column - 1] != 0){
                return true;
            }
        }
    }
    else{
        if(isCar(v)){
            if(board[v.row - 1][v.column] != 0){
                return true;
            }
        }
        else{
            if(board[v.row - 1][v.column] != 0){
                return true;
            }
        }
    }
    return false;
}

/**
*MoveForward  method that indcates whether or not moving a vehicle forward is legal
* and moves the car forward if so
*
*@return bool indicating if the vehicle was moved forward
*
*@param board board that the game is played on
*
*@param v a vehicle
*
*@pre vehicle v, 2d board
*
*@post a boolean value indicating if the car was moved. A car in a new position on the board.
*
**/
bool moveForward(Vehicle& v, int board[][MAX_ARR]){
    
    int set = board[v.row][v.column];
    if(isHorizontal(v)){
        if(isCar(v)){
            if(v.column + CAR < MAX_ARR){
                if(!isCollisionForward(v, board)){
                    board[v.row][v.column] = 0;
                    v.column++;
                    for(int i = 0; i < CAR; i++){
                        board[v.row][v.column + i] = set;
                    }
                    return true;
                }
            }
        }
        else{
            if(v.column + TRUCK < MAX_ARR){
                if(!isCollisionForward(v, board)){
                    board[v.row][v.column] = 0;
                    v.column++;
                    for(int i = 0; i < TRUCK; i++){
                        board[v.row][v.column + i] = set;
                    }
                    return true;
                }
            }
        }
    }
    else{
        if(isCar(v)){
            if(v.row + CAR < MAX_ARR){
                if(!isCollisionForward(v, board)){
                    board[v.row][v.column] = 0;
                    v.row++;
                    for(int i = 0; i < CAR; i++){
                        board[v.row + i][v.column] = set;
                    }
                    return true;
                }
            }
        }
        else{
            if(v.row + TRUCK < MAX_ARR){
                if(!isCollisionForward(v, board)){
                    board[v.row][v.column] = 0;
                    v.row++;
                    for(int i = 0; i < TRUCK; i++){
                        board[v.row + i][v.column] = set;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}


/**
*MoveBackward  method that indcates whether or not moving a vehicle backward is legal
* and moves the car forward if so
*
*@return bool indicating if the vehicle was moved backward
*
*@param board board that the game is played on
*
*@param v a vehicle
*
*@pre vehicle v, 2d board
*
*@post a boolean value indicating if the car was moved. A car in a new position on the board.
*
**/
bool moveBackward(Vehicle& v, int board[][MAX_ARR]){
    int set = board[v.row][v.column];
        //cout << "attempt to move " << set << " backward: "<<endl;
        if(isHorizontal(v)){
            if(isCar(v)){
                if(v.column - 1 >= 0){
                    if(!isCollisionBackward(v, board)){
                        board[v.row][v.column + 1] = 0;
                        v.column--;
                        for(int i = 0; i < CAR; i++){
                            board[v.row][v.column + i] = set;
                        }
                        return true;
                    }
                }
            }
            else{
                if(v.column - 1 >= 0){
                    if(!isCollisionBackward(v, board)){
                        board[v.row][v.column + 2] = 0;
                        v.column--;
                        for(int i = 0; i < TRUCK; i++){
                            board[v.row][v.column + i] = set;
                        }
                        return true;
                    }
                }
            }
        }
        else{
            if(isCar(v)){
                if(v.row - 1 >= 0){
                    if(!isCollisionBackward(v, board)){
                        board[v.row + 1][v.column] = 0;
                        v.row--;
                        for(int i = 0; i < CAR; i++){
                            board[v.row + i][v.column] = set;
                        }
                        return true;
                    }
                }
            }
            else{
                if(v.row - 1 >= 0){
                    if(!isCollisionBackward(v, board)){
                        board[v.row + 2][v.column] = 0;
                        v.row--;
                        for(int i = 0; i < TRUCK; i++){
                            board[v.row + i][v.column] = set;
                        }
                        return true;
                    }
                }
            }
        }
        return false;
}


/**
*isComplete used as base case. Determines whether or not to still play the game.
*
*@return boolean Whether or not the first car is at the far right position
*
*@param board board that the game is played on
*
*@param v a vehicle
*
*@pre vehicle v, 2d board
*
*@post a boolean value indicating if the game is complete.
*
**/
bool isComplete(const Vehicle& v, const int board[][MAX_ARR]){
    
    if(isHorizontal(v)){
        if(isCar(v)){
            if(v.column + CAR == MAX_ARR)
                return true;
        }
        else{
            if(v.column + TRUCK == MAX_ARR){
                return true;
            }
        }
        return false;
    }
    else{
        if(isCar(v)){
            if(v.row + CAR == MAX_ARR)
                return true;
        }
        else{
            if(v.row + TRUCK == MAX_ARR){
                return true;
            }
        }
        return false;
    }
}

/**
*solve  method that recursivley checks every possible move and calculates the minimum possible 
*moves it requires to complete the game (if such moves exist)
*
*@return void
*
*@param board board that the game is played on
*
*@param cars an array containing every car on the board
*
*@param numMoves the number of moves currently used
*
*@param best the best score thus far
*
*@param result indicates whether or not the puzzle is solvable
*
*@param numCars number of cars currently on the board
*
*@pre vehicle v, 2d board
*
*@post a boolean value indicating if the car was moved. A car in a new position on the board.
*
**/

void solve(int numMoves, Vehicle cars[], int board[][MAX_ARR], int& best, const int& numCars, bool& result){
    if(isComplete(cars[0], board) || numMoves > best){
        if(numMoves < best){
            best = numMoves;
            result = true;
            return;
        }
        else{
            return;
        }
        //return;
    }
    else{ 
        for(int i = 0; i < numCars; i++){
            if(moveForward(cars[i], board)){
                solve(numMoves + 1, cars, board, best, numCars, result);
                moveBackward(cars[i], board);
            }
            
            if(moveBackward(cars[i], board)){
                solve(numMoves +  1, cars, board, best, numCars, result);
                moveForward(cars[i], board);
            }

        }
    return;
    }
}
