#include "../src/game.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace vb;
static Game hatched(){Game g;g.choose(1);g.clockSet=1;g.tick(1000);for(int i=1;i<=30;i++)g.tick(1000+i);assert(g.pet().hatch==30);return g;}
int main(){
    Game egg;assert(!egg.encounter(0));assert(egg.choose(0));egg.tick(1000);egg.tick(1001);assert(egg.pet().hatch==0);egg.clockSet=1;egg.tick(1002);assert(egg.pet().hatch==1);egg.sleeping=1;egg.tick(1003);assert(egg.pet().hatch==1);egg.sleeping=0;egg.paused=1;egg.tick(1004);assert(egg.pet().hatch==1);
    auto g=hatched();assert(g.neededSeconds()==129600);g.tick(1031);assert(g.pet().stageAge==1);g.sleeping=1;g.tick(1032);assert(g.pet().stageAge==1);g.sleeping=0;g.paused=1;g.tick(1033);assert(g.pet().stageAge==1);g.paused=0;g.tick(1000000);assert(g.pet().stageAge==1);g.tick(1000001);assert(g.pet().stageAge==2);
    auto evo=hatched();evo.pet().stageAge=evo.pet().age=129569;evo.pet().stageXp=evo.pet().xp=220;assert(!evo.ready());evo.tick(1031);assert(evo.ready());evo.pet().food=evo.pet().joy=evo.pet().clean=0;assert(evo.evolve());assert(evo.pet().id==5&&evo.pet().stageAge==0&&evo.pet().stageXp==0);assert(evo.neededSeconds()==259200&&evo.neededXp()==620&&evo.neededLevel()==36&&evo.pet().age==129570&&evo.level()==16);assert(!evo.evolve());
    auto insufficient=hatched();insufficient.pet().stageAge=insufficient.pet().age=259200;assert(!insufficient.ready());insufficient.pet().stageXp=insufficient.pet().xp=59;assert(!insufficient.ready());
    for(int b=0;b<3;b++){auto e=hatched();e.choose(4);e.pet().hatch=30;e.pet().stageAge=e.pet().age=259200;e.pet().xp=e.pet().stageXp=620;assert(e.evolve(b));assert(e.pet().id==134+b);assert(!e.ready());}
    auto resting=hatched();resting.sleeping=1;assert(!resting.care(0)&&!resting.encounter(0));resting.sleeping=0;resting.paused=1;assert(!resting.encounter(0));
    auto battle=hatched();assert(!battle.encounter(1));for(int i=0;i<10;i++)assert(battle.care(3));assert(!battle.care(3));assert(battle.encounter(0));assert(!battle.choose(0));assert(!battle.care(0));battle.battle.enemyHp=1;assert(battle.move(0));assert(battle.wins==1&&battle.pet().stageXp==20);assert(!battle.move(0));assert(battle.wins==1);assert(battle.encounter(0));battle.battle.hp=1;battle.move(2);assert(battle.losses==1&&battle.pet().stageXp==28);assert(battle.encounter(0));int xp=battle.pet().xp;battle.move(4);assert(battle.pet().xp==xp);
    auto focus=hatched();focus.encounter(0);focus.battle.focus=0;auto saved=focus.save();assert(!focus.move(1));assert(focus.save()==saved);focus.battle.potion=0;saved=focus.save();assert(!focus.move(3));assert(focus.save()==saved);
    auto round=hatched();round.encounter(0);round.move(2);auto data=round.save();Game loaded;assert(loaded.load(data));assert(loaded.battle.active&&loaded.battle.focus==5);assert(loaded.lastTime==0);loaded.tick(2000000);assert(loaded.pet().stageAge==0);auto clean=loaded.save();assert(!loaded.load("bad save")&&loaded.save()==clean);assert(!loaded.load(data+"trailing")&&loaded.save()==clean);assert(!loaded.load(std::string(17000,'x')));auto wrong=round;wrong.pet().id=999;assert(!loaded.load(wrong.save()));wrong=round;wrong.pet().stageAge=10;wrong.pet().age=0;assert(!loaded.load(wrong.save()));wrong=round;wrong.battle.hp=10000;assert(!loaded.load(wrong.save()));
    assert(speciesCount==151);for(int id=1;id<=151;id++){assert(dex(id).id==id);assert(dex(id).family>=0&&dex(id).family<Families);if(dex(id).next)assert(dex(dex(id).next).family==dex(id).family);}
    int completed=0;for(int family=0;family<Families;family++)for(int seed=1;seed<=20;seed++){auto sim=hatched();sim.wins=100;sim.choose(family);sim.pet().hatch=30;sim.rng=seed;for(int i=0;i<10;i++)sim.care(3);sim.encounter(seed%4);int turns=0;while(sim.battle.active&&turns++<100){int a=sim.battle.hp<sim.battle.maxHp/3&&sim.battle.potion?3:sim.battle.focus>=2?1:0;assert(sim.move(a));assert(sim.battle.hp>=0&&sim.battle.enemyHp>=0);}assert(!sim.battle.active);assert(sim.wins==101||sim.losses==1);Game copy;assert(copy.load(sim.save()));completed++;}
    // All 151 are reachable through an egg or a Gen 1 evolution chain.
    bool reachable[152]={};for(int f=0;f<Families;f++){Game chain;chain.wins=100;assert(chain.choose(f));chain.pet().hatch=30;reachable[chain.pet().id]=true;for(int stage=0;stage<3&&chain.nextId();stage++){chain.pet().age+=259200;chain.pet().stageAge=259200;chain.pet().xp+=1000;chain.pet().stageXp=1000;assert(chain.evolve());reachable[chain.pet().id]=true;}}
    reachable[135]=reachable[136]=true;for(int id=1;id<=151;id++)assert(reachable[id]);
    Game encounters;encounters.choose(0);encounters.pet().hatch=30;encounters.wins=100;bool met[152]={};for(int i=0;i<5000;i++){assert(encounters.encounter(3));met[encounters.battle.enemy]=true;encounters.move(4);}for(int id=1;id<=151;id++)assert(met[id]);
    // An early ten-family preview can still be imported after the rename.
    auto old=hatched();std::istringstream source(old.save());std::string row,legacy="POCKET" "_PARTNER 1\n";std::getline(source,row);std::getline(source,row);legacy+=row+'\n';for(int i=0;i<Families;i++){std::getline(source,row);if(i<10)legacy+=row+'\n';}while(std::getline(source,row))legacy+=row+'\n';Game upgraded;assert(upgraded.load(legacy));assert(upgraded.pet().id==4);assert(upgraded.pets[78].id==151);
    auto fixture=hatched();fixture.choose(4);fixture.pet().hatch=30;fixture.seen[133]=1;fixture.pet().age=fixture.pet().stageAge=259170;fixture.pet().xp=fixture.pet().stageXp=620;std::ofstream("build/evolution-ready.save")<<fixture.save();

    // Every evolving species rejects both a missing level and a missing time milestone.
    const int expected[][2]={{1,16},{2,32},{4,16},{5,36},{7,16},{8,36},{10,7},{11,10},{13,7},{14,10},{16,18},{17,36},{19,20},{21,20},{23,22},{25,36},{27,22},{29,16},{30,36},{32,16},{33,36},{35,36},{37,36},{39,36},{41,22},{43,21},{44,36},{46,24},{48,31},{50,26},{52,28},{54,33},{56,28},{58,36},{60,25},{61,36},{63,16},{64,36},{66,28},{67,36},{69,21},{70,36},{72,30},{74,25},{75,36},{77,40},{79,37},{81,30},{84,31},{86,34},{88,38},{90,36},{92,25},{93,36},{96,26},{98,28},{100,30},{102,36},{104,28},{109,35},{111,42},{116,32},{118,33},{120,36},{129,20},{133,36},{138,40},{140,40},{147,30},{148,55}};
    int checked=0;
    for(auto& rule:expected){
        Game boundary;boundary.wins=100;assert(boundary.choose(dex(rule[0]).family));
        auto& p=boundary.pet();p.id=rule[0];p.hatch=30;p.age=259170;p.stageAge=0;
        assert(boundary.neededLevel()==rule[1]);
        p.xp=(rule[1]-5)*20-1;p.stageXp=p.xp;
        assert(boundary.level()==rule[1]-1&&!boundary.ready()&&!boundary.evolve());
        p.xp++;p.stageXp=0;p.age=boundary.neededSeconds()-31;
        assert(boundary.level()==rule[1]&&!boundary.ready()&&!boundary.evolve());
        p.age++;p.food=p.joy=p.clean=0;
        assert(boundary.ready());auto age=p.age;auto xp=p.xp;int target=boundary.nextId();
        assert(boundary.evolve()&&p.id==target&&p.age==age&&p.xp==xp);
        checked++;
    }
    assert(checked==70);
    // A complete chain shares one 72-hour clock, including the initial 30 seconds.
    for(int f=0;f<Families;f++){
        Game chain;chain.wins=100;chain.choose(f);chain.pet().hatch=30;chain.pet().xp=1000;
        int stages=0;while(chain.nextId()){
            int target=chain.neededSeconds();chain.pet().age=target-31;
            assert(!chain.ready());chain.pet().age++;assert(chain.evolve());stages++;
        }
        if(stages)assert(chain.pet().age+chain.pet().hatch==259200);
    }
    // A delayed first evolution can still reach the final form at total hour 72.
    auto late=hatched();late.pet().age=259170;late.pet().xp=620;
    assert(late.evolve()&&late.pet().id==5);assert(late.ready()&&late.evolve()&&late.pet().id==6);
    // Old saves keep already evolved forms, XP and cumulative time.
    auto previous=hatched();previous.pet().id=5;previous.pet().xp=60;previous.pet().stageXp=0;
    previous.pet().age=259200;previous.pet().stageAge=0;Game migrated;
    assert(migrated.load(previous.save())&&migrated.pet().id==5&&migrated.pet().age==259200);
    assert(migrated.neededLevel()==36&&!migrated.ready());
    auto levelFixture=hatched();levelFixture.pet().age=levelFixture.pet().stageAge=129570;
    levelFixture.pet().xp=levelFixture.pet().stageXp=219;
    std::ofstream("build/level-below.save")<<levelFixture.save();
    levelFixture.pet().xp=levelFixture.pet().stageXp=220;
    std::ofstream("build/level-ready.save")<<levelFixture.save();
    levelFixture.pet().age=levelFixture.pet().stageAge=129569;
    std::ofstream("build/time-below.save")<<levelFixture.save();
    std::ofstream("build/test-transfer.save")<<round.save();std::cout<<"PASS: hatching, 36/72h cumulative boundaries, 70 level thresholds, sleep/pause/suspension, care independence, branching evolution, rewards, invalid saves, cross-save fixture and "<<completed<<" simulated battles\n";
}
