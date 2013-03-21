/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 *  XXXXXXXXXXXX.c
 *  Automated Power Management tests
 *
 *  Created by Ethan Bold on 7/25/08.
 *  Copyright 2008 Apple. All rights reserved.
 *
 */

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SCValidation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/pwr_mgt/IOPMLibPrivate.h>

#include <servers/bootstrap.h>
#include <mach/mach_port.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "PMTestLib.h"

#define kMaxTestIterations 25

#define kMaxTestThreads 3

void threadLoopConnection(void *context)
{
    IOReturn                ret = kIOReturnSuccess;
    kern_return_t           kern_result = KERN_SUCCESS;
    mach_port_t             new_pm_connection = MACH_PORT_NULL;
    static  int             use_profile = 1;
    CFDictionaryRef         the_profiles = NULL;
    CFMutableDictionaryRef  changeable_profiles = NULL;
    CFNumberRef             putThisNumberIn = NULL;
    int                     thread_num = (int)context;
    int                     testiterations = 0;

    do {
    
    
    
        /*
         * Open a mach port connection to pm configd
         */
        PMTestLog("Thread %d running\n", thread_num);
        kern_result = bootstrap_look_up(bootstrap_port, kIOPMServerBootstrapName, &new_pm_connection);    
        if (KERN_SUCCESS != kern_result || MACH_PORT_NULL == new_pm_connection) {
                PMTestFail("Error establing PM configd connection: return %d, mach port = %d\n", kern_result, new_pm_connection);            
        }    
        PMTestPass("Open PM configd connection");

        if (MACH_PORT_NULL == new_pm_connection) {
            continue;
        }

        /*
         * Set power profiles, as seen in Energy Saver.
         */
        the_profiles = IOPMCopyActivePowerProfiles();
        if (!the_profiles) {
            PMTestFail("NULL return from IOPMCopyActivePowerProfiles().\n");
        }
        changeable_profiles = CFDictionaryCreateMutableCopy(0, 0, the_profiles);
        putThisNumberIn = CFNumberCreate(0, kCFNumberIntType, &use_profile);
        if (1 == use_profile) {
            use_profile = 2;
        } else {
            use_profile = 1;
        }
        CFDictionarySetValue(changeable_profiles, CFSTR(kIOPMACPowerKey), putThisNumberIn);
        ret = IOPMSetActivePowerProfiles(changeable_profiles);
        if(kIOReturnSuccess != ret) {
            PMTestFail("IOPMSetActivePowerProfiles returned 0x%08x; dictionary = %p; number = %p",
                        ret, changeable_profiles, putThisNumberIn);
        }
        PMTestPass("Set a Power Profile");


        /*
         * Close the open mach port connection
         */        
        if (MACH_PORT_NULL == new_pm_connection) {
            PMTestFail("Error - cannot close NULL mach port.\n");
        } else {
            mach_port_destroy(mach_task_self(), new_pm_connection);
        }
        PMTestPass("Completed %d of %d iterations.", testiterations, kMaxTestIterations);
    } while (testiterations++ < kMaxTestIterations);

    return;
}

int main(int argc, char *argv[])
{
    pthread_t           myThreads[kMaxTestThreads];
    int                 pthread_result;
    int                 i;
    IOReturn            ret = 0;

    ret = PMTestInitialize("PM configd server connection", "com.apple.iokit.ethan");     
    if (kIOReturnSuccess != ret)
    {
        printf("PMTestInitialize fails. IOReturn error code = 0x%08x\n", ret);
        exit(1);
    }
    for (i=0; i<kMaxTestThreads; i++) 
    {
        pthread_result = pthread_create(&myThreads[i], NULL, (void *)threadLoopConnection, (void *)i);
        if (0 != pthread_result) {
            printf("Error %d return from pthread_create(\"%d\")\n", pthread_result, i);
        }
    }

    for (i=0; i<kMaxTestThreads; i++) 
    {
        pthread_result = pthread_join(myThreads[i], NULL);
    }

    return 0;
}



