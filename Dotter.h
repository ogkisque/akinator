//===============================================================================
//! @file    Dotter.h
//! @brief ���������� ��� dot.
//!
//! $copyright: (C) Danila Baygushev$ <br>
//===============================================================================
#ifndef DOTTER__
#define DOTTER__

#include "stdio.h"
#include "windows.h"

//===============================================================================
//! @defgroup System ���������
//! @defgroup Main   �������
//===============================================================================

//==============================================================================
#include "DotDef.h"  //Auto-gen file. Creates autamatically.
//===============================================================================

//===============================================================================
//! @ingroup Main
//! @brief   ��������� ����.
//===============================================================================
struct newDotNodeStyle
{
    char Shape     [30]; //!< �����. ��������:<br>
                         //!1. box            - "�������"(�������������)<br>
                         //!2. ellipse        - ������<br>
                         //!3. point          - �����<br>
                         //!4. egg            - ����<br>
                         //!5. triangle       - �����������<br>
                         //!6. diamond        - ����<br>
                         //!7. trapezium      - ��������<br>
                         //!8. parallelogram  - ��������������<br>
                         //!9. hexagon        - �������������<br>
                         //!10.octagon        - ��������������<br>
                         //!11.invtriangle    - ����������� �����������<br>
                         //!12.invtrapezium   - ����������� ��������<br>
                         //!13.folder         - �����<br>
                         //!14.tab            - ��������<br>
                         //!15.component      - ������<br>
                         //!15.box3d          - 3D "�������"(�������������)<br>
                         //!15.house          - ���<br>

    char Color     [30]; //!< ���� �������. ��������:<br>
                         //!1.���������� �������� ������ (��. Dotguide.pdf)<br>
                         //!2.# � �� ��� ��� � html<br>
                         //!3.��� ����� double = ���� / 256<br>

    char FillColor [30]; //!< ���� ������. ��������:<br>
                         //!1.���������� �������� ������ (��. Dotguide.pdf)<br>
                         //!2.# � �� ��� ��� � html<br>
                         //!3.��� ����� double = ���� / 256<br>

    char FontColor [30]; //!< ���� ����. ��������:<br>
                         //!1.���������� �������� ������ (��. Dotguide.pdf)<br>
                         //!2.# � �� ��� ��� � html<br>
                         //!3.��� ����� double = ���� / 256<br>

    char Style     [30]; //!< �����. ��������: <br>
                         //!1.solid     - �������<br>
                         //!2.dashed    - ����������<br>
                         //!3.dotted    - �������<br>
                         //!4.bold      - ������<br>
                         //!5.invis     - �� ��������<br>
                         //!6.filled    - ��������<br>
                         //!7.diagonals - ���������<br>
                         //!8.round     - ����������� ����<br>

    newDotNodeStyle ();

    newDotNodeStyle& shape     (const char Value[]);
    newDotNodeStyle& color     (const char Value[]);
    newDotNodeStyle& fillcolor (const char Value[]);
    newDotNodeStyle& fontcolor (const char Value[]);
    newDotNodeStyle& style     (const char Value[]);
};

//------------------------------------------------------------------------------

newDotNodeStyle::newDotNodeStyle ()
{
    strcpy (Shape,     "ellipse");
    strcpy (Color,     "black");
    strcpy (FillColor, "black");
    strcpy (FontColor, "black");
    strcpy (Style,     "solid");
}

newDotNodeStyle& newDotNodeStyle::shape     (const char Value[])
{
    strcpy (Shape, Value);

    return *this;
}
newDotNodeStyle& newDotNodeStyle::color     (const char Value[])
{
    strcpy (Color, Value);

    return *this;
}
newDotNodeStyle& newDotNodeStyle::fillcolor (const char Value[])
{
    strcpy (FillColor, Value);

    return *this;
}
newDotNodeStyle& newDotNodeStyle::fontcolor (const char Value[])
{
    strcpy (FontColor, Value);

    return *this;
}
newDotNodeStyle& newDotNodeStyle::style     (const char Value[])
{
    strcpy (Style, Value);

    return *this;
}

//==============================================================================

//===============================================================================
//! @ingroup Main
//! @brief   ��������� �����.
//===============================================================================
struct newDotLinkStyle
{
    char Style [30]; //!< �����. ��������: <br>
                     //!1.solid     - �������<br>
                     //!2.dashed    - ����������<br>
                     //!3.dotted    - �������<br>
                     //!4.bold      - ������<br>
                     //!5.invis     - �� ��������<br>

    char Color [30]; //!< ���� �����. ��������:<br>
                     //!1.���������� �������� ������ (��. Dotguide.pdf)<br>
                     //!2.# � �� ��� ��� � html<br>
                     //!3.��� ����� double = ���� / 256<br>

    newDotLinkStyle ();

    newDotLinkStyle& style (const char Value[]);
    newDotLinkStyle& color (const char Value[]);
};

//------------------------------------------------------------------------------

newDotLinkStyle::newDotLinkStyle ()
{
    strcpy (Color, "black");
    strcpy (Style, "solid");
}

newDotLinkStyle& newDotLinkStyle::style (const char Value[])
{
    strcpy (Style, Value);

    return *this;
}
newDotLinkStyle& newDotLinkStyle::color (const char Value[])
{
    strcpy (Color, Value);

    return *this;
}

//==============================================================================

//===============================================================================
//! @ingroup System
//! @brief   ����� �������� ��������� ��������� ������.
//===============================================================================
namespace DotterSystem
{
    FILE* CurrentFile = NULL; //!< ������� ����
    bool CloseAtEnd = false; //!< ��������� ���� ����� �����, ��� ���

    newDotNodeStyle DefDotNode; //!< ����������� ����� ����.
    newDotLinkStyle DefDotLink; //!< ����������� ����� ������.

    newDotNodeStyle CurrentNodeStyle; //!< ������� ����� ����.
    newDotLinkStyle CurrentLinkStyle; //!< ������� ����� ������.

    int Ind = 4; //!< ������ ����� ������� � ���-�� ��������.
};
using namespace DotterSystem;

//==============================================================================

//===============================================================================
//! @ingroup System
//! @brief   ������� ������ ������.
//!
//! @param Message - ���������, ���������� ������
//===============================================================================
void dtError (const char Message []);

//==============================================================================

//===============================================================================
//! @ingroup Main
//! @brief   ������ ������ dot �����.
//!
//! @param To - ����, ���� ������. ��� ������� (��. @ref dtNode, @ref dtLink) �� @ref dtEnd ����� �������� � ���� ����
//===============================================================================
void dtBegin (FILE* To);

//===============================================================================
//! @ingroup Main
//! @brief   ������� ���� � ������ ������ dot �����.
//!
//! @param Name - ��� �����, ���� ������. ��� ������� (��. @ref dtNode, @ref dtLink) �� @ref dtEnd ����� �������� � ���� ����
//===============================================================================
inline void dtBegin (const char Name []);

//===============================================================================
//! @ingroup Main
//! @brief   ��������� �������� dot �����.
//===============================================================================
void dtEnd  ();

//===============================================================================
//! @ingroup Main
//! @brief   ������� ������ � ��������� ��� Dot �������.
//===============================================================================
void dtOut (const char Str []);

//==============================================================================

//===============================================================================
//! @ingroup Main
//! @brief   ������������� ������� ����� ����.
//!
//! @param NodeStyle - ���������, � ��������� ������ ����. �� �����������
//!
//! @return ������ �� ������� �����. ��������� ������ ���:
//!
//! @usage
//! @code
//!
//!   dtNodeStyle ().shape ("diamond")
//!                 .color ("red");
//!
//! @endcode
//===============================================================================
newDotNodeStyle& dtNodeStyle  (const newDotNodeStyle NodeStyle = CurrentNodeStyle);

//===============================================================================
//! @ingroup Main
//! @brief   ������������� ������� ����� ����.
//!
//! @param LinkStyle - ���������, � ��������� ������ ����. �� �����������
//!
//! @return ������ �� ������� �����. ��������� ������ ���:
//!
//! @usage
//! @code
//!
//!   dtLinkStyle ().style ("bold")
//!                 .color ("red");
//!
//! @endcode
//===============================================================================
newDotLinkStyle& dtLinkStyle  (const newDotLinkStyle LinkStyle = CurrentLinkStyle);

//===============================================================================
//! @ingroup Main
//! @brief   ������� ��������� ���� � dot.
//!
//! @param NodeN       - ����� ����
//! @param Label       - �����, ������� ���� ���������� ������
//! @param NodeStyle   - ���������, � ��������� ������ ����. ���� �� �������, ������������ �������(��. @ref dtNodeStyle)
//! @param ExtraParams - �������������� ���������(��. dotguide)
//===============================================================================
void dtNode  (const int NodeN, const char Label [] = "", const newDotNodeStyle NodeStyle = CurrentNodeStyle, const char ExtraParams [] = "");

//===============================================================================
//! @ingroup Main
//! @brief   ������� ��������� ���������� ����� � dot.
//!
//! @param FromNode    - ����� ���� ������
//! @param ToNode      - ����� ���� ����
//! @param Label       - �����, ������� ���� ���������� �����.
//! @param LinkStyle   - ���������, �� ������ �����. ���� �� �������, ������������ ������� (��. @ref dtLinkStyle)
//! @param ExtraParams - �������������� ���������(��. dotguide)
//===============================================================================
void dtLink (const int FromNode, const int ToNode, const char Label [] = "", const newDotLinkStyle LinkStyle = CurrentLinkStyle, const char ExtraParams [] = "");

//==============================================================================

//===============================================================================
//! @ingroup Main
//! @brief   ������� ������������� ����������� � dot.
//!
//! @param From - ��� dot �����
//! @param To   - ��� ��������� �����
//! @param Type - ��� (jpg, bmp, ...)
//! @param Show - ���������� ���� ����� ��������, ��� ���
//===============================================================================
void dtRender (const char From [], const char To [] = "Graph.jpg", const char Type [] = "jpg", const bool Show = true);

//==============================================================================

void dtError (const char Message [])
{
    throw Message;

    printf ("==Dotter error!==\n");
    printf ("Message = %s\n");

    throw "Dotter error!!!"; //����� ���-�� �����. )

    system ("pause");

    exit (0);
}

//==============================================================================

void dtBegin (FILE* To)
{
    if (!To) dtError ("No output file");
    if (CurrentFile) dtError ("Close previous file first");
    CurrentFile = To;

    fprintf (CurrentFile, "digraph DotGraph\n");
    fprintf (CurrentFile, "{\n");
}

inline void dtBegin (const char Name [])
{
    FILE* To = NULL;
    To = fopen (Name, "w");

    CloseAtEnd = true;

    dtBegin (To);
}

void dtEnd  ()
{
    if (!CurrentFile) dtError ("No output file");

    fprintf (CurrentFile, "}\n");

    if (CloseAtEnd)
    {
        CloseAtEnd = false;

        fclose (CurrentFile);
    }

    CurrentFile = NULL;
}

void dtOut (const char Str [])
{
    if (!CurrentFile) dtError ("No output file");

    for (int i = 0; Str [i] != 0; i ++)
    {
        switch (Str [i])
        {
            case '\n':
            {
                fprintf (CurrentFile, "\\n");
                break;
            }
            case '\r':
            {
                fprintf (CurrentFile, "\\r");
                break;
            }
            case '\\':
            {
                fprintf (CurrentFile, "\\\\");
                break;
            }
            case '"':
            {
                fprintf (CurrentFile, "\\\"");
                break;
            }

            default:
            {
                fprintf (CurrentFile, "%c", Str [i]);
                break;
            }

        }
    }
}

//==============================================================================

newDotNodeStyle& dtNodeStyle  (const newDotNodeStyle NodeStyle)
{
    if (!CurrentFile) dtError ("No output file");

    CurrentNodeStyle = NodeStyle;

    return CurrentNodeStyle;
}
newDotLinkStyle& dtLinkStyle  (const newDotLinkStyle LinkStyle)
{
    if (!CurrentFile) dtError ("No output file");

    CurrentLinkStyle = LinkStyle;

    return CurrentLinkStyle;
}

void dtNode (const int NodeN, const char Label [], const newDotNodeStyle NodeStyle, const char ExtraParams [])
{
    if (!CurrentFile) dtError ("No output file");

    for (int i = 0; i < DotterSystem::Ind; i ++)
    {
        fprintf (CurrentFile, " ");
    }

    fprintf (CurrentFile, "Node%d ", NodeN);
    fprintf (CurrentFile, "[");

#ifndef BeautifulDot
    fprintf (CurrentFile, "shape=\"%s\", ",     NodeStyle.Shape);
    fprintf (CurrentFile, "color=\"%s\", ",     NodeStyle.Color);
    fprintf (CurrentFile, "fontcolor=\"%s\", ", NodeStyle.FontColor);
    fprintf (CurrentFile, "fillcolor=\"%s\", ", NodeStyle.FillColor);
    fprintf (CurrentFile, "style=\"%s\", ",     NodeStyle.Style);
    fprintf (CurrentFile, "weight=\"1\", ");
#else
    if (strcmp (NodeStyle.Shape,     DefDotNode.Shape)     != 0) fprintf (CurrentFile, "shape=\"%s\", ",     NodeStyle.Shape);
    if (strcmp (NodeStyle.Color,     DefDotNode.Color)     != 0) fprintf (CurrentFile, "color=\"%s\", ",     NodeStyle.Color);
    if (strcmp (NodeStyle.FontColor, DefDotNode.FontColor) != 0) fprintf (CurrentFile, "fontcolor=\"%s\", ", NodeStyle.FontColor);
    if (strcmp (NodeStyle.FillColor, DefDotNode.FillColor) != 0) fprintf (CurrentFile, "fillcolor=\"%s\", ", NodeStyle.FillColor);
    if (strcmp (NodeStyle.Style,     DefDotNode.Style)     != 0) fprintf (CurrentFile, "style=\"%s\", ",     NodeStyle.Style);
#endif

    fprintf (CurrentFile, "label=\"");
    dtOut (Label);
    fprintf (CurrentFile, "\"");

    if (strcmp (ExtraParams, "") != 0) fprintf (CurrentFile, ", %s", ExtraParams);

    fprintf (CurrentFile, "];\n");
}

void dtLink (const int FromNode, const int ToNode, const char Label [], const newDotLinkStyle LinkStyle, const char ExtraParams [])
{
    if (!CurrentFile) dtError ("No output file");

    for (int i = 0; i < DotterSystem::Ind; i ++)
    {
        fprintf (CurrentFile, " ");
    }

    fprintf (CurrentFile, "Node%d -> Node%d ", FromNode, ToNode);
    fprintf (CurrentFile, "[");

#ifndef BeautifulDot
    fprintf (CurrentFile, "color=\"%s\", ", LinkStyle.Color);
    fprintf (CurrentFile, "style=\"%s\", ", LinkStyle.Style);
    fprintf (CurrentFile, "weight=\"1\", ");
#else
    if (strcmp (LinkStyle.Color, DefDotLink.Color) != 0) fprintf (CurrentFile, "color=\"%s\", ", LinkStyle.Color);
    if (strcmp (LinkStyle.Style, DefDotLink.Style) != 0) fprintf (CurrentFile, "style=\"%s\", ", LinkStyle.Style);
#endif

    fprintf (CurrentFile, "label=\"");
    dtOut (Label);
    fprintf (CurrentFile, "\"");

    if (strcmp (ExtraParams, "") != 0) fprintf (CurrentFile, ", %s", ExtraParams);

    fprintf (CurrentFile, "];\n");
}

//==============================================================================

void dtRender (const char From [], const char To [], const char Type [], const bool Show)
{
    const int ComandSize = 200;

    char Comand [ComandSize] = "";

    const int OtherSymbols = 14;

    if (strlen (__DOT__) + strlen (From) + strlen (To) + strlen (Type) + OtherSymbols >= ComandSize) dtError ("Names too long!");

    sprintf (Comand, "%s -T %s %s -o %s", __DOT__, Type, From, To);

    system (Comand);

    if (Show)
    {
        strcpy (Comand, "");

        sprintf (Comand, "start %s", To);

        system (Comand);
    }
}

//==============================================================================

#endif
