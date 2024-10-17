/*   CString is acroname of color string, ... 



typedef struct _CHAR_INFO {
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } Char;
    WORD Attributes;
} CHAR_INFO, *PCHAR_INFO;

*/


#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <console project/basic/ka_IDrawable.hpp>
#include <include/Macros.hpp>


namespace console{

    // structrue for generale colored character
    template<typename TChar, typename TAttribute>
    struct COLORCHAR {
        TChar           Char;
        TAttribute      Attributes;
    };

    // Colored char type with short :
    template<typename TChar>
    using CCHAR = COLORCHAR<TChar, unsigned short>;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//          COLORED STRING CLASS    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


    template<typename TChar>
    class IColorString : public std::vector<CCHAR<TChar>>
    {
    public:
        using char_type = TChar;
        using container_type = std::vector;
        using value_type = CCHAR<TChar> ;

        using std::vector<CCHAR<TChar>>::vector;

        IColorString() {}

        //IColorString(std::basic_string_view<TChar> str)
        //    :IColorString(std::basic_string<TChar>(str), std::vector<int>{})
        //{}

        IColorString(const std::basic_string<TChar>& str)
            :IColorString(str, std::vector<int>{})
        {}

        IColorString(const std::basic_string<TChar>& str, const std::vector<int>& colors)
        {
            set_stringAndColor(str, colors);
        }
        
        IColorString(const std::basic_string<TChar>& str,const int& color_fg,const int& color_bg)
        {
           set_stringAndColor(str, std::vector<int>(str.size(), color_fg | color_bg << 4 ));
        }

        void set_string(const std::basic_string<TChar>& _str) {
            // this will clear vector from any character and reset new one 
            this->clear();
            this->resize(_str.size());
            int i{};
            for (auto& e : *this)
            {
                e.Char = _str[i++];
                e.Attributes = COLOR::BG_BLACK | COLOR::FG_WHITE;
            }
        }

        void set_string(const TChar& _tchar) {
            std::basic_string<TChar> _str(1,_tchar);
            set_string(_str);
        }

        void add_string(const std::basic_string<TChar>& _str) {
            this->reserve(this->size() + _str.size() + 1);
            for (const auto& c : _str) {
                value_type e{};
                e.Char = c;
                e.Attributes = COLOR::BG_BLACK | COLOR::FG_WHITE;
                this->push_back(e);
            }
        }

        void set_size(size_t size) {
            this->resize(size);
        }

        void color_fg(size_t pos, int color) {
            this->at(pos).Attributes = color;
        }

        void color_bg(size_t pos, int color) {
            this->at(pos).Attributes = color << 4;
        }

        std::vector<int> color_fg() const {
            std::vector<int> colors;
            extract_color(colors, +[](int color)->int {return KA_LOCT(color); });
            return colors;
        }

        std::vector<int> color_bg() const {
            std::vector<int> colors;
            extract_color(colors, +[](int color)->int {return KA_HOCT(color); });
            return colors;
        }

        std::vector<int> get_attributes() const {
            std::vector<int> attributes(this->size(),0);
            int i{};
            for (const auto& c : *this) attributes[i++] = c.Attributes;
            return attributes;
        }

        template<typename T>
        void color_fg(T colorfg) {
            _color_fg = static_cast<int>(colorfg);
            for (auto& e : *this)
                e.Attributes = _color_fg | _color_bg;
        }

        template<typename T>
        void color_bg(T colorbg) {
            _color_bg = static_cast<int>(colorbg) << 4;
            for (auto& e : *this)
                e.Attributes = _color_fg | _color_bg;
        }


        std::basic_string<TChar> string() const {
            std::basic_string<TChar> string(this->size(), TChar(0));
            int i{};
            for (auto& e : *this) {
                string[i] = e.Char;
                ++i;
            }
            return string;
        }
        
        virtual ~IColorString() = default;
    protected:

        // set string with vector of colors
        void set_stringAndColor(const std::basic_string<TChar>& str, const  std::vector<int>& colors)
        {
            // fill up 
            int i{};
            for (const auto& e : str) {
                value_type  gc{};
                if (i < colors.size())
                    gc.Attributes = colors[i];
                else
                    gc.Attributes = COLOR::BG_BLACK | COLOR::FG_WHITE;
                ++i;
                gc.Char = e;
                this->push_back(gc);
            }
        }
   
    private:

        void extract_color(std::vector<int>& colors, int(*f)(int)) {
            colors.clear();
            colors.resize(this->size());
            int i{};
            //for (auto& e : colors)
            //    e = f((this->at(i++)).Attributes);
            for (const auto& e : *this)
                colors[i++] = f(int(e.Attributes));
        }

        const value_type Char_Null = { .Char = (TChar)(0), .Attributes = 0 };

        int _color_fg{};
        int _color_bg{};

    };// End ColorString


}

