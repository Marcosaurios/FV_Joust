/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Game.cpp
 * Author: marcos
 * 
 * Created on 13 de febrero de 2018, 17:14
 */
#include <iostream>
#include <vector> // Storing sprites
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <random>
#include <ctime>
#include <cstdlib>

#define kVel 0.018
#define jump 0.04
#define gravity 0.2

using namespace sf;
using namespace std;

struct nivel{
    int round = 0;
    int enemies = 0;
    int killed = 0;
    int tdelay = 0; /* seconds */
    
    bool win = false;
    
    nivel(int r, int e, int k, int t){
        round = r;
        enemies = e;
        killed = k;
        tdelay = t;
    }
    
    
    
};


Game::Game(int w, int h, string t) {
    width = w;
    height = h;
    title = t;
    die = false;
}



Game::Game(const Game& orig) {
}

Game::~Game() {
}

bool Game::collision(Sprite& sprite, RectangleShape& rect6, bool& hit_x, bool& isJumping, bool& tierra,Vector2f& speed){
    bool x = false;
    if(sprite.getGlobalBounds().intersects(rect6.getGlobalBounds()))
    {
        // from LEFT face
        if(sprite.getPosition().x <= rect6.getPosition().x
           && sprite.getPosition().y > rect6.getPosition().y) 
        {
            speed.x=speed.x*-1;
            sprite.setPosition(sprite.getPosition().x-sprite.getGlobalBounds().width/2,sprite.getPosition().y);
            hit_x = true;
        }
        // from RIGHT face
        else if(sprite.getPosition().x >= rect6.getPosition().x+rect6.getGlobalBounds().width
           && sprite.getPosition().y > rect6.getPosition().y)
        {
            speed.x=speed.x*-1;
            sprite.setPosition(sprite.getPosition().x+sprite.getGlobalBounds().width/2,sprite.getPosition().y);
            hit_x = true;
        }

        // from UPPER face
        else if(sprite.getPosition().y <= rect6.getPosition().y+rect6.getGlobalBounds().height)
        {
            sprite.setPosition(sprite.getPosition().x,rect6.getPosition().y-sprite.getGlobalBounds().height/2);
            isJumping = false;
            tierra = true;
        } 
        // from LOWER face
        else if(sprite.getPosition().y >= rect6.getPosition().y+rect6.getGlobalBounds().height)
        {
            speed.y = speed.y*-1;
            sprite.setPosition(sprite.getPosition().x,sprite.getPosition().y+sprite.getGlobalBounds().height/2);
        }
        x = true;
    }
    return x;
}

void Game::inScreen(Sprite& sprite, RenderWindow& window, Vector2f speed )
{
    if(sprite.getPosition().x>window.getSize().x)
    {
        
        // If sprite goes right
        sprite.setPosition(0,sprite.getPosition().y);
    }
    else if(sprite.getPosition().x<0)
    {
        // If sprite goes left
        sprite.setPosition(window.getSize().x,sprite.getPosition().y);
    }
    else if(sprite.getPosition().y<0)
    {
        // If sprite goes up
        speed.y *= -1;
        sprite.setPosition(sprite.getPosition().x,sprite.getPosition().y+sprite.getGlobalBounds().height);
        
    }
}

void Game::moveEnemies(vector<Enemy*> v1, vector<Enemy*> v2){
    for(int i=0; i<v1.size();i++){
        Vector2f speed = v1.at(i)->vel;
        v1.at(i)->setMove(speed);
    }
}

bool Game::fight(Sprite &sprite, Sprite enemy, Clock c){
    bool value = false;
    
    if(sprite.getGlobalBounds().intersects(enemy.getGlobalBounds()))
    {
        if(sprite.getPosition().y<enemy.getPosition().y) value = true;
        else { 
            die=true;
            c.restart();
            cout << "fightea, die: " << die << endl;

        }          
    }
    
    return value;
}

float Game::get_random( float a, float b)
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(a, b);
    return dis(e);
}


void Game::run()
{
    
    // Window (640x480) 550 394
    RenderWindow window(sf::VideoMode(width, height), title);
    // Vector to store sprites pixel positions
    vector<IntRect> SP_walk;
    vector<IntRect> SP_fly;
    vector<IntRect> SE_walk;
    vector<IntRect> SE_fly;
    vector<IntRect> boss;
    SP_walk.push_back(IntRect(8,35,16,20));
    SP_walk.push_back(IntRect(28,35,16,20));
    SP_walk.push_back(IntRect(48,35,16,20));
    SP_walk.push_back(IntRect(69,35,16,20));
    SP_fly.push_back(IntRect(8,14,16,13));
    SP_fly.push_back(IntRect(30,14,16,13));
    SE_walk.push_back(IntRect(303,38,16,20));
    SE_walk.push_back(IntRect(324,38,16,20));
    SE_walk.push_back(IntRect(345,38,16,20));
    SE_walk.push_back(IntRect(368,38,16,20));
    SE_fly.push_back(IntRect(217,38,16,13));
    SE_fly.push_back(IntRect(241,38,16,13));
    boss.push_back(IntRect(8,177,28,11));
    boss.push_back(IntRect(43,180,25,7));
    boss.push_back(IntRect(74,175,24,10));
    
    
    // Rectangles and bounding boxes for collisions
    RectangleShape rect1({60,10});
    rect1.setPosition({0,120});
    rect1.setFillColor(Color(255,255,255,0));
    
    RectangleShape rect2({160,17});
    rect2.setPosition({180,140});
    rect2.setFillColor(Color(255,255,255,0));
    
    RectangleShape rect3({75,10});
    rect3.setPosition({475,120});
    rect3.setFillColor(Color(255,255,255,0));
    
    RectangleShape rect4({110,15});
    rect4.setPosition({0,235});
    rect4.setFillColor(Color(255,255,255,0));
    
    RectangleShape rect5({110,15});
    rect5.setPosition({205,265});
    rect5.setFillColor(Color(255,255,255,0));
    
    RectangleShape rect6({110,15});
    rect6.setPosition({380,220});
    rect6.setFillColor(Color(255,255,255,0));
    
    RectangleShape rect7({75,10});
    rect7.setPosition({475,235});
    rect7.setFillColor(Color(255,255,255,0));
    
    RectangleShape base({550,50});
    base.setPosition({0,344});
    base.setFillColor(Color(255,255,255,0));
    
    ////////////////////////////////////////
    // LOADING RESOURCES
    ////////////////////////////////////////
    
    // SPRITES
    Texture tex_ini;
    if (!tex_ini.loadFromFile("resources/spritesheet.png")) {
        cerr << "Error cargando la imagen sprites.png";
        exit(0);
    }
    // HEART
    Texture heart;
    if( !heart.loadFromFile("resources/heart.png")){
        cerr << "Error cargando la imagen heart.png";
        exit(0);
    }
    
    // BACKGROUND
    Texture bg;
    if (!bg.loadFromFile("resources/joust_bg.png")) {
        cerr << "Background not loaded!";
    }
    Sprite background(bg);
    
    // FONT
    sf::Font font;
    if (!font.loadFromFile("resources/ARCADECLASSIC.TTF"))
    {
        cerr << "Font not loaded!!" << endl;
    }
    
    //////////////////////////////
    // Strings generators
    /////////////////////////////
    Text score;
    score.setFont(font);
    score.setCharacterSize(22);
    score.setColor(Color::Yellow);
    score.setPosition(200,345); // center of score place
    //score.setPosition(250,345);

    
    Text died;
    died.setFont(font);
    //died.setOrigin(died.getLocalBounds().width/2,died.getLocalBounds().height/2);
    died.setCharacterSize(30);
    died.setColor(Color::Yellow);
    died.setString("YOU DIED!");
    died.setPosition(window.getSize().x/2-died.getGlobalBounds().width/2,window.getSize().y/2-died.getGlobalBounds().height/2);
    cout << "rect: " << died.getLocalBounds().left << ", " << died.getLocalBounds().top << endl; 

    
    Text over;
    over.setFont(font);
    over.setCharacterSize(35);
    over.setColor(Color::Yellow);
    over.setString("GAME OVER!");
    over.setPosition(window.getSize().x/2-over.getGlobalBounds().width/2,window.getSize().y/2-over.getGlobalBounds().height/2);
   
    
    Text exit;
    exit.setFont(font);
    exit.setCharacterSize(20);
    exit.setColor(Color::Yellow);
    exit.setString("Please  press  Q  to  exit");
    exit.setPosition(window.getSize().x/2-exit.getGlobalBounds().width/2,window.getSize().y/2-exit.getGlobalBounds().height/2+28);
   
    
    Text appear;
    appear.setFont(font);
    appear.setCharacterSize(20);
    appear.setColor(Color::Yellow);
    appear.setString("Respawning in 1 seconds");
    appear.setPosition(window.getSize().x/2-appear.getGlobalBounds().width/2,window.getSize().y/2-appear.getGlobalBounds().height/2+28);
   
    
    Text loading_txt;
    loading_txt.setFont(font);
    loading_txt.setCharacterSize(30);
    loading_txt.setColor(Color::Yellow);
    loading_txt.setString("Round 1 in 1 seconds");
    loading_txt.setPosition(window.getSize().x/2-loading_txt.getGlobalBounds().width/2,window.getSize().y/2-loading_txt.getGlobalBounds().height/2+28);
   
    
    Text win_txt;
    win_txt.setFont(font);
    win_txt.setCharacterSize(35);
    win_txt.setColor(Color::Yellow);
    win_txt.setString("YOU WIN!");
    win_txt.setPosition(window.getSize().x/2-win_txt.getGlobalBounds().width/2,window.getSize().y/2-win_txt.getGlobalBounds().height/2+28);
   
    ////////////////////////////
    // Timers init
    ////////////////////////////
    Clock c1; // 5s clock to check enemies etc
    Clock enemychange; // change enemy flying sprite
    Clock enemyspawn; // whenever enemy spawns
    int count = 0;
    int count2 = 0;
    Clock respawn; // time spawner for P1
    Clock tscore; // to move score left each .5s
    bool onlyone = false;
    Clock backtimer;
    int intbacktime = 4;
    Clock loading;
    
    //////////////////////////
    
    // ======= Main player sprite =======
    
    //Y creo el spritesheet a partir de la imagen anterior
    Sprite sprite(tex_ini);
    //Cojo el sprite que me interesa por defecto del sheet
    sprite.setTextureRect(sf::IntRect(8, 35, 16, 20));
    //Le pongo el centroide donde corresponde
    sprite.setOrigin({sprite.getGlobalBounds().width/2,sprite.getGlobalBounds().height/2});
    // Lo dispongo en el centro de la pantalla
    sprite.setPosition(rect5.getPosition().x+rect5.getGlobalBounds().width/2, rect5.getPosition().y);
    bool firstspawn = false;
    
    Sprite heart_sp(tex_ini);    
    heart_sp.setTextureRect(sf::IntRect(30, 14, 16, 13));
    heart_sp.setPosition(445,200);
    Sprite heart_sp2(tex_ini);
    heart_sp2.setTextureRect(sf::IntRect(8, 14, 16, 13));
    heart_sp2.setPosition(242,119);
    
    // Positions to respawn player
    // 445,200
    // 35,215
    // 248, 323
    // 242, 119
    vector<Vector2f> positions;
    positions.push_back({445,200});
    positions.push_back({35,215});
    positions.push_back({248,323});
    positions.push_back({242,119});

    
    // Sprites
    int state = 0;
    // 0 walk
    // 1 fly
    int count_spw = 0;
    //int count_spf = 0;
    
    // Jump
    bool isJumping = false;
    Vector2f speed{0,-0.10};
    Clock clock;
    Time ct = clock.getElapsedTime();
    Time pt;
    bool switcher = false;
    
    /*
     Loop:
     * 1. Update
     * 2. Draw
     * 3. Repeat
     */
    
    
    vector<Sprite*> lives;
    
    /* Round values */
    int points = 0;
    int helper = points;
    // Parse int to str << ostringstream
    
    //bool endround = false;
    //bool win = false;
    
    /* Loading round settings */
    nivel nivel0(0 /* round */, 5 /* enemies */, 0 /* killed */, 1/* time delay */);
    nivel nivel1(1,10,0,1);
    
    ////////////////////////////////////
    // ROUND 0 CONFIG
    ////////////////////////////////////
    vector<Enemy*> v_enemies_0;
    Vector2f enepos1 = {-10,10};
    Vector2f enev1 = {0.0,0.0};
        
    /////////////////////////////////////
    // ROUND 1 CONFIG
    /////////////////////////////////////
    vector<Enemy*> v_enemies_1;
    Vector2f enepos2 = {-10,10};
    
    // LIVES
    int livesX = 283;
    int livesY = 352;
    for(int i=0;i<3;i++){
        Sprite *s = new Sprite(heart);
        s->setPosition(livesX,livesY);
        lives.push_back(s);
        livesX += 15;
    }
    
    
     //COLISIONS
    Vector2f pos0 = sprite.getPosition();
    Vector2f pos1 = {0,0};    
    bool hit_x = false;    
    bool tierra = false;
    
    
    int index=0;
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    //               GAME LOOP
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    while (window.isOpen())
    {
        // Jump time stuff
        pt = ct;
        ct = clock.getElapsedTime();
        float dt = ct.asSeconds() - pt.asSeconds();
        
        
        
        ////////////////////////////
        // TIMERS
        ////////////////////////////
        // 5s checker
        float s5 = c1.getElapsedTime().asSeconds();
        if(s5>2){
            // Debugging messages
            
            c1.restart();
            
        }
        if(respawn.getElapsedTime().asSeconds()>3 && die && !lives.empty())
        {
            die=false;
        }
        
        
        // Scores
        /*if(tscore.getElapsedTime().asSeconds()>0.5){
            // score positioning
            
            if(helper != points)
            {
                helper = points;
                int counter = 0;
                while(helper>1)
                {
                    helper = helper / 10;
                    counter ++;
                    cout << score.getPosition().x << ", " << score.getPosition().y << " ::::::: " << counter << endl;
                }
                score.setPosition(score.getPosition().x-counter*10,score.getPosition().y);
                // save latest points
                helper = points;
                tscore.restart();
                cout << "sale" ;
            }
            
        }*/
        ///////////////////////////
        // Enemy spawner LVL 0
        ///////////////////////////
        if(!nivel0.win){
            // Spawn
            if(enemyspawn.getElapsedTime().asSeconds()>nivel0.tdelay && count<nivel0.enemies)
            {
                float v1 = get_random(0.75,2.75);
                float v2 = get_random(0.75,2.75);
                enev1 = {v1,v2};

                // Generates random number for y-axis spawning
                Enemy* ene = new Enemy(enepos1, enev1, tex_ini, SE_fly.at(0));
                int randnumb = rand()%(10-0 + 300) + 0;
                enepos1.y = randnumb;
                
                float max = 0.03;
                float min = 0.01;
                
                Vector2f velocity = {max, min};
                ene->setPosition({ene->pos.x+15,ene->pos.y});
                ene->setMove(velocity);
                ene->visible=true;
                v_enemies_0.push_back(ene);

                enemyspawn.restart();
                count++;
            }
            // Moves
            for(int a=0; !v_enemies_0.empty() && a<v_enemies_0.size();a++)
            {
                // If it can be displayed
                if(v_enemies_0.at(a)->visible)
                {
                    v_enemies_0.at(a)->setMove(v_enemies_0.at(a)->getVel());
                    inScreen(*v_enemies_0.at(a)->sp,window,v_enemies_0.at(a)->vel);
                    
                    if(!die && fight(sprite,v_enemies_0.at(a)->getSprite(),respawn) && !v_enemies_0.empty())
                    {
                        Vector2f posSp = sprite.getPosition();
                        Vector2f posEne = v_enemies_0.at(a)->getSprite().getPosition();
                        
                        delete v_enemies_0[a];
                        v_enemies_0.erase(v_enemies_0.begin()+a);
                        
                        points += 500;
                        nivel0.killed++;
                        //nivel0.win=true;
                        //cout << "tamano: " << v_enemies_0.size() << " :: empty?: " << v_enemies_0.empty() << endl;
                        
                    }
                }
            }
            if(v_enemies_0.empty() && nivel0.killed == nivel0.enemies)
            {
                //ROUND 0 FINISHED
                cout << "round 0 finished " << endl;
                nivel0.win = true;
                intbacktime=6;
                loading.restart();
            }
            // Enemy sprites
            if(enemychange.getElapsedTime().asSeconds()>1.5)
            {
                for(int i=0; i<v_enemies_0.size();i++)
                {
                    index++;
                    if(index>=2)index=0;
                    v_enemies_0.at(i)->changeSprite(SE_fly.at(index));
                    v_enemies_0.at(i)->setMove({v_enemies_0.at(i)->vel.x,v_enemies_0.at(i)->vel.y * -1});
                }
                enemychange.restart();
            }
        }
        ///////////////////////
        // ENEMY SPAWNER LVL 1
        ///////////////////////
        else if(nivel0.win && !nivel1.win && loading.getElapsedTime().asSeconds()>5)
        {
            // Spawn
            if(enemyspawn.getElapsedTime().asSeconds()>nivel1.tdelay && count2<nivel1.enemies)
            {
                float v1 = get_random(0.75,2.75);
                float v2 = get_random(0.75,2.75);
                enev1 = {v1,v2};

                // Generates random number for y-axis spawning
                Enemy* ene = new Enemy(enepos1, enev1, tex_ini, SE_fly.at(0));
                int randnumb = rand()%(10-0 + 300) + 0;
                enepos1.y = randnumb;
                
                float max = 0.03;
                float min = 0.01;
                
                Vector2f velocity = {max, min};
                ene->setPosition({ene->pos.x+15,ene->pos.y});
                ene->setMove(velocity);
                ene->visible=true;
                v_enemies_1.push_back(ene);

                enemyspawn.restart();
                count2++;
            }
            // Moves
            for(int a=0; !v_enemies_1.empty() && a<v_enemies_1.size();a++)
            {
                // If it can be displayed
                if(v_enemies_1.at(a)->visible)
                {
                    v_enemies_1.at(a)->setMove(v_enemies_1.at(a)->getVel());
                    inScreen(*v_enemies_1.at(a)->sp,window,v_enemies_1.at(a)->vel);
                    
                    if(!die && fight(sprite,v_enemies_1.at(a)->getSprite(),respawn) && !v_enemies_1.empty())
                    {
                        Vector2f posSp = sprite.getPosition();
                        Vector2f posEne = v_enemies_1.at(a)->getSprite().getPosition();
                        
                        delete v_enemies_1[a];
                        v_enemies_1.erase(v_enemies_1.begin()+a);
                        
                        points += 500;
                        nivel1.killed++;
                        //nivel0.win=true;
                        //cout << "tamano: " << v_enemies_0.size() << " :: empty?: " << v_enemies_0.empty() << endl;
                        
                    }
                }
            }
            if(v_enemies_1.empty() && nivel1.killed == nivel1.enemies)
            {
                //ROUND 1 FINISHED
                cout << "round 1 finished " << endl;
                nivel1.win = true;
                intbacktime=6;
                loading.restart();
            }
            // Enemy sprites
            if(enemychange.getElapsedTime().asSeconds()>1.5)
            {
                for(int i=0; i<v_enemies_1.size();i++)
                {
                    index++;
                    if(index>=2)index=0;
                    v_enemies_1.at(i)->changeSprite(SE_fly.at(index));
                    v_enemies_1.at(i)->setMove({v_enemies_1.at(i)->vel.x,v_enemies_1.at(i)->vel.y * -1});
                }
                enemychange.restart();
            }
        }
        
        // Score int parser to string
        ostringstream ss;
        ss << points;
        score.setString(ss.str());
        
        // Dies
        if(respawn.getElapsedTime().asSeconds()>4 && die && !lives.empty())
        {   
            respawn.restart();   
        }
        
        
        //////////////////////////////////////
        // IN SCREEN WINDOW?
        //////////////////////////////////////
        inScreen(sprite,window,speed);
        
        /////////////////////////////////////
        // COLISIONS
        /////////////////////////////////////
        bool crec1 = collision(sprite,rect1,hit_x,isJumping,tierra,speed);
        bool crec2 = collision(sprite,rect2,hit_x,isJumping,tierra,speed);
        bool crec3 = collision(sprite,rect3,hit_x,isJumping,tierra,speed);
        bool crec4 = collision(sprite,rect4,hit_x,isJumping,tierra,speed);
        bool crec5 = collision(sprite,rect5,hit_x,isJumping,tierra,speed);
        bool crec6 = collision(sprite,rect6,hit_x,isJumping,tierra,speed);
        bool crec7 = collision(sprite,rect7,hit_x,isJumping,tierra,speed);
        
        // Base surface collision
        if(sprite.getPosition().y+sprite.getGlobalBounds().height/2>base.getPosition().y)
        {
            sprite.setPosition(sprite.getPosition().x,base.getPosition().y-sprite.getGlobalBounds().height/2);
            isJumping = false;
            tierra = true;
        }
        
        // Collide with surfaces 
        else if( !(crec1 || crec2 || crec3 || crec4 || crec5 || crec6 || crec7) && !tierra)
        {
            isJumping = true;
        }
        
        
        //Bucle de obtención de eventos
        sf::Event event;
        while (window.pollEvent(event))
        {
           
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                if(state==0 && !hit_x) // walks and didn't hit any object in X axis
                {
                    speed.x=kVel;
                }
                
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                if(state==0 && !hit_x) // walks and didn't hit any object in X axis
                {
                    speed.x=-kVel;
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Space) )
            {
                
            }
            
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Q:
                            window.close();
                            break;
                        case sf::Keyboard::V:
                            nivel0.win=true;
                            break;
                        case sf::Keyboard::Space:
                            // ADD jump
                            speed.y=-jump;
                            isJumping = true;
                            tierra = false;
                            cout << switcher << endl;
                            sprite.setTextureRect(SP_fly.at(switcher));
                            switcher = !switcher;
                            break;
                        default:
                            std::cout << event.key.code << std::endl;
                            break;
                            
                    }
            }
        }
        
        //////////////////////////////
        // JUMP
        /////////////////////////////
        if(isJumping)
        {
            // if is FLYING -> ADD gravity
            if(dt>0.15f) dt=0.15f;
            speed.y += (gravity * dt);
        }
        // Colision in X-axis
        sprite.move(speed);
        hit_x = false;
        
        
        //////////////////////////////
        // P1 SPRITE MOVEMENT
        //////////////////////////////
        // Check pixels position to change sprites while walks
        pos1 = sprite.getPosition();
        if(pos1.x-pos0.x > 0)
        {
            // if moves to the RIGHT 
            if(isJumping)
            {
                sprite.setTextureRect(SP_fly.at(switcher));
            }
            else
            {
                // walking
                if(count_spw == 4)count_spw = 0;
                sprite.setTextureRect(SP_walk.at(count_spw));
                count_spw++;
            }
            sprite.setScale(1,1);
        }
        else if(pos1.x - pos0.x < 0)
        {
            // if moves to the LEFT
            if(isJumping)
            {
                sprite.setTextureRect(SP_fly.at(switcher));
            }
            else
            {
                if(count_spw == 4)count_spw = 0;
                sprite.setTextureRect(SP_walk.at(count_spw));
                count_spw++;
            }
            sprite.setScale(-1,1);
        }
        pos0 = pos1;   
        
        
        
        
        ///////////////////////////////
        // DRAW
        //////////////////////////////
        window.clear();
        window.draw(background);
        if(!die)
        {
            // ALIVE 
            window.draw(sprite);
            if(!onlyone)
            {
                srand(time(0));
                int min = 0;
                int max = 3;
                int randnumb = min + (rand() % static_cast<int>(max - min + 1));
                cout << positions[randnumb].x << ", " << positions[randnumb].y << endl;
                sprite.setPosition(positions[randnumb]);
                cout << "position sprite: " << sprite.getPosition().x << ", " << sprite.getPosition().y << endl;
                onlyone = true;
                
                isJumping = true;
                tierra = false;
            }
        }
        else
        { 
            if(lives.empty())
            {
                // GAME OVER
                window.draw(over);
                window.draw(exit);
            }
            else
            {
                // YOU DIED ; user lives yet
                if(onlyone && die)
                {
                    int last = lives.size()-1;
                    delete lives.at(last);
                    lives.erase(lives.begin()+last);
                    //cout << "lets seeee" << endl;
                    onlyone = false;
                    //cout << "die: " << die << endl << "onlyone: " << onlyone << endl;
                    
                    //respawn.restart();
                }
                window.draw(died);
                String tpoints = "Respawning in ";

                /*ostringstream pnts;
                int rounded = int(respawn.getElapsedTime().asSeconds()+0.5);
                pnts << rounded;*/
                ostringstream intstr;
                intstr << intbacktime;
                tpoints += intstr.str() + " seconds";
                appear.setString(tpoints);
                window.draw(appear);
                //cout << intstr.str() << endl;
                if(backtimer.getElapsedTime().asSeconds()>1)
                {
                    --intbacktime;
                    if(intbacktime==0)
                    {
                        intbacktime=3;
                    }
                    backtimer.restart();

                }
            } 
        }
        
        if(nivel0.win && loading.getElapsedTime().asSeconds()<5)
        {
            ostringstream tback;
            tback << intbacktime;
            String loading_str = "Round 1 in " + tback.str() + " seconds";
            loading_txt.setString(loading_str);
            window.draw(loading_txt);
            if(backtimer.getElapsedTime().asSeconds()>1)
            {
                --intbacktime;
                backtimer.restart();

            }
        }
        window.draw(rect1);
        window.draw(rect2);
        window.draw(rect3);
        window.draw(rect4);
        window.draw(rect5);
        window.draw(rect6);
        window.draw(rect7);
        window.draw(base);
        window.draw(score);
        
        //window.draw(heart_sp);
        //window.draw(heart_sp2);

        
        // ---- round 0 stuff -----
        if(!v_enemies_0.empty()){
           for(int i=0;i<v_enemies_0.size();i++)
            {
                if (v_enemies_0.at(i)->visible){
                    //cout << &v_enemies_0.at(i) << endl;
                    window.draw(v_enemies_0.at(i)->getSprite());
                }
            } 
        }
        
        // ---- round 1 stuff -----
        if(!v_enemies_1.empty() && nivel0.win){
            //cout << "draw no vacio" << endl;
           for(int i=0;i<v_enemies_1.size();i++)
            {
                if (v_enemies_1.at(i)->visible){
                    //cout << &v_enemies_1.at(i) << endl;
                    window.draw(v_enemies_1.at(i)->getSprite());
                    //cout << "he dibujado el enemigo " << endl;
                }
            } 
        }
        
        if(v_enemies_1.empty() && nivel1.win)
        {
            window.draw(win_txt);
        }
        
        if(!lives.empty()){
            for(int i=0;i<lives.size();i++)
            {
                window.draw(*lives[i]);
            }
        }
        
        
        window.display();
    }

}
