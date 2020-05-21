class PM_RPC_S_handler
{
	ref PM_RPC_S_invites			m_invites;

    void PM_RPC_S_handler()
    {
        m_invites = new PM_RPC_S_invites;
    }
	
	void AddRPC()
	{
		GetRPCManager().AddRPC("PartyMe", "GetPlayerIdentityTest", this, SingleplayerExecutionType.Both);
	}
};