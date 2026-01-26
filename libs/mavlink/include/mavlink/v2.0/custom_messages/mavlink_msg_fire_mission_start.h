#pragma once
// MESSAGE FIRE_MISSION_START PACKING

#define MAVLINK_MSG_ID_FIRE_MISSION_START 60000


typedef struct __mavlink_fire_mission_start_t {
 int32_t target_lat; /*< [degE7] Target Latitude (deg * 1E7)*/
 int32_t target_lon; /*< [degE7] Target Longitude (deg * 1E7)*/
 float target_alt; /*< [m] Target Altitude (meters)*/
 uint8_t target_system; /*<  Target System ID*/
 uint8_t target_component; /*<  Target Component ID*/
 uint8_t auto_fire; /*<  Auto fire enabled (0: False, 1: True)*/
 uint8_t max_projectiles; /*<  Maximum number of projectiles*/
} mavlink_fire_mission_start_t;

#define MAVLINK_MSG_ID_FIRE_MISSION_START_LEN 16
#define MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN 16
#define MAVLINK_MSG_ID_60000_LEN 16
#define MAVLINK_MSG_ID_60000_MIN_LEN 16

#define MAVLINK_MSG_ID_FIRE_MISSION_START_CRC 100
#define MAVLINK_MSG_ID_60000_CRC 100



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FIRE_MISSION_START { \
    60000, \
    "FIRE_MISSION_START", \
    7, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_fire_mission_start_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_fire_mission_start_t, target_component) }, \
         { "target_lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_fire_mission_start_t, target_lat) }, \
         { "target_lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_fire_mission_start_t, target_lon) }, \
         { "target_alt", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_fire_mission_start_t, target_alt) }, \
         { "auto_fire", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_fire_mission_start_t, auto_fire) }, \
         { "max_projectiles", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_fire_mission_start_t, max_projectiles) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FIRE_MISSION_START { \
    "FIRE_MISSION_START", \
    7, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_fire_mission_start_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_fire_mission_start_t, target_component) }, \
         { "target_lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_fire_mission_start_t, target_lat) }, \
         { "target_lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_fire_mission_start_t, target_lon) }, \
         { "target_alt", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_fire_mission_start_t, target_alt) }, \
         { "auto_fire", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_fire_mission_start_t, auto_fire) }, \
         { "max_projectiles", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_fire_mission_start_t, max_projectiles) }, \
         } \
}
#endif

/**
 * @brief Pack a fire_mission_start message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @param target_lat [degE7] Target Latitude (deg * 1E7)
 * @param target_lon [degE7] Target Longitude (deg * 1E7)
 * @param target_alt [m] Target Altitude (meters)
 * @param auto_fire  Auto fire enabled (0: False, 1: True)
 * @param max_projectiles  Maximum number of projectiles
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fire_mission_start_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, int32_t target_lat, int32_t target_lon, float target_alt, uint8_t auto_fire, uint8_t max_projectiles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRE_MISSION_START_LEN];
    _mav_put_int32_t(buf, 0, target_lat);
    _mav_put_int32_t(buf, 4, target_lon);
    _mav_put_float(buf, 8, target_alt);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);
    _mav_put_uint8_t(buf, 14, auto_fire);
    _mav_put_uint8_t(buf, 15, max_projectiles);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#else
    mavlink_fire_mission_start_t packet;
    packet.target_lat = target_lat;
    packet.target_lon = target_lon;
    packet.target_alt = target_alt;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.auto_fire = auto_fire;
    packet.max_projectiles = max_projectiles;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FIRE_MISSION_START;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
}

/**
 * @brief Pack a fire_mission_start message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @param target_lat [degE7] Target Latitude (deg * 1E7)
 * @param target_lon [degE7] Target Longitude (deg * 1E7)
 * @param target_alt [m] Target Altitude (meters)
 * @param auto_fire  Auto fire enabled (0: False, 1: True)
 * @param max_projectiles  Maximum number of projectiles
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fire_mission_start_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, int32_t target_lat, int32_t target_lon, float target_alt, uint8_t auto_fire, uint8_t max_projectiles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRE_MISSION_START_LEN];
    _mav_put_int32_t(buf, 0, target_lat);
    _mav_put_int32_t(buf, 4, target_lon);
    _mav_put_float(buf, 8, target_alt);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);
    _mav_put_uint8_t(buf, 14, auto_fire);
    _mav_put_uint8_t(buf, 15, max_projectiles);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#else
    mavlink_fire_mission_start_t packet;
    packet.target_lat = target_lat;
    packet.target_lon = target_lon;
    packet.target_alt = target_alt;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.auto_fire = auto_fire;
    packet.max_projectiles = max_projectiles;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FIRE_MISSION_START;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#endif
}

/**
 * @brief Pack a fire_mission_start message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @param target_lat [degE7] Target Latitude (deg * 1E7)
 * @param target_lon [degE7] Target Longitude (deg * 1E7)
 * @param target_alt [m] Target Altitude (meters)
 * @param auto_fire  Auto fire enabled (0: False, 1: True)
 * @param max_projectiles  Maximum number of projectiles
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_fire_mission_start_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,int32_t target_lat,int32_t target_lon,float target_alt,uint8_t auto_fire,uint8_t max_projectiles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRE_MISSION_START_LEN];
    _mav_put_int32_t(buf, 0, target_lat);
    _mav_put_int32_t(buf, 4, target_lon);
    _mav_put_float(buf, 8, target_alt);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);
    _mav_put_uint8_t(buf, 14, auto_fire);
    _mav_put_uint8_t(buf, 15, max_projectiles);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#else
    mavlink_fire_mission_start_t packet;
    packet.target_lat = target_lat;
    packet.target_lon = target_lon;
    packet.target_alt = target_alt;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.auto_fire = auto_fire;
    packet.max_projectiles = max_projectiles;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FIRE_MISSION_START;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
}

/**
 * @brief Encode a fire_mission_start struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param fire_mission_start C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fire_mission_start_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_fire_mission_start_t* fire_mission_start)
{
    return mavlink_msg_fire_mission_start_pack(system_id, component_id, msg, fire_mission_start->target_system, fire_mission_start->target_component, fire_mission_start->target_lat, fire_mission_start->target_lon, fire_mission_start->target_alt, fire_mission_start->auto_fire, fire_mission_start->max_projectiles);
}

/**
 * @brief Encode a fire_mission_start struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fire_mission_start C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fire_mission_start_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_fire_mission_start_t* fire_mission_start)
{
    return mavlink_msg_fire_mission_start_pack_chan(system_id, component_id, chan, msg, fire_mission_start->target_system, fire_mission_start->target_component, fire_mission_start->target_lat, fire_mission_start->target_lon, fire_mission_start->target_alt, fire_mission_start->auto_fire, fire_mission_start->max_projectiles);
}

/**
 * @brief Encode a fire_mission_start struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param fire_mission_start C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_fire_mission_start_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_fire_mission_start_t* fire_mission_start)
{
    return mavlink_msg_fire_mission_start_pack_status(system_id, component_id, _status, msg,  fire_mission_start->target_system, fire_mission_start->target_component, fire_mission_start->target_lat, fire_mission_start->target_lon, fire_mission_start->target_alt, fire_mission_start->auto_fire, fire_mission_start->max_projectiles);
}

/**
 * @brief Send a fire_mission_start message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system  Target System ID
 * @param target_component  Target Component ID
 * @param target_lat [degE7] Target Latitude (deg * 1E7)
 * @param target_lon [degE7] Target Longitude (deg * 1E7)
 * @param target_alt [m] Target Altitude (meters)
 * @param auto_fire  Auto fire enabled (0: False, 1: True)
 * @param max_projectiles  Maximum number of projectiles
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_fire_mission_start_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, int32_t target_lat, int32_t target_lon, float target_alt, uint8_t auto_fire, uint8_t max_projectiles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FIRE_MISSION_START_LEN];
    _mav_put_int32_t(buf, 0, target_lat);
    _mav_put_int32_t(buf, 4, target_lon);
    _mav_put_float(buf, 8, target_alt);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);
    _mav_put_uint8_t(buf, 14, auto_fire);
    _mav_put_uint8_t(buf, 15, max_projectiles);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRE_MISSION_START, buf, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
#else
    mavlink_fire_mission_start_t packet;
    packet.target_lat = target_lat;
    packet.target_lon = target_lon;
    packet.target_alt = target_alt;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.auto_fire = auto_fire;
    packet.max_projectiles = max_projectiles;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRE_MISSION_START, (const char *)&packet, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
#endif
}

/**
 * @brief Send a fire_mission_start message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_fire_mission_start_send_struct(mavlink_channel_t chan, const mavlink_fire_mission_start_t* fire_mission_start)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_fire_mission_start_send(chan, fire_mission_start->target_system, fire_mission_start->target_component, fire_mission_start->target_lat, fire_mission_start->target_lon, fire_mission_start->target_alt, fire_mission_start->auto_fire, fire_mission_start->max_projectiles);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRE_MISSION_START, (const char *)fire_mission_start, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
#endif
}

#if MAVLINK_MSG_ID_FIRE_MISSION_START_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_fire_mission_start_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, int32_t target_lat, int32_t target_lon, float target_alt, uint8_t auto_fire, uint8_t max_projectiles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, target_lat);
    _mav_put_int32_t(buf, 4, target_lon);
    _mav_put_float(buf, 8, target_alt);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);
    _mav_put_uint8_t(buf, 14, auto_fire);
    _mav_put_uint8_t(buf, 15, max_projectiles);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRE_MISSION_START, buf, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
#else
    mavlink_fire_mission_start_t *packet = (mavlink_fire_mission_start_t *)msgbuf;
    packet->target_lat = target_lat;
    packet->target_lon = target_lon;
    packet->target_alt = target_alt;
    packet->target_system = target_system;
    packet->target_component = target_component;
    packet->auto_fire = auto_fire;
    packet->max_projectiles = max_projectiles;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FIRE_MISSION_START, (const char *)packet, MAVLINK_MSG_ID_FIRE_MISSION_START_MIN_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN, MAVLINK_MSG_ID_FIRE_MISSION_START_CRC);
#endif
}
#endif

#endif

// MESSAGE FIRE_MISSION_START UNPACKING


/**
 * @brief Get field target_system from fire_mission_start message
 *
 * @return  Target System ID
 */
static inline uint8_t mavlink_msg_fire_mission_start_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field target_component from fire_mission_start message
 *
 * @return  Target Component ID
 */
static inline uint8_t mavlink_msg_fire_mission_start_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Get field target_lat from fire_mission_start message
 *
 * @return [degE7] Target Latitude (deg * 1E7)
 */
static inline int32_t mavlink_msg_fire_mission_start_get_target_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field target_lon from fire_mission_start message
 *
 * @return [degE7] Target Longitude (deg * 1E7)
 */
static inline int32_t mavlink_msg_fire_mission_start_get_target_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field target_alt from fire_mission_start message
 *
 * @return [m] Target Altitude (meters)
 */
static inline float mavlink_msg_fire_mission_start_get_target_alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field auto_fire from fire_mission_start message
 *
 * @return  Auto fire enabled (0: False, 1: True)
 */
static inline uint8_t mavlink_msg_fire_mission_start_get_auto_fire(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field max_projectiles from fire_mission_start message
 *
 * @return  Maximum number of projectiles
 */
static inline uint8_t mavlink_msg_fire_mission_start_get_max_projectiles(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  15);
}

/**
 * @brief Decode a fire_mission_start message into a struct
 *
 * @param msg The message to decode
 * @param fire_mission_start C-struct to decode the message contents into
 */
static inline void mavlink_msg_fire_mission_start_decode(const mavlink_message_t* msg, mavlink_fire_mission_start_t* fire_mission_start)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    fire_mission_start->target_lat = mavlink_msg_fire_mission_start_get_target_lat(msg);
    fire_mission_start->target_lon = mavlink_msg_fire_mission_start_get_target_lon(msg);
    fire_mission_start->target_alt = mavlink_msg_fire_mission_start_get_target_alt(msg);
    fire_mission_start->target_system = mavlink_msg_fire_mission_start_get_target_system(msg);
    fire_mission_start->target_component = mavlink_msg_fire_mission_start_get_target_component(msg);
    fire_mission_start->auto_fire = mavlink_msg_fire_mission_start_get_auto_fire(msg);
    fire_mission_start->max_projectiles = mavlink_msg_fire_mission_start_get_max_projectiles(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FIRE_MISSION_START_LEN? msg->len : MAVLINK_MSG_ID_FIRE_MISSION_START_LEN;
        memset(fire_mission_start, 0, MAVLINK_MSG_ID_FIRE_MISSION_START_LEN);
    memcpy(fire_mission_start, _MAV_PAYLOAD(msg), len);
#endif
}
