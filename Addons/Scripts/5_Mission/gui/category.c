class PM_UI_Category
{
    private static const string DEFAULT_LAYOUT      = "partyme/gui/layouts/widgets/menuoption.layout";
    private static const string ICON_ISACTIVE       = "partyme/gui/images/options/icons/arrow.tga";
    protected int                                   m_categoryIndex;

    protected string                                m_icon;
    protected Widget                                m_w_parent;
    protected Widget                                m_w_scrollMenu;

    protected Widget                                m_w_root;
    protected ImageWidget                           m_img_icon;
    protected TextWidget                            m_txt_optionName;
    protected ImageWidget                           m_img_isActive;

    protected ref PM_UI_Menu                        m_menu;

    void Init(int index)
    {
        m_categoryIndex = index;
        GetWidgets();
        ConfigureWidget();

        m_w_root.SetPos(0, (20 + 58 * index));
    }

    void GetWidgets()
    {
        m_w_scrollMenu = m_w_parent.FindAnyWidget("ScrollMenu");
        m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, m_w_scrollMenu);
        m_img_icon = ImageWidget.Cast(m_w_root.FindAnyWidget("image"));
        m_txt_optionName = TextWidget.Cast(m_w_root.FindAnyWidget("text"));
        m_img_isActive = ImageWidget.Cast(m_w_root.FindAnyWidget("isActive"));
    }

    // To override
    void ConfigureWidget()
    {
        m_img_icon.LoadImageFile(0, m_icon);
    }

    void Show(bool isVisible)
    {
        m_menu.Show(isVisible);
    }

    bool OnClick(Widget w, int x, int y, int button)
    {
        if (button == MouseState.LEFT)
        {
            if (w == m_w_root)
            {
                PM_GroupMenu menu = PM_GroupMenu.Cast(GetGame().GetUIManager().GetMenu());
                if (menu)
                {
                    menu.GetGroupContextMenu().EnableMenu(m_categoryIndex);
                }
                return true;
            }
        }
        return false;
    }

    bool OnMouseEnter(Widget w, int x, int y)
    {
        return false;
    }

    bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        return false;
    }
    
    bool OnDrag(Widget w, int x, int y)
    {
        return false;
    }

    bool OnDrop(Widget w, int x, int y, Widget reciever)
    {
        return false;
    }

    void Update(float timeslice)
    {
        if (m_menu && m_menu.IsVisible())
        {
            m_menu.Update(timeslice);
        }
    }
};

class PM_UI_Menu
{
    protected string                            m_layoutPath;

    protected Widget                            m_w_parent;
    protected Widget                            m_w_menu;
    protected Widget                            m_w_root;

    void Init()
    {
        m_w_menu = m_w_parent.FindAnyWidget("Menu");
        GetWidgets();
    }

    void Show(bool isVisible)
    {
        if (isVisible)
        {
            m_w_root.Show(true);
        }
        else
        {
            m_w_root.Show(false);
        }
    }

    void GetWidgets()
    {
        m_w_root = GetGame().GetWorkspace().CreateWidgets(m_layoutPath, m_w_menu);
    }

    bool OnClick(Widget w, int x, int y, int button)
    {
        return false;
    }

    bool OnMouseEnter(Widget w, int x, int y)
    {
        return false;
    }

    bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        return false;
    }
    
    bool OnDrag(Widget w, int x, int y)
    {
        return false;
    }

    bool OnDrop(Widget w, int x, int y, Widget reciever)
    {
        return false;
    }

    void Update(float timeslice)
    {

    }

    bool IsVisible()
    {
        if (!m_w_root || !m_w_root.IsVisible())
        {
            return false;
        }
        return true;
    }
};