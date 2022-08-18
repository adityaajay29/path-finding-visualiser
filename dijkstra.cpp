#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <functional>
#include <math.h>    
#include <float.h>
#include <vector>
#include <string.h>
#include <string>
#include <sstream>
#include <queue>
using namespace std;
using namespace sf;

// nber of rows and cols (n * n)
#define n 60

// 8 directions to explore from all points
vector<int> dx{-1, -1, 0, 1, 1, 1, 0, -1};
vector<int> dy{0, 1, 1, 1, 0, -1, -1, -1};

// to store the shortest path
vector<pair<int,int> > destPath;
bool visited[n][n];

// storing the min path cost
int cost;

// function to validate the current cocordinates
bool isValid(int grid[n][n], int x, int y)
{
    return grid[x][y] == 1 && visited[x][y] == 0;
}

void findPath(pair<int, int> previous[n][n], float distance[n][n], int xStart, int yStart, int xEnd, int yEnd)
{
    cost = distance[xEnd][yEnd];
    cout<<"the shortest distance from start to end is "<<cost<<endl;

    // as long as destination is not reached, print the current coordinats
    while(previous[xEnd][yEnd].first != xStart || previous[xEnd][yEnd].second != yStart)
    {
        // adding delay of 1 ms for exploration
        sleep(milliseconds(1));

        int x = previous[xEnd][yEnd].first;
        int y = previous[xEnd][yEnd].second;
        cout<<"visiting x-coordinate : "<<x<<" and y-coordinate "<<y<<endl;
        destPath.push_back({x, y});
        int currX = xEnd;
        int currY = yEnd;
        xEnd = previous[currX][currY].first;
        yEnd = previous[currX][currY].second;
    }
}


void dijkstra(int grid[n][n], int xStart, int yStart, int xEnd, int yEnd)
{
    // to keep track of the minimum distance from source cell to any cell
    float distance[n][n];
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            distance[i][j]=FLT_MAX;
        }
    }
    // to store the current cells in the path
    pair<int, int> previous[n][n];

    // for greedy algorithm, we use min heap to store the {distance of cell, x coordinate, y coordinate}
    priority_queue<pair<float, pair<int, int> >, vector<pair<float, pair<int, int> > >, greater<pair<float, pair<int, int> > > > pq;

    distance[xStart][yStart] = 0.0;
    pq.push({distance[xStart][yStart], {xStart, yStart}});

    // applying BFS
    while(!pq.empty())
    {
        float celldistance = pq.top().first;
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        pq.pop();

        // mark current as visited
        visited[x][y] = true;

        // if we reach the destination, just come out of BFS
        if(visited[xEnd][yEnd])
        break;

        // adding delay of 1 ms for exploration
        sleep(milliseconds(1));

        // looking in all 8 directions
        for(int i=0;i<8;i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];
            if(isValid(grid, newX, newY))
            {
                if(distance[newX][newY] > celldistance + 1.0)
                {
                    distance[newX][newY] = celldistance + 1.0;
                    pq.push({distance[newX][newY], {newX, newY}});
                    previous[newX][newY] = {x, y};
                    // cout<<previous[newX][newY].first<<" "<<previous[newX][newY].second<<endl;
                }
            }
        }
    }
    findPath(previous, distance, xStart, yStart, xEnd, yEnd);
}

int main(){
    int filled[n][n];       //whether cell is colored
    int grid[n][n];       //map with obstacle
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            if(i==0||i==n-1||j==0||j==n-1)        //walls
                grid[i][j]=0;
            else
                grid[i][j]=1;
        }
     for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            visited[i][j]=false;     //dijkstra all unexplored
            filled[i][j]=0;     //all uncolored
        }     //Astar all unexplored
    int source_x=2,source_y=2,dest_x=45,dest_y=46;      //Origin(2,3)->Goal(50,56)
    Thread threadD(std::bind(&dijkstra,grid,source_x,source_y,dest_x,dest_y));
    RenderWindow window(VideoMode(1080,720),"Grid");
    // // Text
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text1("DIJKSTRA",font,15);

    // Shapes
    RectangleShape buttonStartD(Vector2f(75,25));       //button dijkstra
    buttonStartD.setFillColor(Color::Green);
   
    RectangleShape rectangle(Vector2f(10,10));      //default box :White
    rectangle.setFillColor(Color::White);
    RectangleShape brectangle(Vector2f(10,10));     //Black box
    brectangle.setFillColor(Color::Black);
    RectangleShape grectangle(Vector2f(10,10));     //Green
    grectangle.setFillColor(Color::Green);
    grectangle.setOutlineThickness(2);
    grectangle.setOutlineColor(Color::Red);
    RectangleShape mrectangle(Vector2f(10,10));     //Magenta
    mrectangle.setFillColor(Color::Magenta);
    mrectangle.setOutlineThickness(2);
    mrectangle.setOutlineColor(Color::Red);
    RectangleShape blueRectangle(Vector2f(10,10));
    blueRectangle.setFillColor(Color::Blue);
    blueRectangle.setOutlineThickness(2);
    blueRectangle.setOutlineColor(Color::Black);
    RectangleShape rrectangle(Vector2f(10,10));
    rrectangle.setFillColor(Color::Red);
    rrectangle.setOutlineThickness(2);
    rrectangle.setOutlineColor(Color::Red);
    RectangleShape yrectangle(Vector2f(10,10));
    yrectangle.setFillColor(Color::Yellow);
    // Display
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type==Event::Closed)
                window.close();
            if(event.type==Event::KeyPressed && event.key.code==Keyboard::Space)
                window.close();
            if(event.type==Event::MouseButtonPressed && event.mouseButton.button==Mouse::Left){
                int X=event.mouseButton.x;
                int Y=event.mouseButton.y;
                int row=Y/10;       //Reversed notion of row & column
                int col=X/10;
                if(grid[row][col]==0&&row<60&&col<60)
                    grid[row][col]=1;
                else if(row<60&&col<60)
                    grid[row][col]=0;
                if(row<60&col<60)cout<<"Cell "<<row<<" , "<<col<<" state is: "<<grid[row][col]<<endl;
                if(X>600&&X<675&&Y>0&&Y<25){
                    threadD.launch();
                }
            }
        }
        window.clear();
        buttonStartD.setPosition(600,0);
        window.draw(buttonStartD);      //Dijkstra launch
        text1.setPosition(600,0);       //Dijkstra text
        window.draw(text1);     //cost text
        stringstream ss1,ss2;
        ss1<<destPath.size();       //int2string
        
        if(!destPath.empty()){
            for(int i=0;i<int(destPath.size());i++){
                grectangle.setPosition(destPath[i].second*10,destPath[i].first*10);     //Reversed notion of row & column
                window.draw(grectangle);        //final destPath
                filled[destPath[i].first][destPath[i].second]=1;
            }
        }
        blueRectangle.setPosition(source_y*10,source_x*10);
        window.draw(blueRectangle);     //source
        filled[source_x][source_y]=1;
        rrectangle.setPosition(dest_y*10,dest_x*10);
        window.draw(rrectangle);        //destination
        filled[dest_x][dest_y]=1;
        for(int i=0;i<=590 ;i+=10)
            for(int j=0;j<=590;j+=10){
                if(grid[i/10][j/10]==0){
                    brectangle.setOutlineThickness(2);
                    brectangle.setOutlineColor(Color::Red);
                    brectangle.setPosition(j,i);
                    window.draw(brectangle);        //User's obstacle
                }
                if(visited[i/10][j/10]==true && filled[i/10][j/10]==0){
                    yrectangle.setOutlineThickness(2);
                    yrectangle.setOutlineColor(Color::Red);
                    yrectangle.setPosition(j,i);
                    window.draw(yrectangle);        // Explored Cells by dijkstra
                }
                if(grid[i/10][j/10]==1 && visited[i/10][j/10]==false && filled[i/10][j/10]==0){     //not in dijkstra & A*
                    rectangle.setOutlineThickness(2);
                    rectangle.setOutlineColor(Color::Red);
                    rectangle.setPosition(j,i);
                    window.draw(rectangle);     //default white cells
                }
            }
        sf::Text text2("the cost is " + to_string(cost), font, 15);
        text2.setPosition(600,30);       //cost text
        window.draw(text2);
        window.display();
    }
    return 0;
}