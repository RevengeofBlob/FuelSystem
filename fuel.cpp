// UMBC - CMSC 341 - Spring 2023 - Proj1
#include "fuel.h"

//FuelSys
//Constructor (Default). Creates a new Fuel System object and sets the current tank to null
FuelSys::FuelSys(){
  m_current = nullptr;
}

//FuelSys
//Destructor. Loops through each tank and each pump in the tanks. Deallocates the pumps first then the tanks
FuelSys::~FuelSys(){
  Tank* traverseTank = m_current->m_next; //Tank used to loop through the tank list
  Tank* toDelTank = m_current; //Tank used to delete each tank pointer
  Pump* toDelPump = nullptr; //Pump used to delete each pump pointer
  Pump* traversePump = nullptr; //Pump used to loop through the pump list

  //Looping through each tank in the list
  while (toDelTank != nullptr && traverseTank != m_current){
    toDelPump = toDelTank->m_pumps;
    traversePump = toDelTank->m_pumps;

    //Looping through each pump in the tank
    while (traversePump != nullptr){
      traversePump = traversePump->m_next;
      delete toDelPump;
      toDelPump = traversePump;
    }
    
    delete toDelTank;
    toDelTank = traverseTank;
    traverseTank = traverseTank->m_next;
  }
}

//AddTank
//Takes in 3 ints: a tank ID, tank fuel capacity, and current tank fuel
//Creates a new tank with the arguments and sets the first pump and next tank to null
//Sets the next tank to another tank if there is at least one existing tank or itself if there is no other tanks
//Returns true if the tank was added, false otherwise
bool FuelSys::addTank(int tankID, int tankCap, int tankFuel = 0) {
  //Validates the arguments
  if (tankCap >= MINCAP && tankID > 0 && tankFuel >= 0 && tankFuel <= tankCap){
    if (!findTank(tankID)){
      Tank* newTank = new Tank(tankID, tankCap, tankFuel, nullptr, nullptr); //Tank to insert in the list

      //If there is no current tank, sets the first tank and next tank to the new tank
      if (m_current == nullptr){
	m_current = newTank;
	newTank->m_next = newTank;
      }
      //else set the set the next of first tank to new tank and the next of new tank to the original second tank
      else{
	newTank->m_next = m_current->m_next;
	m_current->m_next = newTank;
	m_current = newTank;

      }

      return true;
    }
  }

  return false;
}

//removeTank
//Takes in 1 int: a tank ID and searches for it in the list of tanks
//If found in the list, removes it and sets the previous tank to the next tank
//Returns true if the tank removed else false
bool FuelSys::removeTank(int tankID){
  Tank* temp = m_current; //tank to traverse the tank list

  if (m_current != nullptr){
    if (m_current->m_tankID == tankID){
      if (m_current->m_next != nullptr){
	m_current = m_current->m_next;
      }

      //Deletes the pumps in the tank if there are any
      if (temp->m_pumps != nullptr){
	Pump* pumpToDel = temp->m_pumps; //Pump used to delete
	Pump* traversePump = temp->m_pumps; //Pump to traverse the pump list

	while (pumpToDel != nullptr){
	  traversePump = traversePump->m_next;
	  delete pumpToDel;
	  pumpToDel = traversePump;
	}

	delete temp;
	temp = nullptr;
	return true;
      }
    }
    else{
      //Loops through the list until it finds the id or it ends at the beginning
      while (temp->m_next->m_tankID != tankID && temp->m_next->m_next != m_current){
	temp = temp->m_next;
      }

      if (temp->m_next->m_tankID){
	Tank* tankToDel = temp->m_next; //Tank used to delete
	temp->m_next = tankToDel->m_next;

	//Deletes the pumps in the tank if there are any
	if (tankToDel->m_pumps != nullptr){
	  Pump* pumpToDel = tankToDel->m_pumps; //Pump used to delete
	  Pump* traversePump = tankToDel->m_pumps; //Pump to traverse the pump list

	  while (pumpToDel != nullptr){
	    traversePump = traversePump->m_next;
	    delete pumpToDel;
	    pumpToDel = nullptr;
	    pumpToDel = traversePump;
	  }
	
	  delete tankToDel;
	  tankToDel = nullptr;
	  return true;
	}
      }
    }
  }

  return false;
}

//findTank
//Takes in 1 in: a tank ID and loops through the tank list to find the ID
//Returns true if the tank was found, false otherwise
bool FuelSys::findTank(int tankID){
  if (m_current != nullptr){
    Tank* traverse = m_current; //Tank to traverse the tank list

    //Loops through the list until it finds the id or it ends at the beginning
    while (traverse->m_next->m_tankID != tankID && traverse->m_next->m_next != m_current){
      traverse = traverse->m_next;
    }

    //Sets the current tank to the tank before the tank ID if it exists in the list
    if (traverse->m_next->m_tankID == tankID){
      m_current = traverse;
      
      return true;
    }
  }
  return false;
}

//addPump
//Takes in 3 ints: two tank IDs and a pump ID. First is the origin tank, the second is the target tank of the pump
//Returns true if the pump was added, false otherwise
bool FuelSys::addPump(int tankID, int pumpID, int targetTank){
  if (pumpID >= 0){
    if (m_current->m_tankID == tankID){
      Pump* traversePump = m_current->m_pumps; //Pump to traverse the pump list

      if (traversePump != nullptr){
	//Loops through the pumps until it matches the pump id or it hits the end of the list
	while (traversePump->m_pumpID != pumpID && traversePump->m_next != nullptr){
	  traversePump = traversePump->m_next;
	}
	if (traversePump->m_pumpID != pumpID){
	  Pump* newPump = new Pump(pumpID, targetTank, m_current->m_pumps); //pump to add to the pump list
	  m_current->m_pumps = newPump;
	  return true;
	}
      }
    }
    else{
      Tank* traverse = m_current; //Tank to traverse the tank list

      //Loops through the list until it matches the tank id or it ends at the beginning
      while (traverse->m_tankID != tankID && traverse->m_next->m_next != m_current){
	traverse = traverse->m_next;
      }

      if (traverse->m_tankID == tankID){
	Pump* traversePump = traverse->m_pumps; //Pump to traverse the pump list

	if (traversePump != nullptr){
	  //Loops through the list until the it matches the pump id or it reaches the end of the list
	  while (traversePump->m_pumpID != pumpID && traversePump->m_next != nullptr){
	    traversePump = traversePump->m_next;
	  }
	  if (traversePump->m_pumpID != pumpID){
	    Pump* newPump = new Pump(pumpID, targetTank, m_current->m_pumps); //Pump to add to the list
	    traverse->m_pumps = newPump;
	    return true;
	  }
	}
      }
    }
  }
  return false;
}

//removePump
//Takes in 2 ints: a tank ID and pump ID. Looks for the tank and then the pump in that tank
//Returns true if the pump was removed, false otherwise
bool FuelSys::removePump(int tankID, int pumpID){

  if (m_current->m_tankID == tankID){
    if (m_current->m_pumps->m_pumpID == pumpID){
      Pump* toDel = m_current->m_pumps; //Pump to delete
      m_current->m_pumps = m_current->m_pumps->m_next;
      delete toDel;
      toDel = nullptr;
      return true;
    }
    else{
      Pump* traversePump = m_current->m_pumps; //Pump to traverse the pump list

      //Loops through the list until it matches the pump id or it reaches the end of the list
      while (traversePump->m_next->m_pumpID == pumpID && traversePump->m_next->m_next != nullptr){
	traversePump = traversePump->m_next;
      }

      if (traversePump->m_next->m_pumpID == pumpID){
	Pump* toDel = traversePump->m_next; //Pump to delete
	traversePump->m_next = toDel->m_next;
	delete toDel;
	toDel = nullptr;
	return true;
      }
    }
  }
  else {
    Tank* traverse = m_current; //Tank to traverse the tank list

    //Loops through the list until it matches the tank id or the end is the beginning
    while (traverse->m_tankID != tankID && traverse->m_next != m_current){
      traverse = traverse->m_next;
    }
    if (traverse->m_tankID == tankID){
      if (traverse->m_pumps->m_pumpID == pumpID){
	Pump* toDel = traverse->m_pumps; //Pump to delete
	traverse->m_pumps = traverse->m_pumps->m_next;
	delete toDel;
	toDel = nullptr;
	return true;
      }
      else{
	Pump* traversePump = traverse->m_pumps; //Pump to traverse the pump list

	//Loops through the list until the pump id matches or it reaches the end of the list
	while (traversePump->m_next->m_pumpID == pumpID && traversePump->m_next->m_next != nullptr){
	  traversePump = traversePump->m_next;
	}

	if (traversePump->m_next->m_pumpID == pumpID){
	  Pump* toDel = traversePump->m_next; //Pump to delete
	  traversePump->m_next = toDel->m_next;
	  delete toDel;
	  toDel = nullptr;
	  return true;
	}
      }
    }
  }

  return false;
}

//totalFuel
//Adds up and returns the fuel from each of the tanks in the list
int FuelSys::totalFuel() const{
  int fuel = 0; //Starting fuel

  //If a tank exists
  if (m_current != nullptr){
    Tank* fuelTank = m_current; //Current tank to add fuel
    fuel = fuelTank->m_tankFuel;
    fuelTank = fuelTank->m_next;

    //Loops thrugh the list until it reaches the beginning
    while (fuelTank != m_current){
      fuel += fuelTank->m_tankFuel;
      fuelTank = fuelTank->m_next;
    }
  }
  
  return fuel;
}

//drain
//Takes in 3 ints: a tank ID, pump ID, and a fuel amount
bool FuelSys::drain(int tankID, int pumpID, int fuel){
  Tank* drainTank = m_current; //Tank to take fuel from
  Pump* drainPump = m_current->m_pumps; //Pump to transfer the fuel
  int fuelNeeded = fuel; //Amount of fuel needed
  
  if (m_current->m_tankID == tankID){
    if (drainPump->m_pumpID == pumpID){
      fuelNeeded = targetFuelNeeded(drainPump->m_target);
      //If there is room in the target tank
      if (fuelNeeded != -1){
	//If the fuel in the current tank is less than or equal to than amount needed
	if (m_current->m_tankFuel <= fuelNeeded){
	  drainTank->m_tankFuel = 0;
	  return fill(drainPump->m_target, m_current->m_tankFuel);
	}
	else {
	  drainTank->m_tankFuel -= fuelNeeded;
	  return fill(drainPump->m_target, fuelNeeded);
	}
      }
    }
    else{
      //Loops through the list until the pump id matches or it reaches the end of the list
      while (drainPump->m_next != nullptr && drainPump->m_pumpID != pumpID){
	drainPump = drainPump->m_next;
      }

      if (drainPump->m_pumpID == pumpID){
	fuelNeeded = targetFuelNeeded(drainPump->m_target);
	//If the target tank has space
	if (fuelNeeded != -1){
	  //If the current tank is less than or equal to the amount needed
	  if (m_current->m_tankFuel <= fuelNeeded){
	    drainTank->m_tankFuel = 0;
	    return fill(drainPump->m_target, m_current->m_tankFuel);
	  }
	  else {
	    drainTank->m_tankFuel -= fuelNeeded;
	    return fill(drainPump->m_target, fuelNeeded);
	  }
	}
      }
    }
  }
  else {
    //Loops through the list until the tank id matches or it reaches the beginning
    while (drainTank->m_next != m_current && drainTank->m_tankID != tankID){
      drainTank = drainTank->m_next;
    }
    if (drainTank->m_tankID == tankID){
      if (drainPump != nullptr){
	if (drainPump->m_pumpID == pumpID){
	  fuelNeeded = targetFuelNeeded(drainPump->m_target);
	  //If the target tank has space
	  if (fuelNeeded != -1){
	    //If the current tank is less than or equal to the fuel needed
	    if (m_current->m_tankFuel <= fuelNeeded){
	      drainTank->m_tankFuel = 0;
	      return fill(drainPump->m_target, m_current->m_tankFuel);
	    }
	    else {
	      drainTank->m_tankFuel -= fuelNeeded;
	      return fill(drainPump->m_target, fuelNeeded);
	    }
	  }
	}
	else{
	  //Loops through the list until the pump id matches or it reaches the end
	  while (drainPump->m_next != nullptr && drainPump->m_pumpID != pumpID){
	    drainPump = drainPump->m_next;
	  }
	  if (drainPump->m_pumpID == pumpID){
	    fuelNeeded = targetFuelNeeded(drainPump->m_target);
	    if (fuelNeeded != -1){
	      if (m_current->m_tankFuel <= fuelNeeded){
		drainTank->m_tankFuel = 0;
		return fill(drainPump->m_target, m_current->m_tankFuel);
	      }
	      else {
		drainTank->m_tankFuel -= fuelNeeded;
		return fill(drainPump->m_target, fuelNeeded);
	      }
	    }
	  }
	}
      }
    }
  }

  return false;
}

//targetFuelNeeded
//Takes in the target tank ID from the drain pump and returns the difference between the tank capacity and current fuel
//Returns -1 if the target tank is not found
int FuelSys::targetFuelNeeded(int targetID){
  if (m_current->m_tankID == targetID){
    return m_current->m_tankCapacity - m_current->m_tankFuel;
  }
  else{
    Tank* target = m_current; //Tank to traverse list
    //Loops through list until the tank id matches or it reaches the beginning
    while (target->m_next != m_current && target->m_tankID != targetID){
      target = target->m_next;
    }
    if (target->m_tankID == targetID){
      return m_current->m_tankCapacity - m_current->m_tankFuel;
    }
  }

  return -1;
}

//fill
//Takes in the target tank ID and the amount of fuel, returns true if the tank is filled, false if the tank isn't in the list
bool FuelSys::fill(int tankID, int fuel){
  if (m_current->m_tankID == tankID){
    m_current->m_tankFuel += fuel;
    return true;
  }
  else{
    Tank* fillTank = m_current; //Tank to traverse the list

    //Loops through the list until it the tank id matches or it is at the beginning
    while (fillTank->m_next != m_current && fillTank->m_tankID != tankID){
      fillTank = fillTank->m_next;
    }

    if (fillTank->m_tankID){
      fillTank->m_tankFuel += fuel;
      return true;
    }
  }

  return false;
}

//operator=
//Takes in a FuelSys object and copies each tank and pump into this FuelSys object
const FuelSys & FuelSys::operator=(const FuelSys & rhs){
  //If the current system's address does not equal the passed system
  if (this != &rhs){
    Tank* traverseTank = m_current; //Tank to traverse this tank list
    Tank* toDelTank = m_current; //Tank to delete from this tank list
    Pump* toDelPump = nullptr; //Pump to delete from this pump list
    Pump* traversePump = nullptr; //Pump to traverse this pump list

    //Loops through and deletes and tank that is not null
    while (toDelTank != nullptr){
      toDelPump = toDelTank->m_pumps;
      traversePump = toDelTank->m_pumps;

      //Loops through and deletes each pump in the tank that is not null
      while (toDelPump != nullptr){
	traversePump = traversePump->m_next;
	delete toDelPump;
	toDelPump = traversePump;
      }

      delete toDelTank;
      toDelTank = traverseTank;
      traverseTank = traverseTank;
    }
  }

  m_current = rhs.m_current;
  m_current->m_pumps = rhs.m_current->m_pumps;

  Tank* thisTank = m_current; //Tank to add and traverse this tank list
  Tank* rhsTank = rhs.m_current; //Tank to traverse the passed tank list
  Pump* tankPump = m_current->m_pumps; //Pump to add and traverse to the this pump list
  Pump* rhsPump = rhs.m_current->m_pumps; //Pump to traverse the passed pump list

  //Loops through the passed tanks until it reaches the beginning
  while (rhsTank->m_next != rhs.m_current){
    //Loops through the passed pumps until it reaches the end
    while (rhsPump != nullptr){
      tankPump->m_next = rhsPump->m_next;
      tankPump = tankPump->m_next;
      rhsPump = rhsPump->m_next;
    }

    //Sets the next this tank and pump to the next of the passed tanks
    //Moves each tank and pump to their next
    thisTank->m_next = rhs.m_current->m_next;
    thisTank = thisTank->m_next;
    rhsTank = rhsTank->m_next;
    tankPump = thisTank->m_pumps;
    rhsPump = rhsTank->m_pumps;
  }

  return *this;
}

void FuelSys::dumpSys() const{
  if (m_current != nullptr){
    Tank* tempTank = m_current->m_next;//we start at front item
    //we continue visiting nodes until we reach the cursor
    while(m_current != nullptr && tempTank != m_current){
      cout << "Tank " << tempTank->m_tankID << "(" << tempTank->m_tankFuel << " kg)" << endl;
      // now dump the targets for all pumps in this tank
      // we need to traverse the list of pumps
      dumpPumps(tempTank->m_pumps);
      tempTank = tempTank->m_next;
    }
    //at the end we visit the cursor (current)
    //this also covers the case that there is only one item
    cout << "Tank " << m_current->m_tankID << "(" << m_current->m_tankFuel << " kg)" << endl;
    dumpPumps(tempTank->m_pumps);
    cout << "The current tank is " << m_current->m_tankID << endl;
  }
  else
    cout << "There is no tank in the system!\n\n";
}

void FuelSys::dumpPumps(Pump* pumps) const{
  // this traverses the linked list to the end
  Pump* tempPump = pumps;
  while (tempPump != nullptr){
    cout << " => pump " << tempPump->m_pumpID << "(To tank " << tempPump->m_target << ")" << endl;
    tempPump = tempPump->m_next;
  }
}
