#include "_interface.hpp"

#include <common/common.hpp>
#include <auth/packets.hpp>

#if DC_CLIENT
#include <auth/client.hpp>
#endif
#if DC_SERVER
#include <auth/server.hpp>
#endif

namespace Auth
{

void init()
{
    auth_token_CtoS::register_server_packet();

    #if DC_CLIENT
    client_init();
    #endif
    #if DC_SERVER
    server_init();
    #endif    
}

void teardown()
{
    #if DC_CLIENT
    client_teardown();
    #endif
    #if DC_SERVER
    server_teardown();
    #endif
}

bool parse_auth_token(const char* token, int* user_id, time_t* expiration_time, char** hash)
{
    // copy token into modifiable buffer
    char* _token = (char*)malloc((AUTH_TOKEN_LENGTH+1)*sizeof(char));
    strncpy(_token, token, AUTH_TOKEN_LENGTH);
    _token[AUTH_TOKEN_LENGTH] = '\0';

    // replace delimiters with NUL
    int delims = 0;
    unsigned int len = 0;
    for (unsigned int i=0; i<AUTH_TOKEN_LENGTH; i++)
    {
        if (_token[i] == '|')
        {
            len = 0;
            _token[i] = '\0';
            delims++;
        }
        else
        {
            len++;
            if ((delims == 0 && len > AUTH_TOKEN_ID_LENGTH) ||
                (delims == 1 && len > AUTH_TOKEN_TIMESTAMP_LENGTH) ||
                (delims == 2 && len > AUTH_TOKEN_HASH_LENGTH))
            {
                free(_token);
                return false;
            }
        }
    }

    // should be 2 delimiters
    GS_ASSERT(delims == 2);
    if (delims != 2)
    {
        free(_token);
        return false;
    }

    // convert user id to integer
    *user_id = atoi(_token);
    GS_ASSERT(user_id > 0);
    if (user_id <= 0)
    {
        free(_token);
        return false;
    }

    // convert expiration time to time_t
    *expiration_time = atott(&token[AUTH_TOKEN_ID_LENGTH + 1]);

    // copy hash
    const char* phash = &token[AUTH_TOKEN_ID_LENGTH + 1 + AUTH_TOKEN_TIMESTAMP_LENGTH + 1]; 
    size_t hash_len = strlen(phash);
    GS_ASSERT(hash_len == AUTH_TOKEN_HASH_LENGTH);
    if (hash_len != AUTH_TOKEN_HASH_LENGTH) return false;
    char* _hash = (char*)malloc((AUTH_TOKEN_HASH_LENGTH+1)*sizeof(char));
    strncpy(_hash, phash, AUTH_TOKEN_HASH_LENGTH);
    _hash[AUTH_TOKEN_HASH_LENGTH] = '\0';
    *hash = _hash;

    free(_token);
    return true;
}

bool auth_token_expired(const time_t timestamp, const time_t expiration_window)
{   // check if token needs to be refreshed
    if (timestamp < AUTH_TOKEN_LIFETIME) return true;   // bad data

    time_t now = utc_now();
    time_t created_at = timestamp - AUTH_TOKEN_LIFETIME;
    double dt = difftime(now, created_at);
    return (dt < 0 || dt >= (double)expiration_window);
}

}   // Auth
