#include "all.h"

//long long count_compare = 0;

#if 1
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
TrieNode* root = createNode();
int k = 1;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   // loadDictionary(root, "words.txt");
    const wchar_t CLASS_NAME[] = L"TRIE Application";

    // Cau hinh cua so
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;  // Con tro ham xu ly thong diep
    wc.hInstance = hInstance;    // Handle cua ung dung
    wc.lpszClassName = CLASS_NAME; // Ten cua so

    // Dang ky lop cua cua so
    RegisterClass(&wc);

    // Tao cua so chinh
    HWND hwnd = CreateWindowEx(
        0,                  // Khong co kieu mo rong
        CLASS_NAME,         // Ten cua so
        L"TRIE Application",// Tieu de cua so
        WS_OVERLAPPEDWINDOW,// Kieu cua so
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, // Vi tri, kich thuoc
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0; // Kiem tra loi khi tao cua so

    // Hien thi cua so
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if (!loadDictionary(root, "words.txt")) {
        MessageBox(hwnd, L"Failed to load dictionary.", L"Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0); 
        return 0; 
    }
    

    // Vong lap thong diep chinh
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // Xu ly thong diep ban phim
        DispatchMessage(&msg);  // Gui thong diep toi ham WindowProc
    }

    return 0;
}

// Xu ly thong diep cua so
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Khai bao cac handle cho cac thanh phan giao dien
    static HWND hTextBox, hButtonSearch, hButtonAdd, hButtonDelete, hListBox, hButtonCheck, hButtonTime;

    switch (uMsg) {


    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);

        HBRUSH hBrush = CreateSolidBrush(RGB(230, 240, 250)); // Màu nền xanh nhạt (Alice Blue)
        FillRect(hdc, &rc, hBrush);

        DeleteObject(hBrush); // Xóa brush
        return 1; // Báo đã xử lý xong
    }


    case WM_CTLCOLOREDIT: { // Textbox
        HDC hdcEdit = (HDC)wParam;
        SetTextColor(hdcEdit, RGB(0, 0, 139)); // Màu chữ xanh đậm
        SetBkColor(hdcEdit, RGB(255, 255, 255)); // Nền trắng
        return (INT_PTR)GetStockObject(WHITE_BRUSH); // Brush tĩnh không cần xóa
    }

    case WM_CTLCOLORLISTBOX: { // Listbox
        HDC hdcList = (HDC)wParam;
        SetTextColor(hdcList, RGB(0, 100, 0)); // Màu chữ xanh lá cây đậm
        SetBkColor(hdcList, RGB(240, 255, 240)); // Nền xanh lá nhạt
        HBRUSH hBrush = CreateSolidBrush(RGB(240, 255, 240));
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)hBrush);
        return (INT_PTR)hBrush;
    }
    

    //case WM_CTLCOLORBTN: {
    //    HDC hdcButton = (HDC)wParam;
    //    SetBkColor(hdcButton, RGB(100, 149, 237)); // Màu nền xanh dương nhạt
    //    SetTextColor(hdcButton, RGB(255, 255, 255)); // Màu chữ trắng
    //    SetBkMode(hdcButton, TRANSPARENT); // Đảm bảo nền trong suốt

    //    // Tạo brush mới cho button
    //    HBRUSH hBrush = CreateSolidBrush(RGB(100, 149, 237)); // Tạo brush mới cho button
    //    return (INT_PTR)hBrush; // Trả về brush cho button
    //}


    
    

    case WM_CREATE: {
        // Tao Textbox
        hTextBox = CreateWindow(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER, // Kieu TextBox
            20, 20, 300, 25,                  // Vi tri kich thuoc
            hwnd, (HMENU)ID_TEXTBOX, NULL, NULL);

        // Tạo nút Search
        hButtonSearch = CreateWindow(L"BUTTON", L"Search",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Kieu nut bam
            340, 20, 100, 25,                                     // Vi tri kich thuoc
            hwnd, (HMENU)ID_BUTTON_SEARCH, NULL, NULL);

        // Tao nut Add
        hButtonAdd = CreateWindow(L"BUTTON", L"Add",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 60, 100, 25,
            hwnd, (HMENU)ID_BUTTON_ADD, NULL, NULL);

        // Tao nut Delete
        hButtonDelete = CreateWindow(L"BUTTON", L"Delete",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            140, 60, 100, 25,
            hwnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);

        // Tao Listbox hien thi ket qua
        hListBox = CreateWindow(L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, // Kieu Listbox
            20, 100, 420, 200,                    // Vi tri kich thuoc
            hwnd, (HMENU)ID_LISTBOX, NULL, NULL);

        // Tao nut Check Word
        hButtonCheck = CreateWindow(L"BUTTON", L"Check Word",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Kiểu nút bấm
            260, 60, 100, 25,                                     // Vị trí và kích thước
            hwnd, (HMENU)ID_BUTTON_CHECK, NULL, NULL);

        // Tạo nút Time
        hButtonTime = CreateWindow(L"BUTTON", L"Time",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Kiểu nút bấm
            380, 60, 100, 25,                                     // Vị trí và kích thước
            hwnd, (HMENU)ID_BUTTON_TIME, NULL, NULL);

        break;
    }
    case WM_COMMAND: {
        // Xu ly su kien khi nguoi dung nhan nut
        if (LOWORD(wParam) == ID_BUTTON_SEARCH) {
            // Lay tu khoa prefix tu Textbox
            wchar_t buffer[256];
            GetWindowText(hTextBox, buffer, 256);
            std::string prefix = std::string(buffer, buffer + wcslen(buffer));

            count_compare = 0;

            // Tim kiem tu voi prefix
            std::vector<std::string> results;
            double time = suggestWordsTime(root, prefix, results, k);

            SendMessage(hListBox, LB_RESETCONTENT, 0, 0);  // Xoa noi dung cu trong Listbox
            
            /*std::wstring timeInfo = L"Time: " + std::to_wstring(time) + L" microseconds";
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)timeInfo.c_str());

            std::wstring compareInfo = L"Compare: " + std::to_wstring(count_compare);
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)compareInfo.c_str());*/

            for (const string& word : results) {
                SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)std::wstring(word.begin(), word.end()).c_str());
            }
        }

        else if (LOWORD(wParam) == ID_BUTTON_CHECK) {
            wchar_t buffer[256];
            GetWindowText(hTextBox, buffer, 256);
            std::string word = std::string(buffer, buffer + wcslen(buffer));

            bool found = search(root, word);
            if (found) {
                MessageBox(hwnd, L"The word exists in the dictionary!", L"Check Word", MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBox(hwnd, L"The word is NOT in the dictionary!", L"Check Word", MB_OK | MB_ICONWARNING);
            }
        }

        else if (LOWORD(wParam) == ID_BUTTON_ADD) {
            // Lay tu can them tu Textbox
            wchar_t buffer[256];
            GetWindowText(hTextBox, buffer, 256);
            std::string word = std::string(buffer, buffer + wcslen(buffer));

            std::string message = addNewWord(root, word);
            MessageBox(hwnd, std::wstring(message.begin(), message.end()).c_str(), L"Add Word", MB_OK);
        }

        else if (LOWORD(wParam) == ID_BUTTON_DELETE) {
            // Lấy từ cần xóa từ TextBox
            wchar_t buffer[256];
            GetWindowText(hTextBox, buffer, 256);
            std::string word = std::string(buffer, buffer + wcslen(buffer));

            std::string message = deleteWord(root, word);
            MessageBox(hwnd, std::wstring(message.begin(), message.end()).c_str(), L"Delete Word", MB_OK);
        }

        else if (LOWORD(wParam) == ID_BUTTON_TIME) {
            vector<string>prefixTest;
            loadPrefixTest("prefixTest1.txt", prefixTest);
            vector<double>timeEachPrefix;
            vector<long long>cnt_cp;
            double time = averageTime(root, prefixTest, k, timeEachPrefix, cnt_cp);

            // Xóa nội dung cũ trong ListBox
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

            // Thêm thông tin thời gian vào ListBox
            std::wstring timeInfo = L"Average Time: " + std::to_wstring(time) + L" microseconds";
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)timeInfo.c_str());

            // Thêm thông tin PrefixTest vào ListBox
            //std::wstring prefixInfo = L"PrefixTest Info:";
            //SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)prefixInfo.c_str());

            std::wstringstream header;
            header << std::left << std::setw(20) << "PREFIX "
                << std::setw(20) << "TIME "
                << std::setw(20) << "COMPARISONS ";

            // Thêm vào ListBox
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)header.str().c_str());

            for (int i = 0; i < prefixTest.size(); ++i) {
                // Chuyển đổi từ sang wide string
                std::wstring prefix(prefixTest[i].begin(), prefixTest[i].end());

                // Định dạng kết quả với độ rộng cố định cho mỗi cột
                std::wstringstream lineStream;
                lineStream << std::left << std::setw(20) << prefix  
                    << std::setw(20) << to_wstring(timeEachPrefix[i])   
                    << std::setw(20) << to_wstring(cnt_cp[i]);         

                // Thêm vào ListBox
                SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)lineStream.str().c_str());
            }

        }
        break;
    }
    case WM_DESTROY: {
        Release(root);

        HBRUSH hBrush = (HBRUSH)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (hBrush) {
            DeleteObject(hBrush);
        }

        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
#endif