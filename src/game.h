#pragma once
#include <string>
#include <cstdint>
namespace pp {
constexpr int Families=10;
enum Type { Normal,Grass,Fire,Water,Electric,Bug,Flying,Psychic,Ghost };
struct Species { int id; const char* name; Type type; int next; int stage; int family; };
extern const Species species[32];
extern const int speciesCount;
const Species& dex(int id);
const char* typeName(int type);
struct Pet { int id=0, xp=0, stageXp=0, training=0, owned=0; double age=0, stageAge=0, food=85, joy=85, clean=85, hatch=0; };
struct Battle { int active=0, enemy=0, hp=0, maxHp=0, enemyHp=0, enemyMax=0, focus=3, potion=1, zone=0, turn=0; };
struct Game {
    Pet pets[Families]; int selected=0, wins=0, losses=0, seen[152]={}; uint32_t rng=73421;
    double lastTime=0; int clockSet=0, sleeping=0, paused=0; Battle battle; std::string message="Choose your first partner.";
    Game();
    void tick(double now);
    bool choose(int family);
    bool care(int action);
    bool encounter(int zone);
    bool move(int action);
    bool evolve(int branch=0);
    int unlockAt(int family) const;
    int nextId(int branch=0) const;
    int neededXp() const;
    int neededSeconds() const;
    bool ready() const;
    int level() const;
    int maximumHp() const;
    int random(int n);
    std::string save() const;
    bool load(const std::string& text);
    Pet& pet() { return pets[selected]; }
    const Pet& pet() const { return pets[selected]; }
};
float effectiveness(int attack,int defender);
}
extern "C" {
int pp_get(int key);
int pp_pet(int family,int field);
int pp_species(int index,int field);
const char* pp_name(int id);
const char* pp_message();
void pp_tick(double now);
int pp_action(int action,int argument);
const char* pp_save();
int pp_load(const char* text);
}
