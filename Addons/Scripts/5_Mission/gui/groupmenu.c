class PM_GroupMenu extends UIScriptedMenu
{
    private ref PM_GroupContextMenu         m_group_context_menu;

    //--------------------------------------------------------------------------
    void PM_GroupMenu()
    {
        m_group_context_menu = new PM_GroupContextMenu;
    }

    override Widget Init()
	{
        layoutRoot = m_group_context_menu.GetMainWidget();
        return layoutRoot;
    }

    Widget GetRoot()
    {
        return m_group_context_menu.GetMainWidget();
    }

    //--------------------------------------------------------------------------
    PM_GroupContextMenu GetGroupContextMenu()
	{
		return m_group_context_menu;
	}

    // PPEffects.SetBlurMenu( 0.6 );

    //--------------------------------------------------------------------------
    override void OnShow()
    {
        super.OnShow();
        PPEffects.SetBlurMenu(0.4);
    }

    override void OnHide()
	{
        super.OnHide();
        PPEffects.SetBlurMenu(0);
    }

    override void Update(float timeslice)
	{
	    m_group_context_menu.Update(timeslice);
	}
};