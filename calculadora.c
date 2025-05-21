#include <windows.h>
#include <stdio.h>

#define ID_DISPLAY 100
#define ID_BTN_0 200
#define ID_BTN_1 201
#define ID_BTN_2 202
#define ID_BTN_3 203
#define ID_BTN_4 204
#define ID_BTN_5 205   
#define ID_BTN_6 206
#define ID_BTN_7 207
#define ID_BTN_8 208
#define ID_BTN_9 209
#define ID_BTN_ADD 210
#define ID_BTN_SUB 211
#define ID_BTN_MUL 212
#define ID_BTN_DIV 213
#define ID_BTN_EQ  214
#define ID_BTN_CLR 215

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hDisplay;
    static char operando[2][100] = {"", ""};
    static char operador = 0;
    static int fase = 0;
    static int resultadoMostrado = 0;  // Flag para indicar se um resultado está sendo mostrado

    switch (uMsg) {
        case WM_CREATE: {
            hDisplay = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
                                    20, 20, 220, 30, hwnd, (HMENU)ID_DISPLAY, NULL, NULL);

            const char *botoes[4][4] = {
                {"7", "8", "9", "/"},
                {"4", "5", "6", "*"},
                {"1", "2", "3", "-"},
                {"C", "0", "=", "+"}
            };

            int idMap[4][4] = {
                {ID_BTN_7, ID_BTN_8, ID_BTN_9, ID_BTN_DIV},
                {ID_BTN_4, ID_BTN_5, ID_BTN_6, ID_BTN_MUL},
                {ID_BTN_1, ID_BTN_2, ID_BTN_3, ID_BTN_SUB},
                {ID_BTN_CLR, ID_BTN_0, ID_BTN_EQ, ID_BTN_ADD}
            };

            int startX = 20, startY = 70;
            int btnW = 50, btnH = 40;
            int padding = 10;

            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    CreateWindow("BUTTON", botoes[row][col], WS_VISIBLE | WS_CHILD | BS_CENTER,
                                 startX + col * (btnW + padding), startY + row * (btnH + padding),
                                 btnW, btnH,
                                 hwnd, (HMENU)idMap[row][col], NULL, NULL);
                }
            }
            break;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);

            if (id >= ID_BTN_0 && id <= ID_BTN_9) {
                if (resultadoMostrado) {
                    // Limpar tudo se um resultado estava sendo mostrado
                    operando[0][0] = operando[1][0] = '\0';
                    operador = 0;
                    fase = 0;
                    resultadoMostrado = 0;
                }

                int digit = id - ID_BTN_0;
                char *atual = operando[fase];
                int len = strlen(atual);
                if (len < 99) {
                    atual[len] = '0' + digit;
                    atual[len + 1] = '\0';
                    SetWindowText(hDisplay, atual);
                }
            } else if (id == ID_BTN_ADD || id == ID_BTN_SUB || id == ID_BTN_MUL || id == ID_BTN_DIV) {
                if (strlen(operando[0]) == 0) break;
                operador = (id == ID_BTN_ADD) ? '+' :
                           (id == ID_BTN_SUB) ? '-' :
                           (id == ID_BTN_MUL) ? '*' : '/';
                fase = 1;
                SetWindowText(hDisplay, "");
                resultadoMostrado = 0;
            } else if (id == ID_BTN_EQ) {
                if (strlen(operando[0]) && strlen(operando[1]) && operador) {
                    float n1 = atof(operando[0]);
                    float n2 = atof(operando[1]);
                    char resultado[100];

                    if (operador == '/' && n2 == 0) {
                        strcpy(resultado, "Erro: divisao por zero!");
                    } else {
                        float r = 0;
                        if (operador == '+') r = n1 + n2;
                        else if (operador == '-') r = n1 - n2;
                        else if (operador == '*') r = n1 * n2;
                        else if (operador == '/') r = n1 / n2;
                        sprintf(resultado, "%.2f", r);
                    }

                    SetWindowText(hDisplay, resultado);
                    strcpy(operando[0], resultado);
                    operando[1][0] = '\0';
                    fase = 0;
                    operador = 0;
                    resultadoMostrado = 1;
                }
            } else if (id == ID_BTN_CLR) {
                operando[0][0] = operando[1][0] = '\0';
                operador = 0;
                fase = 0;
                resultadoMostrado = 0;
                SetWindowText(hDisplay, "");
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "CalculadoraAjustada";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Calculadora", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 330,
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
