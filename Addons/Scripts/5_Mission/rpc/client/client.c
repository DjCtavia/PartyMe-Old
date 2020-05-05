class PM_RPC_C_handler
{
    private ref PM_RPC_C_group                  m_group;

    void PM_RPC_C_handler()
    {
        GetRPCManager().AddRPC("PartyMe", "PlayerJoinServer", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "GetPlayerList", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "PlayerLeaveServer", this, SingleplayerExecutionType.Both);
        m_group = new PM_RPC_C_group;
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

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
        eventParams.name = data.param2;
        PM_GetEvents().CallEvent("PlayerJoinServer", eventParams);
    }

    /*
        Description:
            Called when user is joining the server,
            server send to client every user IDs and names.
        Data:
            Param1: Array of users ID
            Param2: Array of users name
        Event:
            playerIdFrom: ID of a player
            name: Name of a player
    */
    void GetPlayerList(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<array<string>, array<string>> data;
        if (!ctx.Read(data)) return;

        for (int playerIndex = 0; playerIndex < data.param1.Count(); playerIndex++)
        {
            ref PM_Event_Params eventParams = new PM_Event_Params;

            eventParams.playerIdFrom = data.param1.Get(playerIndex);
            eventParams.name = data.param2.Get(playerIndex);
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