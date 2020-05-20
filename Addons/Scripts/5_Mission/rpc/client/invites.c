class PM_RPC_C_invites
{
    void PM_RPC_C_invites()
    {
        GetRPCManager().AddRPC("PartyMe", "InvitationReceived", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "InvitationResponse", this, SingleplayerExecutionType.Both);
    }

    /*
        Data:
            Param1: ID of the inviting player
        Event:
            playerIdFrom: ID of the inviting player
    */
    void InvitationReceived(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
		eventParams.name = data.param2;
        PM_GetEvents().CallEvent("InvitationReceived", eventParams);
    }

    /*
        Data:
            Param1: ID of the player accepting the invitation
        Event:
            playerIdFrom: ID of the player accepting the invitation
			answer: Has accept invitation or no
    */
    void InvitationResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, bool> data;
        if (!ctx.Read(data)) return;

        ref PM_Event_Params eventParams = new PM_Event_Params;
        eventParams.playerIdFrom = data.param1;
		eventParams.answer = data.param2;
        PM_GetEvents().CallEvent("InvitationResponse", eventParams);
    }
};