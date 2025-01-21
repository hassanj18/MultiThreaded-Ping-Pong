 //g++ main.cpp -o ping_pong  -lsfml-graphics -lsfml-window -lsfml-system -lX11
 #include <semaphore.h>
 #include <SFML/Graphics.hpp> 
 #include<pthread.h>
 #include<unistd.h>
 #include <X11/Xlib.h>
 #include<string>

float PADDLE_WIDTH=10;
float PADDLE_HEIGHT=100;
int WINDOW_HEIGHT=500;
int WINDOW_WIDTH=800;
float PADDLE_SPEED=1;
float BALL_RADIUS=10;
    float deltaX=2;
    float deltaY=2;
int PLAYER1_SCORE=0;
int PLAYER2_SCORE=0;
using namespace sf;
sem_t BallPaddleSem;
bool Point=false;
bool Quit=false;
class Ball
{
public:
    int x;
    int y;

    sf::CircleShape shape;
public:
     Ball(int top_x = 0, int top_y = 0) : x(top_x), y(top_y) {
        shape.setRadius(BALL_RADIUS); 
        shape.setPosition(static_cast<float>(x), static_cast<float>(y)); 
        shape.setFillColor(sf::Color::White);
        }
    ~Ball(){}
    void move(){

                 sleep(sf::milliseconds(5));
          sem_wait(&BallPaddleSem);

        if(y+BALL_RADIUS>=WINDOW_HEIGHT) deltaY *=-1;
        if(y<0) deltaY *=-1;

        if(x+BALL_RADIUS>=WINDOW_WIDTH){
             PLAYER1_SCORE++;
             x=400;
             y=250;
         deltaX*=-1;
        Point=true;
         }
        if(x<0){
            PLAYER2_SCORE++;
              x=400;
              y=250;
            deltaX*=-1;
            Point=true;
        }

        x+=deltaX;
        y+=deltaY;
        shape.setPosition(static_cast<float>(x), static_cast<float>(y));

          sem_post(&BallPaddleSem);
    }
};


class Paddle
{
public:
    int x;
    int y;
    sf::RectangleShape shape;
public:
     Paddle(int top_x = 0, int top_y = 0) : x(top_x), y(top_y) {
        shape.setSize(sf::Vector2f(PADDLE_WIDTH,PADDLE_HEIGHT)); 
        shape.setPosition(static_cast<float>(x), static_cast<float>(y)); 
        shape.setFillColor(sf::Color::White);
    }
    void goDown(){
        
        if(y+PADDLE_HEIGHT+PADDLE_SPEED<=WINDOW_HEIGHT){
        y+=PADDLE_SPEED;
        shape.setPosition(static_cast<float>(x), static_cast<float>(y));
        } 
    }
 
    void goUp(){
        if(y-PADDLE_SPEED>=0){
        y-=PADDLE_SPEED;
        shape.setPosition(static_cast<float>(x), static_cast<float>(y)); 
        }
    }
    ~Paddle(){};
};
Paddle P1(0,WINDOW_HEIGHT/2);
Paddle P2(WINDOW_WIDTH-PADDLE_WIDTH,WINDOW_HEIGHT/2);
Ball ball(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
sf::Text Score1;
sf::Text Score2;
int P1_score=0;
int P2_score=0;



void * CheckBallPaddle1Collission(void *){
    while(true){
        sem_wait(&BallPaddleSem);
    // if(ball.x + BALL_RADIUS >= P2.x  && ball.y + BALL_RADIUS >= P2.y  && ball.y <= P2.y+PADDLE_HEIGHT )
    //     deltaX *=-1;
    
    // if(ball.x<P1.x + PADDLE_WIDTH && ball.y +BALL_RADIUS > P1.y && ball.y < P1.y+PADDLE_HEIGHT )
    //     deltaX *=-1;
    if(ball.shape.getGlobalBounds().intersects(P1.shape.getGlobalBounds()))
         deltaX *=-1;
    if(ball.shape.getGlobalBounds().intersects(P2.shape.getGlobalBounds()))
         deltaX *=-1;
        
        sem_post(&BallPaddleSem);
    
    }
        return NULL;

}

void* HandleKeyPresses(void*){

while(1){
    
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){        
            Quit=true;
    }
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){   
        sem_wait(&BallPaddleSem);
                P1.goDown();
        sem_post(&BallPaddleSem);

                sleep(sf::milliseconds(1));
     }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        sem_wait(&BallPaddleSem);
                P1.goUp(); 
        sem_post(&BallPaddleSem);

                 sleep(sf::milliseconds(1));
    }

     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        sem_wait(&BallPaddleSem);

                P2.goDown();
        sem_post(&BallPaddleSem);

                 sleep(sf::milliseconds(1));
     }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        sem_wait(&BallPaddleSem);
                P2.goUp();
        sem_post(&BallPaddleSem);

         sleep(sf::milliseconds(1));
    }

}
    return NULL;
}

void SetScoreValues(sf::Text& t,int score){
    switch (score)
    {
    case 0:
     t.setString("Zero");
        break;
    case 1:
     t.setString("One");
        break;
    case 2:
     t.setString("Two");
        break;
    case 3:
     t.setString("Three");
        break;
    case 4:
     t.setString("Four");
        break;
    case 5:
     t.setString("Five");
        break;
    
    default:
        break;
    }
}
int main()
{   
    XInitThreads();
    sem_init(&BallPaddleSem,0,1);
    pthread_t key_press_thread;
        pthread_attr_t attr1;
        pthread_attr_init(&attr1);
        pthread_create(&key_press_thread,&attr1,&HandleKeyPresses,NULL);

    pthread_t paddle_coll_thread;
        pthread_attr_t attr2;
        pthread_attr_init(&attr2);
        pthread_create(&paddle_coll_thread,&attr1,&CheckBallPaddle1Collission,NULL);
        


        sf::Font font;
            if (!font.loadFromFile("tacone-regular.otf"))
            {
            // error...
            }
            Score1.setFont(font);
            Score2.setFont(font);

   
     Score1.setString("Zero");
        Score1.setPosition(float(330),float(10));
        Score1.setCharacterSize(24);
        Score1.setFillColor(sf::Color::White);

    Text dash;
        dash.setString("-");
            dash.setFont(font);
    dash.setPosition(float(390),float(10));
    dash.setCharacterSize(24);
    dash.setFillColor(sf::Color::White);
    
        
     Score2.setString("Zero");
        Score2.setCharacterSize(24);
        Score2.setPosition(float(410),float(10));
        Score2.setFillColor(sf::Color::White);
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ping Pong!");
    while (window.isOpen())
    {

        
        while(1){
            if(Quit) {
    window.clear();
        Text message;    
        message.setPosition(float(250),float(200));
        message.setCharacterSize(50);
        message.setFillColor(sf::Color::White);
        message.setFont(font);
        sleep(sf::milliseconds(500));
        message.setString("Quiting  Game!");
        window.draw(message);
        window.display();
                break;
                }
            if(PLAYER1_SCORE==5){
    window.clear();
                
        Text message;    
        message.setPosition(float(250),float(200));
        message.setCharacterSize(50);
        message.setFillColor(sf::Color::White);
        message.setFont(font);
        sleep(sf::milliseconds(500));
        message.setString("Player   1   Wins");
        window.draw(message);
        window.display();
                break;
            }
            else if(PLAYER2_SCORE==5){
    window.clear();
        Text message;    
        message.setPosition(float(250),float(200));
        message.setCharacterSize(50);
        message.setFillColor(sf::Color::White);
        message.setFont(font);
        sleep(sf::milliseconds(500));
        message.setString("Player   2     Wins");
        window.draw(message);
        window.display();
                break;
            }
        sf::Event event;
        while (window.pollEvent(event))
        {
           if (event.type == sf::Event::Closed)
               window.close();
                  
            
                
        }
        
        window.clear();
        window.draw(P1.shape);
        window.draw(P2.shape);
        SetScoreValues(Score1,PLAYER1_SCORE);
        SetScoreValues(Score2,PLAYER2_SCORE);
        window.draw(Score1);
        window.draw(Score2);
        window.draw(dash);
        //CheckBallPaddle1Collission(NULL);
        if(Point){
        Text count;    
        int x=280;
        count.setPosition(float(x),float(180));
        count.setCharacterSize(80);
        count.setFillColor(sf::Color::White);
            count.setFont(font);
                 sleep(sf::milliseconds(500));
        count.setString("3");
        window.draw(count);
        window.display();
        sleep(sf::milliseconds(500));
        
        x+=80;
        count.setPosition(float(x),float(180));
        count.setString("2");window.draw(count);
        window.display();
        sleep(sf::milliseconds(500));
       
        x+=80;
        count.setPosition(float(x),float(180));
        count.setString("1");
        window.draw(count);
        window.display();
        sleep(sf::milliseconds(500));

        x+=80;
        count.setPosition(float(x),float(180));
        count.setString("!!!");window.draw(count);
        window.display();
        sleep(sf::milliseconds(500));

        Point =false;
        }
        window.draw(ball.shape);
        ball.move();
        window.display();
        }
        sleep(sf::milliseconds(3000));
    window.close();
    }
    return 0;
}
