#include "game.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <limits>
namespace vb {
const Species species[]={
#include "roster.inc"
};
const int speciesCount=sizeof(species)/sizeof(species[0]);
const Species& dex(int id) { for(auto& s:species) if(s.id==id)return s; return species[0]; }
const char* typeName(int t) { static const char* names[]={"Normal","Grass","Fire","Water","Electric","Bug","Flying","Psychic","Ghost","Poison","Ground","Rock","Fighting","Ice","Dragon"}; return names[std::clamp(t,0,14)]; }
static const int roots[]={1,4,7,25,133,10,16,129,63,92,13,19,21,23,27,29,32,35,37,39,41,43,46,48,50,52,54,56,58,60,66,69,72,74,77,79,81,83,84,86,88,90,95,96,98,100,102,104,106,107,108,109,111,113,114,115,116,118,120,122,123,124,125,126,127,128,131,132,137,138,140,142,143,144,145,146,147,150,151};
int Game::unlockAt(int f)const { static const int gates[]={0,0,0,0,0,2,4,7,10,14}; if(f<0||f>=Families)return 999;if(f<10)return gates[f];return 2+(f-10)/3; }
Game::Game(){ for(int i=0;i<Families;i++) pets[i].id=roots[i]; }
int Game::random(int n){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return int(rng%uint32_t(n));}
void Game::tick(double now){
    if(!std::isfinite(now)||now<=0)return;
    if(lastTime==0){lastTime=now;return;}
    if(now<lastTime)return;
    double delta=now-lastTime;lastTime=now;
    // Suspended or background processes never award offline evolution time.
    if(delta>5||!clockSet||sleeping||paused||!pet().owned)return;
    auto& p=pet();
    if(p.hatch<30){p.hatch=std::min(30.0,p.hatch+delta);if(p.hatch>=30){seen[p.id]=1;message=std::string("Your egg hatched into ")+dex(p.id).name+"!";}return;}
    p.age=std::min(p.age+delta,315360000.0);p.stageAge=std::min(p.stageAge+delta,315360000.0);p.food=std::max(0.0,p.food-delta/300);p.joy=std::max(0.0,p.joy-delta/450);p.clean=std::max(0.0,p.clean-delta/600);
}
bool Game::choose(int f){if(f<0||f>=Families||wins<unlockAt(f)||battle.active)return false;selected=f;pet().owned=1;sleeping=0;message=pet().hatch<30?"A new egg! Set the clock and stay for its first moments.":std::string(dex(pet().id).name)+" is ready for an adventure.";return true;}
bool Game::care(int a){if(pet().hatch<30||!pet().owned||sleeping||paused||battle.active)return false;auto& p=pet();switch(a){case 0:p.food=std::min(100.0,p.food+25);message="Snack time! A little treat goes a long way.";break;case 1:p.clean=100;message="Fresh and sparkling. Ready for anything.";break;case 2:p.joy=std::min(100.0,p.joy+25);message="A little playtime, a lot of happiness.";break;case 3:if(p.training>=10){message="Training mastered! Try a new battle.";return false;}p.training++;p.joy=std::min(100.0,p.joy+8);message="Practice makes progress! Battle strength +1.";break;default:return false;}return true;}
int Game::nextId(int branch)const {if(pet().id==133)return 134+std::clamp(branch,0,2);return dex(pet().id).next;}
int Game::neededLevel()const {return dex(pet().id).evolutionLevel;}
int Game::neededXp()const {return nextId()?(neededLevel()-5)*20:0;}
int Game::neededSeconds()const{if(!nextId())return 0;return (dex(nextId()).next?36:72)*60*60;}
bool Game::ready()const{return pet().owned&&pet().hatch>=30&&nextId()&&pet().age+pet().hatch>=neededSeconds()&&level()>=neededLevel();}
bool Game::evolve(int branch){if(battle.active||sleeping||paused||!ready()||branch<0||branch>2)return false;pet().id=nextId(branch);pet().stageAge=0;pet().stageXp=0;seen[pet().id]=1;message=std::string("Hello, ")+dex(pet().id).name+"! A new chapter begins.";return true;}
int Game::level()const{return 5+std::min(95,pet().xp/20);}
int Game::maximumHp()const{return 60+level()*3+dex(pet().id).stage*18;}
float effectiveness(int a,int d){
    if((a==Fire&& (d==Grass||d==Bug))||(a==Water&&d==Fire)||(a==Grass&&d==Water)||(a==Electric&&(d==Water||d==Flying))||(a==Flying&&(d==Grass||d==Bug))||(a==Bug&&d==Psychic)||(a==Ghost&&(d==Ghost||d==Psychic)))return 1.7f;
    if((a==Fire&&d==Water)||(a==Water&&d==Grass)||(a==Grass&&(d==Fire||d==Flying||d==Bug))||(a==Electric&&d==Grass)||(a==Normal&&d==Ghost)||(a==Ghost&&d==Normal)||(a==Bug&&d==Fire))return .6f;
    if(a==d&&a!=Normal&&a!=Flying)return .75f;return 1;
}
bool Game::encounter(int z){
    static const int gates[]={0,3,8,15}; if(z<0||z>3||wins<gates[z]||battle.active||pet().hatch<30||!pet().owned||sleeping||paused)return false;
    int enemies[151],count=0;for(auto& s:species)if(z==3||(z==0&&s.stage==0&&s.id<129)||(z==1&&s.stage<=1&&s.id<144)||(z==2&&s.stage>0))enemies[count++]=s.id;
    battle=Battle{};battle.active=1;battle.zone=z;battle.enemy=enemies[random(count)];battle.maxHp=maximumHp();battle.hp=battle.maxHp;
    battle.enemyMax=48+z*30+level()*2;battle.enemyHp=battle.enemyMax;seen[battle.enemy]=1;
    message=std::string("A wild ")+dex(battle.enemy).name+" appeared. Choose your move!";return true;
}
bool Game::move(int a){
    auto& b=battle;if(!b.active||sleeping||paused||a<0||a>4)return false;
    if(a==4){b.active=0;message="Back home safely. No battle experience earned.";return true;}
    if(a==1&&b.focus<2){message="Need 2 focus. Use Tackle or Guard to recharge.";return false;}
    if(a==3&&!b.potion){message="Your berry has already been used this battle.";return false;}
    bool guard=a==2;std::string result;int damage=0;
    if(a==0||a==1){float eff=a==1?effectiveness(dex(pet().id).type,dex(b.enemy).type):effectiveness(Normal,dex(b.enemy).type);int power=a==1?19:12;damage=int((power+level()/2+pet().training+dex(pet().id).stage*4+random(5))*eff);b.enemyHp=std::max(0,b.enemyHp-damage);if(a==1)b.focus-=2;else b.focus=std::min(5,b.focus+1);result="You dealt "+std::to_string(damage)+" damage.";if(eff>1)result+=" Super effective!";}
    else if(a==2){b.focus=std::min(5,b.focus+2);result="Guard up. Focus restored.";}
    else{b.hp=std::min(b.maxHp,b.hp+b.maxHp/2);b.potion=0;result="Berry restored half your maximum HP.";}
    b.turn=std::min(100000,b.turn+1);
    if(b.enemyHp==0){b.active=0;wins=std::min(100000,wins+1);int reward=20+b.zone*10;pet().xp=std::min(100000,pet().xp+reward);pet().stageXp=std::min(100000,pet().stageXp+reward);pet().joy=std::min(100.0,pet().joy+10);message="Victory! +"+std::to_string(reward)+" battle XP. "+(ready()?"Evolution is ready!":"Your partner is growing stronger.");return true;}
    int hit=int((9+b.zone*5+level()/3+random(6))*effectiveness(dex(b.enemy).type,dex(pet().id).type));if(guard)hit=std::max(1,hit/4);b.hp=std::max(0,b.hp-hit);result+=" Opponent dealt "+std::to_string(hit)+".";
    if(b.hp==0){b.active=0;losses=std::min(100000,losses+1);pet().xp=std::min(100000,pet().xp+8);pet().stageXp=std::min(100000,pet().stageXp+8);message="A brave effort! +8 battle XP. Your partner is healed at home.";return true;}
    message=result;return true;
}
std::string Game::save()const{
    std::ostringstream o;o<<std::setprecision(17)<<"VIBE_BEASTS 2\n"<<selected<<' '<<wins<<' '<<losses<<' '<<rng<<' '<<lastTime<<' '<<clockSet<<' '<<sleeping<<' '<<paused<<'\n';
    for(auto& p:pets)o<<p.id<<' '<<p.xp<<' '<<p.stageXp<<' '<<p.training<<' '<<p.age<<' '<<p.stageAge<<' '<<p.food<<' '<<p.joy<<' '<<p.clean<<' '<<p.owned<<' '<<p.hatch<<'\n';
    for(int i:seen)o<<i<<' ';o<<'\n';auto& b=battle;o<<b.active<<' '<<b.enemy<<' '<<b.hp<<' '<<b.maxHp<<' '<<b.enemyHp<<' '<<b.enemyMax<<' '<<b.focus<<' '<<b.potion<<' '<<b.zone<<' '<<b.turn<<'\n';return o.str();
}
bool Game::load(const std::string& text){
    if(text.size()>16000)return false;Game g;std::istringstream in(text);std::string magic;int version;
    if(!(in>>magic>>version))return false;const bool legacy=magic=="POCKET" "_PARTNER"&&version==1;if(!legacy&&(magic!="VIBE_BEASTS"||version!=2))return false;
    if(!(in>>g.selected>>g.wins>>g.losses>>g.rng>>g.lastTime>>g.clockSet>>g.sleeping>>g.paused))return false;
    if(g.clockSet<0||g.clockSet>1||g.sleeping<0||g.sleeping>1||g.paused<0||g.paused>1)return false;
    if(g.selected<0||g.selected>=Families||g.wins<0||g.wins>100000||g.losses<0||g.losses>100000||!g.rng||!std::isfinite(g.lastTime)||g.lastTime<0||g.lastTime>4102444800.)return false;
    for(int i=0;i<(legacy?10:Families);i++){auto& p=g.pets[i];if(!(in>>p.id>>p.xp>>p.stageXp>>p.training>>p.age>>p.stageAge>>p.food>>p.joy>>p.clean>>p.owned>>p.hatch))return false;
        if(dex(p.id).id!=p.id||dex(p.id).family!=i||p.xp<0||p.xp>100000||p.stageXp<0||p.stageXp>p.xp||p.training<0||p.training>10)return false;
        for(double v:{p.age,p.stageAge,p.food,p.joy,p.clean,p.hatch})if(!std::isfinite(v)||v<0)return false;
        if(p.owned<0||p.owned>1||p.hatch>30)return false;
        if(p.age>315360000||p.stageAge>p.age||p.food>100||p.joy>100||p.clean>100)return false;
    }
    for(auto& v:g.seen)if(!(in>>v)||v<0||v>1)return false;
    auto& b=g.battle;if(!(in>>b.active>>b.enemy>>b.hp>>b.maxHp>>b.enemyHp>>b.enemyMax>>b.focus>>b.potion>>b.zone>>b.turn))return false;
    if(b.active<0||b.active>1||b.hp<0||b.hp>b.maxHp||b.maxHp<0||b.maxHp>1000||b.enemyHp<0||b.enemyHp>b.enemyMax||b.enemyMax<0||b.enemyMax>1000||b.focus<0||b.focus>5||b.potion<0||b.potion>1||b.zone<0||b.zone>3||b.turn<0||b.turn>100000)return false;
    static const int gates[]={0,3,8,15};
    if(b.active&&(g.sleeping||!g.pet().owned||g.pet().hatch<30||!g.seen[g.pet().id]||b.hp==0||b.enemyHp==0||dex(b.enemy).id!=b.enemy||g.wins<gates[b.zone]))return false;
    if(g.wins<g.unlockAt(g.selected))return false;
    in>>std::ws;if(!in.eof())return false;g.lastTime=0;g.message=b.active?"Battle resumed. Choose your next move.":"Welcome back. Your partner missed you!";*this=g;return true;
}
}
static vb::Game globalGame;
extern "C" {
int vb_get(int k){auto&g=globalGame;auto&p=g.pet();auto&b=g.battle;switch(k){case 0:return p.id;case 1:return g.selected;case 2:return g.wins;case 3:return g.losses;case 4:return g.level();case 5:return p.xp;case 6:return p.stageXp;case 7:return int(p.stageAge);case 8:return g.neededXp();case 9:return g.neededSeconds();case 10:return g.ready();case 11:return int(p.food);case 12:return int(p.joy);case 13:return int(p.clean);case 14:return p.training;case 15:return p.owned;case 16:return b.active;case 17:return b.enemy;case 18:return b.hp;case 19:return b.maxHp;case 20:return b.enemyHp;case 21:return b.enemyMax;case 22:return b.focus;case 23:return b.potion;case 24:return b.zone;case 25:return int(p.age);case 26:return g.nextId();case 27:return vb::dex(p.id).type;case 28:return vb::speciesCount;case 29:return b.turn;case 30:return int(p.hatch);case 31:return g.sleeping;case 32:return g.paused;case 33:return g.clockSet;case 34:return vb::Families;case 35:return g.neededLevel();case 36:return int(p.age+p.hatch);default:return 0;}}
int vb_pet(int f,int field){if(f<0||f>=vb::Families)return 0;auto&g=globalGame;switch(field){case 0:return g.pets[f].id;case 1:return g.wins>=g.unlockAt(f);case 2:return g.unlockAt(f);case 3:return g.seen[g.pets[f].id];case 4:return vb::roots[f];default:return 0;}}
int vb_species(int i,int f){if(i<0||i>=vb::speciesCount)return 0;const auto&s=vb::species[i];switch(f){case 0:return s.id;case 1:return s.type;case 2:return globalGame.seen[s.id];case 3:return s.family;case 4:return s.next;default:return 0;}}
const char* vb_name(int id){return vb::dex(id).name;}
const char* vb_message(){return globalGame.message.c_str();}
void vb_tick(double now){globalGame.tick(now);}
int vb_action(int a,int x){switch(a){case 0:return globalGame.choose(x);case 1:return globalGame.care(x);case 2:return globalGame.encounter(x);case 3:return globalGame.move(x);case 4:return globalGame.evolve(x);case 5:globalGame.clockSet=1;globalGame.message="Clock set. Your egg will hatch in 30 awake seconds.";return 1;case 6:if(globalGame.battle.active)return 0;globalGame.sleeping=!globalGame.sleeping;globalGame.message=globalGame.sleeping?"Goodnight. Evolution time is resting too.":"Good morning! Awake time is counting again.";return 1;case 7:globalGame.paused=x?1:0;return 1;default:return 0;}}
const char* vb_save(){static std::string s;s=globalGame.save();return s.c_str();}
int vb_load(const char* s){return s&&globalGame.load(s);}
}
