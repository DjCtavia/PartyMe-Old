class PM_RPC_C_invites
{
    void PM_RPC_C_invites()
    {
        GetRPCManager().AddRPC("PartyMe", "InviteReceived", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "InviteAccepted", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "InviteRejected", this, SingleplayerExecutionType.Both);
    }

    /*
        Data:
            Param1: ID of the inviting player
        Event:
            playerIdFrom: ID of the inviting player
    */
    void InviteReceived(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<string> data;
        if (!ctx.Read(data)) return;

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
        PM_GetEvents().CallEvent("InviteReceived", eventParams);
    }

    /*
        Data:
            Param1: ID of the player accepting the invitation
        Event:
            playerIdFrom: ID of the player accepting the invitation
    */
    void InviteAccepted(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<string> data;
        if (!ctx.Read(data)) return;

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
        PM_GetEvents().CallEvent("InviteAccepted", eventParams);
    }

    /*
        Data:
            Param1: ID of the player refusing the invitation
        Event:
            playerIdFrom: ID of the player refusing the invitation
    */
    void InviteRejected(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<string> data;
        if (!ctx.Read(data)) return;

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
        PM_GetEvents().CallEvent("InviteRejected", eventParams);
    }
};