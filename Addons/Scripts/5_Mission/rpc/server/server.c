class PM_RPC_S_handler
{
    void PM_RPC_S_handler()
    {
        GetRPCManager().AddRPC("PartyMe", "GetPlayerIdentityTest", this, SingleplayerExecutionType.Both);
    }
};