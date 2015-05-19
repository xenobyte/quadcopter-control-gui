/*
 * TCP_TestConnection.h
 *
 *  Created on: 17.05.2015
 *      Author: Johannes
 */

#ifndef SRC_TCP_TESTCONNECTION_H_
#define SRC_TCP_TESTCONNECTION_H_

#include "Logging.h"

extern Logging LOG;

enum TReadStatus {
    eWouldBlock = -1,
    eFailed     =  0,
    eSuccess    =  1
};


#endif /* SRC_TCP_TESTCONNECTION_H_ */
