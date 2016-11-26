//
//  Xbox360ControllerInputReport.cpp
//  X360ControllerDriver
//
//  Created by Robotex on 24/11/16.
//  Copyright © 2016 Robotex. All rights reserved.
//

#include "Xbox360ControllerInputReport.hpp"

#include <bitset>

void Xbox360ControllerInputReport::Serialize()
{
    // TODO
}

bool Xbox360ControllerInputReport::Deserialize(const uint8_t *data)
{
    type = data[0];
    length = data[1];
    
    if (type != 0x00 && length != 20)
        return false;
    
    std::bitset<16> bitfield(*reinterpret_cast<const uint16_t *>(&data[2]));
    
    dpad_up = bitfield[0];
    dpad_down = bitfield[1];
    dpad_left = bitfield[2];
    dpad_right = bitfield[3];
    
    start = bitfield[4];
    back = bitfield[5];
    thumb_l = bitfield[6];
    thumb_r = bitfield[7];
    
    lb = bitfield[8];
    rb = bitfield[9];
    guide = bitfield[10];
    dummy1 = bitfield[11];
    
    a = bitfield[12];
    b = bitfield[13];
    x = bitfield[14];
    y = bitfield[15];
    
    lt = data[4];
    rt = data[5];
    
    x1 = *reinterpret_cast<const uint16_t *>(&data[6]);
    y1 = *reinterpret_cast<const uint16_t *>(&data[8]);
    
    x2 = *reinterpret_cast<const uint16_t *>(&data[10]);
    y2 = *reinterpret_cast<const uint16_t *>(&data[12]);
    
    dummy2 = *reinterpret_cast<const uint32_t *>(&data[14]);
    dummy3 = *reinterpret_cast<const uint16_t *>(&data[18]);
    
    return true;
}
