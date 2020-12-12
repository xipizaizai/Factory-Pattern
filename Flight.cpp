#include <cassert>
#include<iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
 
using namespace std;

enum Seat { Economy, Premium, Business};
 
enum Airline { Delta, United, SouthWest};


unordered_map<string, Airline> airlines {
  {"Delta", Delta}, 
  {"United", United}, 
  {"SouthWest", SouthWest}
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
    static AirlineCalculator* create(Airline airline);
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


class DeltaCalculator:public AirlineCalculator{
public:
    float calculate(const Ticket& ticket) const override {
        float opCost = getOpCost(ticket);
        return opCost + ticket.distance * 0.5;
    }
     

    // Meyers' Singleton
    static AirlineCalculator* instance(){
        static DeltaCalculator calc;
        return &calc;
    }
 
    virtual ~DeltaCalculator() = default;
   
private:
    DeltaCalculator() = default;
};


class UnitedCalculator:public AirlineCalculator{
public:
    float calculate(const Ticket& ticket) const override {
        float opCost = getOpCost(ticket);
        return opCost + ticket.distance * 0.75;
    }
     
    static AirlineCalculator* instance(){
        static UnitedCalculator calc;
        return &calc;
    }
 
    virtual ~UnitedCalculator() = default;
 
private:
    UnitedCalculator() = default;
 
protected:
    float getPremiumOpCost(float d) const override {
        return 25. + 0.1 * d;
    }
};


class SouthwestCalculator:public AirlineCalculator{
public:
     
    float calculate(const Ticket& ticket) const override{
        return 1. * ticket.distance;
    }
 
    static AirlineCalculator* instance() {
        static SouthwestCalculator calc;
        return &calc;
    }
 
    virtual ~SouthwestCalculator() = default;
 

private:

    SouthwestCalculator() = default;
};


// Factory pattern

AirlineCalculator*  AirlineCalculator::create(Airline airline){
    switch(airline) {
        case Delta:
            // Singleton pattern
            return DeltaCalculator::instance();
        case United:
            // Singleton pattern
            return UnitedCalculator::instance();
        case SouthWest:
            // Singleton pattern
            return SouthwestCalculator::instance();
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


// unordered_map<string, AirlineCalculator*> airclcs{{"Delta",DeltaCalculator::instance()}, {"United",UnitedCalculator::instance()}, {"SouthWest",SouthwestCalculator::instance()}};
 
vector<float> process_tickets(vector<string> tickets){
    vector<float> costs;
    for(const string& ticket_str : tickets) {
        Ticket ticket = parse_ticket(ticket_str);
  
        AirlineCalculator* clc = AirlineCalculator::create(ticket.airline);

        costs.push_back(clc->calculate(ticket));
    }
    return costs;
}
  

int main() {
    vector<string> input{"United 150.0 Premium", "United 120.0 Economy","United 100.0 Business","Delta 60.0 Economy","Delta 60.0 Premium","Delta 60.0 Business", "SouthWest 1000.0 Economy", "SouthWest 4000.0 Economy"};
    vector<float> costs = process_tickets(input);
    for(int i = 0 ; i < input.size(); i++){
        cout << input[i] << " cost: $" << costs[i]<< endl;
    }
    return 0;
}


