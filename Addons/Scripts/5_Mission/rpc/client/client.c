class PM_RPC_C_handler
{
    private ref PM_RPC_C_group                  m_group;

    void PM_RPC_C_handler()
    {
		GetRPCManager().AddRPC("PartyMe", "InitPlayerList", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "PlayerJoinServer", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "PlayerLeaveServer", this, SingleplayerExecutionType.Both);
        m_group = new PM_RPC_C_group;
    }
	
	void InitPlayerList(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param2<array<string>, array<string>> data;
		if (!ctx.Read(data)) return;
		MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
		int dataSize = data.param1.Count();

		for (int iData = 0; iData < dataSize; iData++)
		{
			ref PM_Event_Params eventParams = new PM_Event_Params;

            eventParams.playerIdFrom = data.param1.Get(iData);
            eventParams.name = data.param2.Get(iData);
			mission.m_pm_playersName.Set(data.param1.Get(iData), data.param2.Get(iData));
			PM_GetEvents().CallEvent("PlayerJoinServer", eventParams);
		}
	}

    /*
        Description:
            Mainly called when user is joining the server,
            server send to client every user IDs and names.
        Data:
            Param1: User joining ID
            Param2: User joining name
    */
    void PlayerJoinServer(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if (!ctx.Read(data)) return;
        string playerId;

        if (PM_GetPlayerId(playerId) && playerId != data.param1)
        {
            MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
            ref PM_Event_Params eventParams = new PM_Event_Params;

            eventParams.playerIdFrom = data.param1;
            eventParams.name = data.param2;
            mission.m_pm_playersName.Set(data.param1, data.param2);
            PM_GetEvents().CallEvent("PlayerJoinServer", eventParams);
        }
    }

    /*
        Data:
            Param1: User leaving ID
        Event:
            playerIdFrom: ID of player leaving server
    */
   void PlayerLeaveServer(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
   {
       Param1<string> data;
       if (!ctx.Read(data)) return;

       ref PM_Event_Params eventParams = new PM_Event_Params;
       eventParams.playerIdFrom = data.param1;
       PM_GetEvents().CallEvent("PlayerLeaveServer", eventParams);
   }
};