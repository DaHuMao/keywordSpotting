//*****************************************************************************
// circ_buffer.c
//
// APIs for Implementing circular buffer
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup wifi_audio_app
//! @{
//
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//App include
#include "circ_buff.h"
//static pthread_mutex_t bufmutex = PTHREAD_MUTEX_INITIALIZER;

//*****************************************************************************
//
//! Creating and Initializing the Circular Buffer
//!
//! \param ulBufferSize is the size allocated to the circular buffer.
//!
//! This function  
//!        1. Allocates memory for the circular buffer.
//!     2. Initializes the control structure for circular buffer.
//!
//! \return pointer to the Circular buffer Control Structure.
//
//*****************************************************************************
tCircularBuffer *
CreateCircularBuffer(long ulBufferSize) {
    tCircularBuffer *pTempBuff;
    pTempBuff = (tCircularBuffer *) malloc(sizeof(tCircularBuffer));
    if (pTempBuff == NULL) {
        return NULL;
    }

    pTempBuff->pucBufferStartPtr = (char *) malloc(ulBufferSize);
    if (pTempBuff->pucBufferStartPtr == NULL)
    {
        printf("#####################CreateCircularBuffer malloc error \n");
        return NULL;
    }
    pTempBuff->pucReadPtr = pTempBuff->pucBufferStartPtr;
    pTempBuff->pucWritePtr = pTempBuff->pucBufferStartPtr;
    pTempBuff->ulBufferSize = ulBufferSize;
    pTempBuff->ulFreeSize = ulBufferSize;
    pTempBuff->pucBufferEndPtr = (pTempBuff->pucBufferStartPtr + ulBufferSize);
	
	pthread_mutex_init(&pTempBuff->lock, NULL);

    return (pTempBuff);
}

//*****************************************************************************
//
//! Creating and Initializing the Circular Buffer
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//!
//! This function  
//!    1. free memory allocated to the the circular buffer and its control
//!        structure.
//!
//! \return none
//
//*****************************************************************************
void
DestroyCircularBuffer(tCircularBuffer *pCircularBuffer) {
    if (pCircularBuffer->pucBufferStartPtr) {
        free(pCircularBuffer->pucBufferStartPtr);
    }
    pthread_mutex_destroy(&pCircularBuffer->lock);

    free(pCircularBuffer);
}

//*****************************************************************************
//
//! return the current value of the read pointer.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//!
//! \return current value of the read pointer.
//
//*****************************************************************************
char *
GetReadPtr(tCircularBuffer *pCircularBuffer) {

    pthread_mutex_lock(&pCircularBuffer->lock);
    char * retPtr = pCircularBuffer->pucReadPtr;
    pthread_mutex_unlock(&pCircularBuffer->lock);

    return retPtr;
}

//*****************************************************************************
//
//! return the current value of the write pointer.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//!
//! \return current value of the write pointer.
//
//*****************************************************************************
char *
GetWritePtr(tCircularBuffer *pCircularBuffer) {
    pthread_mutex_lock(&pCircularBuffer->lock);
    char * retPtr = pCircularBuffer->pucWritePtr;
    pthread_mutex_unlock(&pCircularBuffer->lock);

    return retPtr;
}

//*****************************************************************************
//
//! fills the one buffer by the content of other buffer.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//! \param pucBuffer is the pointer to the buffer from where the buffer is
//!        filled.
//! \param uiPacketSize is the amount of data need to be copied from pucBuffer
//!        to the Circular Buffer.
//!
//! This function  
//!        1. Copies the uiPacketSize of data from pucBuffer to the 
//!        pCircularBuffer.
//!
//! \return amount of data copied.
//
//*****************************************************************************
long
FillBuffer(tCircularBuffer *pCircularBuffer, char *pucBuffer,
           int uiPacketSize) {

    int iSizeWirte = uiPacketSize;

    pthread_mutex_lock(&pCircularBuffer->lock);
    if(iSizeWirte > pCircularBuffer->ulFreeSize)
    {
        pthread_mutex_unlock(&pCircularBuffer->lock);
        return -1;
    }

    int uiOffset;
    uiOffset=((pCircularBuffer->pucWritePtr+uiPacketSize) -
              pCircularBuffer->pucBufferEndPtr);

    if(uiOffset <= 0)
    {
        memcpy(pCircularBuffer->pucWritePtr, pucBuffer, uiPacketSize);

        pCircularBuffer->pucWritePtr +=  uiPacketSize;
        
        if(pCircularBuffer->pucWritePtr == pCircularBuffer->pucBufferEndPtr)
        {
            pCircularBuffer->pucWritePtr = pCircularBuffer->pucBufferStartPtr;
        }            
    }
    else
    {
        memcpy(pCircularBuffer->pucWritePtr, pucBuffer,
               (uiPacketSize - uiOffset));
        memcpy(pCircularBuffer->pucBufferStartPtr,
                ((pucBuffer + uiPacketSize) - uiOffset), uiOffset);

        pCircularBuffer->pucWritePtr = (pCircularBuffer->pucBufferStartPtr +
                                        uiOffset);                
    }

    pCircularBuffer->ulFreeSize -= uiPacketSize;

    pthread_mutex_unlock(&pCircularBuffer->lock);
    
    return(uiPacketSize);
}

//*****************************************************************************
//
//! reads the content of one buffer into the other.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//! \param pucBuffer is the pointer to the buffer to which the content is
//!        copied.
//! \param uiPacketSize is the amount of data need to be copied from Circular
//!        Buffer to the pucBuffer.
//!
//! This function  
//!        1. Copies the uiPacketSize of data from pCircularBuffer to the 
//!        pucBuffer.
//!
//! \return amount of data copied.
//
//*****************************************************************************
long
ReadBuffer(tCircularBuffer *pCircularBuffer, char *pucBuffer,
           int uiDataSize) {

    int iSizeRead = uiDataSize;

    pthread_mutex_lock(&pCircularBuffer->lock);
    if(iSizeRead > (pCircularBuffer->ulBufferSize - pCircularBuffer->ulFreeSize))
    {
        pthread_mutex_unlock(&pCircularBuffer->lock);
        return -1;
    }

    int uiOffset;

    uiOffset=((pCircularBuffer->pucReadPtr+uiDataSize) -
              pCircularBuffer->pucBufferEndPtr);

    if(uiOffset <= 0)
    {
        memcpy(pucBuffer, pCircularBuffer->pucReadPtr, uiDataSize);

        pCircularBuffer->pucReadPtr += uiDataSize;
        if(pCircularBuffer->pucReadPtr == pCircularBuffer->pucBufferEndPtr)
        {
            pCircularBuffer->pucReadPtr = pCircularBuffer->pucBufferStartPtr;
        }
       
    }
    else
    {
        memcpy(pucBuffer, pCircularBuffer->pucReadPtr,
               (uiDataSize - uiOffset));
        memcpy(((pucBuffer + uiDataSize) - uiOffset),
               pCircularBuffer->pucBufferStartPtr, uiOffset);

        pCircularBuffer->pucReadPtr = (pCircularBuffer->pucBufferStartPtr +
                                       uiOffset);               
    }

    pCircularBuffer->ulFreeSize += uiDataSize;

    pthread_mutex_unlock(&pCircularBuffer->lock);

    return (uiDataSize);
}

//*****************************************************************************
//
//! fills the buffer with zeores.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//! \param uiPacketSize is the amount of space needed to be filled with zeroes.
//!
//! This function  
//!        1. fill uiPacketSize amount of memory with zeroes.
//!
//! \return amount of zeroes filled.
//
//*****************************************************************************
long
FillZeroes(tCircularBuffer *pCircularBuffer) {

    pthread_mutex_lock(&pCircularBuffer->lock);
    memset(pCircularBuffer->pucBufferStartPtr, 0, pCircularBuffer->ulBufferSize);

    pCircularBuffer->pucReadPtr = pCircularBuffer->pucBufferStartPtr;
    pCircularBuffer->pucWritePtr = pCircularBuffer->pucBufferStartPtr;
    pCircularBuffer->ulFreeSize = pCircularBuffer->ulBufferSize;

    pthread_mutex_unlock(&pCircularBuffer->lock);
    return (pCircularBuffer->ulBufferSize);
}

//*****************************************************************************
//
//! check if the buffer is empty or not.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//!
//! This function  
//!        1. check if the buffer is empty or not.
//!
//! \return true if buffer is empty othr wise returns false.
//
//*****************************************************************************
tboolean
IsBufferEmpty(tCircularBuffer *pCircularbuffer) {
    pthread_mutex_lock(&pCircularbuffer->lock);
    tboolean ret = (pCircularbuffer->ulBufferSize == pCircularbuffer->ulFreeSize);
    pthread_mutex_unlock(&pCircularbuffer->lock);
    return ret;
}

//*****************************************************************************
//
//! check if the specified amount of buffer is filled or not.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//! \param ulsize is the amount of data which is to checked. 
//!
//! This function  
//!  1. check if the buffer is filled with specified amount of data or not.
//!
//! \return true if buffer is filled with specifed amount of data or more,
//!         otherwise false.
//
//*****************************************************************************
tboolean
IsBufferSizeFilled(tCircularBuffer *pCircularBuffer, long ulSize) {
    pthread_mutex_lock(&pCircularBuffer->lock);
    tboolean ret = (pCircularBuffer->ulFreeSize == 0);
    pthread_mutex_unlock(&pCircularBuffer->lock);
    return ret;
}

//*****************************************************************************
//
//! check if the buffer is filled less than the specified amount or not.
//!
//! \param pCircularBuffer is the pointer to the control structure for circular
//!        buffer.
//! \param ulsize is the amount of data which is to checked.
//!
//! This function  
//!  1. checks if the buffer is filled less than the specified amount or not.
//!
//! \return true if buffer is filled less than the specifed amount, otherwise
//!         false.
//
//*****************************************************************************
tboolean
IsBufferVacant(tCircularBuffer *pCircularBuffer, long ulSize) {
    pthread_mutex_lock(&pCircularBuffer->lock);
    tboolean ret = (pCircularBuffer->ulFreeSize != 0);
    pthread_mutex_unlock(&pCircularBuffer->lock);
    return ret;
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

