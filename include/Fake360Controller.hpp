//
//  Fake360Controller.hpp
//  Fake360Driver
//
//  Created by Robotex on 27/11/16.
//  Copyright © 2016 Robotex. All rights reserved.
//

#ifndef Fake360Controller_hpp
#define Fake360Controller_hpp

#include <IOKit/IOKitLib.h>
#include "Xbox360ControllerInputReport.hpp"

class Fake360Controller
{
public:
    Fake360Controller();
    ~Fake360Controller();
    void Send(const Xbox360ControllerInputReportData &report);
private:
    io_connect_t m_connect;
};

#endif /* Fake360Controller_hpp */