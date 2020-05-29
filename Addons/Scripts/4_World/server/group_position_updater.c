class PM_Server_Group_Position_Updater
{
    private ScriptCallQueue timer;
    protected ref map<string, ref array<string>> playerAskingPositionIds;

    void PM_Server_Group_Position_Updater()
    {
        playerAskingPositionIds = new map<string, ref array<string>>;
        timer = GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM);
        timer.CallLater(Update, 1250, true);
		AddEvents();
        AddRPCs();
        Print("[PM] PM_Group_Position_Updater_Server created");
    }

    void ~PM_Server_Group_Position_Updater()
    {
        Print("[PM] PM_Group_Position_Updater_Server destroyed");
    }
	
	private void AddEvents()
	{
		PM_GetEvents().AddEvent("PlayerDisconnect", this);
	}

    // RPC section
    private void AddRPCs()
    {
        GetRPCManager().AddRPC("PartyMe", "AskPlayerPositionUpdate", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "RemovePlayerPositionUpdate", this, SingleplayerExecutionType.Both);
    }

    void AskPlayerPositionUpdate(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if ( !ctx.Read( data ) ) return;

        AskPositionUpdate(data.param1, data.param2);
    }

    void RemovePlayerPositionUpdate(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if ( !ctx.Read( data ) ) return;

        RemovePositionUpdate(data.param1, data.param2);
    }

    // Utility section
    protected bool AskPositionUpdate(string playerAskingPositionId, string otherId)
    {
        ref array<string> players;

        if (IsAskingPosition(playerAskingPositionId, otherId))
            return false;
        if (!playerAskingPositionIds.Get(otherId))
        {
            players = new array<string>;
            playerAskingPositionIds.Set(otherId, players);
        }
        players.Insert(playerAskingPositionId);
        return true;
    }

    protected bool RemovePositionUpdate(string playerAskingPositionId, string otherId)
    {
        ref array<string> players = playerAskingPositionIds.Get(otherId);
        
		if (players)
		{
            players.RemoveItem(playerAskingPositionId);
            if (players.Count() == 0)
                playerAskingPositionIds.Remove(otherId);
            return true;
        }
        return false;
    }

    bool IsAskingPosition(string playerAskingPositionId, string otherId)
    {
        ref array<string> players = playerAskingPositionIds.Get(otherId);

        if (players && players.Find(playerAskingPositionId) != -1)
            return true;
        return false;
    }

    // Update section
    protected void Update()
    {
        ref PM_Group_Manager groupManager = PM_GetGroupManager();
        array<string> groups = groupManager.GetAllGroups();

        foreach (string groupId : groups)
        {
            SendPositionsToGroup(groupManager, groupId);
        }
    }

    protected void SendPositionsToGroup(ref PM_Group_Manager groupManager, string groupId)
    {
        ref array<string> members = groupManager.GetGroupMembers(groupId);

        foreach (string playerDataId : members)
        {
            vector position = "0 0 0";
            PlayerBase player = groupManager.GetPlayerEntity(playerDataId);
            ref array<string> playersToSendPosition = playerAskingPositionIds.Get(playerDataId);

            if (player)
                position = player.GetPosition();
            foreach (string playerId : members)
            {
                if (playerDataId == playerId || !playersToSendPosition || playersToSendPosition.Find(playerId) == -1)
                    continue;
                PlayerIdentity identity = groupManager.GetPlayerIdentity(playerId);

                SendPositionRPC(identity, playerDataId, position);
            }
        }
    }

    protected void SendPositionRPC(PlayerIdentity identity, string playerDataId, vector position)
    {
		if (identity)
		{
        	Print("[PM] Position of " + playerDataId + " has been sent through RPC to " + identity.GetId());
        	GetRPCManager().SendRPC("PartyMe", "UpdatePlayerPositionNetwork", new Param2<string, vector>(playerDataId, position), false, identity);
		}
    }
	
	//-------------------------------------------------------------------------- Events
	void OnPlayerDisconnect(ref PM_Event_Params eventParams)
	{
		array<string> players = playerAskingPositionIds.GetKeyArray();
		ref array<string> askers;
		
		foreach (string player : players)
			RemovePositionUpdate(eventParams.playerIdFrom, player);
		playerAskingPositionIds.Remove(eventParams.playerIdFrom);
	}
};

static ref PM_Server_Group_Position_Updater g_PM_server_group_position_updater;
static ref PM_Server_Group_Position_Updater PM_Server_GetGroupPositionUpdater()
{
    if (!g_PM_server_group_position_updater && GetGame().IsServer())
    {
        g_PM_server_group_position_updater = new PM_Server_Group_Position_Updater();
    }
    return g_PM_server_group_position_updater;
}