#ifndef CONSUMER_PRODUCER_H
#define CONSUMER_PRODUCER_H

#include </usr/include/semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"


void *ConsumerProducer1(void *arg);
void *ConsumerProducer2(void *arg);
void *ConsumerProducer3(void *arg);
#endif