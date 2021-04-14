#include <omp.h>
#include <windows.h>
#include <cmath>
#include <string>
#include <iostream>

#include "IntegralSolver.h"
#include "Function.h"

#define PI 3.1415926535
#define MAX_THREADS 20
#define MIN_THREADS 1
#define MIN_STEPS 10
//#define TESTS

#ifdef TESTS
extern void run_tests();
#endif

Function function1([](double x) { return 1/pow(sin(2 * x), 2); });
Function function2([](double x) { return pow(sin(2 * x), 2); });
IntegralSolver solver(function2);

double a = 0;
double b = PI / 2;

HWND hwnd;

HWND input_steps;
HWND input_threads_count;
HWND btn_start;
HWND label_result;

void start_computation()
{
    TCHAR buff[512];
    GetWindowText(input_steps, buff, 512);
    int steps = max(_wtoi(buff), MIN_STEPS);

    GetWindowText(input_threads_count, buff, 512);
    int num_of_threads = min(max(_wtoi(buff), MIN_THREADS), MAX_THREADS);

    EnableWindow(btn_start, false);

    omp_set_num_threads(num_of_threads);
    double time = omp_get_wtime();
    double integral = solver.solve(a, b, steps);
    time = omp_get_wtime() - time;
    
    std::wstring result = L"Integral result: ";
    result += std::to_wstring(integral);
    result += L"\nElapsed time: ";
    result += std::to_wstring(time);

    SetWindowText(label_result, result.c_str());
    EnableWindow(btn_start, true);
}

void init_window(HINSTANCE hInst)
{
    CreateWindow(L"Static", L"Input number of steps:", WS_VISIBLE | WS_CHILD, 10, 10, 100, 50, hwnd, NULL, hInst, NULL);
    input_steps = CreateWindow(L"Edit", L"10000", WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 120, 10, 100, 50, hwnd, NULL, hInst, NULL);

    CreateWindow(L"Static", L"Input number of threads:", WS_VISIBLE | WS_CHILD, 10, 70, 100, 50, hwnd, NULL, hInst, NULL);
    input_threads_count = CreateWindow(L"Edit", L"8", WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 120, 70, 100, 50, hwnd, NULL, hInst, NULL);

    btn_start = CreateWindow(L"Button", L"Start", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, 10, 130, 60, 65, hwnd, NULL, hInst, NULL);

    label_result = CreateWindow(L"Static", L"", WS_VISIBLE | WS_CHILD, 80, 130, 140, 65, hwnd, NULL, hInst, NULL);
}

LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        if (lparam != 0 && HIWORD(wparam) == BN_CLICKED)
        {
            HWND btn = (HWND)lparam;
            if (btn == btn_start)
            {
                start_computation();
            }
        }
        break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInst,
    HINSTANCE hPreviousInst,
    LPSTR lpCommandLine,
    int nCommandShow)
{
#ifdef TESTS
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    run_tests();
    FreeConsole();
#endif

    const wchar_t classname[] = L"My window";

    WNDCLASS wc = {};
    wc.lpfnWndProc = wndproc;
    wc.hInstance = hInst;
    wc.lpszClassName = classname;
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, classname, L"Lab 3 Buslaiev", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT, 245, 245, NULL, NULL, hInst, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Couldn't create the window", L"Error", MB_OK);
        return NULL;
    }

    init_window(hInst);

    ShowWindow(hwnd, nCommandShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    
    return NULL;
}
