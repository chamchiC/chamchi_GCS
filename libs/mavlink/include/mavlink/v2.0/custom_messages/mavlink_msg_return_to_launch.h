#pragma once
// MESSAGE RETURN_TO_LAUNCH PACKING

#define MAVLINK_MSG_ID_RETURN_TO_LAUNCH 60003


typedef struct __mavlink_return_to_launch_t {
 uint8_t target_system; /*<  Target System ID*/
 uint8_t target_component; /*<  Target Component ID*/
} mavlink_return_to_launch_t;

#define MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN 2
#define MAVLINK_MSG_ID_RETURN_TO_LAUNCH_MIN_LEN 2
#define MAVLINK_MSG_ID_60003_LEN 2
#define MAVLINK_MSG_ID_60003_MIN_LEN 2

#define MAVLINK_MSG_ID_RETURN_TO_LAUNCH_CRC 103
#define MAVLINK_MSG_ID_60003_CRC 103



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_RETURN_TO_LAUNCH { \
    60003, \
    "RETURN_TO_LAUNCH", \
    2, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_return_to_launch_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_return_to_launch_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_RETURN_TO_LAUNCH { \
    "RETURN_TO_LAUNCH", \
    2, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_return_to_launch_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_return_to_launch_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a return_to_launch message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_return_to_launch_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN);
#else
    mavlink_return_to_launch_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RETURN_TO_LAUNCH;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_MIN_LEN, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_CRC);
}

/**
 * @brief Pack a return_to_launch message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_return_to_launch_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN);
#else
    mavlink_return_to_launch_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_RETURN_TO_LAUNCH;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_MIN_LEN, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_CRC);
}

/**
 * @brief Encode a return_to_launch struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param return_to_launch C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_return_to_launch_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_return_to_launch_t* return_to_launch)
{
    return mavlink_msg_return_to_launch_pack(system_id, component_id, msg, return_to_launch->target_system, return_to_launch->target_component);
}

/**
 * @brief Encode a return_to_launch struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param return_to_launch C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_return_to_launch_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_return_to_launch_t* return_to_launch)
{
    return mavlink_msg_return_to_launch_pack_chan(system_id, component_id, chan, msg, return_to_launch->target_system, return_to_launch->target_component);
}

// MESSAGE RETURN_TO_LAUNCH UNPACKING

/**
 * @brief Get field target_system from return_to_launch message
 *
 * @return  Target System ID
 */
static inline uint8_t mavlink_msg_return_to_launch_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field target_component from return_to_launch message
 *
 * @return  Target Component ID
 */
static inline uint8_t mavlink_msg_return_to_launch_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a return_to_launch message into a struct
 *
 * @param msg The message to decode
 * @param return_to_launch C-struct to decode the message contents into
 */
static inline void mavlink_msg_return_to_launch_decode(const mavlink_message_t* msg, mavlink_return_to_launch_t* return_to_launch)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    return_to_launch->target_system = mavlink_msg_return_to_launch_get_target_system(msg);
    return_to_launch->target_component = mavlink_msg_return_to_launch_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN? msg->len : MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN;
        memset(return_to_launch, 0, MAVLINK_MSG_ID_RETURN_TO_LAUNCH_LEN);
    memcpy(return_to_launch, _MAV_PAYLOAD(msg), len);
#endif
}
