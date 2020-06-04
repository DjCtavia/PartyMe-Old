class PM_RPC_S_handler
{
	ref PM_RPC_S_invites			m_invites;
	ref PM_RPC_S_group				m_group;

    void PM_RPC_S_handler()
    {
        m_invites = new PM_RPC_S_invites;
		m_group = new PM_RPC_S_group;
    }
};