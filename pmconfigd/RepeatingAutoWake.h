/*
 * Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef _RepeatingAutoWake_h_
#define _RepeatingAutoWake_h_

#include <IOKit/pwr_mgt/IOPMLib.h>
#include <IOKit/pwr_mgt/IOPMLibPrivate.h>

/* For distribution of notifications from pmconfigd.c */
__private_extern__ void RepeatingAutoWake_prime(void);

__private_extern__ void RepeatingAutoWakePrefsHaveChanged(void);

__private_extern__ void RepeatingAutoWakeSleepWakeNotification(natural_t);

/* For shutdown/sleep handling from AutoWakeScheduler.c */
__private_extern__ void RepeatingAutoWakeTimeForPowerOff(void);
__private_extern__ void RepeatingAutoWakeTimeForPowerOn(void);

#endif // _RepeatingAutoWake_h_
