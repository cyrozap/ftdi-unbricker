/*
brick.c - Set the PID of any connected FTDI chips to zero.
Copyright (C) 2014  Forest Crossman <cyrozap@gmail.com>
based on code by Intra2net AG and the libftdi developers copyright (C) 2003-2014 libftdi

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the
License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>

int bad_pid = 0x0000;

int main(void)
{
    int ret;
    int num_devices;

    struct ftdi_device_list *devlist;
    struct ftdi_device_list *curdev;
    struct ftdi_context *ftdi;
    struct ftdi_version_info version;

    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }

    version = ftdi_get_library_version();
    printf("Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
        version.version_str, version.major, version.minor, version.micro,
        version.snapshot_str);

    // Find and open devices
    if ((num_devices = ftdi_usb_find_all(ftdi, &devlist, 0, 0)) < 0)
    {
        fprintf(stderr, "Unable to list devices: %d (%s)\n", num_devices, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        ftdi_list_free(&devlist);
        return EXIT_FAILURE;
    }
    else if (num_devices == 0)
    {
        fprintf(stderr, "No devices found.\n");
        ftdi_free(ftdi);
        ftdi_list_free(&devlist);
        return EXIT_FAILURE;
    }
    else if (num_devices > 0)
    {
        printf("%d devices found!\n", num_devices);
        curdev = devlist;
        while (curdev != NULL)
        {
            // Get device serial
            char serial[128];
            if ((ret = ftdi_usb_get_strings(ftdi, curdev->dev, NULL, 0, NULL, 0, serial, 128)) < 0)
            {
                fprintf(stderr, "Unable to read device EEPROM: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
                continue;
            }

            // Open the selected device
            if ((ret = ftdi_usb_open_dev(ftdi, curdev->dev)) < 0)
            {
                fprintf(stderr, "Unable to open device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
            }
            else
            {
                // Read and parse the device EEPROM
                printf("Reading EEPROM of device with serial number %s...\n", serial);
                if ((ret = ftdi_read_eeprom(ftdi)) < 0)
                {
                    fprintf(stderr, "Unable to read device EEPROM: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
                    continue;
                }
                if ((ret = ftdi_eeprom_decode(ftdi, 0)) < 0)
                {
                    fprintf(stderr, "Unable to parse device EEPROM: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
                    continue;
                }

                // Write the bad PID to the device
                printf("Writing EEPROM of device with serial number %s...\n", serial);
                ftdi_set_eeprom_value(ftdi, PRODUCT_ID, bad_pid);
                if ((ret = ftdi_eeprom_build(ftdi)) < 0)
                {
                    fprintf(stderr, "Failed to build EEPROM: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
                    continue;
                }
                if ((ret = ftdi_write_eeprom(ftdi)) < 0)
                {
                    fprintf(stderr, "Unable to write device EEPROM: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
                    continue;
                }

                // Close the selected device
                if ((ret = ftdi_usb_close(ftdi)) < 0)
                {
                    fprintf(stderr, "Unable to close device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
                }
            }
            curdev = curdev->next;
        }
        return EXIT_SUCCESS;
    }

    // Cleanup
    ftdi_free(ftdi);
    ftdi_list_free(&devlist);
}
