#include "fuel.h"
#include <random>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
  Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
  {
    if (type == NORMAL){
      //the case of NORMAL to generate integer numbers with normal distribution
      m_generator = std::mt19937(m_device());
      //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
      //the mean and standard deviation can change by passing new values to constructor
      m_normdist = std::normal_distribution<>(mean,stdev);
    }
    else if (type == UNIFORMINT) {
      //the case of UNIFORMINT to generate integer numbers
      // Using a fixed seed value generates always the same sequence
      // of pseudorandom numbers, e.g. reproducing scientific experiments
      // here it helps us with testing since the same sequence repeats
      m_generator = std::mt19937(10);// 10 is the fixed seed value
      m_unidist = std::uniform_int_distribution<>(min,max);
    }
    else{
      //the case of UNIFORMREAL to generate real number
      m_generator = std::mt19937(10);// 10 is the fixed seed value
      m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
    }
  }
  void setSeed(int seedNum){
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
  }

  int getRandNum(){
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if(m_type == NORMAL){
      //returns a random number in a set with normal distribution
      //we limit random numbers by the min and max values
      result = m_min - 1;
      while(result < m_min || result > m_max)
	result = m_normdist(m_generator);
    }
    else if (m_type == UNIFORMINT){
      //this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum(){
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = std::floor(result*100.0)/100.0;
    return result;
  }

private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;//normal distribution
  std::uniform_int_distribution<> m_unidist;//integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};
class Tester{
    public:

  //Testing if valid tanks are added to sys after calling addTank()
  bool addTankNormal(FuelSys & sys, Random & randomFuel, int numOfTanks){
    bool result = true;

    //We expect that adding numOfTanks tanks will return true
    for (int i = 1; i <= numOfTanks; i++){
      result = result && sys.addTank(i, DEFCAP, randomFuel.getRandNum());
    }

    cout << "After creating tanks: \n\n";
    sys.dumpSys();
    cout << "\n";
    
    return result;
  }

  //Testing if invalid tanks are not added to sys after calling addTank()
  bool addTankError(FuelSys & sys, Random & randomFuel, int numOfTanks){
    bool result = true;

    //We expect that adding tanks with the same id will return false
    for (int i = 1; i <= numOfTanks; i++){
      result = result && (sys.addTank(i, DEFCAP, randomFuel.getRandNum()) == false);
    }

    //We expect that adding tanks with an id with 0 or less will return false
    for (int j = -1; j > -numOfTanks; j--){
      result = result && (sys.addTank(j, DEFCAP, randomFuel.getRandNum()) == false);
    }

    //We expect that adding tanks with a cap less than MINCAP will return false
    for (int k = (numOfTanks + 1); k <= (numOfTanks * 2); k++){
      result = result && (sys.addTank(k, MINCAP - 1, randomFuel.getRandNum()) == false);
    }

    //We expect that adding tanks with current fuel greater than the cap will return false
    for (int m = (numOfTanks + 1); m <= (numOfTanks * 2); m++){
      result = result && (sys.addTank(m, DEFCAP, DEFCAP + 1) == false);
    }

    int fuelError = numOfTanks; //uses to test negative fuel
    
    //We expect that adding tanks with current fuel less than or equal to 0 will return false
    for (int n = (numOfTanks + 1); n < (numOfTanks * 2); n++){
      result = result && (sys.addTank(n, DEFCAP, fuelError - n) == false);
    }
    
    cout << "After creating no tanks: \n\n";
    sys.dumpSys();
    cout << "\n";
    
    return result;
  }

  //Testing if all of the tanks can be removed
  bool removeTankNormal(FuelSys & sys, int numOfTanks){
    bool result = true;

    //We expect each tank to be removed and return true
    for (int i = 1; i <= numOfTanks; i++){
      result = result && sys.removeTank(i);
    }

    cout << "After removing all the tanks: \n\n";
    sys.dumpSys();
    cout << "\n";
    
    return result;
  }

  //Testing if removeTank() will return an error (false) since there are no tanks remaining
  bool removeTankError(FuelSys & sys, int numOfTanks){
    bool result = true;

    //We expect no tanks to be removed because there are none
    for (int i = 1; i <= numOfTanks; i++){
      result = result && (sys.removeTank(i) == false);
    }

    return result;
  }

  //Testing if findTank() will find the correct tank and set the next of m_current to it
  bool findTankNormal(FuelSys & sys, int numOfTanks){
    bool result = true;

    //We expect that every second tank will be found in findTank() and become the next of m_current
    for (int i = 1; i <= numOfTanks; i++){
      if (i % 2 == 0){
	result = result && sys.findTank(i);
	result = result && (sys.m_current->m_next->m_tankID == i); 
      }
    }
    
    return result;
  }

  //Testing if findTank() will return false if a tank is not in the list
  bool findTankError(FuelSys & sys, int numOfTanks){
    bool result = true;

    //We expect that none of the ids will be found and m_current will not change
    for (int i = numOfTanks + 1; i <= (numOfTanks * 2); i++){
      result = result && (sys.findTank(i) == false);
      result = result && (sys.m_current->m_next->m_tankID != i);
    }
    
    return result;
  }

  //Testing if sys will return the amount of fuel in all the tanks
  bool totalFuelNormal(FuelSys & sys){
    bool result = true;

    //We expect the total fuel after adding tanks to be greater than 0
    result = result && (sys.totalFuel() > 0);
    
    return result;
  }

  //Testing if sys will return that it has 0 fuel because there are no tanks
  bool totalFuelError(FuelSys & sys){
    bool result = true;

    //We expect the total fuel before adding tanks to be 0
    result = result && (sys.totalFuel() == 0);
    
    return result;
  }

  //Testing if each pump will be added to a tank
  bool addPumpNormal(FuelSys & sys, Random & randomTank, int numOfPumps){
    bool result = true;
    
    //We expect that adding numOfPumps pumps will return true
    for (int i = 1; i <= numOfPumps; i++){
      for (int j = 1; j <= numOfPumps; j++){
	result = result && sys.addPump(i, j, randomTank.getRandNum());
      }
    }

    cout << "After creating Pumps: \n\n";
    sys.dumpSys();
    cout << "\n";
    
    return result;
  }

  //Testing if invalid pumps will not be added to a tank
  bool addPumpError(FuelSys & sys, Random & randomTank, int numOfPumps){
    bool result = true;

    //We expect that adding Pumps with the same id will return false
    for (int i = 1; i <= numOfPumps; i++){
      for (int j = 1; j <= numOfPumps; j++){
	result = result && (sys.addPump(i, j, randomTank.getRandNum()) == false);
      }
    }

    //We expect that adding Pumps with an id with 0 or less will return false
    for (int k = 1; k <= numOfPumps; k++){
      for (int n = -1; n > -numOfPumps; n--){
	result = result && (sys.addPump(k, n, randomTank.getRandNum()) == false);
      }
    }

    cout << "After creating no Pumps: \n\n";
    sys.dumpSys();
    cout << "\n";
    
    return result;
  }

  //Testing if sys will remove existing pumps
  bool removePumpNormal(FuelSys & sys, int numOfPumps){
    bool result = true;

    //We expect every second pump to be removed
    for (int i = 1; i <= numOfPumps; i++){
      for (int j = 1; j <= numOfPumps; j++){
	if (j % 2 == 0){
	  result = result && sys.removePump(i, j);
	}
      }
    }

    return result;
  }

  //Testing if sys will not remove nonexistent pumps
  bool removePumpError(FuelSys & sys, int numOfPumps){
    bool result = true;

    //We expect nonexisting pumps to be removed from an existing tank
    for (int i = 1; i <= numOfPumps; i++){
      for (int j = (numOfPumps + 1); j <= (numOfPumps * 2); j++){
	result = result && (sys.removePump(i, j) == false);
      }
    }
    //We expect nonexisting pumps to not be removed from nonexisting tanks
    for (int k = (numOfPumps + 1); k <= (numOfPumps * 2); k++){
      for (int m = 1; m <= numOfPumps; m++){
	result = result && (sys.removePump(k, m) == false);
      }
    }
    return result;
  }

  //Testing if a tank can correctly drain it's fuel into another
  bool drainPumpNormal(FuelSys & sys, Random & randomTank, Random & randomFuel){
    bool result = true;

    //We expect fuel to move from the tank to the target tank in the pump
    result = result && sys.drain(randomTank.getRandNum(), randomTank.getRandNum(), randomFuel.getRandNum());
    
    return result;

    cout << "After draining pump 1: \n\n";
    sys.dumpSys();
    cout << "\n";
  }

  //Testing if a tank will not drain with invalid arguments
  bool drainPumpError(FuelSys & sys, Random & randomTank, Random & randomFuel){
    bool result = true;

    //We expect an invalid pump to return false
    result = result && (sys.drain(randomTank.getRandNum(), -1, randomFuel.getRandNum()) == false);
    //We expect an invalid tank to return false
    result = result && (sys.drain(-1, randomTank.getRandNum(), randomFuel.getRandNum()) == false);

    return result;
  }

  //Testing if the tanks and pumps from sys are copied to a new fuel system using the overloaded operator
  bool overloadedNormal(FuelSys & sys){
    bool result = true;
    FuelSys newOne; //Creates an empty, secondary fuel system
    newOne = sys; //Sets the empty system to sys.

    //We should expect newOne to be populated and have the same amount of fuel as sys
    result = result && newOne.findTank(1) && (newOne.totalFuel() == sys.totalFuel());

    cout << "After copying the data: \n\n";
    newOne.dumpSys();
    cout << "\n";
    
    return result;
  }
};
int main(){
  Tester test; //Tester object to test the fuel systems
  FuelSys sys; //Fuel system to test normal and error cases
  int numTanksAndPumps = 10; //Number of tanks and pumps 
  Random randFuel(MINCAP,DEFCAP); //Random amount of fuel in a tank
  Random randTank(1,numTanksAndPumps); //Random tank in the list

  //Before we add any tanks, the system shouldn't have any fuel
  if (test.totalFuelError(sys)){
    cout << "\ttotalFuelError() returned true\n";
  }
  else {
    cout << "\ttotalFuelError() returned false\n";
  }

  //Adding valid tanks should create a list of tanks and return true
  if (test.addTankNormal(sys, randFuel, numTanksAndPumps)){
    cout << "\taddTankNormal() returned true\n";
  }
  else {
    cout << "\taddTankNormal() returned false\n";
  }

  //After we add the tanks, the system should have more than 0 fuel
  if (test.totalFuelNormal(sys)){
    cout << "\ttotalFuelNormal() returned true\n";
  }
  else {
    cout << "\ntotalFuelNormal() returned false\n";
  }
  
  //Adding invalid tanks should not add any tanks and return true
  if (test.addTankError(sys, randFuel, numTanksAndPumps)){
    cout << "\taddTankError() returned true\n";
  }
  else {
    cout << "\taddTankError() returned false\n";
  }

  //Adding valid pumps should add a list of pumps under each tank and returned true
  if (test.addPumpNormal(sys, randTank, numTanksAndPumps)){
    cout << "\taddPumpNormal() returned true\n";
  }
  else {
    cout << "\taddPumpNormal() returned false\n";
  }

  //Adding invalid pumps should not add any pumps and return true
  if (test.addPumpError(sys, randTank, numTanksAndPumps)){
    cout << "\taddPumpError() returned true\n";
  }
  else {
    cout << "\taddPumpError() returned false\n";
  }

  //Draining a valid tank with a valid pump should transfer fuel to the target tank
  if (test.drainPumpNormal(sys, randTank, randFuel)){
    cout << "\tdrainPumpNormal() returned true\n";
  }
  else {
    cout << "\tdrainPumpNormal() returned false\n";
  }

  //Draining an invalid tank or pump should do nothing
  if (test.drainPumpError(sys, randTank, randFuel)){
    cout << "\tdrainPumpError() returned true\n";
  }
  else {
    cout << "\tdrainPumpError() returned false\n";
  }
  
  //Finding a tank should return true and set it to the next of m_current
  if (test.findTankNormal(sys, numTanksAndPumps)){
    cout << "\tfindTankNormal() returned true\n";
  }
  else {
    cout << "\tfindTankNormal() returned false\n";
  }

  //Should not find an invalid tank or set it to the next of m_current; returns true
  if (test.findTankError(sys, numTanksAndPumps)){
    cout << "\tfindTankError() returned true\n";
  }
  else {
    cout << "\tfindTankError() returned false\n";
  }
  
  //Using an overloaded = should create a deep copy of sys
  if (test.overloadedNormal(sys)){
    cout << "\toverloadedNormal() returned true\n";
  }
  else {
    cout << "\toverloadedNormal() returned false\n";
  }

  //Sys should have no tanks left after removing all of the tanks
  if (test.removeTankNormal(sys, numTanksAndPumps)){
    cout << "\tremoveTankNormal() returned true\n";
  }
  else {
    cout << "\tremoveTankNormal() returned false\n";
  }

  //Trying to remove another tank should not work
  if (test.removeTankError(sys, numTanksAndPumps)){
    cout << "\tremoveTankError() returned true\n";
  }
  else {
    cout << "\tremovetankError() returned false\n";
  }

  //Sys should remove every other tank
  if (test.removePumpNormal(sys, numTanksAndPumps)){
    cout << "\tremovePumpNormal() returned true\n";
  }
  else {
    cout << "\tremovePumpNormal() returned false\n";
  }

  //Sys should not remove any tanks
  if (test.removePumpError(sys, numTanksAndPumps)){
    cout << "\tremovePumpError() returned true\n";
  }
  else {
    cout << "\tremovePumpError() returned false\n";
  }
  
  return 0;
}
