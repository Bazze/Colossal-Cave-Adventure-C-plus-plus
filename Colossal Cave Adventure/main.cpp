//
//  main.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include <iostream>
#include "Data.h"

int main(int argc, const char * argv[])
{
    
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Data *data = new Data();
    
    data->loadData("advent.dat");
    
    delete data;
    
    return 0;
}

