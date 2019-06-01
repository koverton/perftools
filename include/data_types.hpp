#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <memory>
#include <string>
#include <cstring>
#include <algorithm>
#if !defined(WIN32) && !defined(__VMS)
#include <stdint.h>
#endif
#if defined(__VMS)
#include <inttypes.h>
#endif

// ceil(num/den)
#define DIV_ROUND_UP(num, den) ((num + den - 1) / den)

// For even denominators, we round up when the result is exactly x.5
#define DIV_ROUND_OFF(num, den) ((num + (den >> 1)) / den)

// floor(num/den)
#define DIV_ROUND_DOWN(num, den) (num / den)


/// <summary>
/// Common data types and declarations.
/// </summary>

// Thread return types.


typedef char            INT8;
typedef unsigned char   UINT8;
typedef short           INT16;
typedef unsigned short  UINT16;

#if UINT_MAX == 0xffffffff
  typedef int           INT32;         /**< 32-bit signed integer type. */
  typedef unsigned int  UINT32;        /**< 32-bit unsigned integer type. */
#elif ULONG_MAX == 0xffffffff
  typedef long          INT32;         /**< 32-bit signed integer type. */
  typedef unsigned long UINT32;        /**< 32-bit unsigned integer type. */
#else
    typedef int32_t     INT32;
    typedef uint32_t    UINT32;
#endif

#ifdef WIN32
  typedef __int64          INT64;      /**< 64-bit signed integer type. */
  typedef unsigned __int64 UINT64;     /**< 64-bit unsigned integer type. */
# else
  typedef long long          INT64;    /**< 64-bit signed integer type. */
  typedef unsigned long long UINT64;   /**< 64-bit unsigned integer type. */
#endif

#ifdef __VMS
/*
 * In OpenVMS pointers can have a mixture of 32 and 64 bit pointers.
 * Certain RTL functions use only 32 bit pointers as well as the main
 * function argV.
 * Both 32, and 64 bit pointer types are available using the
 * #pragma pointer_size SIZE=(32,64,short,long).
 *
 * */
#pragma pointer_size save
#pragma pointer_size 64
typedef char* charPtr64; /** 64 bit char pointer type */
typedef void* ptr64;     /** 64 bit pointer type */
#pragma pointer_size 32
typedef char* charPtr32; /** 32 bit char pointer type */
typedef void* ptr32;     /** 32 bit pointer type */
#pragma pointer_size restore

#if __INITIAL_POINTER_SIZE==64
typedef charPtr64 charPtr;
#else
typedef charPtr32 charPtr;
#endif

#else
typedef char* charPtr;
typedef charPtr charPtr32;
typedef charPtr charPtr64;
#endif


// Constants for client modes
const char CLIENT_MODE_REPLY[]       = "reply";
const char CLIENT_MODE_REFLECT[]     = "reflect";
const char CLIENT_MODE_SINK[]        = "sink";

const char TRANS_TYPE_HTTP[]         ="http";
const char TRANS_TYPE_SMF[]          ="smf";

const char DEF_CLIENT[]              = "perf_client";


const char DEFAULT_PUB_XML_MSG_START_TAG[] = "<x>";
const char DEFAULT_PUB_XML_MSG_END_TAG[]="</x>";
const UINT64 MILLION = 1000000;

const int   ALL_CLIENTS_INDEX      = -1;

const std::string DEFAULT_USERDATA = "abcdefghijklmnopqrstuvwxyz0123456789";
const std::string EMPTY_STRING = "";

const char CACHE_LIVE_DATA_ACTION_FULFILL[] = "FULFILL";
const char CACHE_LIVE_DATA_ACTION_QUEUE[] = "QUEUE";
const char CACHE_LIVE_DATA_ACTION_FLOW_THRU[] = "FLOW_THRU";

enum SubscriptionType
{
    SUBSCRIPTION_TYPE_UNKNOWN = 0,
    SUBSCRIPTION_TYPE_XPE     = 1,
    SUBSCRIPTION_TYPE_TOPIC   = 2
};

enum EndpointType
{
    ENDPOINT_TYPE_UNKNOWN = 0,
    ENDPOINT_TYPE_DTE     = 1,
    ENDPOINT_TYPE_TTE     = 2,
    ENDPOINT_TYPE_DQE     = 3,
    ENDPOINT_TYPE_TQE     = 4
};

enum ToolDataType
{
    TOOL_DATA_TYPE_BIN_ATTACH   = 0,
    TOOL_DATA_TYPE_USER_PROP    = 1
};

// Constants for message types
const char MSG_TYPE_DIRECT[]         = "direct";
const char MSG_TYPE_NON_PERSISTENT[] = "nonpersistent";
const char MSG_TYPE_PERSISTENT[]     = "persistent";

// Constants for ack event mode
const char AD_ACK_EVENT_MODE_PER_MESSAGE[] = "per-message";
const char AD_ACK_EVENT_MODE_WINDOWED[]    = "windowed";

const char AUTHENTICATION_SCHEME_BASIC[]                = "basic";
const char AUTHENTICATION_SCHEME_CLIENT_CERTIFICATE[]   = "client-certificate";
const char AUTHENTICATION_SCHEME_GSS_KRB[]              = "kerberos";

enum PubSubMessageType
{
    PUBSUB_MSG_TYPE_NON_PERSISTENT = 0,
    PUBSUB_MSG_TYPE_PERSISTENT     = 1,
    PUBSUB_MSG_TYPE_DIRECT         = 2
};

// Constants for message rate target.
const char MSG_RATE_TARGET_AVG[] = "avg";
const char MSG_RATE_TARGET_MAX[] = "max";


// Number are different then CCSMP.  They match the user input from the cli.
// Each SDK must map these to their values.
enum PubSubCosType
{
    PUBSUB_COS_NO = -1,
    PUBSUB_COS_1 = 1,
    PUBSUB_COS_2 = 2,
    PUBSUB_COS_3 = 3
};

enum PubSubTransportType
{
    PUBSUB_TRANS_TYPE_HTTP = 0,
    PUBSUB_TRANS_TYPE_SMF = 1
};

const int   SUB_RECEIVE_FOREVER         = -1;
const int   PUB_SEND_VECTOR_MAX_SIZE    = 50;
const int   CCSMP_MAX_PROPS             = 100;
const int   MAX_PUB_WIN_SIZE            = 50;
        
// Structured data message constants
const int PUBSUB_SDM_ID_NULL = -1;
const int PUBSUB_SDM_ID_ALL = 0;
const int PUBSUB_SDM_ID_MIN = 1;
const int PUBSUB_SDM_ID_MAX = 21;
const int PUBSUB_SDM_ID_USER_DEF_BASE = 50;  // SDM messages from files will start with this id. Below is reserved for famous messages.


const INT32 STREAM_ID_DEFAULT = -1;
const INT32 STREAM_ID_TOPIC_SEQUENCE = -2;
                


// Use to force a designer to initialize static fixed sized arrays for scalars
// Example usage:
//
// test.hpp
// static const INT32 NUM_ELTS = 2;
//
// test.cpp
// INT32 array_g[NUM_ELTS] = { 1,2 };
//
// The problem with the above is that if someone changes NUM_ELTS but forgets
// to change the array_g you can have problems if zero is not considered a
// valid value. Instead do the the following to cause a compile-time error:
//
// static const INT32 NUM_ELTS = 3;
//
// ArrayInitElt<INT32> array2_g[NUM_ELTS] = { 1, 2 } // error!!
//
// Due to the conversion operator array2_g can be used just like array_g in
// expressions:
//
// INT32 val = array2_g[2];
//
template <typename T>
struct ArrayInitElt {
    ArrayInitElt(T elt) : elt_m(elt) {}
    inline operator T() const { return elt_m; }
    T elt_m;
};


#endif// DATA_TYPES_HPP
