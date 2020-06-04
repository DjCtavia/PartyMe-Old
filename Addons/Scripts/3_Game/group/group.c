/**
 * Handle group members informations and
 * principals informations for group logic.
*/
class PM_Group
{
    bool isLeader = true;
    ref map<string, ref PM_Player_Infos_t> players;

    void PM_Group()
    {
        players = new map<string, ref PM_Player_Infos_t>;
        AddEvents();
    }

    void AddEvents()
    {
        PM_GetEvents().AddEvent("PlayerJoinGroup", this);
        PM_GetEvents().AddEvent("PlayerLeaveGroup", this);
        PM_GetEvents().AddEvent("GroupDestroyed", this);
		PM_GetEvents().AddEvent("UpdatePlayerPosition", this);
    }

    // Return if player is in group
    bool IsInGroup()
    {
		if (!isLeader || players.Count() > 0)
		{
			return true;
		}
        return true;
    }

    // Return if player is group leader
    bool IsLeader()
    {
        return isLeader;
    }

    /*
        Description:
            Calculate max number of player allowed in a party,
            and determine if the player is lead of group for inviting
            a new player.
    */
    bool CanInvite()
    {
        return isLeader;
    }

    // Add new player to group
    bool AddPlayer(string id, string name, vector position, float health)
    {
        Print("[PartyMe][AddPlayer] " + players.ToString());
        if (players && !players.Contains(id))
        {
            ref PM_Player_Infos_t member = new PM_Player_Infos_t();
            member.name = name;
            member.id = id;
            member.position = position;
            member.health = health;
            member.w_marker = new PM_Marker_Widget();
            member.isLocal = true;
            players.Set(id, member);
            Print("[PartyMe] Joueur crée" + players.ToString());
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
                playerInfos.position = position;
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
        if (!IsInGroup())
            return;
        isLeader = true;
        players.Clear();
    }
	
	//-------------------------------------------------------------------------- Getters
	array<PM_Player_Infos_t> GetPlayers()
	{
		return players.GetValueArray();
	}

    //-------------------------------------------------------------------------- Events
    void OnPlayerJoinGroup(PM_Event_Params eventParams)
    {
        AddPlayer(eventParams.playerIdFrom, eventParams.name, eventParams.position, eventParams.health);
    }

    void OnPlayerLeaveGroup(PM_Event_Params eventParams)
    {
        RemovePlayer(eventParams.playerIdFrom);
    }

    void OnGroupDestroyed(PM_Event_Params eventParams)
    {
        LeaveGroup();
    }
	
	void OnUpdatePlayerPosition(PM_Event_Params eventParams)
    {
		UpdatePlayerPosition(eventParams.playerIdFrom, eventParams.position);
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