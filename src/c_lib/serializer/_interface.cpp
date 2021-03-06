/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#include "_interface.hpp"

#include <common/files.hpp>
#include <options/server_options.hpp>
#include <serializer/mechs.hpp>
#include <chat/_interface.hpp>

#if GS_SERIALIZER
# include <serializer/logger.hpp>
# include <serializer/constants.hpp>
# include <serializer/containers.hpp>
# include <serializer/entities.hpp>
# include <serializer/map.hpp>
# include <serializer/redis.hpp>
# include <serializer/items.hpp>
#endif

namespace serializer
{

static bool world_found = false;

static char save_folder[SAVE_FOLDER_LEN+1];

char map_folder[GS_FN_MAX+1];
char mech_folder[GS_FN_MAX+1];
char player_folder[GS_FN_MAX+1];
char container_folder[GS_FN_MAX+1];
char entity_folder[GS_FN_MAX+1];

char map_path[GS_FN_MAX+1];
char map_path_tmp[GS_FN_MAX+1];
char map_path_bak[GS_FN_MAX+1];
char map_palette_path[GS_FN_MAX+1];
char map_palette_path_tmp[GS_FN_MAX+1];
char map_palette_path_bak[GS_FN_MAX+1];
char mech_path[GS_FN_MAX+1];
char mech_path_tmp[GS_FN_MAX+1];
char mech_path_bak[GS_FN_MAX+1];
char mech_palette_path[GS_FN_MAX+1];
char mech_palette_path_tmp[GS_FN_MAX+1];
char mech_palette_path_bak[GS_FN_MAX+1];
char player_path[GS_FN_MAX+1];
char player_path_tmp[GS_FN_MAX+1];
char player_path_bak[GS_FN_MAX+1];
char container_path[GS_FN_MAX+1];
char container_path_tmp[GS_FN_MAX+1];
char container_path_bak[GS_FN_MAX+1];
char entity_path[GS_FN_MAX+1];
char entity_path_tmp[GS_FN_MAX+1];
char entity_path_bak[GS_FN_MAX+1];

static bool set_save_folder(int version, time_t timestamp)
{
    int wrote = snprintf(save_folder, SAVE_FOLDER_LEN+1, save_folder_fmt, GS_VERSION, timestamp);
    IF_ASSERT(wrote <= 0 || (size_t)wrote > SAVE_FOLDER_LEN) return false;
    save_folder[SAVE_FOLDER_LEN] = '\0';

    char* save_path = (char*)malloc((sizeof(WORLD_DATA_PATH) + strlen(save_folder)+1) * sizeof(char));
    sprintf(save_path, "%s%s/", WORLD_DATA_PATH, save_folder);
    create_path(save_path);
    free(save_path);

    return true;
}

static void set_data_paths(const char* save_folder)
{
    IF_ASSERT(save_folder[0] == '\0') return;

    int wrote = 0;

    // map
    // set & create folder
    wrote = snprintf(map_folder, GS_FN_MAX+1, "%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_folder[GS_FN_MAX] = '\0';

    // set full paths for filenames
    wrote = snprintf(map_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH, MAP_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_path[GS_FN_MAX] = '\0';
    wrote = snprintf(map_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH, MAP_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(map_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH, MAP_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_path_bak[GS_FN_MAX] = '\0';

    wrote = snprintf(map_palette_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH, MAP_PALETTE_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_palette_path[GS_FN_MAX] = '\0';
    wrote = snprintf(map_palette_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH, MAP_PALETTE_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_palette_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(map_palette_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MAP_DATA_PATH, MAP_PALETTE_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    map_palette_path_bak[GS_FN_MAX] = '\0';

    // mechs
    wrote = snprintf(mech_folder, GS_FN_MAX+1, "%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_folder[GS_FN_MAX] = '\0';

    wrote = snprintf(mech_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH, MECH_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_path[GS_FN_MAX] = '\0';
    wrote = snprintf(mech_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH, MECH_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(mech_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH, MECH_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_path_bak[GS_FN_MAX] = '\0';

    wrote = snprintf(mech_palette_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH, MECH_PALETTE_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_palette_path[GS_FN_MAX] = '\0';
    wrote = snprintf(mech_palette_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH, MECH_PALETTE_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_palette_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(mech_palette_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, MECH_DATA_PATH, MECH_PALETTE_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    mech_palette_path_bak[GS_FN_MAX] = '\0';

    // players
    wrote = snprintf(player_folder, GS_FN_MAX+1, "%s%s%s", WORLD_DATA_PATH, save_folder, PLAYER_DATA_PATH);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    player_folder[GS_FN_MAX] = '\0';

    wrote = snprintf(player_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, PLAYER_DATA_PATH, PLAYER_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    player_path[GS_FN_MAX] = '\0';
    wrote = snprintf(player_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, PLAYER_DATA_PATH, PLAYER_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    player_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(player_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, PLAYER_DATA_PATH, PLAYER_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    player_path_bak[GS_FN_MAX] = '\0';

    // containers
    wrote = snprintf(container_folder, GS_FN_MAX+1, "%s%s%s", WORLD_DATA_PATH, save_folder, CONTAINER_DATA_PATH);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    container_folder[GS_FN_MAX] = '\0';

    wrote = snprintf(container_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, CONTAINER_DATA_PATH, CONTAINER_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    container_path[GS_FN_MAX] = '\0';
    wrote = snprintf(container_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, CONTAINER_DATA_PATH, CONTAINER_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    container_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(container_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, CONTAINER_DATA_PATH, CONTAINER_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    container_path_bak[GS_FN_MAX] = '\0';

    // entities
    wrote = snprintf(entity_folder, GS_FN_MAX+1, "%s%s%s", WORLD_DATA_PATH, save_folder, ENTITY_DATA_PATH);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    entity_folder[GS_FN_MAX] = '\0';

    wrote = snprintf(entity_path, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, ENTITY_DATA_PATH, ENTITY_FILENAME);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    entity_path[GS_FN_MAX] = '\0';
    wrote = snprintf(entity_path_tmp, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, ENTITY_DATA_PATH, ENTITY_FILENAME_TMP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    entity_path_tmp[GS_FN_MAX] = '\0';
    wrote = snprintf(entity_path_bak, GS_FN_MAX+1, "%s%s%s%s", WORLD_DATA_PATH, save_folder, ENTITY_DATA_PATH, ENTITY_FILENAME_BACKUP);
    GS_ASSERT_ABORT(wrote <= GS_FN_MAX);
    entity_path_bak[GS_FN_MAX] = '\0';

    set_log_paths(save_folder);
}

static void create_data_paths()
{
    create_path(map_folder);
    create_path(mech_folder);
    create_path(player_folder);
    create_path(entity_folder);
    create_path(container_folder);
}

bool begin_new_world_version()
{
    bool set = set_save_folder(GS_VERSION, utc_now());
    IF_ASSERT(!set) return false;
    set_data_paths(save_folder);
    return true;
}

bool load_data()
{
    if (!world_found) return false;

    if (!load_map()) return false;
    if (!load_mechs()) return false;

    if (!Options::serializer) return true;

    #if GS_SERIALIZER
    if (!load_containers()) return false;
    if (!load_entities()) return false;
    //if (!load_players()) return false;
    #endif

    return true;
}

bool save_palettes()
{
    bool mapps = save_map_palette_file();
    bool mechps = save_mech_palette_file();
    return (mapps && mechps);
}

bool save_remaining_data()
{
    bool ms = save_mechs();
    GS_ASSERT(ms);

    bool rps = true;
    bool cs = true;
    bool es = true;

    #if GS_SERIALIZER
    if (Options::serializer)
    {
        rps = save_remote_player_data();
        cs = save_containers();
        es = save_entities();
        GS_ASSERT(rps);
        GS_ASSERT(cs);
        GS_ASSERT(es);
    }
    #endif

    if (!rps || !ms || !cs || !es) return false; // ERROR

    ms = true;
    cs = true;
    es = true;

    // copy all tmp files over
    ms = save_tmp_file(mech_path, mech_path_tmp, mech_path_bak);
    GS_ASSERT(ms);

    #if GS_SERIALIZER
    if (Options::serializer)
    {
        cs = save_tmp_file(container_path, container_path_tmp, container_path_bak);
        GS_ASSERT(cs);
        es = save_tmp_file(entity_path, entity_path_tmp, entity_path_bak);
        GS_ASSERT(es);
    }
    #endif

    return (ms && cs && es);
}

bool save_data()
{
    printf("Saving data to %s\n", save_folder);
    Chat::broadcast_server_message("Saving the world...");

    static int paths_created = 0;
    if (!(paths_created++))
        create_data_paths();

    // Save the palettes, but only once
    static bool palettes_saved = false;
    if (!palettes_saved)
    {
        palettes_saved = save_palettes();
        if (!palettes_saved) return false;
        save_tmp_file(map_palette_path, map_palette_path_tmp, map_palette_path_bak);
        save_tmp_file(mech_palette_path, mech_palette_path_tmp, mech_palette_path_bak);
    }

    if (!save_map()) return false;

    #if !GS_SERIALIZER
    return save_remaining_data();
    #endif

    return true;
}

// actually, 3 states:
// not in progress
// in progress, and unfinished
// in progress, and finished

// returns true on completion
WorldSaveState update_save_state(int max_ms)
{
    #if !GS_SERIALIZER
    return WORLD_SAVE_IDLE;
    #else

    if (!map_save_memcpy_in_progress) return WORLD_SAVE_IDLE;
    if (!save_map_iter(max_ms)) return WORLD_SAVE_UNFINISHED;
    save_remaining_data();

    return WORLD_SAVE_FINISHED;
    #endif
}

void wait_for_save_complete()
{
    while (update_save_state(100) == WORLD_SAVE_UNFINISHED);
}

void update()
{
    update_completed_map_save();

    if (!Options::serializer) return;

    #if GS_SERIALIZER
    update_redis();
    #endif
}

static bool parse_world_dirname(const char* dir, int& version, time_t& timestamp)
{
    version = 0;
    timestamp = 0;

    // copy name over
    char _buf[GS_FN_MAX+1];
    strncpy(_buf, dir, GS_FN_MAX+1);
    _buf[GS_FN_MAX] = '\0';

    char* buf = &_buf[0];

    // replace the delimiter with \0, and check the component count
    // the lookback var is to indicated we found the delimiter, and on the next char
    // we should increment the parts count.
    // this prevents part count errors when the string ends with the delimiter
    size_t pts = 1;
    bool lookback = false;
    for (size_t i=0; buf[i] != '\0'; i++)
    {
        if (buf[i] == WORLD_DATA_FOLDER_DELIMITER[0])
            lookback = true;
        if (lookback)
        {
            lookback = false;
            pts++;
            buf[i] = '\0';
        }
    }
    if (lookback) return false;

    if (pts != WORLD_DATA_FOLDER_PARTS) return false;

    // check the prefix
    if (strcmp(buf, WORLD_DATA_PREFIX) != 0) return false;

    // extract and check version
    buf = &buf[sizeof(WORLD_DATA_PREFIX)];
    bool err = false;
    long long int _version = parse_int(buf, err);
    if (err) return false;
    if (_version <= 0 || _version > GS_VERSION) return false;
    version = _version;

    // extract and check timestamp
    buf = &buf[strlen(buf)+1];
    long long int _timestamp = parse_int(buf, err);
    if (err) return false;
    if (_timestamp <= 0 || _timestamp > utc_now()) return false;
    timestamp = _timestamp;

    return true;
}

static bool find_existing_world(char filename[GS_FN_MAX+1], int& version, time_t& timestamp)
{
    DIR* dp;
    struct dirent* ep;

    dp = opendir(WORLD_DATA_PATH);
    if (dp == NULL) return false;

    time_t most_recent = 0;
    int highest_version = 0;

    while ((ep = readdir(dp)) != NULL)
    {
        #ifndef __MINGW32__
        if (ep->d_type != DT_DIR) continue;
        #endif
        bool is_world = parse_world_dirname(ep->d_name, version, timestamp);
        if (is_world && difftime(timestamp, most_recent) >= 0)
        {
            GS_ASSERT(version >= highest_version);
            highest_version = version;
            most_recent = timestamp;
            strncpy(filename, ep->d_name, GS_FN_MAX+1);
            GS_ASSERT(filename[GS_FN_MAX] == '\0')
            filename[GS_FN_MAX] = '\0';
        }
    }
    int ret = closedir(dp);
    GS_ASSERT(ret == 0);

    version = highest_version;
    timestamp = most_recent;

    return (most_recent != 0 && highest_version != 0);
}

void init()
{
    verify_config();

    create_path(WORLD_DATA_PATH);
    init_logger();

    // find the most up to date world data, if present
    int version = 0;
    time_t timestamp = 0;
    char filename[GS_FN_MAX+1] = {'\0'};
    world_found = find_existing_world(filename, version, timestamp);
    if (world_found)
    {   // save the folder name
        printf("Found world: %s\n", filename);
        GS_ASSERT_ABORT(strlen(filename) <= SAVE_FOLDER_LEN);
        set_save_folder(version, timestamp);
        set_data_paths(save_folder);
    }

    init_map_serializer();
    init_state();
    init_mechs();

    if (!Options::serializer) return;

    #if GS_SERIALIZER
    // ORDER DEPENDENT
    init_redis();
    init_items();
    init_players();
    #endif
}

void teardown()
{
    // finish saves in progress
    wait_for_save_complete();
    wait_for_threads();

    // guarantee a final save in production
    #if PRODUCTION
    should_save_world = true;
    #endif

    // guarantee a final save if in serializer mode
    if (Options::serializer) should_save_world = true;

    // final save
    if (should_save_world) save_data();
    wait_for_save_complete();

    // wait for replies/threads
    wait_for_threads();
    update_completed_map_save();
    wait_for_redis_replies();

    // tear it all down

    teardown_map_serializer();
    teardown_mechs();

    #if GS_SERIALIZER
    // ORDER DEPENDENT
    teardown_redis();
    teardown_players();
    teardown_items();
    teardown_state();
    #endif

    teardown_logger();
}

}   // serializer
