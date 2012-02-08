#include "client.hpp"

#include <limits.h>
#include <ctype.h>

/* ChatMessage */

void ChatMessage::set_name()
{
    if (sender == CHAT_SENDER_SYSTEM)
        strcpy(name, (char*)"System");
    else
    {
        Agent_state* a = ClientState::agent_list.get(sender);
        if (a==NULL || !a->status.identified)
            strcpy(name, (char*)"UNKNOWN");
        else
            strcpy(name, a->status.name);
    }
}

void ChatMessage::set_color()
{
    if (sender == CHAT_SENDER_SYSTEM)
    {   // system msg
        r = CHAT_SYSTEM_COLOR_R;
        g = CHAT_SYSTEM_COLOR_G;
        b = CHAT_SYSTEM_COLOR_B;
        return;
    }
    else if (
        ClientState::playerAgent_state.you != NULL
     && ClientState::playerAgent_state.agent_id + CHANNEL_ID_AGENT_OFFSET == channel
    )
    {   // PM
        r = CHAT_PM_COLOR_R;
        g = CHAT_PM_COLOR_G;
        b = CHAT_PM_COLOR_B;
        return;
    }
    else if (channel >= CHANNEL_ID_TEAM_OFFSET && channel < (int)(CHANNEL_ID_TEAM_OFFSET+N_TEAMS))
    {   // team
        ClientState::ctf.get_team_color(channel - CHANNEL_ID_TEAM_OFFSET + 1, &r, &g, &b);
        return;
    }
    else
    {   // global
        r = CHAT_GLOBAL_COLOR_R;
        g = CHAT_GLOBAL_COLOR_G;
        b = CHAT_GLOBAL_COLOR_B;
        return;
    }

    r = CHAT_UNKNOWN_COLOR_R;
    g = CHAT_UNKNOWN_COLOR_G;
    b = CHAT_UNKNOWN_COLOR_B;

}

ChatMessage::ChatMessage(int id)
:
id(id),
timestamp(0),
r(255),g(255),b(255)
{}

/* ChatMessageHistoryObject */

ChatMessageHistoryObject::ChatMessageHistoryObject(ChatMessage* m)
:
m(m),
next(NULL),
prev(NULL)
{}

ChatMessageHistoryObject::~ChatMessageHistoryObject()
{
    chat_message_list.destroy(m->id);
}

/* ChatClientChannel */

void ChatClientChannel::add_message(ChatMessage* m)
{
    ChatMessageHistoryObject* in = new ChatMessageHistoryObject(m);
        
    if (history_size == CHAT_CLIENT_MESSAGE_HISTORY_MAX)
    {
        // remove back
        if (this->tail == NULL)
            printf("WARNING ChatInput::add_message -- tail is NULL but history full\n");
            
        ChatMessageHistoryObject* delete_me = this->tail;
        this->tail = delete_me->next;
        this->tail->prev = NULL;
        delete delete_me;
        history_size--;
    }

    // append to front
    if (this->history == NULL)
        this->history = in;
    else
    {
        in->prev = this->history;
        this->history->next = in;
        if (this->tail == NULL)
            this->tail = this->history;
        this->history = in;
    }
    history_size++;
}

void ChatClientChannel::clear_history()
{
    if (history == NULL) return;
    int freed = 0;

    while (history != NULL)
    {
        ChatMessageHistoryObject* delete_me = history;
        history = delete_me->prev;
        delete delete_me;
        freed++;
    }
    if (freed != history_size)
        printf("WARNING ~ChatClientChannel -- freed %d but history_size=%d\n", freed, history_size);
    this->history = NULL;
}

ChatClientChannel::ChatClientChannel()
:
id(-1),
history_size(0),
history(NULL),
tail(NULL)
{}

ChatClientChannel::~ChatClientChannel()
{
    this->clear_history();
}


/* ChatInput */

ChatInputHistoryObject::ChatInputHistoryObject(char* m)
:
next(NULL),
prev(NULL)
{
    this->m = (char*)malloc(sizeof(char) * (strlen(m)+1));
    strcpy(this->m, m);
}

ChatInputHistoryObject::~ChatInputHistoryObject()
{
    free(this->m);
}

void ChatInput::clear_history()
{
    while (history != NULL)
    {
        ChatInputHistoryObject* delete_me = history;
        history = delete_me->prev;
        delete delete_me;
    }
    history = NULL;
    history_tail = NULL;
    history_size = 0;
}

void ChatInput::add_to_history(char *s)
{
    ChatInputHistoryObject* in = new ChatInputHistoryObject(s);

    if (history_size >= CHAT_CLIENT_INPUT_HISTORY_MAX)
    {   // remove tail
        if (this->history_tail == NULL)
            printf("WARNING ChatInput::add_to_history -- tail is NULL but history full\n");

        ChatInputHistoryObject* delete_me = this->history_tail;
        this->history_tail = delete_me->next;
        this->history_tail->prev = NULL;
        delete delete_me;
        history_size--;
    }

    // add to front
    if (this->history == NULL)
        this->history = in;
    else
    {
        in->prev = this->history;
        this->history->next = in;
        if (this->history_tail == NULL)
            this->history_tail = in;
        this->history = in;
    }
    history_size++;
}

void ChatInput::submit(int channel)
{
    if (!buffer_len) return;

    if (!route_command())
    {   // create, send packet
        ChatMessage_CtoS msg;
        strcpy(msg.msg, buffer);
        msg.channel = channel;
        msg.send();
    }
    
    // add to history
    this->add_to_history(buffer);

    // reset buffer
    this->clear_buffer();
}

void ChatInput::clear_buffer()
{
    buffer[0] = '\0';
    buffer_len = 0;
    cursor = 0;
    history_index = -1;
}

void ChatInput::add(char x)
{
    if (cursor == CHAT_BUFFER_SIZE) return;
    if (buffer_len == CHAT_BUFFER_SIZE) return;

    for (int i=buffer_len; i>=cursor; i--)
        buffer[i+1] = buffer[i];

    buffer[cursor++] = x;
    buffer[++buffer_len] = '\0';
}

void ChatInput::remove()
{
    if (!buffer_len || !cursor) return;

    int i=cursor;
    char c;
    while ((c = buffer[i++]) != '\0')
        buffer[i-2] = c;
        
    buffer[--buffer_len] = '\0';
    cursor--;
}

void ChatInput::cursor_left()
{
    cursor--;
    cursor = (cursor < 0) ? 0 : cursor;
}

void ChatInput::cursor_right()
{
    cursor++;
    cursor = (cursor > buffer_len) ? buffer_len : cursor;
}

void ChatInput::history_newer()
{
    if (history < 0 || history_index < 0) return;

    history_index--;
    history_index = (history_index < -1) ? -1 : history_index;
    if (history_index < 0)
    {
        clear_buffer();
        return;
    }

    ChatInputHistoryObject* o = history;
    for (int i=0; i<history_index; i++)
    {
        o = o->prev;
    }
    strcpy(buffer, o->m);
    buffer_len = strlen(buffer);
    cursor = buffer_len;
}

void ChatInput::history_older()
{
    if (history==NULL) return;
    if (!history_size) return;
    
    history_index++;
    history_index = (history_index > history_size-1) ? history_size-1 : history_index;

    ChatInputHistoryObject* o = history;
    for (int i=0; i<history_index; i++)
    {
        o = o->prev;
    }
    strcpy(buffer, o->m);
    buffer_len = strlen(buffer);
    cursor = buffer_len;
}

bool ChatInput::route_command()
{
    if (!buffer_len) return false;
    if (this->buffer[0] != '/' || this->buffer[1] == '\0' || isspace(this->buffer[1]))
        return false;

    char cmd[CHAT_BUFFER_SIZE] = {'\0'};
    char c;
    int i=1;
    while((c = this->buffer[i++]) != '\0' && !isspace(c))
    {
        cmd[i-2] = c;
    }

    if (!strcmp(cmd, (char*)"team"))
    {
        if (buffer_len <= (strlen((char*)"/team "))) return true;
        char team_str[2];
        team_str[1] = '\0';
        team_str[0] = buffer[i];
        int team;
        if (team_str[0] == '0') //atoi returns 0 for failure, so check this case
            team = 0;
        else
        {
            team = atoi(team_str);
            if (!team) return true; // failure
        }
        ClientState::ctf.join_team(team);
    }
    else
    if (!strcmp(cmd, (char*)"name"))
    {
        if (buffer_len <= (strlen((char*)"/name "))) return true;
        char name[PLAYER_NAME_MAX_LENGTH+1] = {'\0'};
        int j = 0;
        while ((c = buffer[i++]) != '\0' && !isspace(c))
        {
            name[j++] = c;
            if (j==(int)PLAYER_NAME_MAX_LENGTH) break;
        }
        ClientState::send_identify_packet(name);
    }

    return true;
}

ChatInput::ChatInput()
:
history(NULL),
history_tail(NULL),
history_size(0),
history_index(-1),
buffer_len(0),
cursor(0)
{
    this->buffer = (char*)malloc(sizeof(char)*(CHAT_BUFFER_SIZE+1));
    this->buffer[CHAT_BUFFER_SIZE] = '\0';
    this->buffer[0] = '\0';
}

ChatInput::~ChatInput()
{
    this->clear_history();
    free(this->buffer);
}


/* ChatClient */

void ChatClient::received_message(int channel, int sender, char* payload)
{
    ChatClientChannel* chan = NULL;

    for (int i=0; i<CHAT_CLIENT_CHANNELS_MAX; i++)
    {
        if (this->channels[i]->id == channel)
        {
            chan  = this->channels[i];
            break;
        }
    }

    if (chan == NULL)
    {
        printf("ChatClient::add_message -- unknown message channel %d\n", channel);
        return;
    }

    ChatMessage* m = chat_message_list.create();
    m->sender = sender;
    m->channel = channel;
    strcpy(m->payload, payload);
    m->set_color();
    m->set_name();
    m->timestamp = _GET_MS_TIME();
    chan->add_message(m);
}

void ChatClient::subscribe_channels()
{   // call after playerAgent_state has been assigned by server
    if (ClientState::playerAgent_state.you == NULL)
    {
        printf("ChatClient::init -- playerAgent_state.you is NULL, abort\n");
        return;
    }
    ChatClientChannel* chan;
    for (int i=0; i<CHAT_CLIENT_CHANNELS_MAX; i++)
    {
        if (this->channels[i] == NULL)
            this->channels[i] = new ChatClientChannel();

        chan = this->channels[i];
        chan->type = (ChannelTypes)i;
        switch (chan->type)
        {
            case CHANNEL_SYSTEM:
                chan->id = 0;
                break;
            case CHANNEL_GLOBAL:
                chan->id = 1;
                break;
            case CHANNEL_PRIVATE:
                chan->id = ClientState::playerAgent_state.agent_id + CHANNEL_ID_AGENT_OFFSET;
                break;
            case CHANNEL_TEAM:
                chan->id = ClientState::playerAgent_state.you->status.team + CHANNEL_ID_TEAM_OFFSET - 1;
                break;
            default: break;
        }
    }
}

void ChatClient::submit()
{
    this->input.submit(this->channel);
}

void ChatClient::teardown()
{
    this->input.clear_buffer();
    this->input.clear_history();
    for (int i=0; i<CHAT_CLIENT_CHANNELS_MAX; i++)
        if (this->channels[i] != NULL)
            this->channels[i]->clear_history();
}

void ChatClient::use_team_channel()
{
    this->channel = 1;
    if (ClientState::playerAgent_state.you == NULL) return;
    if (ClientState::playerAgent_state.you->status.team == 0) return;
    this->channel = ClientState::playerAgent_state.you->status.team + CHANNEL_ID_TEAM_OFFSET - 1;
}

void ChatClient::use_global_channel()
{
    this->channel = 1;
}


ChatClient::ChatClient()
:
channel(1)
{
    this->channels = (ChatClientChannel**)malloc(sizeof(ChatClientChannel*)*CHAT_CLIENT_CHANNELS_MAX);
    for (int i=0; i<CHAT_CLIENT_CHANNELS_MAX; channels[i++] = NULL);
}

ChatClient::~ChatClient()
{
    if (this->channels != NULL)
    {
        for (int i=0; i<CHAT_CLIENT_CHANNELS_MAX; i++)
        {
            if (this->channels[i] == NULL) continue;
            delete this->channels[i];
        }
        free(this->channels);
    }
}

/* ChatMessageList */

void ChatMessageList::quicksort_timestamp_desc(int beg, int end)
{
    if (end > beg + 1)
    {
        int timestamp;
        if (this->filtered_objects[beg] == NULL)
            timestamp = INT_MIN;
        else
            timestamp = this->filtered_objects[beg]->timestamp;
        int l = beg + 1, r = end;
        while (l < r)
        {
            int t;
            t = (this->filtered_objects[l] == NULL) ? INT_MIN : this->filtered_objects[l]->timestamp;
            if (t >= timestamp)
                l++;
            else
                swap_object_state(&this->filtered_objects[l], &this->filtered_objects[--r]);
        }
        swap_object_state(&this->filtered_objects[--l], &this->filtered_objects[beg]);
        quicksort_timestamp_desc(beg, l);
        quicksort_timestamp_desc(r, end);
    }
}

void ChatMessageList::quicksort_timestamp_asc(int beg, int end)
{
    if (end > beg + 1)
    {
        int timestamp;
        if (this->filtered_objects[beg] == NULL)
            timestamp = INT_MAX;
        else
            timestamp = this->filtered_objects[beg]->timestamp;
        int l = beg + 1, r = end;
        while (l < r)
        {
            int t;
            t = (this->filtered_objects[l] == NULL) ? INT_MAX : this->filtered_objects[l]->timestamp;
            if (t <= timestamp)
                l++;
            else
                swap_object_state(&this->filtered_objects[l], &this->filtered_objects[--r]);
        }
        swap_object_state(&this->filtered_objects[--l], &this->filtered_objects[beg]);
        quicksort_timestamp_asc(beg, l);
        quicksort_timestamp_asc(r, end);
    }
}

void ChatMessageList::sort_by_most_recent()
{
    this->filter_none();
    this->quicksort_timestamp_desc(0, this->n_filtered);
}

ChatClient chat_client;
ChatMessageList chat_message_list;
