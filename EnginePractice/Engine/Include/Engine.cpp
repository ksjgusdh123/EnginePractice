#include "Engine.h"
#include "GameFramework.h"
#include "SceneManager.h"
#include "CommandQueue.h"
#include "Timer.h"

DEFINITION_SINGLE(CEngine)
bool CEngine::m_loop = true;

CEngine::CEngine()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(100);
}

CEngine::~CEngine()
{
    m_gameFramework->OnDestroy();
    delete m_gameFramework;
    delete m_timer;
}

bool CEngine::Init(HINSTANCE hInst, int windowWidth, int windowHeight)
{
    m_hInst = hInst;

    m_resolution.width = windowWidth;
    m_resolution.height = windowHeight;

    Register();
    Create();

    if (!CSceneManager::GetInst()->Init(m_gameFramework->GetDevice()))
        return false;

    m_timer = new CTimer();
    m_timer->Init();

    return true;
}

int CEngine::Run()
{
    MSG msg;

    while (m_loop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // TODO: 컨텐츠 로직
            Logic();
        }
    }

    return (int)msg.wParam;
}

CGameFramework* CEngine::GetFramework()
{
    return m_gameFramework;
}

void CEngine::Logic()
{
    m_timer->Update();

    float elapsedTime = m_timer->GetDeltaTime();
    // Scene이 교체될 경우 처음부터 다시 동작시킨다
    if (Update(elapsedTime))
        return;

    if (PostUpdate(elapsedTime))
        return;

    Render(elapsedTime);
}

bool CEngine::Update(float elapsedTime)
{
    ShowFps();
    return CSceneManager::GetInst()->Update(elapsedTime);
}

bool CEngine::PostUpdate(float elapsedTime)
{
    return CSceneManager::GetInst()->PostUpdate(elapsedTime);
}

void CEngine::Render(float elapsedTime)
{
    m_gameFramework->RenderBegin();
    CSceneManager::GetInst()->Render(m_gameFramework->GetcmdQueue()->GetCmdList(), elapsedTime);
    m_gameFramework->RenderEnd();
}

void CEngine::Register()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ENGINE));    // 실행파일에 사용할 아이콘 등록
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // 마우스 커서 모양
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY220428);  // 메뉴를 사용할 것인지
    wcex.lpszClassName = TEXT("EnginePractice");
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));   // 윈도우창 좌상단에 표시할 작은 아이콘

    RegisterClassExW(&wcex);
}

bool CEngine::Create()
{
    m_hWnd = CreateWindowW(TEXT("EnginePractice"), TEXT("EnginePractice"), WS_OVERLAPPEDWINDOW, 
        100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
        return false;

    m_gameFramework = new CGameFramework(m_hInst, m_hWnd, m_resolution.width, m_resolution.height);
    m_gameFramework->Init();

    RECT    rc = { 0, 0, m_resolution.width, m_resolution.height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    MoveWindow(m_hWnd, 50, 50, rc.right - rc.left, rc.bottom - rc.top, TRUE);   // 윈도우 창의 크기를 변경한다.
    ShowWindow(m_hWnd, SW_SHOW);    // 윈도우 창을 보여준다.

    // 클라이언트 영역 갱신에 실패했을 경우 0을 반환한다.
    if (!UpdateWindow(m_hWnd))
        return false;

    return true;
}

void CEngine::ShowFps()
{
    UINT fps = m_timer->GetFps();

    WCHAR text[100] = L"";
    ::wsprintf(text, L"FPS : %d", fps);

    ::SetWindowText(m_hWnd, text);
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // 윈도우가 종료될때 들어오는 메세지이다.
        m_loop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
