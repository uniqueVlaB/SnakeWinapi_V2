#include "Classes.h"

#define MAX_LOADSTRING 100
int sizze = 20; // розмір елементу ігрового поля
int shift = sizze + 1 //відстань, на якій потрібно малювати наступний елемент поля
, left = 10, top = 10, right = left + sizze, bottom = top + sizze // координати першого елемента поля
, cxClient, cyClient, //розміри клієнтської області вікна 
lenght = 3,//початкова довжина змії
colorPart = 0, //необхідна для вибору кольору певного елементу гри
delay = 500 //значенння затримки перемальовування за умовчуванням
, score = 0; //рахунок
bool created = false,//флаг створення ігрового поля та змії
eaten = true,//флаг створення яблука
pause = true;//флаг паузи
wstring str;//строка виводу результатів гри
HBRUSH bodyColor = CreateSolidBrush(0x99FF99),//колір тіла
headColor = CreateSolidBrush(0x336600),//колір голови
bkColor = CreateSolidBrush(0xE5E4E2),//колір ігрового поля
appleColor = CreateSolidBrush(0x1702fa);//колір яблука
Field field;//об`єкт классу Field
Snake snake;//об`єкт классу Snake
Apple apple;//об`єкт классу Apple
int IDT_Timer;//ідентификатор таймеру перемальовування вікна
HWND hWnd;//дескриптор вікна програми
HINSTANCE hInst;//дескриптор програми                                
WCHAR szTitle[MAX_LOADSTRING];     //заголовок вікна             
WCHAR szWindowClass[MAX_LOADSTRING];//назва класу вікна            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKEWINAPIV2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEWINAPIV2));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEWINAPIV2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKEWINAPIV2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CHOOSECOLOR cc = { 0 };
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hWnd;
    COLORREF cust_colors[16] = { 0 };
    cc.lpCustColors = cust_colors;
    switch (message)
    {
    case WM_CREATE:
        srand(static_cast<unsigned int>(time(0)));
        SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
        break;
    case WM_TIMER:
            InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {

        case VK_UP:
            if (!pause) 
            snake.changeDirection(2);
            break;
        case VK_DOWN:
            if (!pause)
            snake.changeDirection(3);
            break;
        case VK_RIGHT:
            if (!pause)
            snake.changeDirection(0);
            break;
        case VK_LEFT:
            if (!pause)
            snake.changeDirection(1);
            break;
            
        case VK_BACK:
        {HDC hdc = GetDC(hWnd);
        field.set(left, top, right, bottom, shift, cxClient, cyClient, hWnd);
        field.setBK(bkColor, hdc);
        snake.create(field.getCoords(), lenght, hdc, headColor, bodyColor);
        apple.create(hdc, field.getCoords(), appleColor, snake.getSnake());
        ReleaseDC(hWnd, hdc);
        }
            break;
        }
        break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_PAUSE:
                pause = !pause;
                break;
            case IDM_scale_1:
                InvalidateRect(hWnd, NULL, TRUE);
                sizze = 20;
                shift = sizze + 1, left = 10, top = 10, right = left + sizze, bottom = top + sizze;
                created = false;
                eaten = true;
                break;
            case IDM_scale_1_5:
                InvalidateRect(hWnd, NULL, TRUE);
                sizze = 20 * 1.5;
                shift = sizze + 1, left = 10, top = 10, right = left + sizze, bottom = top + sizze;
                created = false;
                eaten = true;
                break;
            case IDM_scale_2:
                InvalidateRect(hWnd, NULL, TRUE);
                sizze = 20 * 2;
                shift = sizze + 1, left = 10, top = 10, right = left + sizze, bottom = top + sizze;
                created = false;
                eaten = true;
                break;
            case IDM_scale_2_5:
                InvalidateRect(hWnd, NULL, TRUE);
                sizze = 20 * 2.5;
                shift = sizze + 1, left = 10, top = 10, right = left + sizze, bottom = top + sizze;
                created = false;
                eaten = true;
                break;
            case IDM_scale_3:
                InvalidateRect(hWnd, NULL, TRUE);
                sizze = 20 * 3;
                shift = sizze + 1, left = 10, top = 10, right = left + sizze, bottom = top + sizze;
                created = false;
                eaten = true;
                break;
            case IDM_bodyColor:
                colorPart = 1;
                break;
            case IDM_headColor:
                colorPart = 2;
                break;
            case IDM_bkColor:
                colorPart = 3;
                break;
            case IDM_appleColor:
                colorPart = 4;
                break;
            case IDM_speed_1:
                delay = 250;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case IDM_speed_1_5:
                delay = 250/1.5;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case IDM_speed_2:
                delay = 250 / 2;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case IDM_speed_2_5:
                delay = 250 / 2.5;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case IDM_speed_3:
                delay = 250 / 3;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

            switch (colorPart)
            {
            case 1:
                KillTimer(hWnd, IDT_Timer);
                ChooseColor(&cc);
                bodyColor = CreateSolidBrush(cc.rgbResult);
                colorPart = 0;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case 2:
                KillTimer(hWnd, IDT_Timer);
                ChooseColor(&cc);
                headColor = CreateSolidBrush(cc.rgbResult);
                colorPart = 0;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case 3:
                KillTimer(hWnd, IDT_Timer);
                ChooseColor(&cc);
                bkColor = CreateSolidBrush(cc.rgbResult);
                field.setBK(bkColor, hdc);
                apple.paint(hdc, field.getCoords(), appleColor);
                colorPart = 0;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            case 4:
                KillTimer(hWnd, IDT_Timer);
                ChooseColor(&cc);
                appleColor = CreateSolidBrush(cc.rgbResult);
                apple.paint(hdc, field.getCoords(), appleColor);
                colorPart = 0;
                SetTimer(hWnd, IDT_Timer, delay, (TIMERPROC)NULL);
                break;
            }
        
        if (!created) {
            field.set(left, top, right, bottom, shift, cxClient, cyClient, hWnd);
            field.setBK(bkColor, hdc);
            snake.create(field.getCoords(), lenght, hdc, headColor, bodyColor);
            created = true;
        }
        if (!pause) {
            snake.move(hdc, field.getCoords(), hWnd, headColor, bodyColor, bkColor);
            if (eaten) {
                apple.create(hdc, field.getCoords(), appleColor, snake.getSnake());
                eaten = false;
            }
            else {
                if (apple.checkEaten(snake.getHead(field.getCoords()), field.getCoords(), hdc)) {
                    snake.add(hdc, bodyColor, field.getCoords());
                    eaten = true;
                    score++;
                }
            }
            if (snake.collided(field.getCoords(), hdc)) {
                pause = true;
                str = L"Ваш счет: ";
                str += to_wstring(score);
                MessageBox(hWnd, str.c_str() , L"Вы проиграли!", MB_ICONWARNING);
                created = false;
                eaten = true;
                score = 0;
            }

        }
        EndPaint(hWnd, &ps);
        UpdateWindow(hWnd);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


