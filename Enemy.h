/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enemy.h
 * Author: marcos
 *
 * Created on 6 de marzo de 2018, 13:54
 */

#ifndef ENEMY_H
#define ENEMY_H
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Enemy {
public:
    Vector2f pos;
    Vector2f vel;
    Sprite *sp;
    bool visible;
    
    Enemy(Vector2f p, Vector2f v, Texture &text, IntRect init);
    Enemy(const Enemy& orig);
    virtual ~Enemy();
    Sprite getSprite();
    void changeSprite(IntRect a);
    void setLive(bool a);
    void setMove(Vector2f v);
    void setPosition(Vector2f p);
    Vector2f getVel();
private:

};

#endif /* ENEMY_H */

