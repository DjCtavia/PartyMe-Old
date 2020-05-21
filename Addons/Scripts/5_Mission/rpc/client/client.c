class PM_RPC_C_handler
{
    private ref PM_RPC_C_group                  m_group;

    void PM_RPC_C_handler()
    {
        GetRPCManager().AddRPC("PartyMe", "PlayerJoinServer", this, SingleplayerExecutionType.Both);
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
        string playerId;

        if (PM_GetPlayerId(playerId) && playerId != data.param1)
        {
            ref PM_Event_Params eventParams = new PM_Event_Params;

            eventParams.playerIdFrom = data.param1;
            eventParams.name = data.param2;
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