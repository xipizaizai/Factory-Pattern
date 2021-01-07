#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;


enum Seat{
  Economy,
  Premium,
  Business
};

enum Airline{
  United,
  Delta,
  SouthWest,
  LuigiAir
};

unordered_map<string,Airline> airlines{
  {"Delta", Delta}, 
  {"United", United}, 
  {"SouthWest", SouthWest},
  {"LuigiAir", LuigiAir}
};

unordered_map<string,Seat> seats{
  {"Economy", Economy}, 
  {"Premium", Premium}, 
  {"Business",Business}
};

struct Ticket{
  float distance;
  Seat seat;
  Airline airline;
};

class AirlineCalculator{
  public:
    AirlineCalculator() = default;
    static AirlineCalculator* getInstance(Airline airline);
    virtual float calculate(const Ticket& ticket) const=0;
    virtual ~AirlineCalculator()=default;
  protected:
    virtual float getOpCost(Ticket ticket) const{
      Seat seat = ticket.seat;
      float res = 0.0;
      switch(seat){
        case Economy:
          res = getEconomyCost(ticket.distance);
          break;
        case Premium:
          res = getPremiumCost(ticket.distance);
          break;
        case Business:
          res = getBusinessCost(ticket.distance);
          break;
      }
    return res;
  }
  virtual float getEconomyCost(float dis) const{
    return 0.0;
  }
  virtual float getPremiumCost(float dis) const{
    return 25.0;
  }
  virtual float getBusinessCost(float dis) const{
    return 50.0+0.25*dis;
  }
};

class UnitedCalculator: public AirlineCalculator{
public:
  float calculate(const Ticket& ticket) const override{
      float opcost = getOpCost(ticket);
      return opcost+0.75*ticket.distance;
  }
  static UnitedCalculator* getInstance(){

      if(calc == nullptr){
        calc = new UnitedCalculator();
      }
      return calc;
  }
  virtual ~UnitedCalculator() = default;

private:
  UnitedCalculator() = default;
  static UnitedCalculator* calc;

protected:
  float getPremiumCost(float dis) const override{
      return 25.0+0.1*dis;
  }
};
UnitedCalculator* UnitedCalculator::calc = nullptr;


// derive class
class DeltaCalculator: public AirlineCalculator{
public:
  float calculate(const Ticket& ticket) const override {
    float opcost = getOpCost(ticket);
    return 0.5*ticket.distance + opcost;
  }
  static DeltaCalculator* getInstance(){
      if(calc == nullptr){
        calc = new DeltaCalculator();
      }
      return calc;
  }
  virtual ~DeltaCalculator() = default;

private:
  DeltaCalculator() = default;
  static DeltaCalculator* calc;

};
DeltaCalculator* DeltaCalculator::calc = nullptr;


// derive class
class SouthWestCalculator: public AirlineCalculator{
public:
  float calculate(const Ticket& ticket) const override{
    return 1. * ticket.distance;
  }
  static SouthWestCalculator* getInstance(){
    if(calc == nullptr){
      calc = new SouthWestCalculator();
    }
    return calc;
  }
  virtual ~SouthWestCalculator() = default;

private:
  SouthWestCalculator() = default; // constructor is private, because this is singleton pattern
  static SouthWestCalculator* calc;

};
SouthWestCalculator* SouthWestCalculator::calc = nullptr;


// derive class
class LuigiAirCalculator: public AirlineCalculator{
public:
  float calculate(const Ticket& ticket) const override{
    float opcost = getOpCost(ticket);
    return max(2*opcost, 100.f);
  }
  static LuigiAirCalculator* getInstance(){
    if(calc == nullptr){
      calc = new LuigiAirCalculator();
    }
    return calc;
  }
  virtual ~LuigiAirCalculator() = default;

private:
  LuigiAirCalculator() = default; // constructor is private, because this is singleton pattern
  static LuigiAirCalculator* calc;

};
LuigiAirCalculator* LuigiAirCalculator::calc = nullptr;

static Ticket parse_ticket(string& str){

    vector<string> arr;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ') ) {
        arr.push_back(token);
    }
 
    Ticket ticket;
    ticket.airline = airlines[arr[0]];
    ticket.distance = std::stof(arr[1]);
    ticket.seat = seats[arr[2]];
    return ticket;
}

AirlineCalculator* AirlineCalculator::getInstance(Airline airline){
  switch(airline){
      case United:
        return UnitedCalculator::getInstance();
      case Delta:
        return DeltaCalculator::getInstance();
      case SouthWest:
        return SouthWestCalculator::getInstance();
    case LuigiAir:
        return LuigiAirCalculator::getInstance();
  }
  return nullptr;
}

vector<float> process(vector<string>ticket){
  vector<float> cost;
  for(string ticket_str: ticket){
    Ticket tct = parse_ticket(ticket_str);
    AirlineCalculator* clc = AirlineCalculator::getInstance(tct.airline);
    cost.push_back(clc->calculate(tct));
  }
  return cost;
}

int main() {
  vector<string>input = {"United 150.0 Premium", "United 120.0 Economy","United 100.0 Business","United 60.0 Economy","United 60.0 Premium","Delta 60.0 Business", "SouthWest 1000.0 Economy", "SouthWest 4000.0 Economy", "LuigiAir 50.0 Business"};
  vector<float>cost = process(input);
  for(size_t i = 0 ; i < input.size(); i++){
        cout << input[i] << " cost: $" << cost[i]<< endl;
    }
    return 0;
}