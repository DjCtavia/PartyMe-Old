modded class MissionServer
{
    ref PM_Group_Manager groupManager;

    void MissionServer()
    {
        groupManager = new PM_Group_Manager();
    }

    void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        groupManager.SetPlayerGroup(identity.GetId(), string.Empty);
        groupManager.SetPlayerGroup(identity.GetId(), "FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs="); // For tests
    }
}