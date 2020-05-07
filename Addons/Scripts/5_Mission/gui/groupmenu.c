class PM_GroupMenu extends UIScriptedMenu
{
    private ref PM_GroupContextMenu         m_group_context_menu;

    //--------------------------------------------------------------------------
    void PM_GroupMenu()
    {
        m_group_context_menu = new PM_GroupContextMenu;
        m_group_context_menu.Init();
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
    ref PM_GroupContextMenu GetGroupContextMenu()
    {
        return m_group_context_menu;
    }

    //--------------------------------------------------------------------------
    override void OnShow()
    {
        super.OnShow();
        PPEffects.SetBlurMenu(0.4);
        SetFocus(layoutRoot);
    }

    override void OnHide()
    {
        super.OnHide();
        PPEffects.SetBlurMenu(0);
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);
	    m_group_context_menu.Update(timeslice);
    }

    //--------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (m_group_context_menu.OnClick(w, x, y, button))
        {
            return true;
        }
        return false;
    }
};