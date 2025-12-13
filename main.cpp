/*
For playing the game:
Press "A" for shifting the ball to the left side,
Press "D" for shifting the ball to the left side,
Lastly if the game over,
Press "R" to restart the game.

*/

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
struct Ob {
    sf::RectangleShape shape;
    bool gate;
    float gx, gw;
};
int main() {
    const int W=800, H=600;
    sf::RenderWindow win(sf::VideoMode(W,H),"Gate Game");
    win.setFramerateLimit(60);
    sf::Font font; 
    font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    sf::Text tScore("",font,24); 
    tScore.setPosition(10,10);
    sf::CircleShape player(20);
    player.setOrigin(20,20);
    player.setFillColor(sf::Color::Green);
    player.setPosition(W/2,H-80);
    std::vector<Ob> obs;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> rx(60,W-60), rg(120,220), rtype(0,1);
    float dt, speed=150, spawn=0, spawnInt=1.6f;
    int score=0; bool over=false;
    sf::Clock clk;
    while(win.isOpen()) {
        dt = clk.restart().asSeconds();
        sf::Event e;
        while(win.pollEvent(e)) {
            if(e.type==sf::Event::Closed) win.close();
            if(e.type==sf::Event::KeyPressed){
                if(e.key.code==sf::Keyboard::Escape) win.close();
                if(e.key.code==sf::Keyboard::R && over){
                    over=false; score=0; obs.clear();
                    player.setPosition(W/2,H-80);
                    speed=150; spawnInt=1.6f;
                }
            }
        }
        if(!over){
            float vx=0;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) vx-=300;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) vx+=300;
            player.move(vx*dt,0);
            player.setPosition(std::clamp(player.getPosition().x,20.f,(float)W-20), player.getPosition().y);
            spawn+=dt;
            if(spawn>=spawnInt){
                spawn=0;
                if(rtype(rng)<0.65f){
                    float gw=rg(rng), gx=rx(rng);
                    gx=std::clamp(gx,gw/2+30.f,(float)W-gw/2-30.f);
                    Ob L{sf::RectangleShape({gx-gw/2,24}),true,gx,gw};
                    L.shape.setFillColor({150,100,40});
                    L.shape.setPosition(0,-30);
                    obs.push_back(L);
                    Ob R{sf::RectangleShape({W-(gx+gw/2),24}),true,gx,gw};
                    R.shape.setFillColor({150,100,40});
                    R.shape.setPosition(gx+gw/2,-30);
                    obs.push_back(R);
                } else {
                    float w=60+rg(rng)/4, x=rx(rng);
                    Ob o{sf::RectangleShape({w,28}),false,0,0};
                    o.shape.setFillColor({200,40,40});
                    o.shape.setPosition(x-w/2,-30);
                    obs.push_back(o);
                }
                speed+=4; 
                if(spawnInt>0.6) spawnInt-=0.03f;
            }
            for(auto &o:obs) o.shape.move(0,speed*dt);
            for(auto &o:obs){
                if(player.getGlobalBounds().intersects(o.shape.getGlobalBounds())){
                    if(o.gate){
                        float L=o.gx-o.gw/2, R=o.gx+o.gw/2, px=player.getPosition().x;
                        if(!(px>L+6 && px<R-6)) over=true;
                    } else over=true;
                }
            }
            for(auto &o:obs) if(o.gate){
                float y=o.shape.getPosition().y;
                if(y>player.getPosition().y && y<player.getPosition().y+40){
                    float L=o.gx-o.gw/2, R=o.gx+o.gw/2, px=player.getPosition().x;
                    if(px>L+6 && px<R-6){
                        score++;
                        o.shape.setPosition(o.shape.getPosition().x,H+300);
                    }
                }
            }
        }
        win.clear({30,30,40});
        for(auto &o:obs) win.draw(o.shape);
        win.draw(player);
        tScore.setString("Score: " + std::to_string(score));
        win.draw(tScore);
        if(over){
            sf::Text T("GAME OVER! Press R",font,32);
            T.setOrigin(T.getLocalBounds().width/2,0);
            T.setPosition(W/2,H/2-40);
            win.draw(T);
            sf::Text FS("Final Score: "+std::to_string(score),font,26);
            FS.setOrigin(FS.getLocalBounds().width/2,0);
            FS.setPosition(W/2,H/2+10);
            win.draw(FS);
        }
        win.display();
    }
}
