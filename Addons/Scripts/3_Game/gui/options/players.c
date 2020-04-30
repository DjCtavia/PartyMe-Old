class PM_UI_Option_Party extends PM_UI_Category
{
    void PM_UI_Option_Party(Widget parent, int index)
    {
        m_icon = "partyme/gui/images/options/icons/party.tga";
        m_w_parent = parent;
        Init(index);
    }

    override void GetWidgets()
    {
        super.GetWidgets();

        m_menu = new PM_UI_Menu_Party(m_w_parent);
    }

    override void ConfigureWidget()
    {
        super.ConfigureWidget();
        m_txt_optionName.SetText("Party");
    }
};

class PM_UI_Menu_Party extends PM_UI_Menu
{
    protected ScrollWidget                      m_scroll_playerList;
    protected ButtonWidget                      m_btn_leaveParty;

    void PM_UI_Menu_Party(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/group/options/party/option.layout";
        m_w_parent = parent;
        Init();
    }

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
        m_btn_leaveParty = ButtonWidget.Cast(m_w_root.FindAnyWidget("leaveParty"));
    }
};