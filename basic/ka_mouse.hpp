#ifndef KA_MOUSE_HPP
#define KA_MOUSE_HPP
#include <console project/console/ka_utility.hpp>

namespace console {

    /// @brief A mouse event. It contains the coordinate of the mouse, the button
    /// pressed and the modifier (shift, ctrl, meta).
    /// @ingroup component
    /// Make it as template 
    /// with coordinate structure
    struct Mouse {
        enum Button {
            Left = 0,
            Middle = 1,
            Right = 2,
            None = 3,
            WheelUp = 4,
            WheelDown = 5,
            Left2 = 6,
            Left3 = 7,
            Left4 = 8,
            Shift = 9,
            LCtrl = 10,
            RCtrl = 11,
            LAlt = 12,
            RAlt = 13,
            CapsLock_On = 14,
            NumsLock_On = 15,
            ScrlLock_On = 16,
            Enhanced_key = 17,
        };

        short VK_Code[18] = {
            short(VK_LBUTTON),
            short(VK_MBUTTON)  ,
            short(VK_RBUTTON)  ,
            short(0) ,
            short(-1),
            short(-2),
            short(-3),
            short(-4),
            short(-5),
            short(VK_SHIFT    ),
            short(VK_LCONTROL ),
            short(VK_RCONTROL ),
            short(VK_LMENU    ),
            short(VK_RMENU    ),
            short(VK_CAPITAL  ),
            short(VK_NUMLOCK  ),
            short(VK_SCROLL   ),
            short(0)
        };

        short to_VKcode(Button button) const {
            if (button < 18)
            return VK_Code[static_cast<int>(button)];
        }

        enum Motion {
            Released = 0,
            Pressed = 1,
            Undefined = 2,
        };

        enum Event {
            Nothing = 0,
            Moved = 2,
            DoubleClick = 3,
            
            HWheel = 4,
            VWheel = 5,
        };


        // Button
        Button button = Button::None;

        // Motion
        Motion motion = Motion::Pressed;

        // Event
        Event event = Event::Nothing;

        // Modifiers:
        bool shift = false;
        bool meta = false;
        bool control = false;

        // Coordinates:
        int x = 0;
        int y = 0;

        // Static function 
        static bool is_buttonPressed(Button button) {
            
            return button == Button::Left   || button == Button::Right
                || button == Button::Middle || button == Button::Left2
                || button == Button::Left3  || button == Button::Left4;
        }

        
        //static iVec2 get_position() {
        //   return iVec2( konsole->get_mouseXY());
        //}

        static void set_position() {
            // set mouse position 
        }
    };


    void process_mouse_event(Mouse& mouse, const MOUSE_EVENT_RECORD& mer)
    {
        // coordinate
        mouse.x = mer.dwMousePosition.X;
        mouse.y = mer.dwMousePosition.Y;

        // dwButtonState
        switch (mer.dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED: mouse.button = Mouse::Button::Left;
            break;
        case FROM_LEFT_2ND_BUTTON_PRESSED: mouse.button = Mouse::Button::Left2;
            break;
        case FROM_LEFT_3RD_BUTTON_PRESSED: mouse.button = Mouse::Button::Left3;
            break;
        case FROM_LEFT_4TH_BUTTON_PRESSED: mouse.button = Mouse::Button::Left4;
            break;
        case RIGHTMOST_BUTTON_PRESSED: mouse.button = Mouse::Button::Right;
            break;
        //default:
        //    mouse.motion = Mouse::Motion::Released;
        }

        // dwControlKeyState
        switch (mer.dwControlKeyState) {
        case CAPSLOCK_ON: mouse.button = Mouse::Button::CapsLock_On;
            break;
        case ENHANCED_KEY:mouse.button = Mouse::Button::Enhanced_key;
            break;
        case LEFT_ALT_PRESSED:mouse.button = Mouse::Button::LAlt;
            break;
        case LEFT_CTRL_PRESSED:mouse.button = Mouse::Button::LCtrl;
            break;
        case NUMLOCK_ON:mouse.button = Mouse::Button::NumsLock_On;
            break;
        case RIGHT_ALT_PRESSED:mouse.button = Mouse::Button::RAlt;
            break;
        case RIGHT_CTRL_PRESSED:mouse.button = Mouse::Button::RCtrl;
            break;
        case SCROLLLOCK_ON:mouse.button = Mouse::Button::ScrlLock_On;
            break;
        case SHIFT_PRESSED:mouse.button = Mouse::Button::Shift;
            break;
        }

        // dwEventFlags
        switch (mer.dwEventFlags) {
        case DOUBLE_CLICK: mouse.event = Mouse::Event::DoubleClick;
            break;
        case MOUSE_HWHEELED:mouse.event = Mouse::Event::HWheel;
            break;
        case MOUSE_MOVED:mouse.event = Mouse::Event::Moved;
            break;
        case MOUSE_WHEELED:mouse.event = Mouse::Event::VWheel;
            break;
       
        }

        mouse.shift = mouse.button == Mouse::Button::Shift;
        mouse.control = mouse.button == Mouse::Button::LCtrl ||
            mouse.button == Mouse::Button::RCtrl;

       /* if (mer.dwButtonState == 0)
        {
			if (mer.dwEventFlags == 0)
				mouse.motion = Mouse::Motion::Released;
        }*/
    }

}  // namespace console

// this copied 100 % from ftxui project of Arthur Sonzogni.
// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#endif /* end of include guard: KA_MOUSE_HPP */
