#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////
//                      3.1



//
bool isPower(int n, int base) {
    if (n < 1) return false;
    int val = 1;
    while (val < n) val *= base;
    return val == n;
}

// Kiểm tra tổng các lũy thừa riêng biệt của base
bool canRepresentByPowers(int n, int base, int maxPower) {
    if (n == 0) return true;
    if (n < 0 || maxPower < 0) return false;

    int val = pow(base, maxPower);
    // Chọn hoặc không chọn lũy thừa này
    return canRepresentByPowers(n - val, base, maxPower -1) || canRepresentByPowers(n, base, maxPower -1);
}

bool isSpecialNumber(int S) {
    vector<int> bases = {3,5,7};
    for (int base : bases) {
        int maxPower = 0;
        while (pow(base, maxPower) <= S) maxPower++;
        maxPower--;
        if (canRepresentByPowers(S, base, maxPower)) return true;
    }
    return false;
}



//
void UnitList::sort() {
    if (!head || !head->next) return; //

    bool swapped;
    do {
        swapped = false;
        Node* prev = nullptr;
        Node* curr = head;
        while (curr && curr->next) {
            Unit* u1 = curr->data;
            Unit* u2 = curr->next->data;

            bool shouldSwap = false;

            Infantry* inf1 = dynamic_cast<Infantry*>(u1);
            Infantry* inf2 = dynamic_cast<Infantry*>(u2);

            if (inf1 && inf2) {
                // Cả hai đều Infantry: so sánh InfantryType
                shouldSwap = (inf1->infantryType > inf2->infantryType);
            } else if (inf1 && !inf2) {

                shouldSwap = false;
            } else if (!inf1 && inf2) {
                // Vehicle đứng sau Infantry → đổi chỗ
                shouldSwap = true;
            } else {
                // Cả hai Vehicle: so sánh VehicleType
                Vehicle* v1 = dynamic_cast<Vehicle*>(u1);
                Vehicle* v2 = dynamic_cast<Vehicle*>(u2);
                if (v1 && v2) {
                    shouldSwap = (v1->vehicleType > v2->vehicleType);
                }
            }

            if (shouldSwap) {
                // Đổi chỗ hai node curr và curr->next
                Node* temp = curr->next;
                curr->next = temp->next;
                temp->next = curr;

                if (prev) {
                    prev->next = temp;
                } else {
                    head = temp;
                }

                swapped = true;
                prev = temp;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (swapped);
}

void UnitList::removeNode(Node* nodeToRemove) {
    if (head == nullptr || nodeToRemove == nullptr) return;

    if (head == nodeToRemove) {
        head = head->next;
        if (nodeToRemove->data->getIsDynamic()) {
            delete nodeToRemove->data;  // Giải phóng bộ nhớ đơn vị
        }
        delete nodeToRemove;
        size--;
        return;
    }

    Node* prev = head;
    while (prev->next != nullptr && prev->next != nodeToRemove) {
        prev = prev->next;
    }

    if (prev->next == nullptr) return; // Node không tìm thấy trong danh sách

    // Bỏ qua node cần xóa
    prev->next = nodeToRemove->next;
    if (nodeToRemove->data->getIsDynamic()) {
        delete nodeToRemove->data;
    }
    delete nodeToRemove;
    size--;
}


// Tinh lai totalLF

int recalculateLF(UnitList* ul){
int totalLF=0;
for(Node* p = ul->getHead(); p!= nullptr; p=p->next ){
    Vehicle* v =dynamic_cast<Vehicle*>(p->data);
    if( v!= nullptr){
        totalLF += v->getAttackScore();
    }
}
if(totalLF > 1000) totalLF=1000;
if(totalLF< 0 ) totalLF = 0;
return totalLF;
}

int recalculateEXP(UnitList* ul){

int totalEXP = 0;

for(Node* p = ul->getHead() ; p!= nullptr ; p=p->next){
    Infantry* i =dynamic_cast<Infantry*>(p->data);
    if(i != nullptr){
        totalEXP += i->getAttackScore() ;

    }
}
if(totalEXP > 1000) totalEXP =1000;
if(totalEXP < 0) totalEXP = 0;
return totalEXP;

}






//3.1
Unit::Unit(int quantity, int weight, Position pos)
    : Unit(quantity, weight, pos, 0) {}



Unit::Unit(int quantity, int weight, Position pos, int BeLong){
this->quantity=quantity;
this->weight = weight;
this->pos = pos;
this->armyBeLong = BeLong;
}


Unit::~Unit(){
}



Position Unit::getCurrentPosition() const {
    return pos;
}



//                          3.2

/*
class Vehicle : public Unit{
private :
    VehicleType vehicleType;
public :
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    int getAttackScore()  override  ;
    string str() const override ;

};
*/

string to_string(VehicleType t) {
    switch (t) {
        case TRUCK: return "TRUCK";
        case MORTAR: return "MORTAR";
        case ANTIAIRCRAFT: return "ANTIAIRCRAFT";
        case ARMOREDCAR: return "ARMOREDCAR";
        case APC: return "APC";
        case ARTILLERY: return "ARTILLERY";
        case TANK: return "TANK";
    }
    return "UNKNOWN";
}

string to_string(InfantryType t) {
    switch (t) {
        case SNIPER: return "SNIPER";
        case ANTIAIRCRAFTSQUAD: return "ANTIAIRCRAFTSQUAD";
        case MORTARSQUAD: return "MORTARSQUAD";
        case ENGINEER: return "ENGINEER";
        case SPECIALFORCES: return "SPECIALFORCES";
        case REGULARINFANTRY: return "REGULARINFANTRY";
    }
    return "UNKNOWN";
}

Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType):
    Unit(quantity,weight,pos){
    this->vehicleType = vehicleType; // cach 1
    updateAttackScore();

}

Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType, int belong):
    Unit(quantity,weight,pos,belong){


    this->vehicleType = vehicleType; // cach 1
    updateAttackScore();

}



 void Vehicle::updateAttackScore(){
    int typeValue = static_cast<int>(vehicleType);
    attackScore= ceil((typeValue*304 + (quantity *weight))/30.0);

    }
string Vehicle::str()const {
ostringstream out;
out <<"Vehicle["
    <<"vehicleType=" << to_string(vehicleType)<<","
    <<"quantity=" << quantity << ","
    <<"weight=" << weight << ","
    <<"position=" <<pos.str()
    <<"]";
    return out.str();
}


//                      3.3
/*
class Infantry : public Unit {
private :
InfantryType infantryType;
public :
Infantry(int quantity, int weight , const Position pos , InfantryType infantryType);
int getAttackScore() override ;
string str() const override;

};
*/
static bool isPerfectSquare(int a){
if(a<0) return 0;
int r = static_cast<int>(sqrt(a)+0.5);
return (a == (r*r));

}


static int personalnumber(int quan, int year){
int sum=quan;
int y = year;
while( y >0){
    sum+=(y%10);
    y=y/10;
}

while(sum>=10){
        int temp=0;
    while(sum>0){
    temp += sum%10;
    sum= sum/10;
    }
    sum=temp;
}
return sum;

}


Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
:Unit(quantity,weight,pos){
this->infantryType = infantryType;
updateAttackScore();
}

Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType, int belong)
:Unit(quantity,weight,pos , belong){

this->infantryType = infantryType;
updateAttackScore();

}


/*
void Infantry::updateAttackScore(){
int typeValue=static_cast<int>(infantryType);
int score;
score = typeValue*56 +(quantity*weight);

if(typeValue==SPECIALFORCES && isPerfectSquare(weight)){ // check xem phai dac cong kh
score+=75;
}


int pn = personalnumber(quantity,1975);
int effective_quantity = quantity;
if (pn > 7) {
    effective_quantity = ceil(quantity * 1.2);

}
else if (pn < 3) {
    effective_quantity = ceil(quantity * 0.9);

}
score = typeValue * 56 + (effective_quantity * weight);

if(typeValue==SPECIALFORCES && isPerfectSquare(weight)){ // check xem phai dac cong kh
score+=75;
}
attackScore = ceil(score);
}
*/

void Infantry::updateAttackScore() {
    int typeValue = static_cast<int>(infantryType);


    int score = typeValue * 56 + quantity * weight;


    if (infantryType == SPECIALFORCES && isPerfectSquare(weight)) {
        score += 75;
    }


    int pn = personalnumber(score, 1975);


    if (pn > 7) {
        quantity = static_cast<int>(ceil(quantity * 1.2));
    } else if (pn < 3) {
        quantity = static_cast<int>(ceil(quantity * 0.9));
    }


    score = typeValue * 56 + quantity * weight;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight)) {
        score += 75;
    }

    attackScore = ceil(score);
}
/*
void Infantry::applyMoraleEffect() {
    updateAttackScore();
    int pn = personalnumber(score, 1975);
    if (pn > 7) {
        quantity = static_cast<int>(ceil(quantity * 1.2));
    } else if (pn < 3) {
        quantity = static_cast<int>(ceil(quantity * 0.9));
    }
    updateAttackScore();
}
*/

string Infantry::str() const{
ostringstream os;
os<<"Infantry["
    <<"infantryType="<<to_string(infantryType)<<","
    <<"quantity="<<quantity<<","
    <<"weight="<<weight<<","
    <<"position="<<pos.str()<<"]";
    return os.str(); // no se viet vao dong` os chu khong viet thang ra man` hinh


}


//                 3.4

/*
class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
};
*/
Army::Army(Unit** unitArray, int size, string name, BattleField* battleField) {
    this->name = name;
    this->battleField = battleField;

    // Khởi tạo LF, EXP = 0
    this->LF = 0;
    this->EXP = 0;

    // Tính điểm LF và EXP trước
    for (int i = 0; i < size; i++) {
        Unit* u = unitArray[i];
        int score = u->getAttackScore();
        if (dynamic_cast<Vehicle*>(u)) {
            LF += score;
        } else {
            EXP += score;
        }
    }

    // Giới hạn LF và EXP theo đề
    if (LF > 1000) LF = 1000;
    if (LF < 0) LF = 0;
    if (EXP > 500) EXP = 500;
    if (EXP < 0) EXP = 0;

    int totalScore = LF + EXP;

    // Xác định capacity theo tổng điểm
    int capacity = isSpecialNumber(totalScore) ? 12 : 8;

    // Tạo UnitList với capacity chính xác
    unitList = new UnitList(capacity);

    // Thêm các đơn vị vào UnitList, setIsDynamic(false)
    for (int i = 0; i < size; i++) {
        Unit* u = unitArray[i];
        u->setIsDynamic(false);
        unitList->insert(u);
    }
}

//                  3.4.1


LiberationArmy::LiberationArmy(Unit** unitArray, int size , string name, BattleField* battleField)
:Army(unitArray, size, name , battleField){}

string LiberationArmy::str()const{
ostringstream os;

   os<<name<<"["
    <<"LF="<<LF<<","
    <<"EXP="<<EXP<<","
    <<"unitList="<<unitList->str()<<","
    <<"battleField=";
    if(battleField){
    os<<battleField->str();
    }
    os<<"]";


    return os.str();

}




int nearestFibonacci(int n) { ///// Ham tinh so Fibonacci gan nhat >= n
    if (n <= 1) return 1;
    int a = 1, b = 1;
    while (b < n) {
        int temp = b;
        b = a + b;
        a = temp;
    }
    return b;
}

void increaseQuantityFibonacci(UnitList* ul) { // tang so luong moi don vi theo Fibonacci gan nhat
    for (Node* p = ul->getHead(); p != nullptr; p = p->next) {
        int oldQ = p->data->getQuantity();
        int newQ = nearestFibonacci(oldQ);
        p->data->setQuantity(newQ);
        p->data->updateAttackScore();
    }
}
void reduceQuantityByPercent(UnitList* ul, double percent) {
    for (Node* p = ul->getHead(); p != nullptr; p = p->next) {
        int oldQ = p->data->getQuantity();
        int newQ = static_cast<int>(ceil(oldQ * percent));
        if (newQ < 1) newQ = 1; // Ít nhất còn 1 quân
        p->data->setQuantity(newQ);
        p->data->updateAttackScore();
    }
}
void findBestSubset(const vector<Unit*>& units, int target, int index,
                    vector<Unit*>& currentSet, int currentSum,
                    vector<Unit*>& bestSet, int& bestSum) {
    if (currentSum >= target) {
        if (currentSum < bestSum) {
            bestSum = currentSum;
            bestSet = currentSet;
        }
        return; // cắt tỉa vì đã >= target
    }
    if (index == units.size()) return;

    // Không chọn đơn vị hiện tại
    findBestSubset(units, target, index + 1, currentSet, currentSum, bestSet, bestSum);

    // Chọn đơn vị hiện tại
    currentSet.push_back(units[index]);
    findBestSubset(units, target, index + 1, currentSet, currentSum + units[index]->getAttackScore(), bestSet, bestSum);
    currentSet.pop_back();
}

vector<Unit*> UnitList::findMinSubsetInfantry(int target) {
    vector<Unit*> infantryUnits;

    // Lọc ra các đơn vị Infantry
    for (Node* p = head; p != nullptr; p = p->next) {
        Infantry* inf = dynamic_cast<Infantry*>(p->data);
        if (inf != nullptr && inf->getQuantity() > 0) {
            infantryUnits.push_back(inf);
        }
    }

    vector<Unit*> currentSet;
    vector<Unit*> bestSet;
    int bestSum = INT_MAX;  // Dùng INT_MAX từ <climits>

    findBestSubset(infantryUnits, target, 0, currentSet, 0, bestSet, bestSum);

    if (bestSum == INT_MAX) {
        // Không tìm được tổ hợp thoả
        return vector<Unit*>();
    } else {
        return bestSet;
    }
}

vector<Unit*> UnitList::findMinSubsetVehicle(int target) {
    vector<Unit*> vehicleUnits;

    // Lọc ra các đơn vị Vehicle
    for (Node* p = head; p != nullptr; p = p->next) {
        Vehicle* veh = dynamic_cast<Vehicle*>(p->data);
        if (veh != nullptr && veh->getQuantity() > 0) {
            vehicleUnits.push_back(veh);
        }
    }

    vector<Unit*> currentSet;
    vector<Unit*> bestSet;
    int bestSum = INT_MAX; // hoặc std::numeric_limits<int>::max()

    findBestSubset(vehicleUnits, target, 0, currentSet, 0, bestSet, bestSum);

    if (bestSum == INT_MAX) {
        return vector<Unit*>(); // Không tìm được tổ hợp thoả
    }
    return bestSet;
}




void UnitList::xoatohopUnit(const vector<Unit*>& units) {
    for (Unit* uRemove : units) {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur != nullptr) {
            if (cur->data == uRemove) {
                // Xóa node cur
                if (prev == nullptr) {
                    // Xóa đầu danh sách
                    head = cur->next;
                } else {
                    prev->next = cur->next;
                }
                if (cur->data->getIsDynamic()) delete cur->data;

                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
                size--;
                break; // thoát vòng while, chuyển sang đơn vị tiếp theo
            } else {
                prev = cur;
                cur = cur->next;
            }
        }
    }
}



void LiberationArmy::fight( Army * enemy, bool defense){
if(!defense){// attack

updateLF(ceil(this->getLF() *1.5 ));
updateEXP(ceil(this->getEXP() *1.5 ));

vector<Unit*> comboA = this->unitList->findMinSubsetInfantry(enemy->getEXP());
vector<Unit*> comboB = this->unitList->findMinSubsetVehicle(enemy->getLF());
bool foundA= !comboA.empty();
bool foundB = !comboB.empty();

if(foundA && foundB) {// chien thang thi xoa' o enemy, insert o quan giai phong
 this->unitList->xoatohopUnit(comboA);
this->unitList->xoatohopUnit(comboB);
/*
for(Unit* u : comboA) unitList->insert(u);
 for(Unit* u : comboB) unitList->insert(u);
    */
vector<Unit*> enemyUnits;
for (Node* p = enemy->unitList->getHead(); p != nullptr; p = p->next) {
    enemyUnits.push_back(p->data);
}


while (enemy->unitList->getHead() != nullptr) {
    Node* toDelete = enemy->unitList->getHead();
    enemy->unitList->removeNode(toDelete);
}
enemy->updateLF(0);
enemy->updateEXP(0);

for (Unit* u : enemyUnits) {
    unitList->insert(u);
}

    unitList->sort();
 updateLF(recalculateLF(unitList));
 updateEXP(recalculateEXP(unitList));



 return;

}

if(foundA || foundB) {
        if(foundA){
            if(getLF() > enemy->getLF() ){
                this->unitList->xoatohopUnit(comboA);
                // xoa nhung unit k thoa man
                vector<Unit*> enemyVehUnits;
                for(Node* p = enemy->unitList->getHead(); p != nullptr ; p = p->next){
                    enemyVehUnits.push_back(p->data);
                }// lay enemy->unitList xong

                // gio ta se xoa enemy->unitList

                Node* p = enemy->unitList->getHead();
                while(p != nullptr){
                    Node* next = p->next;
                    enemy->unitList->removeNode(p);
                    p = next;
                }
                enemy->updateLF(0);
                enemy->updateEXP(0);

                // sau do ta insert enemyVehUnits vao this->unitList

                for(Unit* p : enemyVehUnits){
                    this->unitList->insert(p);
                }

                unitList->sort();
                updateLF(recalculateLF(unitList));
                 updateEXP(recalculateEXP(unitList));



                return;
            }
        }
        else{ // neu khong tim dc A ma tim dc B (foundB = 1)
            if(getEXP() > enemy->getEXP() ){
                this->unitList->xoatohopUnit(comboB);
                // xoa nhung unit k thoa man
                vector<Unit*> enemyUnits;
                for(Node* p = enemy->unitList->getHead(); p != nullptr ; p = p->next){
                    enemyUnits.push_back(p->data);
                }// lay enemy->unitList xong

                // gio ta se xoa enemy->unitList

                Node* p = enemy->unitList->getHead();
                while(p != nullptr){
                    Node* next = p->next;
                    enemy->unitList->removeNode(p);
                    p = next;
                }
                enemy->updateLF(0);
                enemy->updateEXP(0);

                // sau do ta insert enemyVehUnits vao this->unitList

                for(Unit* un : enemyUnits){
                    this->unitList->insert(un);
                }

                unitList->sort();
                updateLF(recalculateLF(unitList));
                 updateEXP(recalculateEXP(unitList));



                return;
            }
        }

}
// Khong tim duoc to hop nao, khong chien'

for(Node* p = unitList->getHead() ; p!=nullptr; p=p->next){
    int newWeight = static_cast<int>(ceil(p->data->getWeight() * 0.9));
    if (newWeight < 1) newWeight = 1;
    p->data->setWeight(newWeight);
    p->data->updateAttackScore();

}
    updateLF(recalculateLF(unitList));
    updateEXP(recalculateEXP(unitList));

} else {  // tan cong ket thuc

 // Defense mode

updateLF ( ceil(getLF() *1.3));
updateEXP( ceil (getEXP() *1.3));

bool LFge = (getLF() >= enemy->getLF()) ;
bool EXPge = (getEXP() >= enemy->getEXP());

if(LFge && EXPge){

    // phong thu thanh cong
    return;


}
if( !LFge || !EXPge){

//giam 10% so luong moi don vi
    reduceQuantityByPercent(unitList,0.9); // neu chi co' 1 LFge hoac EXPge = 0
    if( !LFge && !EXPge){// ca 2 deu` nho hon thi can chi vien
        increaseQuantityFibonacci(unitList);

        updateLF(recalculateLF(unitList));
        updateEXP(recalculateEXP(unitList));

        LFge = (getLF() >= enemy->getLF() );
        EXPge = (getEXP() >= enemy->getEXP() );

        if( LFge && EXPge) return; // sau khi chi vien quan ta thang'
    }


}
}



}

ARVN::ARVN(Unit** unitArray, int size, string name, BattleField* battleField)
: Army(unitArray, size, name, battleField){}




void ARVN::fight(Army* enemy, bool defense) {
    if (!defense) { // ARVN tấn công
        Node* prev = nullptr;
        Node* cur = unitList->getHead();

        while (cur != nullptr) {
            Unit* u = cur->data;
            int oldQ = u->getQuantity();
            int newQ = static_cast<int>(ceil(oldQ * 0.8));
            if (newQ < 1) newQ = 1;

            u->setQuantity(newQ);
            u->updateAttackScore();

            Node* nextNode = cur->next;

            if (newQ <= 1) {
                // Xóa node cur khỏi danh sách
                unitList->removeNode(cur);
                if (prev == nullptr) {
                    // Nếu xóa node đầu, cập nhật cur bằng head mới
                    cur = unitList->getHead();
                } else {
                    cur = prev->next;
                }
                // Không cập nhật prev vì cur bị xóa
            } else {
                prev = cur;
                cur = nextNode;
            }
        }

        // Cập nhật lại LF, EXP
        updateLF(recalculateLF(unitList));
        updateEXP(recalculateEXP(unitList));
        return;
    }

    // Phòng thủ (defense = true)
    // Quân Giải phóng tấn công

    // Quân Giải phóng thắng nếu LF hoặc EXP lớn hơn ARVN
    bool enemyWins = (enemy->getLF() > getLF()) || (enemy->getEXP() > getEXP());

    if (enemyWins) {
        // Tịch thu: xóa đơn vị bị mất khỏi ARVN
        // Ở đây đơn giản giả sử xóa hết toàn bộ đơn vị của ARVN (có thể điều chỉnh nếu đề chi tiết)
        while (unitList->getHead() != nullptr) {
            Node* toDelete = unitList->getHead();
            unitList->removeNode(toDelete);
        }

        // Giảm 20% trọng số weight các đơn vị còn lại (nếu có, nhưng hiện tại đã xoá hết)
        // Nếu cần, xử lý cho các đơn vị còn lại khác

        // Cập nhật lại LF, EXP (hiện tại là 0 do xoá hết)
        updateLF(0);
        updateEXP(0);
        return;
    }

    // Nếu không có giao tranh hoặc không thua thì giữ nguyên lực lượng ARVN
}




string ARVN::str() const{
ostringstream os;

os  <<name<<"["
    <<"LF="<<LF<<","
    <<"EXP="<<EXP<<","
    <<"unitList="<<unitList->str()<<","
    <<"battleField=";
    if(battleField){
        os<<battleField->str();
    }
    os<<"]";
    return os.str();
}
/*
class UnitList
{
private:
    struct Node{

    Unit* data;
    Node* next;
            Node(Unit* u){
            data=u;
            next=nullptr;
            }
    };
    int size; // de cap nhat xem co bao nhieu Unit hien tai.
    int capacity;
    Node* head;

    // TODO
public:
    UnitList(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
};

*/

//                  3.5

UnitList::UnitList(int capacity){
this->capacity = capacity;
head= nullptr;
size= 0;
}

void insertLast(Node* &head, Unit* aunit){
Node* tmp = new Node(aunit);
if(head == nullptr){
    head=tmp;
}
else{
    Node* p = head;
    while(p->next != nullptr){
        p=p->next;
    }
    p->next=tmp;

}



}



/*

// Tim to hop Infantry
void findBestSubset(const vector<Unit*>& units, int target, int index,
                    vector<Unit*>& currentSet, int currentSum,
                    vector<Unit*>& bestSet, int& bestSum) {
    if (currentSum >= target) {
        if (currentSum < bestSum) {
            bestSum = currentSum;
            bestSet = currentSet;
        }
        return; // cắt tỉa vì đã >= target
    }
    if (index == units.size()) return;

    // Không chọn đơn vị hiện tại
    findBestSubset(units, target, index + 1, currentSet, currentSum, bestSet, bestSum);

    // Chọn đơn vị hiện tại
    currentSet.push_back(units[index]);
    findBestSubset(units, target, index + 1, currentSet, currentSum + units[index]->getAttackScore(), bestSet, bestSum);
    currentSet.pop_back();
}

vector<Unit*> UnitList::findMinSubsetInfantry(int target) {
    vector<Unit*> infantryUnits;

    // Lọc ra các đơn vị Infantry
    for (Node* p = head; p != nullptr; p = p->next) {
        Infantry* inf = dynamic_cast<Infantry*>(p->data);
        if (inf != nullptr && inf->getQuantity() > 0) {
            infantryUnits.push_back(inf);
        }
    }

    vector<Unit*> currentSet;
    vector<Unit*> bestSet;
    int bestSum = INT_MAX;  // Dùng INT_MAX từ <climits>

    findBestSubset(infantryUnits, target, 0, currentSet, 0, bestSet, bestSum);

    if (bestSum == INT_MAX) {
        // Không tìm được tổ hợp thoả
        return vector<Unit*>();
    } else {
        return bestSet;
    }
}

vector<Unit*> UnitList::findMinSubsetVehicle(int target) {
    vector<Unit*> vehicleUnits;

    // Lọc ra các đơn vị Vehicle
    for (Node* p = head; p != nullptr; p = p->next) {
        Vehicle* veh = dynamic_cast<Vehicle*>(p->data);
        if (veh != nullptr && veh->getQuantity() > 0) {
            vehicleUnits.push_back(veh);
        }
    }

    vector<Unit*> currentSet;
    vector<Unit*> bestSet;
    int bestSum = INT_MAX; // hoặc std::numeric_limits<int>::max()

    findBestSubset(vehicleUnits, target, 0, currentSet, 0, bestSet, bestSum);

    if (bestSum == INT_MAX) {
        return vector<Unit*>(); // Không tìm được tổ hợp thoả
    }
    return bestSet;
}




void UnitList::xoatohopUnit(const vector<Unit*>& units) {
    for (Unit* uRemove : units) {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur != nullptr) {
            if (cur->data == uRemove) {
                // Xóa node cur
                if (prev == nullptr) {
                    // Xóa đầu danh sách
                    head = cur->next;
                } else {
                    prev->next = cur->next;
                }
                if (cur->data->getIsDynamic()) delete cur->data;

                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
                size--;
                break; // thoát vòng while, chuyển sang đơn vị tiếp theo
            } else {
                prev = cur;
                cur = cur->next;
            }
        }
    }
}
// ket thuc ham xoa to hopunit

*/

bool UnitList::insert(Unit* unit){
if(size >= capacity) return false; // neu' da~ max capacity thi kh them duoc nua

for(Node*p = head; p ; p = p->next ){   // check xem co' trung` hay khong, neu trung` thi tang quantity
    if(Vehicle* v1 = dynamic_cast<Vehicle*> (p->data)){
        if(Vehicle * v2 = dynamic_cast<Vehicle*>(unit)){
            if(v1->vehicleType == v2 ->vehicleType){ // khi khai bao friend class UnitList  thi khong can dung getter nua
                v1->quantity += v2->quantity;
                v1->updateAttackScore();
                return true;
            }
        }

    }

    if(Infantry* i1 = dynamic_cast<Infantry*> (p->data)){
        if(Infantry * i2 = dynamic_cast<Infantry*> (unit)){
            if(i1->infantryType == i2->infantryType){
                i1->quantity += i2->quantity;

                i1->updateAttackScore();
                return true;
            }
        }
    }

}
// Unit khac' loai. thi tao node moi de them vao danh sach head
 Node* tmp = new Node(unit);
    if (!head) {
        head = tmp;
    } else if (dynamic_cast<Infantry*>(unit)) {
        tmp->next = head;
        head = tmp;
        Infantry* i = dynamic_cast<Infantry*>(unit);
        if (i) i->updateAttackScore();
    } else {
        Node* p = head;
        while (p->next) p = p->next;
        p->next = tmp;
        Vehicle* v = dynamic_cast<Vehicle*>(unit);
        if (v) v->updateAttackScore();
    }
    size++;

    return true;
} // ket thuc ham insert

//bool isContain(VehicleType vehicleType);   // return true if it exists
 //   bool isContain(InfantryType infantryType); // return true if it exists
bool UnitList::isContain(VehicleType vehicleType){
for(Node*p = head ; p != nullptr; p=p->next){
    if(Vehicle * tmp = dynamic_cast<Vehicle*> (p->data)){
        if(tmp->vehicleType == vehicleType) return true;
    }
}
return false;
}

bool UnitList::isContain(InfantryType infantryType){

for(Node* p = head ; p!= nullptr ; p=p->next){
    if(Infantry* tmp = dynamic_cast<Infantry*> (p->data)){

        if(tmp->infantryType == infantryType) return true;
    }

}
return false;
}

string UnitList::str() const {
ostringstream os;
int count_veh = 0;
int count_inf = 0;
vector <string> result ;
for( Node* p = head;  p != nullptr; p=p->next ){
    result.push_back(p->data->str());
    if(dynamic_cast<Vehicle*>(p->data)) count_veh++;
    else count_inf ++;
}
os<<"UnitList[count_vehicle="<<count_veh
<<";count_infantry="<<count_inf;
if(count_inf != 0) os<<";";

for(int i = 0 ; i<result.size(); i++){

    os<<result[i];
    if(i+1 <result.size()) os<<",";

}
os<<"]";

return os.str();

}




//                  3.6
/*
class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};

*/

//3.6.2
Position::Position(int r , int c){
this->r=r;
this->c=c;

}
//3.6.3
Position::Position(const string& str_pos){ // đọc để ghi cac so trong 1 string : "(1,15)" de cho no vao r va c

istringstream ss(str_pos);
char c1,c2,c3;
int nr,nc;
// bool ok = ( ss>> c1>>nr>>c2>>nc>>c3);
if( (ss>>c1>>nr>>c2>>nc>>c3) && c1 == '(' && c2 == ',' && c3==')'){
    r=nr;
    c=nc;

}
else{
    r=0;
    c=0;
}
}
// cach 2 :
/*
Position::Position(const string &str_pos) {
    // find positions of delimiters
    size_t start = str_pos.find('(');
    size_t comma = str_pos.find(',');
    size_t end = str_pos.find(')');
    if (start != string::npos && comma != string::npos && end != string::npos) {
        // substring between '(' and ','
        string rowStr = str_pos.substr(start + 1, comma - start - 1);
        // substring between ',' and ')'
        string colStr = str_pos.substr(comma + 1, end - comma - 1);
        r = std::stoi(rowStr);
        c = std::stoi(colStr);
    } else {
        r = 0;
        c = 0;
    }
}
*/

//3.6.4

void Position::setRow( int q){
r=q;

}
void Position::setCol( int b){
c=b;
}

string Position::str() const{
ostringstream os;
os<<"(" <<r<<","<<c<<")";
return os.str();
}


//              3.7

/*
class TerrainElement
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

*/


vector<Unit*>  Army::getAllUnits() const {
    vector<Unit*> result;
     if (!unitList) return result;
    Node* p = unitList->getHead();
    while(p!= nullptr){
        result.push_back(p->data);
        p=p->next;

    }
return result;

}

void TerrainElement::getEffect(Army *army){

}


double euclidD(const Position &p1, const Position &p2){
double a =static_cast<double>(p1.getRow() - p2.getRow());
double  b =static_cast<double> (p1.getCol() - p2.getCol());

return sqrt(a*a + b*b);
}


Road::Road(const Position &pos) : TerrainElement(pos){}

Road::~Road(){}


void Road::getEffect(Army * army){ // khong anh huong nen khong lam gi





}

Forest::Forest(const Position& pos ) :TerrainElement(pos) {}

Forest::~Forest() {}

void Forest::getEffect( Army* army) {
vector<Unit*> allunits = army->getAllUnits();

for(Unit* x : allunits){
    double dis = euclidD(this->pos , x->getCurrentPosition() );
    if(dynamic_cast<LiberationArmy*>(army)){
    if(dis<= 2.0){
        if(Infantry* tmp = dynamic_cast<Infantry*>(x) )  // Bo binh duoc cong them 30% vao score
            army->updateEXP( ceil(army->getEXP() + 0.3 * x->getAttackScore()   )   );
         if(Vehicle * tmp = dynamic_cast<Vehicle*> (x))
            army->updateLF(  ceil (army->getLF() - 0.1 * x->getAttackScore() ) );

    }

    }
    else if(dynamic_cast<ARVN*>(army)){
        if(dis<=4.0){
            if(Infantry* tmp = dynamic_cast<Infantry*>(x)){
                army->updateEXP(ceil( army->getEXP() +0.2 *x->getAttackScore()   ));
            }
            if(Vehicle* tmp = dynamic_cast<Vehicle*> (x)){
                army->updateLF ( ceil( army->getLF() - 0.05*x->getAttackScore()));
            }
        }
    }
}

}
River::River(const Position& pos) : TerrainElement(pos){}


River::~River(){}
void River::getEffect(Army* army){
vector<Unit*> units = army->getAllUnits();

for(Unit* x : units){
        double dis = euclidD(this->pos , x->getCurrentPosition());
        bool isLiberation = (dynamic_cast<LiberationArmy*>(army) != nullptr);
    if(isLiberation){
       if(dis<=2.0){
        if(Infantry* tmp = dynamic_cast<Infantry*>(x)){
             x->setAttackScore(ceil(0.9*x->getAttackScore()));
        }
       }

    }
    else {
         if(dis<=2.0){
             if(Infantry* tmp = dynamic_cast<Infantry*>(x)){
             x->setAttackScore(ceil(0.9 * x->getAttackScore()));
         }
    }
    }

}

}

Urban::Urban(const Position &pos) : TerrainElement(pos){}
Urban::~Urban(){}

void Urban::getEffect(Army* army){
vector<Unit*> units = army->getAllUnits();

bool isLiberation=( dynamic_cast<LiberationArmy*>(army) != nullptr);

for(Unit* x : units){
double dis = euclidD(this->pos, x->getCurrentPosition());
bool isInfantry= (dynamic_cast<Infantry*> (x) != nullptr);
if(isLiberation){
        if(isInfantry){
            if(dis<= 5.0 && ( x->getType() == SPECIALFORCES || x->getType() == REGULARINFANTRY )){
                int oldscore= x->getAttackScore();
                double bonus = (2*x->getAttackScore())/dis;
                x->setAttackScore( ceil ( oldscore + bonus));

            }
        }
        else{ // Vehicle
            if(dis<=2.0 && x->getType() == ARTILLERY){
                x->setAttackScore(ceil(0.5*x->getAttackScore()));
            }
        }

}
else{ // ddối với quân CQ Sai Gon
    if(isInfantry && (x->getType() == REGULARINFANTRY)){
        if(dis<=3.0){
            int oldscore = x->getAttackScore();
            double bonus = (3*oldscore)/(2*dis);
            x->setAttackScore(ceil(oldscore + bonus));
        }
    }


}




}

}

Fortification::Fortification(const Position &pos) : TerrainElement(pos){}

Fortification::~Fortification(){}

void Fortification::getEffect(Army* army){
vector<Unit*> units = army->getAllUnits();
bool isLiberation = (dynamic_cast<LiberationArmy*>(army) != nullptr);
for(Unit* x : units){
double dis = euclidD( this-> pos ,  x->getCurrentPosition());
if(isLiberation){
    if(dis<=2.0){
        x->setAttackScore( ceil(x->getAttackScore() * 0.8 ));
        }
    }
else{
    if(dis<=2.0){
         x->setAttackScore( ceil ( x->getAttackScore() *1.2));

}
}



}
}

SpecialZone::SpecialZone(const Position &pos) : TerrainElement(pos) {}


SpecialZone::~SpecialZone(){}

void SpecialZone::getEffect(Army* army){
vector<Unit*> units = army->getAllUnits() ;
for( auto x : units){
    double dis = euclidD(this->pos , x->getCurrentPosition());
    if(dis<=1){
        x->setAttackScore(0);
    }

}

}


//                  3.8
/*

class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement** terrain;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
};
*/

bool isinbando( vector<Position*> bando, const Position &pos){
for(Position* p : bando){
    if( (p->getRow() == pos.getRow())  && (p->getCol() == pos.getCol() ) )
        return true;
}
return false;
}

BattleField::BattleField(int n_rows, int n_cols, vector<Position*> arrayForest,
                         vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                        vector<Position *> arrayUrban, vector<Position *> arraySpecialZone){

this->n_rows=n_rows;
this->n_cols=n_cols;

this-> terrain = new TerrainElement**[n_rows];
for(int i = 0; i< n_rows ; i++){
   terrain[i] = new TerrainElement*[n_cols];
}
for(int r = 0 ; r< n_rows ; r++){
    for(int c = 0 ; c<n_cols ; c++){
     Position now(r,c);
                 if(isinbando(arrayForest, now))
                    {
                    terrain[r][c]= new Forest(now);}
                else if(isinbando(arrayRiver, now))
                    {
                    terrain[r][c]= new River(now);}
                    else if(isinbando(arrayFortification, now))
                    {
                    terrain[r][c]= new Fortification(now);}
                    else if(isinbando(arrayUrban, now))
                    {
                    terrain[r][c]=new Urban(now);}
                    else if(isinbando(arraySpecialZone, now))
                    {
                    terrain[r][c]=new SpecialZone(now);}
                    else{// Road
                        terrain[r][c] = new Road(now);
                    }


    }
}
}

BattleField::~BattleField(){
for(int i = 0; i< n_rows ; i++){
    for(int j = 0 ; j < n_cols; j++){
        delete  terrain[i][j];
    }
        delete[] terrain[i];

}
delete [] terrain;
}



string BattleField::str() const{
ostringstream os;
os <<"BattleField["
    <<"n_rows="<<n_rows<<","
    <<"n_cols="<<n_cols<<"]";

    return os.str();

}

void BattleField::terrainEffect(Army* army){
for(int r=0; r < n_rows ; r++){
    for(int c=0; c<n_cols; c++){
        terrain[r][c]->getEffect(army);
    }
}
}

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";  // Chuỗi toàn khoảng trắng

    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}
//                  3.9

vector<Position*> tachPosition(const string& value){

vector<Position*> result;
if(value.size() < 2 ) return result; // neu value qua' ngan chi co [] thi tra ve result trong'
string content = value.substr(1,value.size() -2);

size_t start=0;
while(true){
    size_t open = content.find('(', start);
    if(open == string::npos) break; // neu khong tim thay dau mo ngoac thi break;

    size_t close = content.find(')' ,open); // tim` dau ) tu vi tri open
    if(close == string::npos) break; //

    string tachpair = content.substr(open+1 , close-open-1);
    stringstream ss(tachpair);
    string rstr, cstr;
   if(getline(ss,rstr,',') && getline(ss,cstr) ){
    rstr = trim(rstr);
    cstr = trim(cstr);
    int r = stoi(rstr);
    int c = stoi(cstr);
    result.push_back(new Position(r,c));
}
    start = close + 1;
}
return result;




}
/*
TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
*/
VehicleType toVehicleType ( const string& name){
if(name== "TRUCK") return TRUCK;
if(name=="MORTAR") return MORTAR;
if(name== "ANTIAIRCRAFT") return ANTIAIRCRAFT;
if(name== "ARMOREDCAR") return ARMOREDCAR;
if(name=="APC") return APC;
if(name== "ARTILLERY") return ARTILLERY;
if(name == "TANK") return TANK;
throw std::invalid_argument("Unknown VehicleType" +name);

}
/*
 SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
*/
InfantryType toInfantryType( const string& name){

if(name== "SNIPER") return SNIPER;
if(name=="ANTIAIRCRAFTSQUAD") return ANTIAIRCRAFTSQUAD;
if(name=="MORTARSQUAD") return MORTARSQUAD;
if(name=="ENGINEER") return ENGINEER;
if(name=="SPECIALFORCES") return SPECIALFORCES;
if(name=="REGULARINFANTRY") return REGULARINFANTRY;

throw std::invalid_argument("Unknown InfantryType: " + name);
}

bool isVehicleType(const string& unitName){
    static const vector<string> allvehicletype ={
         "TRUCK", "MORTAR", "ANTIAIRCRAFT", "ARMOREDCAR", "APC", "ARTILLERY", "TANK"
    };
    for( auto vt: allvehicletype){
        if(vt==unitName) return true;
    }
    return false;
}
bool isInfantryType(const string& unitName){
static const vector<string> allinfantrytype={
"SNIPER", "ANTIAIRCRAFTSQUAD", "MORTARSQUAD", "ENGINEER", "SPECIALFORCES", "REGULARINFANTRY"
};
for( auto inf : allinfantrytype) {

    if(inf == unitName) return true;
}
return false;
}



// JHam Tach unit
vector<Unit*> tachUnit(const string& value) {
    vector<Unit*> result;

    if (value.size() < 2) return result;
    string content = value.substr(1, value.size() - 2); // remove '[' and ']'

    size_t start = 0;
    int loopCounter = 0; // Diagnostic counter

    while (start < content.size()) {
        loopCounter++;
        if (loopCounter > 100) {
            cout << "Too many unit parsing iterations!" << endl;
            break;
        }

        size_t openParen = content.find('(', start);
        if (openParen == string::npos) break;

        size_t index = openParen;
        int parenCount = 1;
        size_t closeParen = string::npos;

        while (index + 1 < content.size() && parenCount > 0) {
            index++;
            if (content[index] == '(') parenCount++;
            else if (content[index] == ')') parenCount--;
            if (parenCount == 0) closeParen = index;
        }

        if (closeParen == string::npos) break;

        string unitStr = content.substr(start, closeParen - start + 1);
        size_t nameEnd = unitStr.find('(');
        string unitName = unitStr.substr(0, nameEnd);
        string paramStr = unitStr.substr(nameEnd + 1, unitStr.size() - nameEnd - 2);

        int quantity = 0, weight = 0, armyBeLong = 0;
        Position pos;

        try {
            size_t p1 = paramStr.find(',');
            size_t p2 = paramStr.find(',', p1 + 1);
            size_t p3 = paramStr.rfind(',');

            string qStr = paramStr.substr(0, p1);
            string wStr = paramStr.substr(p1 + 1, p2 - p1 - 1);
            string posStr = paramStr.substr(p2 + 1, p3 - p2 - 1);
            string belongStr = paramStr.substr(p3 + 1);

            quantity = stoi(trim(qStr));
            weight = stoi(trim(wStr));
            armyBeLong = stoi(trim(belongStr));
            pos = Position(trim(posStr));
        }
        catch (const exception& e) {
            cout << "Error parsing unit: " << unitStr << " | " << e.what() << endl;
            start = closeParen + 1;
            if (start < content.size() && content[start] == ',') start++;
            continue;
        }

        if (isVehicleType(unitName)) {
            VehicleType vt = toVehicleType(unitName);
            result.push_back(new Vehicle(quantity, weight, pos, vt, armyBeLong));
        } else if (isInfantryType(unitName)) {
            InfantryType it = toInfantryType(unitName);
            result.push_back(new Infantry(quantity, weight, pos, it, armyBeLong));
        } else {
            cout << "Unknown unit type: " << unitName << endl;
        }

        start = closeParen + 1;
        if (start < content.size() && content[start] == ',') start++;
    }

    return result;
}




Configuration::Configuration(const string &filepath){



ifstream inFile(filepath);
if(!inFile.is_open()){// file is not opened.
    cout<<"Error openning file!!!";
    return;
}

vector<string> lines;
string singleline;
while(getline(inFile,singleline)){
    lines.push_back(singleline);
}
inFile.close();

// doc het du lieu vao string lines
// bay gio bat dau tach' du lieu cho vao bien
/*
lines[0] = "NUM_ROWS=10"
lines[1] = "NUM_COLS=8"
lines[2] = "ARRAY_FOREST=[(1,2),(3,5)]"
lines[3] = "ARRAY_RIVER=[(0,0),(0,4)]"
lines[4] = "ARRAY_FORTIFICATION=[(6,6)]"
lines[5] = "ARRAY_URBAN=[(2,0)]"
lines[6] = "ARRAY_SPECIAL_ZONE=[(9,7)]"
lines[7] = "UNIT_LIST=[TANK(5,2,(1,2),0),TANK(5,2,(3,2),1),REGULARINFANTRY(5,2,(1,1),0),REGULARINFANTRY(5,2,(3,3),1)]"
line[8] = "EVENT_CODE=23"


*/


// Ham chuyen ten sang VehicleType






// Lay num_rows

    for(const string& line : lines){// const la tranh' viec khong co y sua~ doi, & truyen tham chieu toi' phan tu lines goc' de do~ tao them string ton' bo nho
        size_t pos = line.find('=');
        if(pos == string::npos) continue; // string::npos la gia tri ham` find se tra ve neu khong tim duoc dau' '='

     string key = trim(line.substr(0,pos));
     string value = trim(line.substr(pos+1)) ;

     if(key == "NUM_ROWS"){
        int row = stoi(value);
        this->num_rows = row;
     }
     else if(key == "NUM_COLS"){
        int col = stoi(value);
        this->num_cols = col;
     }
     else if(key == "EVENT_CODE"){
        int ecode = stoi(value);
        if(ecode <=0) ecode = 0;
        if(ecode>99) ecode=ecode%100;
        this->eventCode = ecode;
     }
     else if(key == "ARRAY_FOREST"){
        arrayForest = tachPosition(value);
     }
     else if(key == "ARRAY_RIVER"){
        arrayRiver = tachPosition(value);
     }
     else if(key == "ARRAY_FORTIFICATION"){
        arrayFortification = tachPosition(value);
     }
     else if(key == "ARRAY_URBAN"){
        arrayUrban = tachPosition(value);
     }
     else if(key == "ARRAY_SPECIAL_ZONE"){
        arraySpecialZone = tachPosition(value);
     }
     else if(key == "UNIT_LIST"){
            vector<Unit*> allunits = tachUnit(value);
     liberationUnits.clear();
     ARVNUnits.clear();
     for(Unit* u : allunits){
        if(u->getArmyBeLong() == 0 ){
            liberationUnits.push_back(u);
        }
        else{
            ARVNUnits.push_back(u);
        }
     }

     }


    }



}

Configuration::~Configuration(){
    for(auto p :arrayForest) delete p;
    for(auto b : arrayRiver) delete b;
    for(auto a : arrayFortification) delete a;
    for(auto y : arrayUrban) delete y;
    for(auto x : arraySpecialZone) delete x;
    for(auto u : liberationUnits) delete u;
    for(auto u : ARVNUnits) delete u;

}

string Configuration::str() const{
    ostringstream os;
    os<<"["
      <<"num_rows="<<num_rows<<","
      <<"num_cols="<<num_cols<<",";


    os<<"arrayForest=[";
    for(int i = 0 ; i < arrayForest.size() ; i++){
        os << arrayForest[i]->str();
        if(i != arrayForest.size() -1 )
            os<<",";

    }
    os<<"]";
    os<<",";

    os <<"arrayRiver=[";
    for( int i = 0 ; i < arrayRiver.size() ; i++){
        os << arrayRiver[i]->str();
        if( i!= arrayRiver.size() -1 )
            os<<",";
    }

        os<<"]";
        os<<",";

    os <<"arrayFortification=[";
    for( int i = 0 ; i < arrayFortification.size() ; i++){
        os << arrayFortification[i]->str();
        if( i!= arrayFortification.size() -1 )
            os<<",";
    }

        os<<"]";
        os<<",";

    os <<"arrayUrban=[";
    for( int i = 0 ; i < arrayUrban.size() ; i++){
        os << arrayUrban[i]->str();
        if( i!= arrayUrban.size() -1 )
            os<<",";
    }

        os<<"]";
        os<<",";

     os <<"arraySpecialZone=[";
    for( int i = 0 ; i < arraySpecialZone.size() ; i++){
        os << arraySpecialZone[i]->str();
        if( i!= arraySpecialZone.size() -1 )
            os<<",";
    }

        os<<"]";
        os<<",";

    os <<"liberationUnits=[";
    for( int i = 0 ; i < liberationUnits.size() ; i++){
        os << liberationUnits[i]->str();
        if( i!= liberationUnits.size() -1 )
            os<<",";
    }

        os<<"]";
        os<<",";
        os <<"ARVNUnits=[";
    for( int i = 0 ; i < ARVNUnits.size() ; i++){
        os << ARVNUnits[i]->str();
        if( i!= ARVNUnits.size() -1 )
            os<<",";
    }

        os<<"]";
        os<<",";
        os<<"eventCode="<<eventCode<<"]";
return os.str();
}

/*
BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
*/

HCMCampaign::HCMCampaign(const string& config_file_path){
config = new Configuration(config_file_path);

battleField = new BattleField(
config->num_rows,
config->num_cols,
config->arrayForest,
config->arrayRiver,
config->arrayFortification,
config->arrayUrban,
config->arraySpecialZone
);

int libersize = config->liberationUnits.size();
Unit** liberUnitsArray= new Unit*[libersize];
for(int i = 0 ; i < libersize ; i++){
    liberUnitsArray[i] = config->liberationUnits[i];
}

int arvnsize = config->ARVNUnits.size();
Unit** ARVNUnitsArray = new Unit*[arvnsize];
for(int i = 0 ; i < arvnsize ; i++){

    ARVNUnitsArray[i] = config->ARVNUnits[i];

}

liberationArmy = new LiberationArmy(liberUnitsArray, libersize, "LIBERATIONARMY",battleField);
ARVNarmy = new ARVN(ARVNUnitsArray, arvnsize, "ARVN",battleField);

delete []liberUnitsArray;
delete []ARVNUnitsArray;




} // ket thuc constructor


void HCMCampaign::removeWeakUnits(Army* army) {
    vector<Unit*> units = army->getAllUnits();
    for (Unit* u : units) {
        if (u->getAttackScore() <= 5) {
            army->unitList->xoatohopUnit({u});
        }
    }
}

string HCMCampaign::printResult(){
ostringstream os;
os<<"LIBERATIONARMY[LF="<<liberationArmy->getLF() <<",EXP="<<liberationArmy->getEXP()
<<"]-"<<"ARVN[LF="<<ARVNarmy->getLF() <<","<<"EXP="<<ARVNarmy->getEXP() <<"]";
return os.str();

}

void HCMCampaign::run() {
if (!battleField || !liberationArmy || !ARVNarmy) return;

    battleField->terrainEffect(liberationArmy);
    battleField->terrainEffect(ARVNarmy);

if(config->eventCode < 75){ // quan giai phong tan cong
    liberationArmy->fight(ARVNarmy,false );
    ARVNarmy->fight (liberationArmy, true);

}else{

    liberationArmy->fight(ARVNarmy,true);
    ARVNarmy->fight(liberationArmy,false);


}
removeWeakUnits(liberationArmy);

removeWeakUnits(ARVNarmy);


}








////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
