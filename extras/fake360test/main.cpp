//
//  main.cpp
//  Fake360Test
//
//  Created by Robotex on 24/11/16.
//  Copyright © 2016 Robotex. All rights reserved.
//

#include <iostream>
#include <csignal>
#include <libusb-1.0/libusb.h>
#include "Xbox360ControllerInputReport.hpp"

bool g_isAborted = false;

// SIGINT handler
void int_handler(int x)
{
    if (x == SIGINT)
        g_isAborted = true;
}

void on_read_data(libusb_transfer *transfer)
{
    if (transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        Xbox360ControllerInputReport msg;
        msg.Deserialize(transfer->buffer);
        std::cout << "Read " << transfer->actual_length << " bytes\n";
        if (transfer->actual_length == 20)
        {
            printf("   Up:%d, Down:%d, Left:%d, Right:%d\n", msg.dpad_up, msg.dpad_down, msg.dpad_left, msg.dpad_right);
            printf("   Start:%d, Guide:%d, Back:%d, Left Stick Press:%d, Right Stick Press:%d\n", msg.start, msg.guide, msg.back, msg.thumb_l, msg.thumb_r);
            printf("   A:%d, B:%d, X:%d, Y:%d, LB:%d, RB:%d\n", msg.a, msg.b, msg.x, msg.y, msg.lb, msg.rb);
            printf("   Left Trigger: %d, Right Trigger: %d\n", msg.lt, msg.rt);
            printf("   Left Analog (X,Y): (%d,%d)\n", msg.x1, msg.y1);
            printf("   Right Analog (X,Y): (%d,%d)\n", msg.x2, msg.y2);
        }
    }
    else
        std::cout << "Error reading device\n";
}

int usb_find_ep(libusb_device *m_dev, int direction, uint8_t if_class, uint8_t if_subclass, uint8_t if_protocol)
{
    libusb_config_descriptor *config;
    int ret = libusb_get_config_descriptor(m_dev, 0 /* config_index */, &config);
    
    if (ret != LIBUSB_SUCCESS)
    {
        std::cout << "libusb_get_config_descriptor() failed: " << ret << "\n";
        exit(-1);
    }
    else
    {
        int ret_endpoint = -1;
        
        // FIXME: no need to search all interfaces, could just check the one we acutally use
        for(const libusb_interface* interface = config->interface;
            interface != config->interface + config->bNumInterfaces;
            ++interface)
        {
            for(const libusb_interface_descriptor* altsetting = interface->altsetting;
                altsetting != interface->altsetting + interface->num_altsetting;
                ++altsetting)
            {
                std::cout  << "Interface: " << static_cast<int>(altsetting->bInterfaceNumber);
                
                for(const libusb_endpoint_descriptor* endpoint = altsetting->endpoint;
                    endpoint != altsetting->endpoint + altsetting->bNumEndpoints;
                    ++endpoint)
                {
                    std::cout << "    Endpoint: " << int(endpoint->bEndpointAddress & LIBUSB_ENDPOINT_ADDRESS_MASK) <<
                              "(" << ((endpoint->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) ? "IN" : "OUT") << ")\n";
                    
                    if ((endpoint->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == direction &&
                        altsetting->bInterfaceClass    == if_class    &&
                        altsetting->bInterfaceSubClass == if_subclass &&
                        altsetting->bInterfaceProtocol == if_protocol)
                    {
                        ret_endpoint = static_cast<int>(endpoint->bEndpointAddress & LIBUSB_ENDPOINT_ADDRESS_MASK);
                    }
                }
            }
        }
        libusb_free_config_descriptor(config);
        
        if (ret_endpoint < 0)
        {
            std::cout << "couldn't find matching endpoint\n";
            exit(-1);
        }
        else
        {
            return ret_endpoint;
        }
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    signal(SIGINT,int_handler);
    
    libusb_init(NULL);
    libusb_device_handle *hdev = libusb_open_device_with_vid_pid(NULL, 0x045E, 0x028E);
    if (hdev == NULL) {
        std::cout << "Device not found.\n";
        return -1;
    }
    if (libusb_kernel_driver_active(hdev, 0) != 0)
    {
        if (libusb_set_auto_detach_kernel_driver(hdev, true) != LIBUSB_SUCCESS)
            return -1;
    }
    if (libusb_claim_interface(hdev, 0) != 0)
    {
        std::cout << "Error claiming interface.\n";
        return -1;
    }
    
    libusb_device *dev = libusb_get_device(hdev);
    
    libusb_set_configuration(hdev, 1);
    
    int out_ep = usb_find_ep(dev, LIBUSB_ENDPOINT_OUT, LIBUSB_CLASS_VENDOR_SPEC, 93, 1);
    libusb_transfer *out_transfer;
    uint8_t *out_data;
    
    out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 8));
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    
    libusb_fill_control_setup(out_data, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, 1, 0x0000, 0x0000, 0x4);
    
    libusb_fill_control_transfer(out_transfer, hdev,
                                 out_data,
                                 on_read_data, NULL,
                                 0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer.\n";
        return -1;
    }
    
    /*out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 3));
    out_data[0] = 0x01;
    out_data[1] = 0x03;
    out_data[2] = 0x02;
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    libusb_fill_interrupt_transfer(out_transfer, hdev,
                                   out_ep | LIBUSB_ENDPOINT_OUT,
                                   out_data, 3,
                                   on_read_data, NULL,
                                   0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer out 1.\n";
        return -1;
    }*/
    
    /*out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 28));
    out_data[8+0] = 0x16;
    out_data[8+1] = 0x03;
    std::wstring controlle = L"Controlle";
    memcpy(out_data+8+2, controlle.c_str(), sizeof(wchar_t) * controlle.length());
    
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    
    libusb_fill_control_setup(out_data, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, 1, 0x0100, 0x0000, 0x14);
    
    libusb_fill_control_transfer(out_transfer, hdev,
                                   out_data,
                                   on_read_data, NULL,
                                   0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer out 2.\n";
        return -1;
    }*/
    
    /*out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 16));
    out_data[8+0] = 0x12;
    out_data[8+1] = 0x00;
    out_data[8+2] = 0x30;
    out_data[8+3] = 0x00;
    out_data[8+4] = 0x01;
    out_data[8+5] = 0x01;
    out_data[8+6] = 0x00;
    out_data[8+7] = 0xA0;
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    
    libusb_fill_control_setup(out_data, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, 1, 0x0000, 0x0000, 0x8);
    
    libusb_fill_control_transfer(out_transfer, hdev,
                                 out_data,
                                 on_read_data, NULL,
                                 0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer out 2.\n";
        return -1;
    }*/
    
    /*out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 12));
    out_data[8+0] = 0x00;
    out_data[8+1] = 0x14;
    out_data[8+2] = 0xFF;
    out_data[8+3] = 0xF7;
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    
    libusb_fill_control_setup(out_data, LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, 1, 0x0000, 0x0000, 0x4);
    
    libusb_fill_control_transfer(out_transfer, hdev,
                                 out_data,
                                 on_read_data, NULL,
                                 0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer out 2.\n";
        return -1;
    }*/
    
    /*
    out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 3));
    out_data[0] = 0x02;
    out_data[1] = 0x08;
    out_data[2] = 0x03;
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    libusb_fill_interrupt_transfer(out_transfer, hdev,
                                   out_ep | LIBUSB_ENDPOINT_OUT,
                                   out_data, 3,
                                   on_read_data, NULL,
                                   0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer out 2.\n";
        return -1;
    }
    
    out_transfer = libusb_alloc_transfer(0);
    out_data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 3));
    out_data[0] = 0x01;
    out_data[1] = 0x03;
    out_data[2] = 0x02;
    out_transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
    libusb_fill_interrupt_transfer(out_transfer, hdev,
                                   out_ep | LIBUSB_ENDPOINT_OUT,
                                   out_data, 3,
                                   on_read_data, NULL,
                                   0); // timeout
    if (libusb_submit_transfer(out_transfer) != LIBUSB_SUCCESS)
    {
        libusb_free_transfer(out_transfer);
        std::cout << "Error initiating transfer out 3.\n";
        return -1;
    }
    
    
    std::cout << std::flush;*/
    
    while (!g_isAborted) {
        int ep = usb_find_ep(dev, LIBUSB_ENDPOINT_IN, LIBUSB_CLASS_VENDOR_SPEC, 93, 1);
        libusb_transfer *transfer = libusb_alloc_transfer(0);
        uint8_t *data = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * 32));
        transfer->flags |= LIBUSB_TRANSFER_FREE_BUFFER;
        
        libusb_fill_interrupt_transfer(transfer, hdev,
                                       ep | LIBUSB_ENDPOINT_IN,
                                       data, 32,
                                       on_read_data, NULL,
                                       0); // timeout
        if (libusb_submit_transfer(transfer) != LIBUSB_SUCCESS)
        {
            libusb_free_transfer(transfer);
            std::cout << "Error initiating transfer.\n";
            return -1;
        }
        
        libusb_handle_events(NULL);
    }
    
    std::cout << "Hello, World!\n";
    libusb_exit(NULL);
    return 0;
}
