/*
    [WIP]

    These classes will be used later with
    widget constructor, in the current state it has no utility.

    ----------------------------------

    Example of class use:
        ref PM_Font font = new PM_Font("varela", "partyme/fonts", 18)
        string fontPath = font.GetDefault(); // It will return: "partyme/fonts/varela/varela18"
*/
class PM_Font
{
    private string      m_fontName;
    private string      m_fontPath;

    int                 m_defaultSize;
    ref array<int>      m_sizes;

    void PM_Font(string fontName, string fontPath, int defaultSize)
    {
        m_fontName = fontName;
        m_fontPath = fontPath;
        m_defaultSize = defaultSize;
        m_sizes = new array<int>;
    }

    bool SizeAvailable(int size)
    {
        if (m_sizes.Find(size) != -1)
        {
            return true;
        }
        return false;
    }

    void AddSize(int size)
    {
        if (!SizeAvailable(size))
        {
            m_sizes.Insert(size);
        }
    }

    /*
        Description:
            Return the full path to the font, if size is not found
            it will return the default size give at construction of the class.
    */
    string Get(int size)
    {
        string path = "";

        if (SizeAvailable(size))
        {
            path = m_fontPath + "/" + m_fontName + size;
        }
        else
        {
            Print("[PartyMe][Font] Font size not found for size: " + size + " of font name: " + m_fontName);
            path = GetDefault();
        }
        return path;
    }

    string GetDefault()
    {
        return m_fontPath + "/" + m_fontName + m_defaultSize;
    }
};

class PM_Fonts
{
    ref map<string, ref PM_Font>      m_fonts;

    void PM_Fonts()
    {
        m_fonts = new map<string, ref PM_Font>;
        Init();
    }

    private void Init()
    {
        AddFonts();
        AddSizes();
    }

    private void AddFonts()
    {
        /*
        AddFont("varela", "partyme/fonts", 16);
        */
    }

    private void AddSizes()
    {
        /*
        AddSize("varela", 12);
        AddSize("varela", 24);
        AddSize("varela", 32);
        AddSize("varela", 64);
        */
    }

    /*
        Description:
            The default font use for widgets.
        Note:
            Edit this value for changing the font of widgets. 
    */
    string GetDefaultFont()
    {
        return "partyme/fonts/varela/varela16";
    }

    string GetFont(string fontName, int size)
    {
        ref PM_Font font = m_fonts.Get(fontName);

        if (font)
        {
            return font.Get(size);
        }
        return GetDefaultFont();
    }

    private void AddFont(string fontName, string fontPath, int defaultSize)
    {
        if (!m_fonts.Contains(fontName))
        {
            m_fonts.Set(fontName, new PM_Font(fontName, fontPath, defaultSize));
        }
    }

    private void AddSize(string fontName, int size)
    {
        ref PM_Font font = m_fonts.Get(fontName);

        if (font)
        {
            font.AddSize(size);
        }
    }
};