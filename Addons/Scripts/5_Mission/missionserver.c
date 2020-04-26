modded class MissionServer
{
    void MissionServer()
    {
        PM_GetGroupManager();
    }

    void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        PM_GetGroupManager().SetPlayerGroup(identity.GetId(), string.Empty);
        PM_GetGroupManager().SetPlayerGroup(identity.GetId(), "FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs="); // For tests
        PM_GetGroupManager().SetPlayerIdentity(identity.GetId(), identity);
    }
}