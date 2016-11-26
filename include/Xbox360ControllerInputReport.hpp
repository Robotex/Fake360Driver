//
//  Xbox360ControllerInputReport.hpp
//  X360ControllerDriver
//
//  Created by Robotex on 24/11/16.
//  Copyright © 2016 Robotex. All rights reserved.
//

#ifndef Xbox360ControllerInputReport_hpp
#define Xbox360ControllerInputReport_hpp

#include <cstdint>

struct Xbox360ControllerInputReportData
{
    uint8_t type;
    uint8_t length;
    bool dpad_up : 1;
    bool dpad_down : 1;
    bool dpad_left : 1;
    bool dpad_right : 1;
    bool start : 1;
    bool back : 1;
    bool thumb_l : 1;
    bool thumb_r : 1;
    bool lb : 1;
    bool rb : 1;
    bool guide : 1;
    bool dummy1 : 1;
    bool a : 1;
    bool b : 1;
    bool x : 1;
    bool y : 1;
    uint8_t lt;
    uint8_t rt;
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    uint32_t dummy2;
    uint16_t dummy3;
};

class Xbox360ControllerInputReport : public Xbox360ControllerInputReportData
{
public:
    void Serialize();
    bool Deserialize(const uint8_t *data);
private:

};

#endif /* Xbox360ControllerInputReport_hpp */
