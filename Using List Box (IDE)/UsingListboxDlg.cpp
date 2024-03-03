#include "UsingListboxDlg.h"

UsingListboxDlg::UsingListboxDlg()
{
    ptr = this;
}

BOOL CALLBACK UsingListboxDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        ptr->Cls_OnClose(hwnd);
        return TRUE;
    case WM_INITDIALOG:
        return ptr->Cls_OnInitDialog(hwnd, reinterpret_cast<HWND>(wParam), lParam);
    case WM_COMMAND:
        ptr->Cls_OnCommand(hwnd, LOWORD(wParam), reinterpret_cast<HWND>(lParam), HIWORD(wParam));
        return TRUE;
    }
    return FALSE;
}

BOOL UsingListboxDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    hList1 = GetDlgItem(hwnd, IDC_LIST1);
    hList2 = GetDlgItem(hwnd, IDC_LIST2);
    hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
    hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
    hEdit3 = GetDlgItem(hwnd, IDC_EDIT3);


    const TCHAR* items[] = { TEXT("Милан Италия"), TEXT("Ливерпуль Англия"), TEXT("Бавария Германия"), TEXT("Барселона Испания"),
        TEXT("Арсенал Англия"), TEXT("Лион Франция"), TEXT("Интер Италия"), TEXT("Реал Испания") };
    for (const TCHAR* item : items)
        SendMessage(hList1, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item));

    SendMessage(hList2, LB_SETCOLUMNWIDTH, 170, 0);

    const TCHAR* items2[] = { TEXT("Рома Италия"), TEXT("Манчестер Юнайтед Англия"), TEXT("Вердер Германия"), TEXT("Валенсия Испания"),
        TEXT("Челси Англия"), TEXT("Марсель Франция"), TEXT("Ювентус Италия"), TEXT("Атлетико Испания") };
    for (const TCHAR* item : items2)
        SendMessage(hList2, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item));

    return TRUE;
}

void UsingListboxDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDC_ADD:
    {
        int length = GetWindowTextLength(hEdit1);
        std::vector<TCHAR> buffer(length + 1);
        GetWindowText(hEdit1, buffer.data(), length + 1);
        if (length > 0)
        {
            int index = SendMessage(hList1, LB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(buffer.data()));
            if (index == LB_ERR)
                SendMessage(hList1, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(buffer.data()));
            else
                MessageBox(hwnd, TEXT("Такой клуб уже существует!"), TEXT("Добавление клуба"), MB_OK | MB_ICONSTOP);
        }
    }
    break;
    case IDC_DEL:
    {
        int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
        if (index != LB_ERR)
        {
            int length = SendMessage(hList1, LB_GETTEXTLEN, index, 0);
            std::vector<TCHAR> buffer(length + 1);
            SendMessage(hList1, LB_GETTEXT, index, reinterpret_cast<LPARAM>(buffer.data()));
            MessageBox(hwnd, buffer.data(), TEXT("Удаление клуба"), MB_OK | MB_ICONINFORMATION);
            SendMessage(hList1, LB_DELETESTRING, index, 0);
        }
        else
            MessageBox(hwnd, TEXT("Клуб не выбран!"), TEXT("Удаление клуба"), MB_OK | MB_ICONSTOP);
    }
    break;
    case IDC_FIND:
    {
        int length = GetWindowTextLength(hEdit2);
        std::vector<TCHAR> buffer(length + 1);
        GetWindowText(hEdit2, buffer.data(), length + 1);
        if (length > 0)
        {
            int index = SendMessage(hList1, LB_SELECTSTRING, -1, reinterpret_cast<LPARAM>(buffer.data()));
            if (index == LB_ERR)
                MessageBox(hwnd, TEXT("Клуб не найден!"), TEXT("Поиск клуба"), MB_OK | MB_ICONSTOP);
        }
    }
    break;
    case IDC_DELALL:
        SendMessage(hList1, LB_RESETCONTENT, 0, 0);
        break;
    case IDC_GETSELITEMS:
    {
        int nCount = SendMessage(hList2, LB_GETSELCOUNT, 0, 0);
        std::vector<int> selectedItems(nCount);
        SendMessage(hList2, LB_GETSELITEMS, nCount, reinterpret_cast<LPARAM>(selectedItems.data()));
        for (int i : selectedItems)
        {
            int length = SendMessage(hList2, LB_GETTEXTLEN, i, 0);
            std::vector<TCHAR> buffer(length + 1);
            SendMessage(hList2, LB_GETTEXT, i, reinterpret_cast<LPARAM>(buffer.data()));
            MessageBox(hwnd, buffer.data(), TEXT("Список с множественным выбором"), MB_OK | MB_ICONINFORMATION);
        }
    }
    break;
    case IDC_FIND2:
    {
        int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
        if (index != LB_ERR)
        {
            int length = GetWindowTextLength(hEdit3);
            std::vector<TCHAR> buffer(length + 1);
            GetWindowText(hEdit3, buffer.data(), length + 1);
            if (length > 0)
            {
                SendMessage(hList1, LB_DELETESTRING, index, 0);
                SendMessage(hList1, LB_INSERTSTRING, index, reinterpret_cast<LPARAM>(buffer.data()));
            }
        }
        else
            MessageBox(hwnd, TEXT("Клуб не выбран!"), TEXT("Редактирование клуба"), MB_OK | MB_ICONSTOP);
    }
    break;
    }
    if (id == IDC_LIST1 && codeNotify == LBN_SELCHANGE)
    {
        int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
        if (index != LB_ERR)
        {
            int length = SendMessage(hList1, LB_GETTEXTLEN, index, 0);
            std::vector<TCHAR> buffer(length + 1);
            SendMessage(hList1, LB_GETTEXT, index, reinterpret_cast<LPARAM>(buffer.data()));
            SetWindowText(hwnd, buffer.data());
        }
    }
}

void UsingListboxDlg::Cls_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}