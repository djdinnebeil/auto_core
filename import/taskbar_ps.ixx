/**
\file taskbar_ps.ixx
\brief A module for additional taskbar support.
*/
export module taskbar_ps;
import visual;
import <Windows.h>;

export extern HWND taskbar_ps_hwnd;

export {
    void run_taskbar_ps();
    void send_ctrl_alt_n();
    void send_ctrl_alt_p();
    void send_ctrl_alt_r();
    void send_ctrl_alt_x();
    void send_ctrl_alt_t();
    void send_ctrl_alt_l();
}
