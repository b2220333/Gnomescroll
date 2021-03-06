/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

namespace Auth
{

bool login(const char* username, const char* password);
bool create_account(const char* username, const char* email, const char* password);
bool check_version();

void init_curl();
void teardown_curl();

#if !GS_AUTH
bool check_version() { return true; }
bool login(const char* username, const char* password) { return true; }
bool create_account(const char* username, const char* email, const char* password) { return true; }
void init_curl() {}
void teardown_curl() {}
#endif

}   // Auth
