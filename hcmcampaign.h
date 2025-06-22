/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit; // done
class UnitList; // done
class Army; // done
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;
class Forest;
class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};






class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const {return r;}
    int getCol() const {return c;}
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;
    int attackScore;
    int armyBeLong;
    bool isDynamic = true;

public:
    Unit(int quantity, int weight, Position pos , int BeLong);
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    int getAttackScore() {return attackScore;}

    Position getCurrentPosition() const;
    virtual void updateAttackScore() = 0;

    virtual string str() const = 0;

    void setAttackScore(int a) {attackScore=a;}

     int getQuantity() { return quantity;}
   int getWeight(){ return weight;}
    void setQuantity( int a) { quantity = a;}
    void setWeight(int b) {weight = b;}
    int getArmyBeLong() const  {return armyBeLong;}

    virtual int getType() const = 0;

    void setIsDynamic(bool val) { isDynamic = val; }
    bool getIsDynamic() const { return isDynamic; }
};

class Vehicle : public Unit{
private :
    VehicleType vehicleType;
    friend class UnitList;
public :
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType, int belong);
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    void updateAttackScore()  override  ;
    string str() const override ;
    int getType() const override { return static_cast<int>(vehicleType); }

};


class Infantry : public Unit {
private :
InfantryType infantryType;
friend class UnitList;
public :
Infantry(int quantity, int weight , const Position pos , InfantryType infantryType, int belong);
Infantry(int quantity, int weight , const Position pos , InfantryType infantryType);
void updateAttackScore() override ;
string str() const override;
int getType() const override { return static_cast<int>(infantryType);}

void applyMoraleEffect();
};

class Army
{
    friend class LiberationArmy;
    friend class ARVN;
    friend class HCMCampaign;
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;
public:

    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
     UnitList* getUnitList() { return unitList; }
    void updateLF(int a){LF=a;}
    void updateEXP ( int b) { EXP = b;}

    int getLF(){ return LF;}
    int getEXP() { return EXP;}

    vector<Unit*> getAllUnits() const;
};
class LiberationArmy : public Army{
public :
    LiberationArmy(Unit** unitArray, int size, string name, BattleField* battleField );
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;


};

class ARVN : public Army{
public:
    ARVN(Unit** unitArray, int size, string name, BattleField* battleField);
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;

};

struct Node {
    Unit* data;
    Node* next;
    Node(Unit* u) : data(u), next(nullptr) {}
};

class UnitList
{
private:

    int size; // de cap nhat xem co bao nhieu Unit hien tai.
    int capacity;
    Node* head;

    // TODO
public:
    UnitList(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    void xoatohopUnit(const vector<Unit*> &units);
    vector <Unit*>  findMinSubsetInfantry (int target);
    vector <Unit*> findMinSubsetVehicle (int target);
    void removeNode(Node* name);
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    Node* getHead() const { return head;}
    void sort();
    // TODO
};

class TerrainElement
{
protected :
    Position pos;
public:
    TerrainElement(const Position &pos) : pos(pos){}
    TerrainElement() = default;
    ~TerrainElement()= default; // đỡ phai implement trong cpp
    virtual void getEffect(Army *army) = 0;
};


class Road : public TerrainElement{
public:
Road( const Position &pos);
~Road() ;
void getEffect(Army *army) override;
};

class Forest: public TerrainElement{
public:

    Forest(const Position &pos);
    ~Forest();
    void getEffect(Army *army) override;
};
class River : public TerrainElement{
public :
    River(const Position &pos);
    ~River();
    void getEffect(Army* army) override;


};

class Urban : public TerrainElement{
public:
Urban(const Position &pos);
~Urban();
void getEffect(Army * army) override ;


};

class Fortification : public TerrainElement {

public :
    Fortification(const Position &pos);
    ~Fortification();
    void getEffect(Army * army ) override;

};

class SpecialZone : public TerrainElement{

public:
    SpecialZone(const Position &pos);
    ~SpecialZone();
    void getEffect(Army *army) override;

};







class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement*** terrain;

    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    void terrainEffect(Army * army);
    string str() const ;
};
    // 3.9





class Configuration{
    friend class HCMCampaign;
private:
    int num_rows, num_cols;
    vector <Position*> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    vector <Unit*> liberationUnits;
    vector  <Unit*> ARVNUnits;
    int eventCode;

public :
    Configuration(const string& filepath);
    ~Configuration();
    string str() const;

};





class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ARVNarmy;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
    void removeWeakUnits(Army* army);
};

#endif
