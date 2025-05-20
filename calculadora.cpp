#include <windows.h>
#include <string>
#include <sstream>

#define ID_EDIT1 1
#define ID_EDIT2 2
#define ID_RESULT 3
#define ID_ADD 4
#define ID_SUB 5
#define ID_MUL 6
#define ID_DIV 7

class Calculadora {
public:
    Calculadora(HINSTANCE hInstance);
    int Run(int nCmdShow);

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void OnCreate(HWND hwnd);
    void OnCommand(HWND hwnd, WPARAM wParam);

    static Calculadora* instancia;

    HWND hwndMain;
    HWND hEdit1, hEdit2, hResult;
    HINSTANCE hInst;
};

Calculadora* Calculadora::instancia = nullptr;

Calculadora::Calculadora(HINSTANCE hInstance) : hInst(hInstance), hwndMain(NULL), hEdit1(NULL), hEdit2(NULL), hResult(NULL) {
    instancia = this;
}

int Calculadora::Run(int nCmdShow) {
    const char CLASS_NAME[] = "CalculadoraC";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    hwndMain = CreateWindowExA(
        0,
        CLASS_NAME,
        "Calculadora C++ - GUI OO",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 250,
        NULL, NULL, hInst, NULL
    );

    if (!hwndMain) return 0;

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Calculadora::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            if (instancia) instancia->OnCreate(hwnd);
            break;

        case WM_COMMAND:
            if (instancia) instancia->OnCommand(hwnd, wParam);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void Calculadora::OnCreate(HWND hwnd) {
    CreateWindowA("STATIC", "Número 1:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, hInst, NULL);
    hEdit1 = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 100, 20, hwnd, (HMENU)ID_EDIT1, hInst, NULL);

    CreateWindowA("STATIC", "Número 2:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, hInst, NULL);
    hEdit2 = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 100, 20, hwnd, (HMENU)ID_EDIT2, hInst, NULL);

    CreateWindowA("BUTTON", "+", WS_VISIBLE | WS_CHILD, 20, 90, 30, 30, hwnd, (HMENU)ID_ADD, hInst, NULL);
    CreateWindowA("BUTTON", "-", WS_VISIBLE | WS_CHILD, 60, 90, 30, 30, hwnd, (HMENU)ID_SUB, hInst, NULL);
    CreateWindowA("BUTTON", "*", WS_VISIBLE | WS_CHILD, 100, 90, 30, 30, hwnd, (HMENU)ID_MUL, hInst, NULL);
    CreateWindowA("BUTTON", "/", WS_VISIBLE | WS_CHILD, 140, 90, 30, 30, hwnd, (HMENU)ID_DIV, hInst, NULL);

    hResult = CreateWindowA("STATIC", "Resultado:", WS_VISIBLE | WS_CHILD, 20, 140, 300, 20, hwnd, (HMENU)ID_RESULT, hInst, NULL);
}

void Calculadora::OnCommand(HWND hwnd, WPARAM wParam) {
    if (wParam >= ID_ADD && wParam <= ID_DIV) {
        char buffer1[100], buffer2[100];
        GetWindowTextA(hEdit1, buffer1, 100);
        GetWindowTextA(hEdit2, buffer2, 100);

        float num1 = static_cast<float>(atof(buffer1));
        float num2 = static_cast<float>(atof(buffer2));
        std::ostringstream oss;

        switch (wParam) {
            case ID_ADD:
                oss << "Resultado: " << num1 << " + " << num2 << " = " << num1 + num2;
                break;
            case ID_SUB:
                oss << "Resultado: " << num1 << " - " << num2 << " = " << num1 - num2;
                break;
            case ID_MUL:
                oss << "Resultado: " << num1 << " * " << num2 << " = " << num1 * num2;
                break;
            case ID_DIV:
                if (num2 == 0)
                    oss << "Erro: divisão por zero!";
                else
                    oss << "Resultado: " << num1 << " / " << num2 << " = " << num1 / num2;
                break;
        }

        SetWindowTextA(hResult, oss.str().c_str());
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    Calculadora calc(hInstance);
    return calc.Run(nCmdShow);
}
