#ifndef CODEMESSAGE_H
#define CODEMESSAGE_H

#include <stdint.h>

typedef enum
{
	DECODE_STATE_INIT,
	//
	DECODE_STATE_START_FOUND,
	DECODE_STATE_PAYLOAD_LEN_ESCAPE_FOUND,
	DECODE_STATE_PAYLOAD_LEN_FOUND,
	DECODE_STATE_PAYLOAD,
	DECODE_STATE_PAYLOAD_ESCAPE,
	//
	DECODE_STATE_COMPLETE,
	//
	DECODE_STATE_MAX
} DECODE_STATE_t;

//Escaped Characters Defined
#define TRANSPORT_CODE_START_MSG       (0x80)
#define TRANSPORT_CODE_ESCAPE          (0x81)
#define ENCODE_CHAR(c)                 (c + 0x20)
#define DECODE_CHAR(c)                 (c - 0x20)

uint8_t CodeMsg_AddByteToMsg(uint8_t u8Byte, uint8_t *pu8TxBuffer);
uint8_t CodeMsg_CreateHeader(uint8_t u8PayloadLen, uint8_t *pu8TxBuffer);
DECODE_STATE_t CodeMsg_DecodeEscapedMsg(
    DECODE_STATE_t decodeState,
    uint8_t *pu8PayloadLen, //return item
    uint8_t *pu8RxBuffer, //buffer with received message
    uint8_t u8RxBufferLen, //buffer with received message
    uint8_t *pu8RxBufferBytesParsed, //number of bytes parsed in total
    uint8_t *pu8Msg, //message buffer to place data in. First byte is message type, following bytes are data bytes
    uint8_t *pu8MsgLen); //index for pu8Msg buffer
/***************************************************************************//*!
AddByteToMsg()
Description:

    @param u8Byte
    @param pu8TxBuffer

    @return The number of bytes added to the tx buffer.
***************************************************************************** */
uint8_t CodeMsg_AddByteToMsg(
    uint8_t u8Byte,
    uint8_t *pu8TxBuffer)
{
    uint8_t u8NumBytes;

    switch(u8Byte)
    {
    case TRANSPORT_CODE_START_MSG:    
    case TRANSPORT_CODE_ESCAPE:
        *pu8TxBuffer = TRANSPORT_CODE_ESCAPE;
        *(pu8TxBuffer + 1) = ENCODE_CHAR(u8Byte);
        u8NumBytes = 2;
        break;
    default:
        *pu8TxBuffer = u8Byte;
        u8NumBytes = 1;
        break;
    }

    return(u8NumBytes);
}

/***************************************************************************//*!
CreateHeader()
Description:

    @param u8PayloadLen - length of data bytes
    @param pu8TxBuffer - point to buffer which contains the message

    @return The number of bytes added to the tx buffer.
***************************************************************************** */
uint8_t CodeMsg_CreateHeader(
    uint8_t u8PayloadLen,
    uint8_t *pu8TxBuffer)
{
    uint8_t u8NumBytes = 1; //declared as 1 because start msg character is automatically added

    *pu8TxBuffer++ = TRANSPORT_CODE_START_MSG;

    u8NumBytes += CodeMsg_AddByteToMsg(u8PayloadLen, pu8TxBuffer);
    
    return(u8NumBytes);
}

/***************************************************************************//*!
CodeMsg_CreateEscapedTxMsg()
Description:

    @param pu8Msg
    @param u8MsgLen
    @param u8MaxTxBufferLen - size of pu8TxBuffer
    @param pu8TxBuffer

    @return The number of bytes added to the tx buffer.
***************************************************************************** */
uint16_t CodeMsg_CreateEscapedTxMsg(
    uint8_t *pu8Msg,
    uint8_t u8MsgLen,
    uint8_t u8MaxTxBufferLen,
    uint8_t *pu8TxBuffer)
{
    uint8_t u8Cnt;
    uint8_t u8NumBytes;
    uint8_t u8TotalBytes = 0;

    // add header
    u8NumBytes = CodeMsg_CreateHeader(u8MsgLen, pu8TxBuffer); //adds the message length byte
    pu8TxBuffer += u8NumBytes; //increments the buffer pointer by number of bytes added
    u8TotalBytes += u8NumBytes; 

    // add payload data (including the message type)
    for(u8Cnt = 0; u8Cnt < u8MsgLen; u8Cnt++)
    {
        //add one byte at a time
        u8NumBytes = CodeMsg_AddByteToMsg(*pu8Msg++, pu8TxBuffer);
        u8TotalBytes += u8NumBytes;
        
        //if buffer is overflowed...execute this
        if(u8TotalBytes > u8MaxTxBufferLen)
        {
            u8TotalBytes = 0;
            break;
        }
        
        pu8TxBuffer += u8NumBytes; //increment the buffer pointer by number of bytes added (1 or 2 due to escaped character)
    }

    return(u8TotalBytes);
}

/***************************************************************************//*!
DecodeEscapedMsg()
Description:

    Keep calling with new received data until DECODE_STATE_COMPLETE or
    DECODE_STATE_ERROR is received.  For the first call, set decodeState
    to DECODE_STATE_INIT and then set it to the returned value until
    DECODE_STATE_COMPLETE is returned.

    @note - only set decodeState to DECODE_STATE_INIT.  This can be done
    after receiving a DECODE_STATE_COMPLETE to start the next message or
    it can be done to abort the current decoding.

    Typical usage:
    DECODE_STATE_t decodeState = DECODE_STATE_INIT;
    uint16_t u16PayloadLen;
    // pu8RxBuffer and u16BufferLen hold the rx'd escaped message data and length
    // pu8Msg and u16DecodedMsgLen store the decoded payload data.  If
    // DECODE_STATE_COMPLETE is returned, then the entire message is in
    // this buffer with the u16DecodedMsgLen length.
    // Note that if _COMPLETE is returned, there may still be data left in
    // pu8RxBuffer.  The caller has the option of moving this data to the start
    // of this buffer so that more received data can be added to the end.

    decodeState = DecodeEscapedMsg(decodeState, &u16PayloadLen,
                                        pu8RxBuffer, u16RxBufferLen,
                                        &u16RxBufferBytesParsed,
                                        pu8Msg, &u16DecodedMagLen);
    if(decodeState == DECODE_STATE_COMPLETE)
    {
        if(u16RxBufferLen - u16RxBufferBytesParsed != 0)
        {
            // there is more data in the encoded stream for the next call
            memcpy(pu8RxBuffer, &pu8RxBuffer[u16RxBufferBytesParsed],
                   u16RxBufferLen - u16RxBufferBytesParsed);
            u16RxBufferLen -= u16RxBufferBytesParsed;
        }
        else
        {
            u16RxBufferLen = 0;
        }

        decodeState = DECODE_STATE_INIT;
        // -use the decoded message in pu8Msg
    }
    else
    {
        // the encoded rx buffer should be empty
        // -we need to get more data to put at the start of pu8RxBuffer
        // -we can continue parsing or start over (DECODE_STATE_INIT)
        u16RxBufferLen = 0;
    }

    @param decodeState[in] - DECODE_STATE_INIT to start or if DECODE_STATE_COMPLETE
        was returned last time.  Otherwise use the value from the last call to this function.
    @param pu8PayloadLen[int,out] - decoded payload length, only valid if the
        message header has been parsed.  This is maintained and only used by
        this function.
    @param pu8RxBuffer[in] - pointer to encoded receive buffer
    @param u16RxBufferLen[in] - number of bytes in encoded receive buffer
    @param pu16RxBufferBytesParsed[out] - number of bytes parsed in receive buffer
    @param pu8Msg[out] - decoded payload message
    @param pu16MsgLen[out] - decoded payload message length

    @return
    DECODE_STATE_INIT - a message start has not been found
    DECODE_STATE_START_FOUND - the message start has been found
    DECODE_STATE_PAYLOAD_LEN_HIGH_ESCAPE_FOUND - the escape code for the
        payload length high byte has been found
    DECODE_STATE_PAYLOAD_LEN_HIGH_FOUND - the payload length high byte
        has been found
    DECODE_STATE_PAYLOAD_LEN_LOW_ESCAPE_FOUND - the escape code for the
        payload length low byte has been found
    DECODE_STATE_PAYLOAD - the payload is being decoded
    DECODE_STATE_PAYLOAD_ESCAPE - the payload is being decoded and an
        escape character was last found
    DECODE_STATE_COMPLETE - the full message has been received

NOTE: currently just send in one character at a time (u8RxBufferLen should equal 1)
***************************************************************************** */
DECODE_STATE_t CodeMsg_DecodeEscapedMsg(
    DECODE_STATE_t decodeState,
    uint8_t *pu8PayloadLen, //return item
    uint8_t *pu8RxBuffer, //buffer with received message
    uint8_t u8RxBufferLen, //buffer with received message
    uint8_t *pu8RxBufferBytesParsed, //number of bytes parsed in total
    uint8_t *pu8Msg, //message buffer to place data in. First byte is message type, following bytes are data bytes
    uint8_t *pu8MsgLen) //index for pu8Msg buffer
{
    uint8_t u8RxBufId;
    uint8_t u8Char;

    *pu8RxBufferBytesParsed = 0;

    for(u8RxBufId = 0; u8RxBufId < u8RxBufferLen && decodeState != DECODE_STATE_COMPLETE; u8RxBufId++)
    {
        switch(decodeState)
        {
        /////////////////////////////////////////////////////////////
        case DECODE_STATE_INIT:
            // look for start of message
            if(pu8RxBuffer[u8RxBufId] == TRANSPORT_CODE_START_MSG)
            {
                decodeState = DECODE_STATE_START_FOUND;
            }
            break;
        /////////////////////////////////////////////////////////////
        case DECODE_STATE_START_FOUND:
            // look for payload msg len size high byte
            switch(pu8RxBuffer[u8RxBufId])
            {
            case TRANSPORT_CODE_START_MSG:
                // another start msg code, so just ignore it
				break;
            case TRANSPORT_CODE_ESCAPE:
                decodeState = DECODE_STATE_PAYLOAD_LEN_ESCAPE_FOUND;
                break;
            default:
                *pu8PayloadLen = pu8RxBuffer[u8RxBufId];
                decodeState = DECODE_STATE_PAYLOAD;
                *pu8MsgLen = 0;
                break;
            }
            break;
        /////////////////////////////////////////////////////////////
        case DECODE_STATE_PAYLOAD_LEN_ESCAPE_FOUND:
            // get payload msg len high byte
            switch(pu8RxBuffer[u8RxBufId])
            {
            case TRANSPORT_CODE_START_MSG:
                // another start msg code, so start over
                decodeState = DECODE_STATE_START_FOUND;
                break;
            case TRANSPORT_CODE_ESCAPE:
                // this is not valid, so start over
                decodeState = DECODE_STATE_INIT;
                break;
            default:
                u8Char = DECODE_CHAR(pu8RxBuffer[u8RxBufId]);
                *pu8PayloadLen = u8Char;
                *pu8MsgLen = 0;
                decodeState = DECODE_STATE_PAYLOAD;
                break;
            }
            break;
        /////////////////////////////////////////////////////////////
        case DECODE_STATE_PAYLOAD:
            // get payload data
            switch(pu8RxBuffer[u8RxBufId])
            {
            case TRANSPORT_CODE_START_MSG:
                // another start msg code, so start over
                decodeState = DECODE_STATE_START_FOUND;
                break;
            case TRANSPORT_CODE_ESCAPE:
                decodeState = DECODE_STATE_PAYLOAD_ESCAPE;
                break;
            default:
                pu8Msg[(*pu8MsgLen)++] = pu8RxBuffer[u8RxBufId];
                if(*pu8MsgLen == *pu8PayloadLen)
                {
                    decodeState = DECODE_STATE_COMPLETE;
                }
                break;
            }
            break;
        /////////////////////////////////////////////////////////////
        case DECODE_STATE_PAYLOAD_ESCAPE:
            // get payload data
            switch(pu8RxBuffer[u8RxBufId])
            {
            case TRANSPORT_CODE_START_MSG:
                // another start msg code, so start over
                decodeState = DECODE_STATE_START_FOUND;
                break;
            case TRANSPORT_CODE_ESCAPE:
                // this is not valid, so start over
                decodeState = DECODE_STATE_INIT;
                break;
            default:
                pu8Msg[(*pu8MsgLen)++] = DECODE_CHAR(pu8RxBuffer[u8RxBufId]);
                if(*pu8MsgLen == *pu8PayloadLen)
                {
                    decodeState = DECODE_STATE_COMPLETE;
                }
                else
                {
                    decodeState = DECODE_STATE_PAYLOAD;
                }
                break;
            }
            break;
        /////////////////////////////////////////////////////////////
        case DECODE_STATE_COMPLETE:
            break;
        /////////////////////////////////////////////////////////////
        default:
            break;
        }
    }

    *pu8RxBufferBytesParsed = u8RxBufId + 1;

    return(decodeState);
}

#endif
