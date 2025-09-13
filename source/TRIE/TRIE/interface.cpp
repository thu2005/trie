#include "all.h"

HWND hTextBox;

void AddControls(HWND hwnd) {
    // Tạo Label "Keyword:"
    CreateWindowW(L"Static", L"Keyword:", WS_VISIBLE | WS_CHILD,
        20, 20, 70, 20, hwnd, NULL, NULL, NULL);

    // Tạo Edit control để người dùng nhập từ khóa
    hTextBox = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        100, 20, 200, 20, hwnd, (HMENU)ID_EDIT_INPUT, NULL, NULL);

    // Tạo nút Search
    CreateWindowW(L"Button", L"Search", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        320, 20, 80, 20, hwnd, (HMENU)ID_BUTTON_SEARCH, NULL, NULL);

    // Tạo nút Add
    CreateWindowW(L"Button", L"Add", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        410, 20, 80, 20, hwnd, (HMENU)ID_BUTTON_ADD, NULL, NULL);

    // Tạo nút Delete
    CreateWindowW(L"Button", L"Delete", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        500, 20, 80, 20, hwnd, (HMENU)ID_BUTTON_DELETE, NULL, NULL);

    // Tạo ListBox để hiển thị kết quả
    CreateWindowW(L"ListBox", NULL,
        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL,
        20, 60, 560, 300, hwnd, (HMENU)ID_LIST_RESULTS, NULL, NULL);

    // Tạo nút Time (ngay cạnh nút Delete)
    CreateWindowW(L"Button", L"Time", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        590, 20, 80, 20, hwnd, (HMENU)ID_BUTTON_TIME, NULL, NULL);


}




void HandleSearchButton(HWND hwnd) {
    wchar_t inputText[256];
    GetDlgItemText(hwnd, ID_EDIT_INPUT, inputText, 256);

    if (wcslen(inputText) == 0) {
        DisplayMessage(hwnd, L"Please enter a keyword to search.", L"Warning");
        return;
    }

    // Thuc hien logic tim kiem
    SendMessage(GetDlgItem(hwnd, ID_LIST_RESULTS), LB_RESETCONTENT, 0, 0); // Xoa ket qua cu
    SendMessage(GetDlgItem(hwnd, ID_LIST_RESULTS), LB_ADDSTRING, 0, (LPARAM)L"Result 1");
    SendMessage(GetDlgItem(hwnd, ID_LIST_RESULTS), LB_ADDSTRING, 0, (LPARAM)L"Result 2");
}

void HandleAddButton(HWND hwnd) {
    wchar_t inputText[256];
    GetDlgItemText(hwnd, ID_EDIT_INPUT, inputText, 256);

    if (wcslen(inputText) == 0) {
        DisplayMessage(hwnd, L"Please enter a keyword to add.", L"Warning");
        return;
    }

    // Thuc hien logic them tu
    DisplayMessage(hwnd, L"Keyword added successfully.", L"Success");
}

void HandleDeleteButton(HWND hwnd) {
    wchar_t inputText[256];
    GetDlgItemText(hwnd, ID_EDIT_INPUT, inputText, 256);

    if (wcslen(inputText) == 0) {
        DisplayMessage(hwnd, L"Please enter a keyword to delete.", L"Warning");
        return;
    }

    // TThuc hien logic xoa tu
    DisplayMessage(hwnd, L"Keyword deleted successfully.", L"Success");
}

void DisplayMessage(HWND hwnd, LPCWSTR message, LPCWSTR title) {
    MessageBox(hwnd, message, title, MB_OK | MB_ICONINFORMATION);
}

