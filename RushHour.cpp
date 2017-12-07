/** @file rushhour.cpp
 @author Aaron Mcanerney
@version Revision 1.0
@breif solves the rush hour game using DFS
@details Uses DFS to solve the rush hour puzzle game. uses a struct to hold vehicles
and does everything else with pass by reference methods.
@date 10/3/2017
**/


#include<iostream>

#include<map>
#include<set>
#include<queue>
#include<fstream>
#include<string>

using namespace std;

struct Vehicle{
    int length;
    char orientation;
    int row;
    int column;
};

struct Board{

    Board(int numCars){
        cars = new Vehicle[numCars];
        numMoves = 0;
        fillBoard();
        generateID();
    }

    ~Board(){
        delete [] cars;
    }
    Vehicle* cars;
    void fillBoard(){
        for(int i = 0; i < 36; i++){
            state[i / 6][i % 6] = 0;
        }
    }

    Board& operator=(const Board& other){
        if(this != &other){
            id = "";
            int numMoves = other.numMoves;
            cars = new Vehicle[18];
            for(int i = 0; i < 36; i++){
                state[i / 6][i % 6] = other.state[i / 6][i % 6];
            }
            generateID();
        }
        return *this;
    }

    void stringToBoard(const string s){
        for(int i =0; i< 36; i++){
            id[i] = state[i / 6][i % 6];
        }
    }

    Board(const Board& other){
        id ="";
        cars = new Vehicle[18];
        numMoves = other.numMoves;
        for(int i =0; i< 18; i++){
            cars[i] = other.cars[i];
        }
        fillBoard();
        for(int i = 0; i < 36; i++){
            state[i / 6][i % 6] = other.state[i / 6][i % 6];
        }
        generateID();
    }

    int state[6][6];

    string id;
    int numMoves;
    int numCars;

    void incMoves(){
        numMoves++;
    }
    void generateID(){
        for(int i = 0; i < 36; i++){
            id.push_back(state[i / 6][i % 6] + '0');
        }
    }

    string getID(){
        return id;
    }
    void printID(){
        for(int i = 0; i < 36; i++){
            cout << id[i];
        }
    }
};


//consts for array size, car and truck size, and horizontal check
const int CAR = 2;
const int TRUCK = 3;
const char HORIZONTAL = 'H';
const int MAX_VEHICLE = 18;
const int MAX_ARR = 6;
void read(int board[][MAX_ARR], int& numCars, Vehicle* cars);
void setBoard(int board[][MAX_ARR], const Vehicle& v, const int car);
bool isCar(const Vehicle& v);
void print(const int board[][MAX_ARR]);
void fillArray(int board[][MAX_ARR]);
bool moveForward(Vehicle& v, Board& board);
bool moveBackward(Vehicle& v, Board& board);
bool isComplete(const Vehicle& v, const int board[][MAX_ARR]);
bool isHorizontal(const Vehicle& v);
void solve(int& numMoves, Vehicle* cars, Board& board, int& best,const int& numCars, bool& result);
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
    //declare variables needed for board and cars/trucks
    Board board(MAX_VEHICLE);
    int numCars;

    //fill the array board with ambiguous numbers to start (flag of 0 indicating no car or truck)
    fillArray(board.state);
    //read in the board from stdin
    read(board.state, numCars, board.cars);
    //set up game variables
    int moves = 0;
    int best = 11;
    bool result = false;
    //solve with BFS
    solve(moves, board.cars,board,best, numCars, result);

    //print out whether or not we found a solution
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
    ifstream inFile;
    inFile.open("rush.txt");
    inFile >> numCars;
    for(int i = 0; i < numCars; i++){
        Vehicle v;
        inFile >> v.length >> v.orientation >> v.row >> v.column;
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
* isCollissionBackwards  method that indcates whether or not moving a vehicle backwards results in a collision
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
bool moveForward(Vehicle& v, Board& board){
    int set = board.state[v.row][v.column];
    if(isHorizontal(v)){
        if(isCar(v)){
            if(v.column + CAR < MAX_ARR){
                if(!isCollisionForward(v, board.state)){
                    board.state[v.row][v.column] = 0;
                    v.column++;
                    for(int i = 0; i < CAR; i++){
                        board.state[v.row][v.column + i] = set;
                    }
                    return true;
                }
            }
        }
        else{
            if(v.column + TRUCK < MAX_ARR){
                if(!isCollisionForward(v, board.state)){
                    board.state[v.row][v.column] = 0;
                    v.column++;
                    for(int i = 0; i < TRUCK; i++){
                        board.state[v.row][v.column + i] = set;
                    }
                    return true;
                }
            }
        }
    }
    else{
        if(isCar(v)){
            if(v.row + CAR < MAX_ARR){
                if(!isCollisionForward(v, board.state)){
                    board.state[v.row][v.column] = 0;
                    v.row++;
                    for(int i = 0; i < CAR; i++){
                        board.state[v.row + i][v.column] = set;
                    }
                    return true;
                }
            }
        }
        else{
            if(v.row + TRUCK < MAX_ARR){
                if(!isCollisionForward(v, board.state)){
                    board.state[v.row][v.column] = 0;
                    v.row++;
                    for(int i = 0; i < TRUCK; i++){
                        board.state[v.row + i][v.column] = set;
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
bool moveBackward(Vehicle& v, Board& board){
    int set = board.state[v.row][v.column];
        if(isHorizontal(v)){
            if(isCar(v)){
                if(v.column - 1 >= 0){
                    if(!isCollisionBackward(v, board.state)){
                        board.state[v.row][v.column + 1] = 0;
                        v.column--;
                        for(int i = 0; i < CAR; i++){
                            board.state[v.row][v.column + i] = set;
                        }
                        return true;
                    }
                }
            }
            else{
                if(v.column - 1 >= 0){
                    if(!isCollisionBackward(v, board.state)){
                        board.state[v.row][v.column + 2] = 0;
                        v.column--;
                        for(int i = 0; i < TRUCK; i++){
                            board.state[v.row][v.column + i] = set;
                        }
                        return true;
                    }
                }
            }
        }
        else{
            if(isCar(v)){
                if(v.row - 1 >= 0){
                    if(!isCollisionBackward(v, board.state)){
                        board.state[v.row + 1][v.column] = 0;
                        v.row--;
                        for(int i = 0; i < CAR; i++){
                            board.state[v.row + i][v.column] = set;
                        }
                        return true;
                    }
                }
            }
            else{
                if(v.row - 1 >= 0){
                    if(!isCollisionBackward(v, board.state)){
                        board.state[v.row + 2][v.column] = 0;
                        v.row--;
                        for(int i = 0; i < TRUCK; i++){
                            board.state[v.row + i][v.column] = set;
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

void solve(int& numMoves, Vehicle cars[], Board& board, int& best, const int& numCars, bool& result){
    queue<Board> queue;     //queue containing boards
    map<string, int> map;    //maps an indicated state to the number of moves it takes to reach said state
    queue.push(board);
    map[board.getID()] = numMoves;

    while(!queue.empty()){

        Board parentState(queue.front());
        queue.pop();
        //check if is complete
        if(isComplete(parentState.cars[0], parentState.state) || numMoves > best){
            if(map[parentState.getID()] < best){
                best = parentState.numMoves;
                result = true;
                return;
            }
            else{
                return;
            }
        }
        //if a piece was moved that is considered a new state
        for(int i = 0; i < numCars; i++){
            //move every piece and snapshot the board
            Board forwards(parentState);
            Board backwards(parentState);
            if(moveForward(forwards.cars[i], forwards)){
                forwards.generateID();
                if(map.find(forwards.getID()) == map.end()){
                    forwards.incMoves();
                    map[forwards.getID()] = map[parentState.getID()] + 1;
                    queue.push(forwards);
                }
            }
            if(moveBackward(backwards.cars[i], backwards)){
               backwards.generateID();
                if(map.find(backwards.getID()) == map.end()){
                    backwards.incMoves();
                    map[backwards.getID()] = map[parentState.getID()] + 1;
                    queue.push(backwards);
                }
            }
        }
    }
}


