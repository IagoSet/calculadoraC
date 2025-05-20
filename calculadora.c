#include <windows.h>
#include <stdio.h>

#define ID_EDIT1 1
#define ID_EDIT2 2
#define ID_RESULT 3
#define ID_ADD 4
#define ID_SUB 5
#define ID_MUL 6
#define ID_DIV 7

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit1, hEdit2, hResult;

    switch (uMsg) {
        case WM_CREATE:
            CreateWindow("STATIC", "Numero 1:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hwnd, NULL, NULL, NULL);
            hEdit1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 20, 100, 20, hwnd, (HMENU)ID_EDIT1, NULL, NULL);

            CreateWindow("STATIC", "Numero 2:", WS_VISIBLE | WS_CHILD, 20, 50, 80, 20, hwnd, NULL, NULL, NULL);
            hEdit2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 50, 100, 20, hwnd, (HMENU)ID_EDIT2, NULL, NULL);

            CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD, 20, 90, 30, 30, hwnd, (HMENU)ID_ADD, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD, 60, 90, 30, 30, hwnd, (HMENU)ID_SUB, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD, 100, 90, 30, 30, hwnd, (HMENU)ID_MUL, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD, 140, 90, 30, 30, hwnd, (HMENU)ID_DIV, NULL, NULL);

            hResult = CreateWindow("STATIC", "Resultado: ", WS_VISIBLE | WS_CHILD, 20, 140, 300, 20, hwnd, (HMENU)ID_RESULT, NULL, NULL);
            break;

        case WM_COMMAND:
            if (wParam >= ID_ADD && wParam <= ID_DIV) {
                char buffer1[100], buffer2[100];
                GetWindowText(hEdit1, buffer1, 100);
                GetWindowText(hEdit2, buffer2, 100);
                float num1 = atof(buffer1);
                float num2 = atof(buffer2);
                char resultado[100];

                switch (wParam) {
                    case ID_ADD:
                        sprintf(resultado, "Resultado: %.2f + %.2f = %.2f", num1, num2, num1 + num2);
                        break;
                    case ID_SUB:
                        sprintf(resultado, "Resultado: %.2f - %.2f = %.2f", num1, num2, num1 - num2);
                        break;
                    case ID_MUL:
                        sprintf(resultado, "Resultado: %.2f * %.2f = %.2f", num1, num2, num1 * num2);
                        break;
                    case ID_DIV:
                        if (num2 == 0)
                            sprintf(resultado, "Erro: divisão por zero!");
                        else
                            sprintf(resultado, "Resultado: %.2f / %.2f = %.2f", num1, num2, num1 / num2);
                        break;
                }

                SetWindowText(hResult, resultado);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "CalculadoraC";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Calculadora C - GUI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 250,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
