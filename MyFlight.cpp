#include <cassert>
#include<iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
 
using namespace std;

enum Seat { Economy, Premium, Business};
 
enum Airline { Delta, United, SouthWest, LuigiAir};

unordered_map<string, Airline> airlines {
  {"Delta", Delta}, 
  {"United", United}, 
  {"SouthWest", SouthWest},
  {"LuigiAir", LuigiAir}
};
 
unordered_map<string, Seat> seats{
  {"Economy", Economy}, 
  {"Premium", Premium}, 
  {"Business",Business}
};
 
struct Ticket {
    Airline airline;
    Seat seat;
    float distance; // in miles
};
class AirlineCalculator{
public:
// Factory pattern
    static AirlineCalculator* getInstance(Airline airline);
    // Calculate total cost
    virtual float calculate(const Ticket& ticket) const = 0;
    virtual ~AirlineCalculator() = default;
 
protected:
    // Calculate operating cost
    virtual float getOpCost (Ticket ticket) const {
        float opCost = 0.;
        switch(ticket.seat) {
            case(Economy): 
                opCost = getEconomyOpCost(ticket.distance);
                break;
            case(Premium): 
                opCost = getPremiumOpCost(ticket.distance);
                break;
            case(Business):
                opCost = getBusinessOpCost(ticket.distance);
        }
        return opCost;
    }
    virtual float getEconomyOpCost (float d) const {
        return 0.0;
    }
    virtual float getPremiumOpCost (float d) const {
        return 25.0;
    }
    virtual float getBusinessOpCost (float d) const {
        return 50.0 + 0.25 * d;
    }
};
// DeltaAirline
class DeltaCalculator:public AirlineCalculator{
public:
    float calculate(const Ticket& ticket) const override {
        float opCost = getOpCost(ticket);
        return opCost + ticket.distance * 0.5;
    }
    static DeltaCalculator* getInstance();
    virtual ~DeltaCalculator() = default;
   
private:
    DeltaCalculator() = default;

protected:
    static DeltaCalculator* calc;
};
DeltaCalculator* DeltaCalculator::calc = nullptr;

DeltaCalculator* DeltaCalculator::getInstance(){
    if(calc == nullptr){
        cout<<"create DT instance"<<endl;
        calc = new DeltaCalculator();
    }
    return calc;
}
//UnitedAirline
class UnitedCalculator:public AirlineCalculator{
public:
    float calculate(const Ticket& ticket) const override {
        float opCost = getOpCost(ticket);
        return opCost + ticket.distance * 0.75;
    }

    static UnitedCalculator* getInstance();
    virtual ~UnitedCalculator() = default;
 
private:
    UnitedCalculator() = default;

protected:

    float getPremiumOpCost(float d) const override {
        return 25. + 0.1 * d;
    }
    static UnitedCalculator* calc;
};
UnitedCalculator* UnitedCalculator::calc = nullptr;

UnitedCalculator* UnitedCalculator::getInstance(){
    if(calc == nullptr){
        cout<<"create UN instance"<<endl;
        calc = new UnitedCalculator();
    }
    return calc;
}
//SouthWest Airline
class SouthwestCalculator:public AirlineCalculator{
public:
     
    float calculate(const Ticket& ticket) const override{
        return 1. * ticket.distance;
    }
 
    static SouthwestCalculator* getInstance();
 
    virtual ~SouthwestCalculator() = default;
 
private:
    SouthwestCalculator() = default;
protected:
    static SouthwestCalculator* calc;
};
SouthwestCalculator* SouthwestCalculator::calc = nullptr;

SouthwestCalculator* SouthwestCalculator::getInstance(){
    if(calc == nullptr){
        cout<<"create SW instance"<<endl;
        calc = new SouthwestCalculator();
    }
    return calc;
}
//LuigiAirline
class LuigiAirCalculator:public AirlineCalculator{
public:
    float calculate(const Ticket& ticket) const override {
        float opCost = getOpCost(ticket);
        return max(2*opCost, 100.f);
    }
     
    // Meyers' Singleton
    static LuigiAirCalculator* getInstance();
 
    virtual ~LuigiAirCalculator() = default;
   
private:
    LuigiAirCalculator() = default;
protected:
    static LuigiAirCalculator* calc;
};
LuigiAirCalculator* LuigiAirCalculator::calc = nullptr;

LuigiAirCalculator* LuigiAirCalculator::getInstance(){
    if(calc == nullptr){
        cout<<"create LG instance"<<endl;
        calc = new LuigiAirCalculator();
    }
    return calc;
}
// Factory pattern
AirlineCalculator*  AirlineCalculator::getInstance(Airline airline){
    switch(airline) {
        case Delta:
            // Singleton pattern
            return DeltaCalculator::getInstance();
        case United:
            // Singleton pattern
            return UnitedCalculator::getInstance();
        case SouthWest:
            // Singleton pattern
            return SouthwestCalculator::getInstance();
        case LuigiAir:
            //Singleton pattern
            return LuigiAirCalculator::getInstance();
    }
}
static Ticket parse_ticket(const string& s) {
    // split by space
    vector<string> arr;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ' ') ) {
        arr.push_back(token);
    }
    assert(arr.size()== 3);
    assert(airlines.count(arr[0]));
    assert(seats.count(arr[2]));
 
    Ticket ticket;
    ticket.airline = airlines[arr[0]];
    ticket.seat = seats[arr[2]];
    ticket.distance = std::stof(arr[1]);
    return ticket;
} 
vector<float> process_tickets(vector<string> tickets){
    vector<float> costs;
    for(const string& ticket_str : tickets) {
        Ticket ticket = parse_ticket(ticket_str);
  
        AirlineCalculator* clc = AirlineCalculator::getInstance(ticket.airline);
        costs.push_back(clc->calculate(ticket));
    }
    return costs;
}  
int main() {
    vector<string> input{"United 150.0 Premium", "United 120.0 Economy","United 100.0 Business","Delta 60.0 Economy","Delta 60.0 Premium","Delta 60.0 Business", "SouthWest 1000.0 Economy", "SouthWest 4000.0 Economy", "LuigiAir 50.0 Business"};
    vector<float> costs = process_tickets(input);
    for(int i = 0 ; i < input.size(); i++){
        cout << input[i] << " cost: $" << costs[i]<< endl;
    }
    return 0;
}