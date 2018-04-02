/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enemy.cpp
 * Author: marcos
 * 
 * Created on 6 de marzo de 2018, 13:54
 */

#include "Enemy.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Enemy::Enemy(Vector2f p, Vector2f v, Texture &text, IntRect init) {
    pos = p;
    vel = v;
    
    Sprite* a = new Sprite(text);
    a->setTextureRect(init);
    a->setOrigin({a->getGlobalBounds().width/2,a->getGlobalBounds().height/2});
    a->setPosition(pos.x, pos.y);

    cout << "Sprite &sp: " << &sp << endl;
    cout << "pos " << p.x << ", " << p.y << endl;
    sp = a;
    sp->move(vel);
    
}

Enemy::Enemy(const Enemy& orig) {
}

Enemy::~Enemy() {
}

Sprite Enemy::getSprite() {
  return *sp;  
}

void Enemy::changeSprite(IntRect a){
    sp->setTextureRect(a);
}

void Enemy::setMove(Vector2f v){
    vel = v;
    sp->move(vel);
}

