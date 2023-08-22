#include<iostream>
#include<string>
#include<stdlib.h>

#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<ctime>
#include<fstream>

using namespace std;

//Function for the Introduction Menu
char displayMenu(){
    char diffChoice;
    //Introduction
    cout<<endl<<"Welcome to Minesweeper!"<<endl<<endl;
    cout<<"Select difficulty"<<endl;

    //choice of difficulty
    cout<<"a. Beginner (9 x 9 grid, 10 mines)"<<endl;
    cout<<"b. Intermediate (16 x 16 grid, 40 mines)"<<endl;
    cout<<"c. Expert (16 x 30 grid, 99 mines)"<<endl<<endl;

    cout<<"Select: ";
    cin >> diffChoice;
    cout << endl;
    return diffChoice; //return the chosen value
}

//Function to diplay the board from the array
void display(string* board, int rows, int columns){

    for(int i=0; i<rows; i++){  //loops for rows
        for(int j=0; j<columns; j++){   //for columns
            cout<<board[(i*columns)+j];  //print
        }
        cout<<endl;
    }
}

//Auxiliary Function that helps in adding spaces & labels in rows & columns
void cnr(string* board, int rows, int columns){
    //creates spaces
    board[0] = "  ";
    board[columns-1] = "  ";
    board[(rows-1)*columns] = "  ";
    board[(rows*columns)-1] = "  ";

    for(int j=1; j<columns-1; j++){
        char column = j+96;   //assigns the ascii values for labeling
        board[j] = column;
        board[j] += " ";
    }

    for(int i=1; i<rows-1; i++){
        char row = i+96;      //assigns the ascii values for labeling
        board[i*columns] = row;
        board[i*columns] += " ";
    }
}

//Function that creates hidden tile to hide safe and bomb tile
void generateHidden(string* board, int rows, int columns){

    cnr(board, rows, columns); //call cnr function, for label and ordering

    for(int i=1; i<rows-1; i++){
        for(int j=1; j<columns-1; j++){
            board[(i*columns)+j] = ". ";  //symbol for the hidden tile
        }
    }
}

//Function that places the bombs in the map, and also the digits adjacent in the bombs
void placeBombs(string* board, int rows, int columns, int maxBombs){

    cnr(board, rows, columns);  //call cnr function, for label and ordering

    for(int i=1; i<rows-1; i++){
        for(int j=1; j<columns-1; j++){
            board[(i*columns)+j] = "  "; //spaces in the board
        }
    }

    int bombCount = 1;
    srand(time(0)); // so that rand() would not produce the same output

    while (bombCount<maxBombs){
        //randomly places the bombs in the board
        int r = rand() % (rows-2);
        int c = rand() % (columns-2);

        //place bomb if it still doesnt have one
        if (board[(r+1)*columns + (c+1)] != "* "){

            board[(r+1)*columns + (c+1)] = "* ";
            bombCount++;
        }
    }

    int value=0;

    for (int i=1; i<rows-1; i++){
        for (int j=1; j<columns-1; j++){

            //increment "value" if the position has a bomb
            if(board[(i*columns)+j] != "* "){

                if (board[(i-1)*columns + (j-1)] == "* ") {value++;} //upper left
                if (board[(i-1)*columns + j] == "* ") {value++;}     //above
                if (board[(i-1)*columns + (j+1)] == "* ") {value++;} //upper right

                if (board[(i*columns) + (j-1)] == "* ") {value++;}   //left
                if (board[(i*columns) + (j+1)] == "* ") {value++;}   //right

                if (board[(i+1)*columns + (j-1)] == "* ") {value++;} //lower left
                if (board[(i+1)*columns + j] == "* ") {value++;}     //below
                if (board[(i+1)*columns+ (j+1)] == "* ") {value++;}  //lower right

                if (value!=0){
                    board[i*columns + j] = to_string(value) + " ";
                }

                else{     //place a blank tile with no adjacent mine
                    board[i*columns + j] = "  ";
                }

                value = 0;
            }
        }
    }
}

//Functions that tells if certain position has bomb or not
bool isBomb(string* board, int target){
    if (board[target] == "* ") //check if the chosen position is bomb
        return true;
    else
        return false;
}

//Function that inputs the users desired position in the map
int getTarget(int rows, int columns){
    char r;
    char c;
    cout<<"Input your move separated by a space. (ex. a b)"<<endl;
    cout<<"Position: ";
    cin >> r >> c;


    while(true){
        int targetRow = r-96;  //first letter or targeted position
        int targetColumn = c-96;  ////second letter or targeted position

        //invalid if out in the area of the board
        if( (targetRow>rows-2) || (targetColumn>columns-2) ){
            cout<<"Invalid input, try again. "<<endl;
            cin >> r >> c;
        }
        else{   //return target value
            int target = targetRow*columns + targetColumn;
            return target;
        }
    }
}

//Function to recursively reveal the safe tiles with no adjacent bombs
void reveal(string* top, string* under, int target, int rows, int columns){

    //if chosen tile is blank, then open all adjacent safe tile recursively
    if((under[target] == "  ") && (top[target]) == ". "){

        top[target] = under[target]; //reveal

        reveal(top, under, target-columns, rows, columns); //above
        reveal(top, under, target+columns, rows, columns); //below

        reveal(top, under, target-1, rows, columns); //left
        reveal(top, under, target+1, rows, columns); // right

        reveal(top, under, target-columns-1, rows, columns); //upper left
        reveal(top, under, target-columns+1, rows, columns); // upper right

        reveal(top, under, target+columns-1, rows, columns); //lower left
        reveal(top, under, target+columns+1, rows, columns); // lower right
    }
    else{
        //if the chosen tile is a digit with adjacent bombs
        //does not need to open recursively
        top[target] = under[target]; //reveal
    }
}

//Function that executes the users' action in its target either reveal or flag
void getMove(string* top, string* under, int target, bool &gameOn, int rows, int columns){
    char move;
    cout<<"What would you like to do?"<<endl;
    cout<<"a. reveal"<<endl;
    cout<<"b. flag"<<endl;
    cout<<"Choice: ";
    cin>>move;
    cout<<endl;
    string hide = ".";
    string bomb = "*";

    if (move == 'a'){  //a is chosen to reveal the tile
        if(isBomb(under, target)){
            cout<<"Game Over :("<<endl; //if bomb is under it, game over
            gameOn = false;
        }
        else
            reveal(top, under, target, rows, columns); //reveal if not bomb, continue game
    }

    else if (move == 'b'){ //to flag and unflag
        if (top[target] == ". "){
            top[target] = "F ";  //change the symbol of hidden tile to flag tile
        }
        //revert back to hidden tile if flag tile is flag once more
        else if (top[target] == "F "){
            top[target] = ". ";
        }
    }
    else   //other inputs shows invalid input
        cout<<"Invalid input. Try again."<<endl;
}

//Function to check if the Won by checking all revealed tile excepts for the bomb
bool checkWin(string* top, string* under, int rows, int columns){

    for(int i=1; i<rows-1; i++){
        for(int j=1; j<columns-1; j++){
            int target = i*columns + j;
            if ((isBomb(under, target) == false) && (top[target] == ". ")){ //not a mine pero nakatakip pa
                return false;
            }
        }
    }
    return true;
}

//Function to record the player's score based on time
void record(int timeTaken, char difficulty){
    string name;

    cout<<"Insert the name that you want to put in the leaderboard: ";
    cin>>name;
    cout << endl;

    fstream leaderboard; //using fstream to create, read, write files

    if (difficulty == 'a')
        leaderboard.open("leaderboard1.txt", ios::app); //open txt file
    if (difficulty == 'b')
        leaderboard.open("leaderboard2.txt", ios::app); //open txt file
    if (difficulty == 'c')
        leaderboard.open("leaderboard3.txt", ios::app); //open txt file

    //store name and time
    leaderboard<<name<<endl;
    leaderboard<<timeTaken<<endl;

    leaderboard.close(); //close
}

//Function to display leaderboard of player
void displayLeaderboard(char difficulty){
    fstream leaderboard;
    vector<string> scores{};
    vector< pair<int, string> > records {}; //vector to pairs of time and name

    string line;
    if (difficulty == 'a')
        leaderboard.open("leaderboard1.txt"); //open txt file
    if (difficulty == 'b')
        leaderboard.open("leaderboard2.txt"); //open txt file
    if (difficulty == 'c')
        leaderboard.open("leaderboard3.txt"); //open txt file

    while(getline(leaderboard, line)){
        scores.push_back(line); //stores each line to vector scores
    }

    //using vector scores, make a vector of pairs containing player name and their respective game time
    for(int i=0; 2*i<scores.size(); i++){
        records.push_back(make_pair(stoi(scores[2*i+1]), scores[2*i]));
    }

    sort(records.begin(), records.end());   //sort according to game time

    for(int i=0; i<records.size(); i++){
        cout<<records[i].second<<"\t"<<records[i].first<<endl;  //display
    }
    leaderboard.close();
}

//The Main Function
int main(int argc, char* argv[]){ //command line argument for custom board

    int boardRows, boardColumns, maxBombs;
    char leaderboard;
    bool win =false;

    //for custom board
    if (argc > 1){ // if player inputs additional values in the command line
        //get the indexes of the custom value
        int cus1 = atoi(argv[2]);
        int cus2 = atoi(argv[4]);
        int cus3 = atoi(argv[6]);
        boardRows = cus1+2;
        boardColumns = cus2+2;
        maxBombs = cus3+1;
    }

    else{  //for defult boards, either beginner, intermediate, expert
        char difficulty = displayMenu(); //call function choice for difficulty

        if (difficulty == 'a'){ //if a
            boardRows = 11;
            boardColumns = 11;
            maxBombs = 11;
            leaderboard = 'a';
        }
        else if (difficulty == 'b'){ //if b
            boardRows = 18;
            boardColumns = 18;
            maxBombs = 41;
            leaderboard = 'b';
        }
        else if (difficulty == 'c'){ //if c
            boardRows = 18;
            boardColumns = 32;
            maxBombs = 100;
            leaderboard = 'c';
        }
        else  //other input are invalid
            cout<<"Invalid choice, try again. "<<endl;
    }

    int size=boardRows*boardColumns;
    string top[size];
    string under[size];

    generateHidden(top, boardRows, boardColumns); //call to generate hidden board
    placeBombs(under, boardRows, boardColumns, maxBombs); //call to place the bombs

    bool gameOn = true; //game on

    //!!!!!!!!!!!!!!!!!!!!!!!!!
    //Uncomment the code below for cheat sheet to reveal the hidden bombs
    cout<<"Cheat sheet:"<<endl;
    display(under, boardRows, boardColumns); //print out and revealed the board
    //!!!!!!!!!!!!!!!!!!!!!!!!!

    time_t start = time(0); //start the time

    //continuously play the game until gameOne is false
    while((gameOn) && (win == false)){
        cout<<"Current Board:"<<endl;
        display(top, boardRows, boardColumns); //print board
        int target = getTarget(boardRows, boardColumns); //get the chosen target
        getMove(top, under, target, gameOn, boardRows, boardColumns); //perform an action to the target
        win = checkWin(top, under, boardRows, boardColumns); //check if the player has won
    }

    time_t end = time(0); //end the time
    int timeTaken = end - start; // get the time elapsed
    cout<<"Time Taken: "<<timeTaken<<" seconds."<<endl;

    if(win){ //if the player won
        cout<<"Congratulations for winning the game! "<<endl;
        if (argc > 1)
            cout<<endl;
        else
            record(timeTaken, leaderboard); //show time
    }

    else //if the player lose
        cout<<"Nice try."<<endl<<endl;
        display(under, boardRows, boardColumns);

    //no name will show it there has no record yet
    if (leaderboard == 'a'){
        cout << "Beginner Level Leaderboard:" << endl;
        displayLeaderboard(leaderboard); //show best players by rankings
    }
    if (leaderboard == 'b'){
        cout << "Intermediate Level Leaderboard:" << endl;
        displayLeaderboard(leaderboard); //show best players by rankings
    }
    if (leaderboard == 'c'){
        cout << "Expert Level Leaderboard:" << endl;
        displayLeaderboard(leaderboard); //show best players by rankings
    }
}