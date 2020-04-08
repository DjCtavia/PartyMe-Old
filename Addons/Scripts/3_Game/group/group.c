/**
 * Handle group members informations and
 * principals informations for group logic.
*/
class PM_Group
{
    bool isLeader = true;
    ref map<string, ref PM_Player_Infos_t> players;

    void PM_group()
    {
        players = new map<string, ref PM_Player_Infos_t>;
    }

    // Return if player is in group
    bool IsInGroup()
    {
        if (!players || !players.Count())
            return false;
        return true;
    }

    // Return if player is group leader
    bool IsLeader()
    {
        return isLeader;
    }

    // Add new player to group
    bool AddPlayer(string id, string name, vector position, float health)
    {
        if (players && !players.Contains(id))
        {
            ref PM_Player_Infos_t member = new PM_Player_Infos_t();
            member.name = name;
            member.position = position;
            member.health = health;
            players.Set(id, member);
            return true;
        }
        return false;
    }

    // Update health and position of group member
    void UpdatePlayerHealthAndPosition(string id, float health, vector position)
    {
        ref PM_Player_Infos_t playerInfos;

        if (players)
        {
            playerInfos = players.Get(id);
            if (playerInfos)
            {
                playerInfos.health = health;
                playerInfos.position = position;
            }
        }
    }

    // Update health of group member
    void UpdatePlayerHealth(string id, float health)
    {
        ref PM_Player_Infos_t playerInfos;

        if (players)
        {
            playerInfos = players.Get(id);
            if (playerInfos)
            {
                playerInfos.health = health;
            }
        }
    }

    // Update position of group member
    void UpdatePlayerPosition(string id, vector position)
    {
        ref PM_Player_Infos_t playerInfos;

        if (players)
        {
            playerInfos = players.Get(id);
            if (playerInfos)
            {
                playerInfos.position = position;
            }
        }
    }

    // Remove player from group
    bool RemovePlayer(string id)
    {
        if (players && players.Contains(id))
        {
            players.Remove(id);
            return true;
        }
        return false;
    }

    void LeaveGroup()
    {
        if (players)
        {
            players.Clear();
        }
    }
};

static ref PM_Group g_PM_group;
static ref PM_Group PM_GetGroup()
{
    if (!g_PM_group)
    {
        g_PM_group = new PM_Group();
    }
    return g_PM_group;
}