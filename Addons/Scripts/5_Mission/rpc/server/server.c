class PM_RPC_S_handler
{
    void PM_RPC_S_handler()
    {
        GetRPCManager().AddRPC("PartyMe", "GetPlayerList", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "GetPlayerIdentityTest", this, SingleplayerExecutionType.Both);
    }

    /*
        Data:
            Param1: Identifier of the player requesting the list
    */
    void GetPlayerList(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<string> data;
        if (!ctx.Read(data)) return;

        array<Man> players = new array<Man>;
        auto playerIDs = new array<string>;
        auto playerNames = new array<string>;

        GetGame().GetWorld().GetPlayerList(players);
        for (int playerIndex = 0; playerIndex < players.Count(); playerIndex++)
        {
            PlayerBase playerPB = PlayerBase.Cast(players.Get(playerIndex));
            ref PlayerIdentity playerIdentity;

            if (playerPB)
            {
                playerIdentity = playerPB.GetIdentity();
                // if (playerIdentity && playerIdentity.GetId() != data.param1)
                if (playerIdentity)
                {
                    playerIDs.Insert(playerIdentity.GetId());
                    playerNames.Insert(playerIdentity.GetName());
                }
            }
        }
        GetRPCManager().SendRPC("PartyMe", "GetPlayerList", new Param2<array<string>, array<string>>(playerIDs, playerNames), false, sender);
    }
};