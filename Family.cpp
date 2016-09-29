#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Dates.h"

using namespace std;

class Family {
      public:
         int IDNumber;
         string familyID;
         string husbando;
         string husbandoID;
         string waifu;
         string waifuID;
         string marryDate;
         string divDate;
         bool marryFlag;
         bool divFlag;
         
         num_date marryInt;
         num_date divInt;
         
         //Room for expansion
};
