#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <stack>

// LENGTH OF VECTOR: |V| = sqrt(V.x^2 + V.y^2)
// NORMALIZE VECTOR: U = V / |V|
using namespace std;
using namespace sf;

enum button_states
{
    BTN_IDLE = 0,
    BTN_HOVER,
    BTN_ACTIVE
};

enum movement_states
{
    IDLE = 0,
    MOVING,
    MOVING_LEFT,
    MOVING_RIGHT,
    MOVING_UP,
    MOVING_DOWN
};
class MovementComponent
{ //============================  MovementComponent ====================================================
private:
    sf::Sprite *sprite;
    float maxVelocity;
    float acceleration;
    float deceleration;

    Vector2f velocity;

    // initializer functions

public:
    MovementComponent(sf::Sprite *sprit, float maxvelocity, float acceleration, float deceleration)
        : sprite(sprit), maxVelocity(maxvelocity), acceleration(acceleration), deceleration(deceleration) {}
    ~MovementComponent()
    {
    }
    // Accessors
    const float &getMaxVelocity() const
    {
        return this->maxVelocity;
    }

    const Vector2f &getVelocity() const
    {
        return this->velocity;
    }

    // functions

    const bool getState(const short unsigned state) const
    {
        switch (state)
        {
        case IDLE:
            if (this->velocity.x == 0.f && this->velocity.y == 0.f)
            {
                return true;
            }
            break;
        case MOVING:
            if (this->velocity.x != 0.f && this->velocity.y != 0.f)
            {
                return true;
            }
            break;
        case MOVING_LEFT:
            if (this->velocity.x < 0.f)
            {
                return true;
            }
            break;
        case MOVING_RIGHT:
            if (this->velocity.x > 0.f)
            {
                return true;
            }
            break;
        case MOVING_UP:
            if (this->velocity.y < 0.f)
            {
                return true;
            }
            break;
        case MOVING_DOWN:
            if (this->velocity.y > 0.f)
            {
                return true;
            }
            break;
        }

        false;
    }
    return false;
}

void
move(const float dir_x, const float dir_y, const float &dt)
{
    // Assclerating  a sprite untile reaches the max velocity
    // Accecleration

    this->velocity.x += this->maxVelocity * dir_x;
    this->velocity.y += this->maxVelocity * dir_y;
}
void update(const float &dt)
{
    // Decelerate the sprite and controls the mamximum velocity.
    // moves the sprite.
    if (this->velocity.y > 0.f)
    {
        // Chack Max vlecity of positive y
        if (this->velocity.y > this->maxVelocity)
            this->velocity.y = this->maxVelocity;

        // deceleration Y positive
        this->velocity.y -= deceleration;
        if (this->velocity.y < 0.f)
            this->velocity.y = 0.f;
    }
    else if (this->velocity.y < 0.f)
    {
        // Chack Max vlecity of negitive y

        if (this->velocity.y < -this->maxVelocity)
            this->velocity.y = -this->maxVelocity;

        // deceleration Y -VE
        this->velocity.y += deceleration;
        if (this->velocity.y > 0.f)
            this->velocity.y = 0.f;
    }

    if (this->velocity.x > 0.f)
    {
        // Chack Max vlecity of positive x
        if (this->velocity.x > this->maxVelocity)
            this->velocity.x = this->maxVelocity;

        // deceleration X positive
        this->velocity.x -= deceleration;
        if (this->velocity.x < 0.f)
            this->velocity.x = 0.f;
    }
    else if (this->velocity.x < 0.f)
    {
        // Chack Max vlecity of negitive
        if (this->velocity.x < -this->maxVelocity)
            this->velocity.x = -this->maxVelocity;

        // deceleration x -VE
        this->velocity.x += deceleration;
        if (this->velocity.x > 0.f)
            this->velocity.x = 0.f;
    }

    // Final move
    this->sprite->move(this->velocity * dt); // use this->velocity * dt to make it move at the same speed on all computers
}

;

class AnimationComponent
{ //===========================  ANIMATION COMPONENT  ================================================
private:
    class Animation
    { //===============================  Animation  ============================================================
    public:
        // Varibles
        Texture &textureSheet;
        Sprite &sprite;
        float animationTimer;
        float timer;
        bool done;
        float width;
        float height;
        IntRect startRectSourceSprite; //(235,0,108.33,164.75)
        IntRect currentReack;
        IntRect endRectSourceSprite;

        // Contructor / Distructor
        Animation(Sprite &sprite, Texture &texturSheet, float animationTimer, float start_frames_x, float Start_frames_y, float frames_x, float frames_y, float width, float height) : sprite(sprite), textureSheet(texturSheet), animationTimer(animationTimer), timer(0.f), done(false), width(width), height(height)
        {
            this->startRectSourceSprite = IntRect((start_frames_x)*width, Start_frames_y * height, width, height);
            this->currentReack = this->startRectSourceSprite;
            this->endRectSourceSprite = IntRect(frames_x * width, frames_y * height, width, height);

            this->sprite.setTexture(this->textureSheet, true);
            this->sprite.setTextureRect(this->startRectSourceSprite);
        }
        ~Animation()
        {
        }

        // Accessor

        const bool &isDone() const
        {
            return this->done;
        }

        // Funtions
        const bool &play(const float &dt)
        {
            // update timer
            this->done = false;
            this->timer += 100.f * dt;
            if (this->timer >= this->animationTimer)
            {
                // reset timer
                this->timer = 0.f;
                // Animate
                if (this->currentReack.left < this->endRectSourceSprite.left)
                {
                    this->currentReack.left += this->width;
                }
                else
                {
                    // Reset
                    this->currentReack.left = this->startRectSourceSprite.left;
                    this->done = true;
                }

                this->sprite.setTextureRect(this->currentReack);
            }

            return this->done;
        }

        const bool &play(const float &dt, float mod_percent)
        {
            this->done = false;
            // update timer
            if (mod_percent < 0.5f)
            {
                mod_percent = 0.5f;
            }

            this->timer += mod_percent * 100.f * dt;
            if (this->timer >= this->animationTimer)
            {
                // reset timer
                this->timer = 0.f;
                // Animate
                if (this->currentReack.left < this->endRectSourceSprite.left)
                {
                    this->currentReack.left += this->width;
                }
                else
                {
                    // Reset
                    this->currentReack.left = this->startRectSourceSprite.left;
                    this->done = true
                }

                this->sprite.setTextureRect(this->currentReack);
            }

            return this->done;
        }

        void pause()
        {
        }
        void reset()
        {
            this->timer = 0.f;
            this->currentReack = this->startRectSourceSprite;
            this->sprite;
        }
    };
    Sprite &sprite;
    Texture &textureSheet;
    map<string, Animation *> animations;
    Animation *lastAnimation;
    Animation *priorityAnimation;

public:
    AnimationComponent(Sprite &sprite, Texture &textureSheet) : sprite(sprite), textureSheet(textureSheet), lastAnimation(NULL), priorityAnimation(NULL)
    {
    }
    ~AnimationComponent()
    {
        for (auto &it : this->animations)
        {
            delete it.second;
        }
    }

    // Accessor

    const bool &isDone(const string key) const
    {
        return this->animations[key]->isDone();
    }

    // Functions
    void addAnimation(const string key, float animationTimer, float start_frames_x, float Start_frames_y, float frames_x, float frames_y, float width, float height)
    {
        this->animations[key] = new Animation(this->sprite, this->textureSheet, animationTimer, start_frames_x, Start_frames_y, frames_x, frames_y, width, height);
    }

    void startAnimation(const string animation)
    {
    }
    void pauseAnimation(const string animation)
    {
    }
    void reSetAnimation(const string animation)
    {
    }

    const bool &play(const string Key, const float &dt, const bool priority)
    {
        if (this->priorityAnimation) // If there is a priority animation
        {
            if (this->priorityAnimation == this->animations[key])
            {
                if (this->lastAnimation != this->animations[Key])
                {
                    if (this->lastAnimation == NULL)
                    {
                        this->lastAnimation = this->animations[Key];
                    }
                    else
                    {
                        this->lastAnimation->reset();
                        this->lastAnimation = this->animations[Key];
                    }

                    this->lastAnimation->reset();
                    this->lastAnimation = this->animations[Key];
                }

                // If the priority animation is done, remove it
                if (this->animations[Key]->play(dt))
                {
                    this->priorityAnimation = NULL;
                }
            }
        }
        else // Play animation of no other priority animation is set
        {
            // If this is a priority animation, set it.
            if (priority)
            {
                this->priorityAnimation = this->animations[key];
            }

            if (this->lastAnimation != this->animations[Key])
            {
                if (this->lastAnimation == NULL)
                {
                    this->lastAnimation = this->animations[Key];
                }
                else
                {
                    this->lastAnimation->reset();
                    this->lastAnimation = this->animations[Key];
                }

                this->lastAnimation->reset();
                this->lastAnimation = this->animations[Key];
            }

            this->animations[Key]->play(dt);
        }
        return this->animations[key]->isDone();
    }

    const bool &play(const string Key, const float &dt, const float &modifier, const float &modier_max, const bool priority)
    {
        if (this->priorityAnimation) // If there is a priority animation
        {
            if (this->priorityAnimation == this->animations[key])
            {
                if (this->lastAnimation != this->animations[Key])
                {
                    if (this->lastAnimation == NULL)
                    {
                        this->lastAnimation = this->animations[Key];
                    }
                    else
                    {
                        this->lastAnimation->reset();
                        this->lastAnimation = this->animations[Key];
                    }

                    this->lastAnimation->reset();
                    this->lastAnimation = this->animations[Key];
                }

                // If the priority animation is done, remove it
                if (this->animations[Key]->play(dt, abs(modifier / modier_max)))
                {
                    this->priorityAnimation = NULL;
                }
            }
        }
        else // Play animation of no other priority animation is set
        {
            // If this is a priority animation, set it.
            if (priority)
            {
                this->priorityAnimation = this->animations[key];
            }

            if (this->lastAnimation != this->animations[Key])
            {
                if (this->lastAnimation == NULL)
                {
                    this->lastAnimation = this->animations[Key];
                }
                else
                {
                    this->lastAnimation->reset();
                    this->lastAnimation = this->animations[Key];
                }

                this->lastAnimation->reset();
                this->lastAnimation = this->animations[Key];
            }

            this->animations[Key]->play(dt, abs(modifier / modier_max));
        }
        return this->animations[key]->isDone();
    }
};

class Entity
{ // ====================================  ENTITY  ============================================================

protected:
    Sprite sprite;

    HitBoxComponent *hitboxComponent;
    MovementComponent *movementComponent;
    AnimationComponent *animationComponent;

public:
    // constructor / destructor
    Entity()
    {
        this->hitboxComponent = NULL;
        this->movementComponent = NULL;
        this->animationComponent = NULL;
    }
    virtual ~Entity()
    {
        delete this->hitboxComponent;
        delete this->movementComponent;
        delete this->animationComponent;
    }

    // component functions

    void setTexture(Texture &texture)
    {
        this->sprite.setTexture(texture);
        this->sprite.setScale(Vector2f(100.f, 100.f));
    }
    void setHitBoxComponent(Sprite &sprite, float offset_x, float offset_y, float width, float height)
    {
        this->hitboxComponent = new hitboxComponent(sprite, offset_x, offset_y, width, height);
    }
    void setMovementComponent(const float maxVelocity, const float acceleration, const float deceleration)
    {
        this->movementComponent = new MovementComponent(&this->sprite, maxVelocity, acceleration, deceleration);
    }
    void setAnimationComponent(Texture &textureSheet)
    {
        this->animationComponent = new AnimationComponent(this->sprite, textureSheet);
    }

    // Functions
    virtual void setPosition(const float x, const float y)
    {

        this->sprite.setPosition(x, y);
    }
    virtual void move(const float x, const float y, const float &dt)
    {
        if (this->movementComponent)
        {
            this->movementComponent->move(x, y, dt); // sets Velocity
        }
    }

    virtual void update(const float &dt)
    {
    }
    virtual void draw(RenderTarget &target)
    {

        target.draw(this->sprite);

        if (this->hitboxComponent)
        {
            this->hitboxComponent->render(target)
        }
    };
};

class Enemy: public Entity{
private:
    bool attacking;
    Texture enemyTex;
    Sprite enemy;
    
    if (!enemyTex.loadFromFile("")){
        throw "Could not load enemy.png!";
    }

    enemy.setTexture(enemyTex);
    enemy.setScale(Vector2f(0.2f, 0.2f));


    vector<Sprite> enemies;
    enemies.push_back(Sprite(enemy));
    int enemySpawnTimer = 15;
    
    int hp = 10;
    RectangleShape hpBar;
    hpBar.setFillColor(Color::Red);
    hpBar.setSize(Vector2f(hp * 20, 20));
    hpBar.setPosition(200.f,10.f);

    void setVariable()
    {
        this->attacking = false;
    }
    void setComponents()
    {
    }
public:
    Enemy(){
        this->setVariable();
    }

    virtual ~Enemy(){

    }

   virtual void update(const float &dt)
    {
        for (int i = 0; i < cats.size(); i++)
        {
            enemies[i].move(-7.f, 0.f);
            if (enemies[i].getPosition().x < 0 - enemy.getGlobalBounds().width)
            {
                hp--;
                enemies.erase(enemy.begin() + i);
            }
        }

        if (enemySpawnTimer < 40)
        {
            enemySpawnTimer++;
        }
        if (enemySpawnTimer >= 40)
        {
            enemy.setPosition(window.getSize().x, rand() % int(window.getSize().y - enemy.getGlobalBounds().height));
            enemies.push_back(Sprite(enemy));
            enemySpawnTimer = 0;
        }

        // Collision

        for (int i = 0; i < enemies.size(); i++)
        {
            if (.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
            {
                enemies.erase(enemies.begin() + i);
            }
        }

        //UI
        hpBar.setSize(Vector2f((float)hp * 20.f, 20.f));
    }

    void draw(RenderTarget &target)
    {
        for (int i = 0; i < cats.size(); i++)
        {
            target.draw(enemies[i]);
        }
    }
};

class Player : public Entity
{ //=====================  PLAYER  ============================================================
private:
    // Sprite sprite;
    bool attacking;

    void setVariable()
    {
        this->attacking = false;
    }
    void setComponents()
    {
    }

public:
    // constructors/ destructors
    Player(float x, float y, Texture &texture_sheet)
    {
        this->setVariable();
        this->setPosition(x, y);

        this->setHitBoxComponent(this->sprite, 86.f, 74.f, 86.f, 111.f);
        this->setMovementComponent(350.f, 15.f, 5.f);
        this->setAnimationComponent(texture_sheet);
        this->animationComponent->addAnimation("IDLE", 20.f, 0, 0, 7, 0, 112.f, 105.f);  //(235,0,108.33,164.75)// it si the size of my texture sheert
        this->animationComponent->addAnimation("WALK", 8.f, 0, 0, 7, 0, 112.33f, 114.f); //(235,0,108.33,164.75)// it si the size of my texture sheert
        this->animationComponent->addAnimation("ATTACK", 7.f, 0, 2, 13, 2, 192 * 2, 192);
    }
    // Player(){}
    virtual ~Player()
    {
    }

    // Accessors

    // Functions
    void updateAttack()
    {
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            this->attacking = true;
        }
    }

    void updateAnimation(const float &dt)
    {
        if (this->attacking)
        {
            // Set origin depending on direction
            if (this->sprite.getScale().x > 0.f) // facing left
            {
                this->sprite.setOrigin(96.f, 0.f);
            }
            else
            { // Facing right
                this->sprite.setOrigin(258.f + 96.f, 0.f);
            }

            // Animate and check for animation end
            if (this->animationComponent->play("ATTACK", dt, true))
            {
                this->attacking = false;

                if (this->sprite.getScale().x > 0.f) // facing left
                {
                    this->sprite.setOrigin(0.f, 0.f);
                }
                else
                { // Facing right
                    this->sprite.setOrigin(258.f + 96.f, 0.f);
                }
            }
        }
        if (this->movementComponent->getState(IDLE))
        {
            this->animationComponent->play("IDLE", dt);
        }
        else if (this->movementComponent->getState(MOVING_LEFT))
        {
            if (this->sprite.getScale().x < 0.f)
            {
                this->sprite.setOrigin(0.f, 0.f);
                this->sprite.setScale(1.f, 1.f);
            }

            this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
        }
        else if (this->movementComponent->getState(MOVING_RIGHT))
        {
            if (this->sprite.getScale().x > 0.f)
            {
                this->sprite.setOrigin(258.f, 0.f);
                this->sprite.setScale(-1.f, 1.f);
            }

            this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
        }
        else if (this->movementComponent->getState(MOVING_UP))
        {
            this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
        }
        else if (this->movementComponent->getState(MOVING_DOWN))
        {
            this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
        }
    }

    virtual void update(const float &dt)
    {
        // Entity::update(dt);
        this->movementComponent->update(dt);

        this->updateAttack();

        this->updateAnimation(dt);

        this->hitboxComponent->update();
    }

    void draw(RenderTarget &target)
    {
        target.draw(this->sprite);
    }
};

class Button
{ //=======================================  BUTTON  ===========================================================
private:
    int buttonState;

    RectangleShape button;
    Font font;
    Text text;

    Color textIdle;
    Color texthover;
    Color textactive;
    Color idleColor;
    Color hoverColor;
    Color activeColor;

public:
    Button(float x, float y, float width, float height, Font &font, string text, unsigned character_Size,
           Color text_idle_color, Color text_hover_color, Color text_active_color, Color idleColor,
           Color hoverColor, Color activeColor)
        : idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor), font(font), textIdle(text_idle_color), texthover(text_hover_color), textactive(text_active_color)
    {
        button.setPosition(Vector2f(x, y));
        button.setSize(Vector2f(width, height));
        button.setFillColor(idleColor);

        this->text.setFont(font);
        this->text.setString(text);
        this->text.setFillColor(text_idle_color);
        this->text.setCharacterSize(character_Size);
        this->text.setPosition(
            button.getPosition().x + (button.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
            button.getPosition().y + (button.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f);

        buttonState = BTN_IDLE;
    }
    virtual ~Button()
    {
    }

    const bool isPressed() const
    {
        return buttonState == BTN_ACTIVE;
    }

    void update(const Vector2f mousePos)
    {
        // IDLE
        this->buttonState = BTN_IDLE;

        // HOVER
        if (this->button.getGlobalBounds().contains(mousePos))
        {
            this->buttonState = BTN_HOVER;

            // PRESSED
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                this->buttonState = BTN_ACTIVE;
            }
        }

        if (this->buttonState == BTN_HOVER)
        {
            this->button.setFillColor(hoverColor);
            this->text.setFillColor(this->texthover);
        }
        else if (this->buttonState == BTN_ACTIVE)
        {
            this->button.setFillColor(activeColor);
            this->text.setFillColor(this->textactive);
        }
        else if (this->buttonState == BTN_IDLE)
        {
            this->button.setFillColor(idleColor);
            this->text.setFillColor(this->textIdle);
        }
        else
        {
            this->button.setFillColor(Color::Red);
            this->text.setFillColor(Color::Blue);
        }
    }

    void draw(RenderTarget &target)
    {
        target.draw(this->button);
        target.draw(this->text);
    }
};

class State
{ // ABSTRACT CLASSES ======================  STATE  ==========================================================
private:
protected:
    stack<State *> *states;
    RenderWindow *window;
    map<string, int> *Keys;
    map<string, int> keyBinds;
    bool quit;

    Vector2i mousePosScreen;
    Vector2i mousePosWindow;
    Vector2f mousePosView;
    Vector2i mousePosGrid;

    // resources
    map<string, Texture> textures;

    virtual void setKeyBinds() = 0;

public:
    // constructors / destructors
    State(RenderWindow *window, map<string, int> *Keys, stack<State *> *states) : window(window), Keys(Keys), states(states), quit(false)
    {
    }
    virtual ~State()
    {
    }

    // functions
    const bool getQuit() const
    {
        // using in Game
        return this->quit;
    }
    // virtual void checkForQuit(){
    //     if(Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("CLOSE")))){
    //         this->quit = true;
    //     }
    // }

    virtual void updateMousePosition()
    {
        this->mousePosScreen = Mouse::getPosition();
        this->mousePosWindow = Mouse::getPosition(*this->window);
        this->mousePosView = this->window->mapPixelToCoords(Mouse::getPosition(*this->window));
        // this->mousePosGrid =;
    }
    // virtual void updateInput(const float& dt) = 0;
    virtual void update(const float &dt) = 0;
    virtual void draw(RenderTarget *target = nullptr) = 0;
};

class Editorstate : public State
{
private:
    Font font;

    map<string, Button *> buttons;

    void setBackground()
    {
    }
    void setKeyBinds()
    {
        this->keyBinds["CLOSE"] = this->Keys->at("Escape");

        // this->keyBinds["MOVE_LEFT"] = this->Keys->at("A");
        // this->keyBinds["MOVE_RIGHT"] = this->Keys->at("D");
        // this->keyBinds["MOVE_UP"] = this->Keys->at("W");
        // this->keyBinds["MOVE_DOWN"] = this->Keys->at("S");
    }
    void setFonts()
    {
        if (!this->font.loadFromFile("Fonts\\PublicPixel-E447g.ttf"))
        {
        }
    }
    void setButtons()
    {
    }

public:
    // constructor / destructor
    Editorstate(RenderWindow *window, map<string, int> *Keys, stack<State *> *states) : State(window, Keys, states)
    {
        this->setMusic();
        this->setBackground();
        // this->background.setFillColor(Color::White);
        this->setFonts();
        this->setKeyBinds();

        this->setButtons();
    }
    virtual ~Editorstate()
    {
        for (auto it = this->buttons.begin(); it != buttons.end(); ++it)
        {
            delete it->second;
        }
    }

    // functions
    void updateButtons(const float &dt)
    {
        for (auto &button : buttons)
        {
            button.second->update(this->mousePosView);
        }
    }
    void updateInput(const float &dt)
    {
        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("CLOSE"))))
        {
            this->quit = true;
        }
    }
    void checkForQuit()
    {
        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("CLOSE"))))
        {
            this->quit = true;
        }
    }
    void update(const float &dt)
    {
        this->updateMousePosition();
        // this->updateInput(dt);
        this->checkForQuit();
        this->updateButtons(dt);
    }

    void drawButtons(RenderTarget &target)
    {
        for (auto &button : buttons)
        {
            button.second->draw(target);
        }
    }
    void draw(RenderTarget *target)
    {
        if (!target)
            target = this->window;

        this->drawButtons(target);

        // REMOVE LATER !!!
        Text mouseText;
        mouseText.setPosition(this->mousePosView.x, this->mousePosView.y);
        mouseText.setFont(this->font);
        mouseText.setCharacterSize(12);
        stringstream ss;
        ss << this->mousePosView.x << "  " << this->mousePosView.y;
        mouseText.setString(ss.str());
        target->draw(mouseText);
    }
}

class GameState : public State
{ //====================  GAMESTATE  ==========================================================
private:
    // Entity player;
    Player *player;
    RectangleShape background;
    Texture bgtexture;

    void setbackground()
    {
        background.setSize(Vector2f(window->getSize()));
        bgtexture.loadFromFile("Textures\\background.png");
        background.setTexture(&this->bgtexture);
    }
    void setKeyBinds()
    {
        this->keyBinds["CLOSE"] = this->Keys->at("Escape");

        this->keyBinds["MOVE_LEFT"] = this->Keys->at("A");
        this->keyBinds["MOVE_RIGHT"] = this->Keys->at("D");
        this->keyBinds["MOVE_UP"] = this->Keys->at("W");
        this->keyBinds["MOVE_DOWN"] = this->Keys->at("S");
    }
    void setTexture()
    {
    }
    void setPlayer()
    {
        this->textures["PLAYER_SHEET"].loadFromFile("Textures\\TextureSheet.png");
        this->player = new Player(0, 0, this->textures["PLAYER_SHEET"]);
    }

public:
    // constructors / destructors
    GameState(RenderWindow *window, map<string, int> *Keys, stack<State *> *states) : State(window, Keys, states)
    {
        this->setbackground();
        this->setKeyBinds();
        this->setPlayer();
    }
    virtual ~GameState()
    {
        delete this->player;
    }

    // functions
    void updateInput(const float &dt)
    {
        // this->checkForQuit();

        // check it later whether it work correct or not (now doing same as he is teaching)
        // if later it works then remove setkeybinds and remove all the below stuff
        // if(Keyboard::isKeyPressed(Keyboard::A)){
        //     this->player.move(dt, -10.f, 0.f);
        // }
        // if(Keyboard::isKeyPressed(Keyboard::D)){
        //     this->player.move(dt, 10.f, 0.f);
        // }
        // if(Keyboard::isKeyPressed(Keyboard::W)){
        //     this->player.move(dt, 0.f, -10.f);
        // }
        // if(Keyboard::isKeyPressed(Keyboard::S)){
        //     this->player.move(dt, 0.f, 10.f);
        // }

        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("MOVE_LEFT"))))
        {
            this->player->move(-10.f, 0.f, dt);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("MOVE_RIGHT"))))
        {
            this->player->move(10.f, 0.f, dt);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("MOVE_UP"))))
        {
            this->player->move(0.f, -10.f, dt);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("MOVE_DOWN"))))
        {
            this->player->move(0.f, 10.f, dt);
        }

        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("CLOSE"))))
        {
            this->quit = true;
        }
    }
    void update(const float &dt)
    {
        this->updateMousePosition();
        this->updateInput(dt); // Call updateInput to handle player movement
        this->player->update(dt);
    }

    void draw(RenderTarget *target = NULL)
    {
        if (!target)
            target = this->window;

        target->draw(background);
        // target->draw(&player);
        this->player->draw(*target);
        // if(this->player)
    }
};

class MainMenuState : public State
{ //=================  MAINMENUSTATE  =======================================================
private:
    RectangleShape background;
    Texture bgTexture;
    Font font;
    Text text;

    Music music;

    map<string, Button *> buttons;

    void setBackground()
    {
        this->background.setSize(Vector2f(this->window->getSize()));
        // this->bgTexture.loadFromFile("C:\\Users\\azama\\SFML CODE\\RPG game\\Background.png");
        this->bgTexture.loadFromFile("Textures\\mainmenu.jpg");
        this->background.setTexture(&this->bgTexture);
    }
    void setKeyBinds()
    {
        this->keyBinds["CLOSE"] = this->Keys->at("Escape");

        // this->keyBinds["MOVE_LEFT"] = this->Keys->at("A");
        // this->keyBinds["MOVE_RIGHT"] = this->Keys->at("D");
        // this->keyBinds["MOVE_UP"] = this->Keys->at("W");
        // this->keyBinds["MOVE_DOWN"] = this->Keys->at("S");
    }
    void setFonts()
    {
        if (!this->font.loadFromFile("Fonts\\PublicPixel-E447g.ttf"))
        {
        }
    }
    void setButtons()
    {
        this->buttons["GAME_STATE"] = new Button(100.f, 200.f, 150.f, 50.f, this->font, "NEW GAME", 12,
                                                 Color::Magenta, Color(0, 0, 0), Color::Magenta,
                                                 Color(200, 200, 200), Color(150, 150, 150), Color(0, 0, 255));

        this->buttons["SETTINGS"] = new Button(100.f, 300.f, 150.f, 50.f, this->font, "SETTINGS", 12,
                                               Color::Magenta, Color(0, 0, 0), Color::Magenta,
                                               Color(200, 200, 200), Color(150, 150, 150), Color(0, 0, 255));

        this->buttons["EDITOR"] = new Button(100.f, 400.f, 150.f, 50.f, this->font, "EDITOR", 12,
                                             Color::Magenta, Color(0, 0, 0), Color::Magenta,
                                             Color(200, 200, 200), Color(150, 150, 150), Color(0, 0, 255));

        this->buttons["EXIT"] = new Button(100.f, 500.f, 150.f, 50.f, this->font, "EXIT", 12,
                                           Color::Magenta, Color(0, 0, 0), Color::Magenta,
                                           Color(200, 200, 200), Color(150, 150, 150), Color(0, 0, 255));
    }
    void setMusic()
    {
        music.openFromFile("Music\\PUBG Theme Song (2Scratch Trap Remix).ogg");
        music.play();
    }

public:
    // constructor / destructor
    MainMenuState(RenderWindow *window, map<string, int> *Keys, stack<State *> *states) : State(window, Keys, states)
    {
        this->setMusic();
        this->setBackground();
        // this->background.setFillColor(Color::White);
        this->setFonts();
        this->setKeyBinds();

        this->setButtons();
    }
    virtual ~MainMenuState()
    {
        for (auto it = this->buttons.begin(); it != buttons.end(); ++it)
        {
            delete it->second;
        }
    }

    // functions
    void updateButtons(const float &dt)
    {
        for (auto &button : buttons)
        {
            button.second->update(this->mousePosView);
        }

        if (this->buttons["GAME_STATE"]->isPressed())
        {
            music.stop();
            this->states->push(new GameState(this->window, this->Keys, this->states));
        }

        // Settings
        if (this->buttons["EDITOR_STATE"]->isPressed())
        {
            music.stop();
            this->states->push(new Editorstate(this->window, this->Keys, this->states));
        }

        if (this->buttons["EXIT"]->isPressed())
        {
            this->quit = true;
        }
    }
    // void updateInput(const float& dt){
    //     this->checkForQuit();
    // }
    void checkForQuit()
    {
        if (Keyboard::isKeyPressed(Keyboard::Key(this->keyBinds.at("CLOSE"))))
        {
            this->quit = true;
        }
    }
    void update(const float &dt)
    {
        this->updateMousePosition();
        // this->updateInput(dt);
        this->checkForQuit();
        this->updateButtons(dt);
    }

    void drawButtons(RenderTarget &target)
    {
        for (auto &button : buttons)
        {
            button.second->draw(target);
        }
    }
    void draw(RenderTarget *target)
    {
        if (!target)
            target = this->window;

        target->draw(this->background);
        this->drawButtons(*target);

        // REMOVE LATER !!!
        Text mouseText;
        mouseText.setPosition(this->mousePosView.x, this->mousePosView.y);
        mouseText.setFont(this->font);
        mouseText.setCharacterSize(12);
        stringstream ss;
        ss << this->mousePosView.x << "  " << this->mousePosView.y;
        mouseText.setString(ss.str());
        target->draw(mouseText);
    }
};

class HitBoxComponent
{
private:
    Sprite &sprite;
    RectangleShape hitbox;
    float offsetX;
    float offsetY;

public:
    HitBoxComponent(Sprite &sprite, float offset_x, float offset_y, float, float width, float height) : sprite(sprite), offsetX(offset_x), offsetY(offset_y)
    {
        this->hitbox.setPosition(this->sprite.getPosition().x + offset_x, this->sprite.getPosition().y + offset_y);
        this->hitbox.setSize(Vector2f(width, height));
        this->hitbox.setFillColor(Color::Transparent);
        this->hitbox.setOutlineThickness(1.f);
        this->hitbox.setOutlineColor(Color::Green);
    }

    virtual ~HitBoxComponent()
    {
    }

    // Functions
    bool checkIntersect(const FloatRect &frect)
    {
        return this->hitbox.getGlobalBounds().intersects(frect);
    }

    void update()
    {
        this->hitbox.setPosition(this->sprite.getPosition().x + this->offsetX, this->sprite.getPosition().y + this->offsetY);
    }
    void render(RenderTarget &target)
    {
        target.draw(this->hitbox);
    }
};

class Game
{ // ========================================  GAME  ============================================================
private:
    RenderWindow *window;
    Event event;

    Image icon;

    Clock clock;
    float dt;

    stack<State *> states;
    map<string, int> Keys;

    void setVariables()
    {
    }
    void pollevent()
    {
        while (this->window->pollEvent(this->event))
        {
            if (event.type == Event::Closed)
                window->close();

            if (event.type == Keyboard::isKeyPressed(Keyboard::Escape))
                window->close();
        }
    }
    void setWindow()
    {
        this->window = new RenderWindow(VideoMode(1366, 768), "Gear of War", Style::Default);
        this->window->setFramerateLimit(60);
    }

    void setStates()
    {
        this->states.push(new MainMenuState(this->window, &this->Keys, &this->states));
    }
    void setIcon()
    {
        if (!this->icon.loadFromFile("Textures\\gaming-icon-2.jpg"))
        {
        }
        unsigned int width = icon.getSize().x;
        unsigned int height = icon.getSize().y;
        const sf::Uint8 *pixelData = icon.getPixelsPtr();
        window->setIcon(width, height, pixelData);
    }
    void setKeys()
    {
        // using emplace instead using (pair and insert)
        this->Keys["Escape"] = Keyboard::Key::Escape;
        this->Keys["A"] = Keyboard::Key::A;
        this->Keys["S"] = Keyboard::Key::S;
        this->Keys["D"] = Keyboard::Key::D;
        this->Keys["W"] = Keyboard::Key::W;
    }

public:
    // Constructor / Destructor
    Game()
    {
        this->setWindow();
        this->setVariables();
        this->setKeys();
        this->setStates();
        this->setIcon();
    }
    virtual ~Game()
    {
        delete this->window;
        while (!states.empty())
        {
            delete this->states.top();
            this->states.pop();
        }
    }
    // Accessors
    const bool isWindowOpen() const
    {
        return this->window->isOpen();
    }

    // Functions
    // update
    void updateDt()
    {
        this->dt = this->clock.restart().asSeconds();
    }
    void updateState()
    {
        if (!this->states.empty())
        {
            this->states.top()->update(this->dt);
            if (this->states.top()->getQuit())
            {

                // this->states.top()->endState();
                delete this->states.top();
                this->states.pop();
            }
        }
        else
        {
            this->window->close();
        }
    }
    void update()
    {
        this->pollevent();
        this->updateDt();
        this->updateState();
    }

    // draw
    void drawState()
    {
        if (!this->states.empty())
            this->states.top()->draw();
    }
    void draw()
    {
        window->clear(Color::White);

        this->drawState();

        // drawing finish
        window->display();
    }
};

int main()
{ //========================================  MAIN  ============================================================
    Game game;

    while (game.isWindowOpen())
    {
        // updating
        game.update();

        // drawing
        game.draw();
    }
    return 0;
}
