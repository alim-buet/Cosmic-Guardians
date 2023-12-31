#include "iGraphics.h"
#include <math.h>
#include <stdlib.h>

#define screenX 1200      // 1200
#define screenY 800       // 900
#define enemyLIMIT 10     // 10
#define bulletLIMIT 70    // 70
#define beamLIMIT 6       // 6
#define explosionLIMIT 15 // 15
#define powerupLIMIT 1    // 1
#define PlayerHealth 200  // 200
#define ShieldHealth 200  // 200
#define powerupProb 25    // 25
#define typesOfEnemies 6
#define phyDelay 10
#define nameLen 24

#define speedMod 1.5
#define radMod 50
#define healthMod 2

/************************************


STRUCTS AND INTIALIZATION


************************************/

// Entity struct
typedef struct
{
    // Hitbox
    float rad;

    // Spawn
    int posx;
    int posy;

    // Dynamic stats
    int health;
    int dx;
    int dy;
    int model;
    int points;
} entity;

typedef struct
// Shield struct
{
    // Hitbox
    float rad;

    // Spawn
    int posx;
    int posy;

    // Dynamic stats
    int health;
} shield;

// Projectiles structs
typedef struct
{
    // Spawn
    int posx;
    int posy;
    float rad;
    int r, g, b;
    int dx;
    int dy;

    // Mode and damage
    int damage;
    int mode;
} bullet;

typedef struct
{
    // Spawn
    int posx;
    int posy;
    int r, g, b;
    int timer;
    int thickness;

    // Mode and damage
    int damage;
    int mode;
} beam;

// Explosion struct
typedef struct
{
    // Location and radius of explosion
    int posx;
    int posy;
    float rad_1;
    float rad_2;

    // Color
    int r1, g1, b1;
    int r2, g2, b2;

    // Timer
    int time;
    int mode;
} explosion;

// Powerups struct
typedef struct
{
    // Location and hitbox of powerup
    int posx;
    int posy;
    float rad;
    int dy;

    // Color and stats
    int r, g, b;
    int model;
} powerup;

// Player-Score pair struct
typedef struct
{
    char name[18];
    int score;
} player_data;

// Initializing player, shield entity, and shieldstr, scorestr and healthstr
entity Pilayer;
shield Shield;
char scorestr[15];
char healthstr[15];
char shieldstr[15];
char wepmodstr[15];
char playername[nameLen] = "Player";
int len = strlen(playername);

// Game data
int damageMode;
int spreadMode;
int WepMode;
int score;
int enemyCAP;
int enemyGRADE;
int threshold;

char player_skin[22] = "entities\\player1.bmp";
// 0 = menu
// 1 = scores
// 2 = controls
// 3 = play
char background[4][28] = {"backgrounds\\menubg.bmp", "backgrounds\\scoresbg.bmp", "backgrounds\\controlsbg.bmp", "backgrounds\\spacebg.bmp"};
char enemy_skin[6][24] = {"entities\\druid.bmp", "entities\\fighter.bmp", "entities\\asteroid1.bmp", "entities\\asteroid2.bmp", "entities\\fighterb.bmp", "entities\\fighterc.bmp"};
char buttons[5][28] = {"buttons\\playbutton.bmp", "buttons\\scorebutton.bmp", "buttons\\controlsbutton.bmp", "buttons\\quitbutton.bmp", "buttons\\menubutton.bmp"};
char title[23] = "backgrounds\\title.bmp";
char exp_skin[3][22] = {"entities\\Ephase1.bmp", "entities\\Ephase2.bmp", "entities\\Ephase3.bmp"};
char powerup_skins[5][21] = {"powerups\\health.bmp", "powerups\\damage.bmp", "powerups\\bullet.bmp", "powerups\\speed.bmp", "powerups\\shield.bmp"};
char text_field[2][22] = {"backgrounds\\name.bmp", "backgrounds\\over.bmp"};

// Sound and check
int sound_on = 1;
int is_playing[] = {0, 0};
char music[2][17] = {"sounds\\menu.wav", "sounds\\game.wav"};

// Player highscore data
player_data player_scores[6];

// For highscore display only
char display[5][40];

// Keeping count of enemy and assigning memory slots
int enemy_count = 0;

int active_slots[enemyLIMIT];
entity enemies[enemyLIMIT];

// Keeping count of bullets and beams and assigning memory slots
int bullet_count = 0;

int active_bullets[bulletLIMIT];
bullet bullets[bulletLIMIT];

int beam_count = 0;

int active_beams[beamLIMIT];
beam beams[beamLIMIT];

// Collision / hit explosions
int explosion_count = 0;

int active_explosions[explosionLIMIT];
explosion explosions[explosionLIMIT];

// Powerups
int powerup_count = 0;

int active_powerups[powerupLIMIT];
powerup powerups[powerupLIMIT];

// Searching for empty memory slot
int return_index(int slots[], int lim)
{
    for (int i = 0; i < lim; i++)
    {
        if (slots[i] == 0)
            return i;
    }
    printf("Error in return index function\n");
    return 0;
}

/************************************


GAME STATE AND STATE CONTROLLERS, DATA MANAGERS


************************************/

int paused;
int is_dead;
int taking_name = 0;

int game_state;
void game_pause(void)
{
    iPauseTimer(0);
    iPauseTimer(1);
    iPauseTimer(2);
    iPauseTimer(3);
    paused = 1;
    return;
}

void game_resume(void)
{
    iResumeTimer(0);
    iResumeTimer(1);
    iResumeTimer(2);
    iResumeTimer(3);
    paused = 0;
    return;
}

void game_start(void)
{
    game_resume();

    // Player settings
    Pilayer.posx = 600;
    Pilayer.posy = 40;
    Pilayer.rad = radMod / 1.5;
    Pilayer.dx = 5 * speedMod;
    Pilayer.dy = 5 * speedMod;
    Pilayer.health = PlayerHealth;
    is_dead = 0;

    // Shield settings
    Shield.rad = radMod * 1.5;
    Shield.health = 0;
    Shield.posx = Pilayer.posx;
    Shield.posy = Pilayer.posy;

    strcpy(player_scores[5].name, playername);
    player_scores[5].score = 0;

    enemy_count = 0;
    explosion_count = 0;
    bullet_count = 0;
    powerup_count = 0;

    damageMode = 0; // 0
    spreadMode = 0; // 0
    WepMode = 0;    // 0
    score = 0;
    enemyCAP = 3;   // 3
    enemyGRADE = 1; // 1
    threshold = 50;
    // Initializing slots
    for (int i = 0; i < enemyLIMIT; i++)
        active_slots[i] = 0;
    for (int i = 0; i < bulletLIMIT; i++)
        active_bullets[i] = 0;
    for (int i = 0; i < beamLIMIT; i++)
        active_beams[i] = 0;
    for (int i = 0; i < explosionLIMIT; i++)
        active_explosions[i] = 0;

    game_state = 3;
    taking_name = 0;
    return;
}

void game_stop(void)
{
    if (paused == 0)
        game_pause();
    game_state = 0;
    return;
}

void swapdata(player_data *a, player_data *b)
{
    player_data tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}

void bubble_sort(player_data arr[])
{
    int swap_cond;
    while (1)
    {
        swap_cond = 0;
        for (int i = 0; i < 5; i++)
        {
            if (player_scores[i].score < player_scores[i + 1].score)
            {
                swap_cond = 1;
                swapdata(&player_scores[i], &player_scores[i + 1]);
            }
        }

        if (swap_cond == 0)
            break;
    }
    return;
}

void loadHighscores(void)
{
    FILE *scores = fopen("data\\scores.txt", "r");
    if (scores == NULL)
    {
        printf("Error loading scores file\n");
        exit(1);
    }

    int i = 0;
    while (!feof(scores))
    {
        fscanf(scores, "%s %i", &player_scores[i].name, &player_scores[i].score);
        sprintf(display[i], "%i. %s : %i", i + 1, player_scores[i].name, player_scores[i].score);
        i++;
    }

    fclose(scores);
    return;
}

void uploadHighscores(void)
{
    bubble_sort(player_scores);
    FILE *scorewr = fopen("data\\scores.txt", "w");

    if (scorewr == NULL)
    {
        printf("Failed to update scores.txt\n");
        exit(1);
    }

    for (int i = 0; i < 5; i++)
        fprintf(scorewr, "%s %i\n", player_scores[i].name, player_scores[i].score);

    fclose(scorewr);
    return;
}

void printDebug(void)
{
    printf("\n-----------------------\n");
    printf("\nEntity Count:\n\nEnemies: %i\nBullets: %i\n\n", enemy_count, bullet_count);
    printf("Game Data:\n\ndamageMode: %i\nspreadMode: %i\nwepomode: %i\nenemyGRADE = %i\nenemyCAP = %i\nthreshold = %i\n", damageMode, spreadMode, WepMode, enemyGRADE, enemyCAP, threshold);
    printf("Phealth: %i\nShealth: %i\nScore: %i\n", Pilayer.health, Shield.health, score);
    return;
}

/************************************


SPAWNERS


************************************/

// Spawning enemies
void spawnEnemy(void)
{
    if (enemy_count >= enemyCAP)
        return;

    // Finding empty memory slot
    int index = return_index(active_slots, enemyLIMIT);
    active_slots[index] = 1;
    enemy_count++;

    // Setting enemy spawn point
    enemies[index].posy = screenY - 10;
    enemies[index].posx = (rand() % 1000) + 100;

    // 0 = druid disk // 1 = fighter // 2 = asteroid // 3 = big asteroid // 4 = bounty hunter
    // 5 = beam hunter
    int mode = rand() % enemyGRADE;
    // mode = 5;

    switch (mode)
    {
    case 0:
        enemies[index].health = 10 * healthMod;
        enemies[index].dx = speedMod * (3 + rand() % 5);
        enemies[index].dy = -speedMod * (3 + rand() % 5);
        enemies[index].rad = radMod / 2.6;
        enemies[index].model = mode;
        if (rand() % 2)
            enemies[index].dx = -enemies[index].dx;
        break;
    case 1:
        enemies[index].health = 20 * healthMod;
        enemies[index].dx = 0;
        enemies[index].dy = -speedMod * 3;
        enemies[index].rad = radMod / 2;
        enemies[index].model = mode;
        break;
    case 2:
        enemies[index].health = 40 * healthMod;
        enemies[index].dx = 0;
        enemies[index].dy = -speedMod * 3;
        enemies[index].rad = radMod / 1.8;
        enemies[index].model = mode;
        break;
    case 3:
        enemies[index].health = 80 * healthMod;
        enemies[index].dx = 0;
        enemies[index].dy = -speedMod * 2;
        enemies[index].rad = radMod;
        enemies[index].model = mode;
        break;
    case 4:
        enemies[index].health = 50 * healthMod;
        enemies[index].dx = 0;
        enemies[index].dy = -speedMod * 3;
        enemies[index].rad = radMod / 1.8;
        enemies[index].model = mode;
        break;
    case 5:
        enemies[index].health = 50 * healthMod;
        enemies[index].dx = 0;
        enemies[index].dy = -speedMod * 3;
        enemies[index].rad = radMod / 1.8;
        enemies[index].model = mode;
        break;
    default:
        printf("ERROR in enemy spawning\n");
        break;
    }
    enemies[index].points = enemies[index].health / (2 * healthMod);
    return;
}

// Spawning powerups
void spawnPowerup(void)
{
    if (powerup_count >= powerupLIMIT)
        return;
    if (rand() % 100 > powerupProb)
        return;
    int tmp_model = rand() % 5;
    // tmp_model = 4;
    if ((tmp_model == 1 && damageMode >= 2) || (tmp_model == 2 && spreadMode >= 2) ||
        (tmp_model == 3 && Pilayer.dx > 8))
        return;

    // Finding empty memory slot
    int index = return_index(active_powerups, powerupLIMIT);
    active_powerups[index] = 1;
    powerup_count++;

    // Setting spawn point and motion
    powerups[index].posy = screenY - 10;
    powerups[index].posx = (rand() % (screenX - radMod - radMod)) + radMod;
    powerups[index].dy = -speedMod * (3 + (rand() % 2));

    // Other 0 == health, 1 == bullet upgrade, 2 == spread upgrade, 3 == speed upgrade, 4 == shield
    powerups[index].model = tmp_model; // tmp_model

    powerups[index].rad = radMod / 2.5;

    // Color
    switch (powerups[index].model)
    {
    case 0:
        powerups[index].r = 130;
        powerups[index].g = 185;
        powerups[index].b = 30;
        break;
    case 1:
        powerups[index].r = 255;
        powerups[index].g = 0;
        powerups[index].b = 0;
        break;
    case 2:
        powerups[index].r = 160;
        powerups[index].g = 0;
        powerups[index].b = 180;
        break;
    case 3:
        powerups[index].r = 245;
        powerups[index].g = 210;
        powerups[index].b = 75;
        break;
    case 4:
        powerups[index].r = 65;
        powerups[index].g = 85;
        powerups[index].b = 255;
        break;
    default:
        printf("ERROR in powerup spawn-model: %i\n", tmp_model);
        break;
    }
    return;
}

// Spawning bullets
void spawnBullet(int mode, int dx, int x, int y)
{
    if (bullet_count >= bulletLIMIT)
    {
        printf("Error max bullet count reached\n");
        return;
    }

    // Finding empty memory slot
    int index = return_index(active_bullets, bulletLIMIT);
    active_bullets[index] = 1;
    bullet_count++;

    bullets[index].rad = radMod / 16;
    bullets[index].mode = mode;
    bullets[index].posx = x;
    bullets[index].posy = y;
    bullets[index].dy = speedMod * 8;
    bullets[index].dx = dx;

    // Bullet types
    switch (mode)
    {
    // Player Autocannon (y)
    case 0:
        bullets[index].damage = 10 + (5 * damageMode);
        if (damageMode == 0)
        {
            bullets[index].r = 255;
            bullets[index].g = 255;
            bullets[index].b = 0;
        }
        else if (damageMode == 1)
        {
            bullets[index].r = 0;
            bullets[index].g = 191;
            bullets[index].b = 255;
        }
        else if (damageMode == 2)
        {
            bullets[index].r = 255;
            bullets[index].g = 105;
            bullets[index].b = 180;
        }
        else
            printf("ERROR in player bullet colorer\n");
        break;
    // Fighter laser cannon (r)
    case 1:
        bullets[index].damage = 10;
        bullets[index].dy *= -1;
        bullets[index].r = 255;
        bullets[index].g = 0;
        bullets[index].b = 0;
        break;
    // Bounty hunter dual cannon (d)
    case 2:
        bullets[index].damage = 15;
        bullets[index].dy *= -1;
        bullets[index].r = 255;
        bullets[index].g = 130;
        bullets[index].b = 0;
        break;
    default:
        printf("ERROR in bullet spawning\n");
        break;
    }
    return;
}

// Spawning beams
void spawnBeam(int mode, int x, int y)
{
    if (beam_count >= beamLIMIT)
    {
        printf("Error max beam count reached\n");
        return;
    }

    // Finding empty memory slot
    int index = return_index(active_beams, beamLIMIT);
    active_beams[index] = 1;
    beam_count++;

    beams[index].mode = mode;
    beams[index].posx = x;
    beams[index].posy = y;
    beams[index].timer = 0;

    switch (mode)
    {
    case 0:
        beams[index].thickness = 1 + (1 * spreadMode);
        beams[index].damage = 5 + (5 * damageMode);
        if (damageMode == 0)
        {
            beams[index].r = 40;
            beams[index].g = 200;
            beams[index].b = 100;
        }
        else if (damageMode == 1)
        {
            beams[index].r = 100;
            beams[index].g = 100;
            beams[index].b = 240;
        }
        else if (damageMode == 2)
        {
            beams[index].r = 0;
            beams[index].g = 0;
            beams[index].b = 255;
        }
        else
            printf("ERROR in player beam colorer\n");
        break;
    case 1:
        beams[index].thickness = 2;
        beams[index].r = 255;
        beams[index].g = 105;
        beams[index].b = 180;
        ;
        break;
    default:
        printf("ERROR in beam spawning\n");
        break;
    }

    return;
}

// Spawning explosions for entity collisions and deaths
void spawnExplosion(int x, int y)
{
    if (explosion_count >= explosionLIMIT)
    {
        printf("Error max explosion count reached");
        return;
    }

    // Finding empty memory slot
    int index = return_index(active_explosions, explosionLIMIT);
    active_explosions[index] = 1;
    explosion_count++;

    // Entity
    explosions[index].time = 0;
    explosions[index].posx = x;
    explosions[index].posy = y;
    explosions[index].mode = 1;
    return;
}

// Spawning explosions for powerups
void spawnExplosionp(int x, int y, powerup z)
{
    if (explosion_count >= explosionLIMIT)
    {
        printf("Error max explosion count reached");
        return;
    }

    int index = return_index(active_explosions, explosionLIMIT);
    active_explosions[index] = 1;
    explosion_count++;

    // Bullet
    explosions[index].time = 0;
    explosions[index].r1 = abs(z.r - 20);
    explosions[index].g1 = abs(z.g - 20);
    explosions[index].b1 = abs(z.b - 20);
    explosions[index].r2 = abs(explosions[index].r1 - 50);
    explosions[index].g2 = abs(explosions[index].g1 - 50);
    explosions[index].b2 = abs(explosions[index].b1 - 50);
    explosions[index].posx = x;
    explosions[index].posy = y;
    explosions[index].rad_1 = radMod / 2;
    explosions[index].rad_2 = radMod;
    explosions[index].mode = 0;
    return;
}

// Spawning explosions for bullets
void spawnExplosionb(int x, int y, bullet z)
{
    if (explosion_count >= explosionLIMIT)
    {
        printf("Error max explosion count reached");
        return;
    }

    int index = return_index(active_explosions, explosionLIMIT);
    active_explosions[index] = 1;
    explosion_count++;

    // Bullet
    explosions[index].time = 0;
    explosions[index].r1 = abs(z.r - 20);
    explosions[index].g1 = abs(z.g - 20);
    explosions[index].b1 = abs(z.b - 20);
    explosions[index].r2 = abs(explosions[index].r1 - 50);
    explosions[index].g2 = abs(explosions[index].g1 - 50);
    explosions[index].b2 = abs(explosions[index].b1 - 50);
    explosions[index].posx = x;
    explosions[index].posy = y;
    explosions[index].rad_1 = radMod / 8;
    explosions[index].rad_2 = radMod / 4;
    explosions[index].mode = 0;
    return;
}

void spawnExplosionb2(int x, int y, beam z)
{
    if (explosion_count >= explosionLIMIT)
    {
        printf("Error max explosion count reached");
        return;
    }

    int index = return_index(active_explosions, explosionLIMIT);
    active_explosions[index] = 1;
    explosion_count++;

    // eam
    explosions[index].time = 0;
    explosions[index].r1 = abs(z.r - 20);
    explosions[index].g1 = abs(z.g - 20);
    explosions[index].b1 = abs(z.b - 20);
    explosions[index].r2 = abs(explosions[index].r1 - 50);
    explosions[index].g2 = abs(explosions[index].g1 - 50);
    explosions[index].b2 = abs(explosions[index].b1 - 50);
    explosions[index].posx = x;
    explosions[index].posy = y;
    explosions[index].rad_1 = radMod / 6;
    explosions[index].rad_2 = radMod / 3;
    explosions[index].mode = 0;
}

/************************************


PHYSICS AND COLLISIONS


************************************/

// Checking collision entity
int check_collision(entity a, entity b)
{
    float dist_x = a.posx - b.posx;
    float dist_y = a.posy - b.posy;
    float dist = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
    if (dist <= a.rad + b.rad)
    {
        spawnExplosion(a.posx, a.posy);
        return 1;
    }
    return 0;
}

// Checking collision shield
int check_collisions(entity a, shield b)
{
    float dist_x = a.posx - b.posx;
    float dist_y = a.posy - b.posy;
    float dist = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
    if (dist <= a.rad + b.rad)
    {
        spawnExplosion(a.posx, a.posy);
        return 1;
    }
    return 0;
}

// Checking collision powerup
int check_collisionp(powerup a, entity b)
{
    float dist_x = a.posx - b.posx;
    float dist_y = a.posy - b.posy;
    float dist = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
    if (dist <= a.rad + b.rad)
    {
        spawnExplosionp(b.posx, b.posy, a);
        return 1;
    }
    return 0;
}

// Check collision bullet
int check_collisionb(bullet a, entity b)
{
    float dist_x = a.posx - b.posx;
    float dist_y = a.posy - b.posy;
    float dist = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
    if (dist <= a.rad + b.rad)
    {
        spawnExplosionb(a.posx, a.posy, a);
        return 1;
    }
    return 0;
}

// Check collision bullet
int check_collisionsb(bullet a, shield b)
{
    float dist_x = a.posx - b.posx;
    float dist_y = a.posy - b.posy;
    float dist = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
    if (dist <= a.rad + b.rad)
    {
        spawnExplosionb(a.posx, a.posy, a);
        return 1;
    }
    return 0;
}

// Check collision beam
int check_collisionb2(beam a, entity b)
{
    int dist_x = abs(a.posx - b.posx);
    if (dist_x <= a.thickness + b.rad)
    {
        spawnExplosionb2(a.posx, b.posy, a);
        return 1;
    }
    return 0;
}

// Check collision beam
int check_collisionsb2(beam a, shield b)
{
    int dist_x = abs(a.posx - b.posx);
    if (dist_x <= a.thickness + b.rad)
    {
        spawnExplosionb2(a.posx, b.posy, a);
        return 1;
    }
    return 0;
}

// Handling physics
void PhysicsAndMechanics(void)
{
    // Bullet mechanics
    for (int i = 0; i < bulletLIMIT; i++)
    {
        if (active_bullets[i] == 0)
            continue;
        bullets[i].posy += bullets[i].dy;
        bullets[i].posx += bullets[i].dx;
        if (bullets[i].posy > screenY || bullets[i].posy < 0)
        {
            active_bullets[i] = 0;
            bullet_count--;
        }

        // Hit mechanics for enemy bullets
        if (bullets[i].mode != 0)
        {
            if (Shield.health == 0)
            {
                if (check_collisionb(bullets[i], Pilayer))
                {
                    Pilayer.health -= bullets[i].damage;
                    active_bullets[i] = 0;
                    bullet_count--;
                }
            }
            else
            {
                if (check_collisionsb(bullets[i], Shield))
                {
                    Shield.health -= bullets[i].damage;
                    active_bullets[i] = 0;
                    bullet_count--;
                }
            }
        }
        else
        {
            for (int j = 0; j < enemyLIMIT; j++)
            {
                if (active_slots[j] == 0)
                    continue;
                if (check_collisionb(bullets[i], enemies[j]))
                {
                    enemies[j].health -= bullets[i].damage;
                    active_bullets[i] = 0;
                    bullet_count--;
                }
            }
        }
    }

    // Beam mechanics
    for (int i = 0; i < beamLIMIT; i++)
    {
        if (active_beams[i] == 0)
            continue;

        // Hit mechanics for player beam
        if (beams[i].mode == 0)
        {
            for (int j = 0; j < enemyLIMIT; j++)
            {
                if (active_slots[j] == 0)
                    continue;
                if (check_collisionb2(beams[i], enemies[j]))
                {
                    enemies[j].health -= beams[i].damage;
                }
            }
        }
        else
        {
            if (Shield.health == 0)
            {
                if (check_collisionb2(beams[i], Pilayer))
                {
                    Pilayer.health -= beams[i].damage;
                }
            }
            else
            {
                if (check_collisionsb2(beams[i], Shield))
                {
                    Shield.health -= beams[i].damage;
                }
            }
        }
    }

    // Powerup mechanics
    for (int i = 0; i < powerupLIMIT; i++)
    {
        if (active_powerups[i] == 0)
            continue;

        // Motion and termination
        powerups[i].posy += powerups[i].dy;
        if (powerups[i].posy < 0)
        {
            active_powerups[i] = 0;
            powerup_count--;
        }
        if (check_collisionp(powerups[i], Pilayer))
        {
            active_powerups[i] = 0;
            powerup_count--;
            if (powerups[i].model == 0)
                Pilayer.health += 50;
            else if (powerups[i].model == 1 && damageMode < 2)
                damageMode++;
            else if (powerups[i].model == 2 && spreadMode < 2)
                spreadMode++;
            else if (powerups[i].model == 3)
            {
                Pilayer.dx *= 1.5;
                Pilayer.dy *= 1.5;
            }
            else if (powerups[i].model == 4)
                Shield.health += 100;
            else
                printf("ERROR in powerup termination\n");
        }
    }

    // Shield mechanics
    Shield.posx = Pilayer.posx;
    Shield.posy = Pilayer.posy;

    // Player and shield health cap / Player Death
    if (Pilayer.health > PlayerHealth)
        Pilayer.health = PlayerHealth;
    else if (Pilayer.health <= 0)
    {
        Pilayer.health = 0;
        printf("\n--------------------------\n\nYOU HAVE DIED, GAME OVER!!\n\n--------------------------\n");
        player_scores[5].score = score;
        // printDebug();
        is_dead = 1;
        game_pause();
    }

    if (Shield.health > ShieldHealth)
        Shield.health = ShieldHealth;
    else if (Shield.health <= 0)
        Shield.health = 0;

    // Enemy mechanics
    for (int i = 0; i < enemyLIMIT; i++)
    {
        int pos;
        if (active_slots[i] == 0)
            continue;
        if (enemies[i].health <= 0)
        {
            active_slots[i] = 0;
            enemy_count--;
            score += enemies[i].points;
            spawnExplosion(enemies[i].posx, enemies[i].posy);
            continue;
        }
        if (Shield.health == 0)
        {
            if (check_collision(enemies[i], Pilayer))
            {
                active_slots[i] = 0;
                enemy_count--;
                Pilayer.health -= (enemies[i].health / 2);
            }
        }
        else
        {
            if (check_collisions(enemies[i], Shield))
            {
                active_slots[i] = 0;
                enemy_count--;
                Shield.health -= (enemies[i].health / 2);
            }
        }

        switch (enemies[i].model)
        {
        // Druid Disk
        case 0:
            if ((enemies[i].posx + enemies[i].dx > screenX - 25) || (enemies[i].posx + enemies[i].dx < 25))
                enemies[i].dx = -enemies[i].dx;
            if ((enemies[i].posy + enemies[i].dy > screenY) || (enemies[i].posy + enemies[i].dy < 25))
                enemies[i].dy = -enemies[i].dy;
            enemies[i].posy += enemies[i].dy;
            enemies[i].posx += enemies[i].dx;
            break;

        // Fighter
        case 1:
            pos = (1 + rand() % 5) * (60 + rand() % 15);
            if ((enemies[i].posy <= screenY - pos) && (enemies[i].dx == 0))
            {
                enemies[i].dy = 0;
                enemies[i].dx = speedMod * (4 + rand() % 3);
                if (rand() % 2 == 0)
                    enemies[i].dx *= -1;
            }
            else if ((enemies[i].posx + enemies[i].dx > screenX - 41) || (enemies[i].posx + enemies[i].dx < 38))
                enemies[i].dx = -enemies[i].dx;

            enemies[i].posx += enemies[i].dx;
            enemies[i].posy += enemies[i].dy;

            if (rand() % 35 == 0)
                spawnBullet(1, 0, enemies[i].posx, enemies[i].posy - enemies[i].rad);
            break;

        // Asteroid
        case 2:
            enemies[i].posy += enemies[i].dy;
            if (enemies[i].posy < 35)
            {
                active_slots[i] = 0;
                enemy_count--;
            }
            break;
        // Big asteroid
        case 3:
            enemies[i].posy += enemies[i].dy;
            if (enemies[i].posy < 80)
            {
                active_slots[i] = 0;
                enemy_count--;
            }
            break;
        // bounty hunter
        case 4:
            pos = (1 + rand() % 5) * (60 + rand() % 15);
            if ((enemies[i].posy <= screenY - pos) && (enemies[i].dx == 0))
            {
                enemies[i].dy = 0;
                enemies[i].dx = speedMod * (4 + rand() % 3);
                if (rand() % 2 == 0)
                    enemies[i].dx *= -1;
            }
            else if ((enemies[i].posx + enemies[i].dx > screenX - 41) || (enemies[i].posx + enemies[i].dx < 38))
                enemies[i].dx = -enemies[i].dx;

            enemies[i].posx += enemies[i].dx;
            enemies[i].posy += enemies[i].dy;

            if (rand() % 30 == 0)
            {
                spawnBullet(2, 0, enemies[i].posx - 10, enemies[i].posy - enemies[i].rad);
                spawnBullet(2, 0, enemies[i].posx + 10, enemies[i].posy - enemies[i].rad);
            }
            break;
        case 5:
            pos = (1 + rand() % 5) * (60 + rand() % 15);
            if ((enemies[i].posy <= screenY - pos) && (enemies[i].dx == 0))
            {
                enemies[i].dy = 0;
                enemies[i].dx = speedMod * (4 + rand() % 3);
                if (rand() % 2 == 0)
                    enemies[i].dx *= -1;
            }
            else if ((enemies[i].posx + enemies[i].dx > screenX - 41) || (enemies[i].posx + enemies[i].dx < 38))
                enemies[i].dx = -enemies[i].dx;

            enemies[i].posx += enemies[i].dx;
            enemies[i].posy += enemies[i].dy;

            if (rand() % 40 == 0)
            {
                spawnBeam(1, enemies[i].posx, enemies[i].posy - enemies[i].rad);
            }
            break;
        default:
            printf("ERROR in enemy mechanics\n");
        }
    }

    return;
}

/************************************


DRAW FUNCTION & MUSIC


************************************/

void iDraw()
{
    // place your drawing codes here
    iClear();
    int x, y;
    // game_state:
    // 0 = menu
    // 1 = scores
    // 2 = controls
    // 3 = play
    switch (game_state)
    {
    case 0:
        iShowBMP(0, 0, background[game_state]);
        x = 70;
        y = 370;
        for (int i = 0; i < 4; i++)
        {
            iShowBMP2(x, y, buttons[i], 0);
            y -= 100;
        }
        iShowBMP2(195, 600, title, 0);
        if (taking_name == 1)
        {
            iSetColor(151, 174, 255);
            iFilledRectangle(785, 245, 306, 64);
        }
        iShowBMP2(790, 250, text_field[0], 0);
        iSetColor(151, 174, 255);
        iText(805, 270, playername, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(810, 320, "Press enter to insert your name:", GLUT_BITMAP_8_BY_13);
        iText(800, 150, "A game by: Omar Abdur Razzaque", GLUT_BITMAP_9_BY_15);
        iText(800, 130, "CSE 21B2 - 2023", GLUT_BITMAP_9_BY_15);

        break;
    case 1:
        bubble_sort(player_scores);
        uploadHighscores();
        x = 450;
        y = 555;
        iShowBMP(0, 0, background[game_state]);
        iShowBMP2(380, 70, buttons[4], 0);
        iText(450, 700, "Highest Scorers:", GLUT_BITMAP_TIMES_ROMAN_24);
        for (int i = 0; i < 5; i++)
        {
            sprintf(display[i], "%i. %s : %i", i + 1, player_scores[i].name, player_scores[i].score);
            iText(x, y, display[i], GLUT_BITMAP_TIMES_ROMAN_24);
            y -= 50;
        }
        break;
    case 2:
        x = 450;
        y = 555;
        iShowBMP(0, 0, background[game_state]);
        iShowBMP2(380, 70, buttons[4], 0);
        iSetColor(151, 174, 255);
        iText(x - 130, y - 50, "Press W to move up", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x - 130, y - 100, "Press S to move down", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x - 130, y - 150, "Press A to move left", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x - 130, y - 200, "Press D to move right", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x - 130, y - 250, "Press E to switch weapons", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x + 170, y - 50, "Press P to pause", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x + 170, y - 100, "Press R to resume", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x + 170, y - 150, "Press Q to quit early in game", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x + 170, y - 200, "Click with LMB to shoot", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(x + 170, y - 250, "Press M to toggle music", GLUT_BITMAP_TIMES_ROMAN_24);
        break;

    case 3:
        iShowBMP(0, 0, background[game_state]);

        // Updating difficulty
        if (score >= threshold)
        {
            if (threshold < 800)
                threshold += threshold;
            else
                threshold += 400;

            if (enemyCAP < enemyLIMIT)
                enemyCAP++;
            if (enemyGRADE < typesOfEnemies)
                enemyGRADE++;
        }

        // Rendering powerups
        for (int i = 0; i < powerupLIMIT; i++)
        {
            if (active_powerups[i] == 1)
            {
                iSetColor(powerups[i].r, powerups[i].g, powerups[i].b);
                iCircle(powerups[i].posx, powerups[i].posy, powerups[i].rad);
                iCircle(powerups[i].posx, powerups[i].posy, powerups[i].rad - 1);
                iShowBMP2(powerups[i].posx - 12, powerups[i].posy - 12, powerup_skins[powerups[i].model], 0);
            }
        }

        // Rendering enemies
        for (int i = 0; i < enemyLIMIT; i++)
        {
            if (active_slots[i] == 1)
            {
                if (enemies[i].model == 0)
                    iShowBMP2(enemies[i].posx - 23, enemies[i].posy - 23, enemy_skin[enemies[i].model], 0);
                else if (enemies[i].model == 1)
                    iShowBMP2(enemies[i].posx - 35, enemies[i].posy - 47, enemy_skin[enemies[i].model], 0);
                else if (enemies[i].model == 2)
                    iShowBMP2(enemies[i].posx - 54, enemies[i].posy - 57, enemy_skin[enemies[i].model], 0);
                else if (enemies[i].model == 3)
                    iShowBMP2(enemies[i].posx - 108, enemies[i].posy - 95, enemy_skin[enemies[i].model], 0);
                else if (enemies[i].model == 4)
                    iShowBMP2(enemies[i].posx - 33, enemies[i].posy - 40, enemy_skin[enemies[i].model], 0);
                else if (enemies[i].model == 5)
                    iShowBMP2(enemies[i].posx - 30, enemies[i].posy - 40, enemy_skin[enemies[i].model], 0);
                // iSetColor(255, 0, 0);
                // iFilledCircle(enemies[i].posx, enemies[i].posy, enemies[i].rad);
            }
        }

        // Rendering Player
        // iSetColor(Pilayer.r, Pilayer.g, Pilayer.b);
        // iFilledCircle(Pilayer.posx, Pilayer.posy, Pilayer.rad);
        iShowBMP2(Pilayer.posx - 45, Pilayer.posy - 38, player_skin, 0);

        // Rendering Shield
        if (Shield.health > 0)
        {
            iSetColor(65, 85, 255);
            iCircle(Shield.posx, Shield.posy, Shield.rad);
            iCircle(Shield.posx, Shield.posy, Shield.rad - 1);
            iCircle(Shield.posx, Shield.posy, Shield.rad - 2);
        }

        // Rendering bullets
        for (int i = 0; i < bulletLIMIT; i++)
        {
            if (active_bullets[i] == 1)
            {
                iSetColor(bullets[i].r, bullets[i].g, bullets[i].b);
                iFilledCircle(bullets[i].posx, bullets[i].posy, bullets[i].rad);
            }
        }

        // Rendering beams
        for (int i = 0; i < beamLIMIT; i++)
        {
            if (active_beams[i] == 1)
            {
                if (beams[i].timer <= 2)
                {
                    beams[i].timer++;
                    if (beams[i].mode == 0)
                    {
                        iSetColor(beams[i].r, beams[i].g, beams[i].b);
                        iFilledRectangle(beams[i].posx - beams[i].thickness, beams[i].posy,
                                         beams[i].thickness * 2, screenY - beams[i].posy);
                    }
                    else if (beams[i].mode == 1)
                    {
                        iSetColor(beams[i].r, beams[i].g, beams[i].b);
                        iFilledRectangle(beams[i].posx - beams[i].thickness, 0,
                                         beams[i].thickness * 2, beams[i].posy);
                    }
                }
                else
                {
                    active_beams[i] = 0;
                    beam_count--;
                }
            }
        }

        // Rendering explosions
        for (int i = 0; i < explosionLIMIT; i++)
        {
            if (active_explosions[i] == 1)
            {
                if (explosions[i].mode == 0)
                {
                    iSetColor(explosions[i].r1, explosions[i].g1, explosions[i].b1);
                    iCircle(explosions[i].posx, explosions[i].posy, explosions[i].rad_1);
                    iSetColor(explosions[i].r2, explosions[i].g2, explosions[i].b2);
                    iCircle(explosions[i].posx, explosions[i].posy, explosions[i].rad_2);
                    if (explosions[i].time < 3)
                    {
                        explosions[i].time++;
                        continue;
                    }
                    active_explosions[i] = 0;
                    explosion_count--;
                }
                else
                {
                    iShowBMP2(explosions[i].posx - 40, explosions[i].posy - 55, exp_skin[explosions[i].time / 2], 0);
                    if (explosions[i].time < 5)
                    {
                        explosions[i].time++;
                        continue;
                    }
                    active_explosions[i] = 0;
                    explosion_count--;
                }
            }
        }

        sprintf(scorestr, "Score: %i", score);
        iSetColor(20, 200, 200);
        iText(screenX - 130, screenY - 50, scorestr, GLUT_BITMAP_TIMES_ROMAN_24);

        sprintf(healthstr, "Health: %i", Pilayer.health);
        iSetColor(130, 185, 30);
        iText(screenX - 130, screenY - 100, healthstr, GLUT_BITMAP_TIMES_ROMAN_24);

        sprintf(shieldstr, "Shields: %i", Shield.health);
        iSetColor(65, 85, 255);
        iText(screenX - 130, screenY - 150, shieldstr, GLUT_BITMAP_TIMES_ROMAN_24);

        if (WepMode == 0)
            sprintf(wepmodstr, "Autocannon");
        else
            sprintf(wepmodstr, "Laser beam");
        iSetColor(200, 75, 0);
        iText(screenX - 130, screenY - 200, "Weapon:", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(screenX - 130, screenY - 230, wepmodstr, GLUT_BITMAP_TIMES_ROMAN_24);

        if (is_dead == 1)
        {
            iShowBMP2(404, 400, text_field[1], 0);
            iSetColor(151, 174, 255);
            iText(509, 484, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(509, 434, scorestr, GLUT_BITMAP_TIMES_ROMAN_24);
            iShowBMP2(380, 70, buttons[4], 0);
        }
        break;
    }

    if (sound_on == 1)
    {
        if (game_state != 3 && is_playing[0] == 0)
        {
            PlaySound(music[0], NULL, SND_LOOP | SND_ASYNC);
            is_playing[0] = 1;
            is_playing[1] = 0;
        }
        else if (game_state == 3 && is_playing[1] == 0)
        {
            PlaySound(music[1], NULL, SND_LOOP | SND_ASYNC);
            is_playing[0] = 0;
            is_playing[1] = 1;
        }
    }
    else
    {
        PlaySound(0, 0, 0);
        is_playing[0] = 0;
        is_playing[1] = 0;
    }
}

/************************************


INPUT MECHANISMS


************************************/

int addchar(char str[], char key)
{
    int strlength = strlen(str);
    if (strlength < nameLen - 1)
    {
        str[strlength] = key;
        str[strlength + 1] = '\0';
        strlength++;
    }
    return strlength;
}

int removechar(char str[])
{
    int strlength = strlen(str);
    if (strlength > 0)
    {
        str[strlength - 1] = '\0';
        strlength--;
    }
    return strlength;
}

/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
    */
void iMouseMove(int mx, int my)
{
}

/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
    */
void iMouse(int button, int state, int mx, int my)
{
    // for buttons in menu
    // 413 - 126 original dimensions
    // 365 - 78 actual button dimensions
    // dy gap = 24
    // dx gap = 24
    // x = 70
    // y = 370, 270, 170, 70

    switch (game_state)
    {
    case 0:
        // printf("mx = %i || my = %i\n", mx, my);
        if (mx >= 94 && mx <= 459)
        {
            if (my >= 394 && my <= 472)
            {
                game_start();
            }
            else if (my >= 294 && my <= 372)
            {
                game_state = 1;
            }
            else if (my >= 194 && my <= 272)
            {
                game_state = 2;
            }
            else if (my >= 94 && my <= 172)
            {
                printf("\n\n------------------------\n\nTHANK YOU FOR PLAYING!!!\n\n------------------------\n\n");
                uploadHighscores();
                exit(0);
            }
        }
        break;
    case 1:
    case 2:
        if (mx >= 424 && mx <= 789 && my >= 94 && my <= 172)
            game_state = 0;
        break;
    case 3:
        if (mx >= 424 && mx <= 789 && my >= 94 && my <= 172 && is_dead == 1)
            game_stop();
        if (paused == 0 && WepMode == 0)
        {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            {
                if (spreadMode == 0)
                    spawnBullet(0, 0, Pilayer.posx, Pilayer.posy + Pilayer.rad);
                else if (spreadMode == 1)
                {
                    spawnBullet(0, 0, Pilayer.posx + 10, Pilayer.posy + Pilayer.rad);
                    spawnBullet(0, 0, Pilayer.posx - 10, Pilayer.posy + Pilayer.rad);
                }
                else if (spreadMode == 2)
                {
                    spawnBullet(0, 0, Pilayer.posx, Pilayer.posy + Pilayer.rad);
                    spawnBullet(0, speedMod * 2, Pilayer.posx + 10, Pilayer.posy + Pilayer.rad);
                    spawnBullet(0, speedMod * -2, Pilayer.posx - 10, Pilayer.posy + Pilayer.rad);
                }
                else
                {
                    printf("ERROR in mousebullet\n");
                }
            }
        }

        else if (paused == 0 && WepMode == 1)
        {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            {
                spawnBeam(0, Pilayer.posx, Pilayer.posy + Pilayer.rad);
            }
        }
        break;
    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
    */
void iKeyboard(unsigned char key)
{
    if (key == 'm' && taking_name == 0)
        sound_on = 1 - sound_on;
    if (game_state == 3 && is_dead == 0)
    {
        switch (paused)
        {
        case 0:
            if (key == 'a' && Pilayer.posx > 55)
                Pilayer.posx -= Pilayer.dx;
            else if (key == 'd' && Pilayer.posx < screenX - 55)
                Pilayer.posx += Pilayer.dx;
            else if (key == 'w' && Pilayer.posy < screenY - 65)
                Pilayer.posy += Pilayer.dy;
            else if (key == 's' && Pilayer.posy > 40)
                Pilayer.posy -= Pilayer.dy;
            else if (key == 'p')
            {
                game_pause();
                // printDebug();
            }
            else if (key == 'q')
                Pilayer.health = 0;
            else if (key == 'e')
                WepMode = 1 - WepMode;
            break;
        case 1:
            if (key == 'r')
                game_resume();
            break;
        }
    }
    else if (game_state == 0)
    {
        if (key == 13)
            taking_name = 1 - taking_name;
        if (taking_name == 1)
        {
            if (key != '\b' && key != 13)
                len = addchar(playername, key);
            else if (key == '\b')
                len = removechar(playername);
            for (int i = 0; i < len; i++)
                printf("%c", playername[i]);
            printf("\n");
            printf("length = %i\n", len);
        }
    }
}

/*
    function iSpecialKeyboard() is called whenver user hits special keys like-
    function keys, home, end, pg up, pg down, arraows etc. you have to use
    appropriate constants to detect them. A list is:
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
    GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
    GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
    */
void iSpecialKeyboard(unsigned char key)
{
}

/************************************


MAIN


************************************/

int main(void)
{
    // place your own initialization codes here.

    // Loading scores
    loadHighscores();

    // Setting timers and pausing them
    iSetTimer(phyDelay, PhysicsAndMechanics);
    iSetTimer(600, spawnEnemy);
    iSetTimer(6000, spawnPowerup);
    // iSetTimer(1000, printDebug);
    game_stop();

    iInitialize(screenX, screenY, "Into the Void");
    return 0;
}
