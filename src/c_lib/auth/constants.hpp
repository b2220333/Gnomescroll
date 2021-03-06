/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

#include <agent/constants.hpp>

namespace Auth
{

typedef enum
{
    AUTH_ERROR_NONE = 0,            // no error
    AUTH_ERROR_TOKEN_MISSING,   // token was NULL, did not exist
    AUTH_ERROR_TOKEN_MALFORMED, // token failed to parse
    AUTH_ERROR_TOKEN_EXPIRED,   // token had expired
} AuthError;

/* Example auth token:
 * 000000001|1346898331|b2386482b355bd4d5c6cddf01853057f142639f59f994e454ddf2057d87666fb
 *
 * | is the delimiter
 * First piece is user id, formatted to 9 digits
 * Second piece is a unix timestamp representing expiration time (UTC) (always 10 digits)
 * Third piece is the auth token    (always 64 chars)
 *
 * + 2 chars for delimiting
 */

// Switch to gnomescroll.com when site is live

#if PRODUCTION || USE_LIVE_AUTH
# define GNOMESCROLL_DOMAIN "gnomescroll.com"
# define GNOMESCROLL_URL "http://" GNOMESCROLL_DOMAIN
# define GNOMESCROLL_COOKIE_DOMAIN ".gnomescroll.com"    // WARNING -- might have to remove the beginning '.'
#else
# define GNOMESCROLL_DOMAIN "127.0.0.1:5002"
# define GNOMESCROLL_URL "http://" GNOMESCROLL_DOMAIN
# define GNOMESCROLL_COOKIE_DOMAIN "127.0.0.1"
#endif

#define GNOMESCROLL_LOGIN_HTML MEDIA_PATH "web/html/login.html"
#define GNOMESCROLL_SETTINGS_HTML MEDIA_PATH "web/html/settings.html"

#define GNOMESCROLL_CREATE_PATH "/create"
#define GNOMESCROLL_LOGIN_PATH  "/login"
#define GNOMESCROLL_TOKEN_PATH  "/token"
#define GNOMESCROLL_VERSION_PATH "/version"

const char SERVER_FAILURE_MESSAGE[] = "Authentication server failure. Try again soon.";

#define GNOMESCROLL_CREATE_URL GNOMESCROLL_URL GNOMESCROLL_CREATE_PATH
#define GNOMESCROLL_LOGIN_URL  GNOMESCROLL_URL GNOMESCROLL_LOGIN_PATH
#define GNOMESCROLL_TOKEN_URL  GNOMESCROLL_URL GNOMESCROLL_TOKEN_PATH

const int AUTH_TOKEN_RETRY_WAIT = 15 * 30;   // wait 15 seconds between each attempt to get a new token
const int AUTH_MAX_CLIENT_ATTEMPTS = 100;

#if PRODUCTION
const char AUTH_TOKEN_LOCAL_COOKIE_NAME[] = "gstoken";    // name of the cookie value saved locally
#else
const char AUTH_TOKEN_LOCAL_COOKIE_NAME[] = "dbggstoken";    // name of the cookie value saved locally
#endif
const char AUTH_TOKEN_COOKIE_NAME[] = "gstoken";    // name of the cookie value set by the auth server
const char AUTH_TOKEN_NAME[] = "gstoken";    // name of the json field
const char AUTH_TOKEN_DELIMITER = '|';  // delimiter for components of the token

const char UNDEFINED_NAME[] = "undefined-name";

const time_t AUTH_TOKEN_LIFETIME = 60 * 60; // 1 hour, in seconds
const time_t AUTHORIZATION_TIMEOUT = 60;   // 1 minute to authorize before being disconnected

const unsigned int AUTH_TOKEN_ID_LENGTH = 9;
const unsigned int AUTH_TOKEN_TIMESTAMP_LENGTH = 10;
const unsigned int AUTH_TOKEN_HASH_LENGTH = 64;
const unsigned int AUTH_TOKEN_USERNAME_MIN_LENGTH = PLAYER_NAME_MIN_LENGTH;
const unsigned int AUTH_TOKEN_USERNAME_MAX_LENGTH = PLAYER_NAME_MAX_LENGTH;
const unsigned int AUTH_TOKEN_PIECES = 4;
const unsigned int AUTH_TOKEN_LENGTH =
    AUTH_TOKEN_ID_LENGTH + AUTH_TOKEN_TIMESTAMP_LENGTH +
    AUTH_TOKEN_HASH_LENGTH + AUTH_TOKEN_USERNAME_MAX_LENGTH +
    AUTH_TOKEN_PIECES - 1;

}   // Auth
