#pragma once
// MESSAGE AUTO_AIM PACKING

#define MAVLINK_MSG_ID_AUTO_AIM 60001


typedef struct __mavlink_auto_aim_t {
 uint8_t target_system; /*<  Target System ID*/
 uint8_t target_component; /*<  Target Component ID*/
} mavlink_auto_aim_t;

#define MAVLINK_MSG_ID_AUTO_AIM_LEN 2
#define MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN 2
#define MAVLINK_MSG_ID_60001_LEN 2
#define MAVLINK_MSG_ID_60001_MIN_LEN 2

#define MAVLINK_MSG_ID_AUTO_AIM_CRC 101
#define MAVLINK_MSG_ID_60001_CRC 101



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_AUTO_AIM { \
    60001, \
    "AUTO_AIM", \
    2, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_auto_aim_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_auto_aim_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_AUTO_AIM { \
    "AUTO_AIM", \
    2, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_auto_aim_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_auto_aim_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a auto_aim message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_auto_aim_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_AUTO_AIM_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#else
    mavlink_auto_aim_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_AUTO_AIM;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
}

/**
 * @brief Pack a auto_aim message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_auto_aim_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_AUTO_AIM_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#else
    mavlink_auto_aim_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_AUTO_AIM;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#endif
}

/**
 * @brief Pack a auto_aim message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_auto_aim_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_AUTO_AIM_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#else
    mavlink_auto_aim_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_AUTO_AIM_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_AUTO_AIM;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
}

/**
 * @brief Encode a auto_aim struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param auto_aim C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_auto_aim_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_auto_aim_t* auto_aim)
{
    return mavlink_msg_auto_aim_pack(system_id, component_id, msg, auto_aim->target_system, auto_aim->target_component);
}

/**
 * @brief Encode a auto_aim struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param auto_aim C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_auto_aim_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_auto_aim_t* auto_aim)
{
    return mavlink_msg_auto_aim_pack_chan(system_id, component_id, chan, msg, auto_aim->target_system, auto_aim->target_component);
}

/**
 * @brief Encode a auto_aim struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param auto_aim C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_auto_aim_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_auto_aim_t* auto_aim)
{
    return mavlink_msg_auto_aim_pack_status(system_id, component_id, _status, msg,  auto_aim->target_system, auto_aim->target_component);
}

/**
 * @brief Send a auto_aim message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_auto_aim_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_AUTO_AIM_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_AUTO_AIM, buf, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
#else
    mavlink_auto_aim_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_AUTO_AIM, (const char *)&packet, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
#endif
}

/**
 * @brief Send a auto_aim message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_auto_aim_send_struct(mavlink_channel_t chan, const mavlink_auto_aim_t* auto_aim)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_auto_aim_send(chan, auto_aim->target_system, auto_aim->target_component);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_AUTO_AIM, (const char *)auto_aim, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
#endif
}

#if MAVLINK_MSG_ID_AUTO_AIM_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_auto_aim_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_AUTO_AIM, buf, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
#else
    mavlink_auto_aim_t *packet = (mavlink_auto_aim_t *)msgbuf;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_AUTO_AIM, (const char *)packet, MAVLINK_MSG_ID_AUTO_AIM_MIN_LEN, MAVLINK_MSG_ID_AUTO_AIM_LEN, MAVLINK_MSG_ID_AUTO_AIM_CRC);
#endif
}
#endif

#endif

// MESSAGE AUTO_AIM UNPACKING


/**
 * @brief Get field target_system from auto_aim message
 *
 * @return  Target System ID
 */
static inline uint8_t mavlink_msg_auto_aim_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field target_component from auto_aim message
 *
 * @return  Target Component ID
 */
static inline uint8_t mavlink_msg_auto_aim_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a auto_aim message into a struct
 *
 * @param msg The message to decode
 * @param auto_aim C-struct to decode the message contents into
 */
static inline void mavlink_msg_auto_aim_decode(const mavlink_message_t* msg, mavlink_auto_aim_t* auto_aim)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    auto_aim->target_system = mavlink_msg_auto_aim_get_target_system(msg);
    auto_aim->target_component = mavlink_msg_auto_aim_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_AUTO_AIM_LEN? msg->len : MAVLINK_MSG_ID_AUTO_AIM_LEN;
        memset(auto_aim, 0, MAVLINK_MSG_ID_AUTO_AIM_LEN);
    memcpy(auto_aim, _MAV_PAYLOAD(msg), len);
#endif
}
