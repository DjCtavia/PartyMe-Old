class PM_GroupContextMenu : ScriptedWidgetEventHandler
{
    private Widget                                      m_group_context_menu_root_widget;
    protected ref array<ref PM_UI_Category>             m_options;
    protected ref PM_UI_Category                        m_option_displayed;

    void PM_GroupContextMenu()
    {
        m_options = new array<ref PM_UI_Category>;
    }

    /*--------------------------------------------------------------------------
        Note:
            If you want to make any changes to the layout, changes should be made here.
            For further modification of the UI, I highly recommend that you redo a class based on it.
    */
    void Init()
	{
        m_group_context_menu_root_widget = GetGame().GetWorkspace().CreateWidgets("partyme/gui/layouts/group.layout");
        m_group_context_menu_root_widget.SetHandler(this);
        AddCategories();
        EnableMenu(0);
    }

    void AddCategories()
    {
        m_options.Insert(new PM_UI_Option_Party(m_group_context_menu_root_widget, m_options.Count()));
        m_options.Insert(new PM_UI_Option_Playerlist(m_group_context_menu_root_widget, m_options.Count()));
    }

    Widget GetMainWidget()
    {
        return m_group_context_menu_root_widget;
    }

    //--------------------------------------------------------------------------
    void EnableMenu(int menuId)
    {
        ref PM_UI_Category category;

        for (int menuIndex = 0; menuIndex < m_options.Count(); ++menuIndex)
        {
            category = m_options.Get(menuIndex);
            if (category)
            {
                if (menuIndex != menuId)
                {
                    category.Show(false);
                }
                else
                {
                    category.Show(true);
                }
            }
        }
    }

    void Show(int x, int y)
    {
        // m_group_context_menu_root_widget.SetPos(x, y);
        m_group_context_menu_root_widget.Show(true);
    }

    void Hide()
    {
        m_group_context_menu_root_widget.Show(false);
    }

    bool IsVisible()
    {
        return m_group_context_menu_root_widget.IsVisible();
    }

    /*--------------------------------------------------------------------------
        Events
    */
    override bool OnClick(Widget w, int x, int y, int button)
    {
        ref PM_UI_Category option;

        for (int categoryIndex = 0; categoryIndex < m_options.Count(); ++categoryIndex)
        {
            option = m_options.Get(categoryIndex);
            if (option && option.OnClick(w, x, y, button))
            {
                return true;
            }
        }
        return false;
    }

   override bool OnMouseEnter(Widget w, int x, int y)
   {
       foreach (ref PM_UI_Category option : m_options)
       {
           if (option.OnMouseEnter(w, x, y))
           {
               return true;
           }
       }
       if (m_option_displayed.OnMouseEnter(w, x, y))
       {
           return true;
       }
       return false;
   }

   override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
   {
       foreach (ref PM_UI_Category option : m_options)
       {
           if (option.OnMouseLeave(w, enterW, x, y))
           {
               return true;
           }
       }
       if (m_option_displayed.OnMouseLeave(w, enterW, x, y))
       {
           return true;
       }
       return false;
   }

   override bool OnDrag(Widget w, int x, int y)
   {
       foreach (ref PM_UI_Category option : m_options)
       {
           if (option.OnDrag(w, x, y))
           {
               return true;
           }
       }
       if (m_option_displayed.OnDrag(w, x, y))
       {
           return true;
       }
       return false;
   }

   override bool OnDrop(Widget w, int x, int y, Widget reciever)
   {
       foreach (ref PM_UI_Category option : m_options)
       {
           if (option.OnDrop(w, x, y, reciever))
           {
               return true;
           }
       }
       if (m_option_displayed.OnDrop(w, x, y, reciever))
       {
           return true;
       }
       return false;
   }

   void Update(float timeslice)
   {
       foreach (ref PM_UI_Category option : m_options)
       {
           option.Update(timeslice);
       }
   }
};