#pragma once
#include <string>
#include <cstdint>
namespace vb {
constexpr int Families=79;
enum Type { Normal,Grass,Fire,Water,Electric,Bug,Flying,Psychic,Ghost,Poison,Ground,Rock,Fighting,Ice,Dragon };
struct Species { int id; const char* name; Type type; int next; int stage; int family; int evolutionLevel; };
extern const Species species[151];
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
    int neededLevel() const;
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
int vb_get(int key);
int vb_pet(int family,int field);
int vb_species(int index,int field);
const char* vb_name(int id);
const char* vb_message();
void vb_tick(double now);
int vb_action(int action,int argument);
const char* vb_save();
int vb_load(const char* text);
}
