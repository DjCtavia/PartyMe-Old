class PM_widget_smallbutton
{
	private static const string DEFAULT_LAYOUT		= "partyme/gui/layouts/widgets/smallbutton.layout";

	private ButtonWidget							m_w_root;
	private Widget									m_w_background;
	private ImageWidget								m_i_icon;

	private bool									m_isActive = true;
    private int										m_backgroundColor;
	private float									m_backgroundAlpha;
	private bool									m_useBackgroundColorHover = false;
    private int										m_backgroundColorHover;
	private int										m_backgroundAlphaHover;
	private bool									m_useBackgroundColorDisable = false;
	private int										m_backgroundColorDisable;
	private int										m_backgroundAlphaDisable;

	private bool									m_toggleImage = false;
	private int										m_iconColor;
	private int										m_iconAlpha;
	private bool									m_useIconColorDisable = false;
	private int										m_iconColorDisable;
	private int										m_iconAlphaDisable;

	void PM_widget_smallbutton(Widget parent, string iconPath, int backgroundColor, float backgroundAlpha, int iconColor, int iconAlpha)
	{
		m_backgroundColor = backgroundColor;
		m_backgroundAlpha = backgroundAlpha;
		m_iconColor = iconColor;
		m_iconAlpha = iconAlpha;

		m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, parent);
		GetWidgets();
		ConfigWidgets(iconPath);
	}

	//-------------------------------------------------------------------------- Init widgets
	private void GetWidgets()
	{
		m_w_background = m_w_root.FindAnyWidget("background");
		m_i_icon = ImageWidget.Cast(m_w_root.FindAnyWidget("icon"));
	}

	private void ConfigWidgets(string iconPath)
	{
		m_w_background.SetColor(m_backgroundColor);
		m_w_background.SetAlpha(m_backgroundAlpha);
		m_i_icon.SetColor(m_iconColor);
		m_i_icon.SetAlpha(m_iconAlpha);
		m_i_icon.LoadImageFile(0, iconPath);
	}

	//-------------------------------------------------------------------------- Extra init widgets
	void SetPos(float x, float y, bool immedUpdate = true)
	{
		m_w_root.SetPos(x, y, immedUpdate);
	}

	void SetToggleIcon(string iconPath)
	{
		m_toggleImage = true;
		m_i_icon.LoadImageFile(1, iconPath);
	}

	void SetBackgroundHover(int color, float alpha)
	{
		m_useBackgroundColorHover = true;
		m_backgroundColorHover = color;
		m_backgroundAlphaHover = alpha;
	}

	void SetBackgroundDisable(int color, float alpha)
	{
		m_useBackgroundColorDisable = true;
		m_backgroundColorDisable = color;
		m_backgroundAlphaDisable = alpha;
	}

	void SetIconDisable(int color, float alpha)
	{
		m_useIconColorDisable = true;
		m_iconColorDisable = color;
		m_iconAlphaDisable = alpha;
	}

	//-------------------------------------------------------------------------- Setters
	void Enable(bool isActive)
	{
		m_isActive = isActive;

		m_w_root.Enable(isActive);
		if (isActive)
		{
			m_w_background.SetColor(m_backgroundColor);
			m_w_background.SetAlpha(m_backgroundAlpha);
			m_i_icon.SetColor(m_iconColor);
			m_i_icon.SetAlpha(m_iconAlpha);
		}
		else
		{
			UseBackgroundDisable();
			UseIconDisable();
		}
	}

	//-------------------------------------------------------------------------- States
	private void ToggleIcon()
	{
		if (!m_toggleImage) return;
		if (m_i_icon.GetImage())
			m_i_icon.SetImage(0);
		else
			m_i_icon.SetImage(1);
	}

	private void UseBackgroundHover()
	{
		if (!m_useBackgroundColorHover) return;
		m_w_background.SetColor(m_backgroundColorHover);
		m_w_background.SetAlpha(m_backgroundAlphaHover);
	}

	private void UseBackgroundDisable()
	{
		if (!m_useBackgroundColorDisable) return;
		m_w_background.SetColor(m_backgroundColorDisable);
		m_w_background.SetAlpha(m_backgroundAlphaDisable);
	}

	private void UseIconDisable()
	{
		if (!m_useIconColorDisable) return;
		m_i_icon.SetColor(m_iconColorDisable);
		m_i_icon.SetAlpha(m_iconAlphaDisable);
	}

	//-------------------------------------------------------------------------- UI Events
	bool OnClick(Widget w, int x, int y, int button)
	{
		if (!m_isActive)
			return false;
		if (w == m_w_root)
		{
			ToggleIcon();
			return true;
		}
		return false;
	}

	bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!m_isActive)
			return false;
		if (w == m_w_root)
		{
			UseBackgroundHover();
			return true;
		}
		return false;
	}

	bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!m_isActive)
			return false;
		if (w == m_w_root)
		{
			m_w_background.SetColor(m_backgroundColor);
			m_w_background.SetAlpha(m_backgroundAlpha);
			return true;
		}
		return false;
	}
};