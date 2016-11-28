//
//  Fake360Controller.cpp
//  Fake360Driver
//
//  Created by Robotex on 27/11/16.
//  Copyright © 2016 Robotex. All rights reserved.
//

#include "Fake360Controller.hpp"
#include <stdexcept>
#include <cstring>

#define FOOHID_CREATE 0  // create selector
#define FOOHID_DESTROY 1 // destroy selector
#define FOOHID_SEND 2  // send selector
#define FOOHID_LIST 3 // list selector

#define DEVICE_NAME "Robotex Virtual Xbox 360 Controller"
#define DEVICE_SN "SN 123456"

static uint8_t uhid_xb360gp_report_descr[] = {
    0x05, 0x01,		/* USAGE PAGE (Generic Desktop)		*/
    0x09, 0x05,		/* USAGE (Gamepad)			*/
    0xa1, 0x01,		/* COLLECTION (Application)		*/
    /* Unused */
    0x75, 0x08,		/*  REPORT SIZE (8)			*/
    0x95, 0x01,		/*  REPORT COUNT (1)			*/
    0x81, 0x01,		/*  INPUT (Constant)			*/
    /* Byte count */
    0x75, 0x08,		/*  REPORT SIZE (8)			*/
    0x95, 0x01,		/*  REPORT COUNT (1)			*/
    0x05, 0x01,		/*  USAGE PAGE (Generic Desktop)	*/
    0x09, 0x3b,		/*  USAGE (Byte Count)			*/
    0x81, 0x01,		/*  INPUT (Constant)			*/
    /* D-Pad */
    0x05, 0x01,		/*  USAGE PAGE (Generic Desktop)	*/
    0x09, 0x01,		/*  USAGE (Pointer)			*/
    0xa1, 0x00,		/*  COLLECTION (Physical)		*/
    0x75, 0x01,		/*   REPORT SIZE (1)			*/
    0x15, 0x00,		/*   LOGICAL MINIMUM (0)		*/
    0x25, 0x01,		/*   LOGICAL MAXIMUM (1)		*/
    0x35, 0x00,		/*   PHYSICAL MINIMUM (0)		*/
    0x45, 0x01,		/*   PHYSICAL MAXIMUM (1)		*/
    0x95, 0x04,		/*   REPORT COUNT (4)			*/
    0x05, 0x01,		/*   USAGE PAGE (Generic Desktop)	*/
    0x09, 0x90,		/*   USAGE (D-Pad Up)			*/
    0x09, 0x91,		/*   USAGE (D-Pad Down)			*/
    0x09, 0x93,		/*   USAGE (D-Pad Left)			*/
    0x09, 0x92,		/*   USAGE (D-Pad Right)		*/
    0x81, 0x02,		/*   INPUT (Data, Variable, Absolute)	*/
    0xc0,		/*  END COLLECTION			*/
    /* Buttons 5-11 */
    0x75, 0x01,		/*  REPORT SIZE (1)			*/
    0x15, 0x00,		/*  LOGICAL MINIMUM (0)			*/
    0x25, 0x01,		/*  LOGICAL MAXIMUM (1)			*/
    0x35, 0x00,		/*  PHYSICAL MINIMUM (0)		*/
    0x45, 0x01,		/*  PHYSICAL MAXIMUM (1)		*/
    0x95, 0x07,		/*  REPORT COUNT (7)			*/
    0x05, 0x09,		/*  USAGE PAGE (Button)			*/
    0x09, 0x08,		/*  USAGE (Button 8)			*/
    0x09, 0x07,		/*  USAGE (Button 7)			*/
    0x09, 0x09,		/*  USAGE (Button 9)			*/
    0x09, 0x0a,		/*  USAGE (Button 10)			*/
    0x09, 0x05,		/*  USAGE (Button 5)			*/
    0x09, 0x06,		/*  USAGE (Button 6)			*/
    0x09, 0x0b,		/*  USAGE (Button 11)			*/
    0x81, 0x02,		/*  INPUT (Data, Variable, Absolute)	*/
    /* Unused */
    0x75, 0x01,		/*  REPORT SIZE (1)			*/
    0x95, 0x01,		/*  REPORT COUNT (1)			*/
    0x81, 0x01,		/*  INPUT (Constant)			*/
    /* Buttons 1-4 */
    0x75, 0x01,		/*  REPORT SIZE (1)			*/
    0x15, 0x00,		/*  LOGICAL MINIMUM (0)			*/
    0x25, 0x01,		/*  LOGICAL MAXIMUM (1)			*/
    0x35, 0x00,		/*  PHYSICAL MINIMUM (0)		*/
    0x45, 0x01,		/*  PHYSICAL MAXIMUM (1)		*/
    0x95, 0x04,		/*  REPORT COUNT (4)			*/
    0x05, 0x09,		/*  USAGE PAGE (Button)			*/
    0x19, 0x01,		/*  USAGE MINIMUM (Button 1)		*/
    0x29, 0x04,		/*  USAGE MAXIMUM (Button 4)		*/
    0x81, 0x02,		/*  INPUT (Data, Variable, Absolute)	*/
    /* Triggers */
    0x75, 0x08,		/*  REPORT SIZE (8)			*/
    0x15, 0x00,		/*  LOGICAL MINIMUM (0)			*/
    0x26, 0xff, 0x00,	/*  LOGICAL MAXIMUM (255)		*/
    0x35, 0x00,		/*  PHYSICAL MINIMUM (0)		*/
    0x46, 0xff, 0x00,	/*  PHYSICAL MAXIMUM (255)		*/
    0x95, 0x02,		/*  REPORT SIZE (2)			*/
    0x05, 0x01,		/*  USAGE PAGE (Generic Desktop)	*/
    0x09, 0x32,		/*  USAGE (Z)				*/
    0x09, 0x35,		/*  USAGE (Rz)				*/
    0x81, 0x02,		/*  INPUT (Data, Variable, Absolute)	*/
    /* Sticks */
    0x75, 0x10,		/*  REPORT SIZE (16)			*/
    0x16, 0x00, 0x80,	/*  LOGICAL MINIMUM (-32768)		*/
    0x26, 0xff, 0x7f,	/*  LOGICAL MAXIMUM (32767)		*/
    0x36, 0x00, 0x80,	/*  PHYSICAL MINIMUM (-32768)		*/
    0x46, 0xff, 0x7f,	/*  PHYSICAL MAXIMUM (32767)		*/
    0x95, 0x04,		/*  REPORT COUNT (4)			*/
    0x05, 0x01,		/*  USAGE PAGE (Generic Desktop)	*/
    0x09, 0x30,		/*  USAGE (X)				*/
    0x09, 0x31,		/*  USAGE (Y)				*/
    0x09, 0x33,		/*  USAGE (Rx)				*/
    0x09, 0x34,		/*  USAGE (Ry)				*/
    0x81, 0x02,		/*  INPUT (Data, Variable, Absolute)	*/
    /* Unused */
    0x75, 0x30,		/*  REPORT SIZE (48)			*/
    0x95, 0x01,		/*  REPORT COUNT (1)			*/
    0x81, 0x01,		/*  INPUT (Constant)			*/
    0xc0,		/* END COLLECTION			*/
};

Fake360Controller::Fake360Controller()
{
    io_iterator_t iterator;
    io_service_t service;
    
    if (IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("it_unbit_foohid"), &iterator) != KERN_SUCCESS)
        throw std::runtime_error("Unable to access IOService.");
    
    // Iterate till success
    bool found = false;
    while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL)
    {
        if (IOServiceOpen(service, mach_task_self(), 0, &m_connect) == KERN_SUCCESS)
        {
            found = true;
            break;
        }
        
        IOObjectRelease(service);
    }
    IOObjectRelease(iterator);
    
    if (!found)
        throw std::runtime_error("Unable to open IOService.");
    
    // Fill up the input arguments.
    uint32_t input_count = 8;
    uint64_t input[input_count];
    input[0] = (uint64_t) strdup(DEVICE_NAME);  // device name
    input[1] = strlen((char *)input[0]);  // name length
    input[2] = (uint64_t) uhid_xb360gp_report_descr;  // report descriptor
    input[3] = sizeof(uhid_xb360gp_report_descr);  // report descriptor len
    input[4] = (uint64_t) strdup(DEVICE_SN);  // serial number
    input[5] = strlen((char *)input[4]);  // serial number len
    input[6] = (uint64_t) 2;  // vendor ID
    input[7] = (uint64_t) 3;  // device ID
    
    if (IOConnectCallScalarMethod(m_connect, FOOHID_CREATE, input, input_count, NULL, 0) != KERN_SUCCESS) {
        uint32_t input_count = 2;
        uint64_t input[input_count];
        input[0] = (uint64_t) strdup(DEVICE_NAME);  // device name
        input[1] = strlen((char *)input[0]);  // name length
        
        if (IOConnectCallScalarMethod(m_connect, FOOHID_DESTROY, input, input_count, NULL, 0) != KERN_SUCCESS)
            throw std::runtime_error("Unable to create HID device. Probably needs a reboot.");
        else
            throw std::runtime_error("Unable to create HID device. Virtual device cleared successfully. Try launching it again.");
    }
}

Fake360Controller::~Fake360Controller()
{
    uint32_t input_count = 2;
    uint64_t input[input_count];
    input[0] = (uint64_t) strdup(DEVICE_NAME);  // device name
    input[1] = strlen((char *)input[0]);  // name length
    
    if (IOConnectCallScalarMethod(m_connect, FOOHID_DESTROY, input, input_count, NULL, 0) != KERN_SUCCESS)
        throw std::runtime_error("Unable to destroy HID device.");
}

void Fake360Controller::Send(const Xbox360ControllerInputReportData &report)
{
    uint32_t send_count = 4;
    uint64_t send[send_count];
    send[0] = (uint64_t) strdup(DEVICE_NAME);  // device name
    send[1] = strlen((char *)send[0]);  // name length
    send[2] = (uint64_t) &report;  // report struct
    send[3] = sizeof(struct Xbox360ControllerInputReportData);  // report struct len

    if (IOConnectCallScalarMethod(m_connect, FOOHID_SEND, send, send_count, NULL, 0) != KERN_SUCCESS)
        throw std::runtime_error("Unable to send message to HID device.");
}
