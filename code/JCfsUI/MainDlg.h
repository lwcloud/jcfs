// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <IfSearch.h>
#include <combase/IMsVector.h>
#include <functional>
#include "WTLVirtualList.h"
#include "ShellContextMenu.h"

#define WM_EXECUTE_MSG (WM_USER + 55648)


class CDiskSearchNotify : public IDiskSearchNotify, public CMsComBase<CDiskSearchNotify>
{
public:
// 	CDiskSearchNotify()
// 	{
// 		AddRef();
// 	}
	UNKNOWN_IMP1(IDiskSearchNotify);
protected://IDiskSearchNotify
	STDMETHOD(OnDiskSearch_FileChange)(WCHAR cDosName,LPCWSTR lpFile, DWORD dwAction, DWORD dwAttr) = 0;
	STDMETHOD(OnDiskSearch_Progress)(WCHAR cDosName, DWORD dwTotalFile, DWORD dwCur) = 0;
	STDMETHOD(OnDiskSearch_FileCountChange)(WCHAR cDosName, DWORD dwFileCount, DWORD dwDirCount) = 0;
	STDMETHOD(OnDiskSearch_StateChangeNotify)(WCHAR cDosName, INT nMsg, WPARAM wParam, LPARAM lParam) = 0;
	STDMETHOD(OnDiskSearch_Result)( DWORD dwCount, DWORD dwTickCount) = 0;
};

enum _RESULT_LIST_MENU
{
	IDM_OPEN_CUR_PATH = 80000 + 100, //打开当前路径
	IDM_SEARCH_CUR_PATH,

	IDM_PATH_MENU_COMMAND_BEGIN = (IDM_OPEN_CUR_PATH + 200),
	IDM_PATH_MENU_COMMAND_END	= IDM_PATH_MENU_COMMAND_BEGIN + 200,

	IDM_RUNAS_ADMIN,
};


class CMainDlg : public CDiskSearchNotify,public SHostWnd
{
public:
	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv)
	{
		if ( re_uuidof(IDiskSearchNotify) == riid)
		{ 
			*ppv = static_cast<IDiskSearchNotify*>(static_cast<CDiskSearchNotify*>(this)); CDiskSearchNotify::AddRef(); 
			return S_OK;
		}

		return E_NOINTERFACE;
	}
	


	CMainDlg( IMscomRunningObjectTable* pRot);
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, SOUI::CSize size);

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnDestory();
public:
	void Post(const std::function<void()> &f);
	VOID SetTabIndex( int nIndex );
	DWORD GetCurrentSelPath(CString& strPath);
	BOOL GetCurrentSel(SearchResult& Result);
	BOOL GetSelect(SearchResult& Result , int dwSel);
	void SetIcon(UINT nRes);
protected://IDiskSearchNotify
	STDMETHOD(OnDiskSearch_FileChange)(WCHAR cDosName,LPCWSTR lpFile, DWORD dwAction, DWORD dwAttr);
	STDMETHOD(OnDiskSearch_Progress)(WCHAR cDosName, DWORD dwTotalFile, DWORD dwCur);;
	STDMETHOD(OnDiskSearch_FileCountChange)(WCHAR cDosName, DWORD dwFileCount, DWORD dwDirCount);
	STDMETHOD(OnDiskSearch_StateChangeNotify)(WCHAR cDosName, INT nMsg, WPARAM wParam, LPARAM lParam);
	STDMETHOD(OnDiskSearch_Result)( DWORD dwCount, DWORD dwTickCount);

protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)

		EVENT_NAME_COMMAND(L"chk_word_mpp", OnSearch)
		EVENT_NAME_COMMAND(L"chk_case_mpp", OnSearch)
		EVENT_NAME_COMMAND(L"chk_sys_mpp", OnSearch)
		EVENT_NAME_COMMAND(L"chk_hidden_mpp", OnSearch)
		EVENT_NAME_COMMAND(L"inclue_child_path", OnSearch)

		//分类
		EVENT_NAME_COMMAND(_T("opt_type_qb"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_tb"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_wd"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_sp"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_yp"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_ys"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_cx"), OnSearch)
		EVENT_NAME_COMMAND(_T("opt_type_ml"), OnSearch)

		//标记
		EVENT_NAME_COMMAND(_T("opt_mark_w"), OnMark_0)
		EVENT_NAME_COMMAND(_T("opt_mark_h"), OnMark_7)
		EVENT_NAME_COMMAND(_T("opt_mark_c"), OnMark_6)
		EVENT_NAME_COMMAND(_T("opt_mark_q"), OnMark_5)
		EVENT_NAME_COMMAND(_T("opt_mark_l"), OnMark_4)
		EVENT_NAME_COMMAND(_T("opt_mark_lan"), OnMark_3)
		EVENT_NAME_COMMAND(_T("opt_mark_z"), OnMark_2)
		EVENT_NAME_COMMAND(_T("opt_mark_hui"), OnMark_1)

		EVENT_NAME_COMMAND(_T("check_updata"), OnCheckUpdata)
		EVENT_NAME_COMMAND(_T("check_canp_updata"), OnCheckUpdata)

		EVENT_NAME_HANDLER(L"search_input", EVT_RE_NOTIFY, OnSearch)
		EVENT_NAME_HANDLER(L"path_input", EVT_RE_NOTIFY, OnSearch)
		EVENT_MAP_END()
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_DESTROY(OnDestory)

		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, LVN_GETDISPINFO,OnListGetDispinfo )
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, NM_RCLICK, OnListRClick)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, NM_CLICK, OnListClick)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, IDM_OPEN_CUR_PATH, OnListMenuOpenCurPath)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, IDM_COPY_NAME, OnListMenuCopyName)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, IDM_COPY_PATH, OnListMenuCopyPath)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, IDM_COPY_FULL_PATH, OnListMenuCopyFullPath)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, MN_DROPEFFECT_COPY_MSG, OnListDropeffectCopy)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, MN_DROPEFFECT_MOVE_MSG, OnListDropeffectMove)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, NM_CUSTOMDRAW, OnListCustomDraw)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, NM_DBLCLK, OnListDBClick)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, LVN_KEYDOWN, OnListKeyDown)
		NOTIFY_HANDLER_EX(IDC_RESULT_LIST, LVN_ITEMCHANGED, OnListItemChanged)
		MESSAGE_HANDLER(WM_EXECUTE_MSG, OnExecute)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	//List
	LRESULT OnListGetDispinfo(LPNMHDR lParam );
	LRESULT OnListRClick(LPNMHDR lParam );
	LRESULT OnListClick(LPNMHDR lParam );
	LRESULT OnListMenuOpenCurPath(LPNMHDR lParam );
	LRESULT OnListMenuCopyName(LPNMHDR lParam );
	LRESULT OnListMenuCopyPath(LPNMHDR lParam );
	LRESULT OnListMenuCopyFullPath(LPNMHDR lParam );
	LRESULT OnListDropeffectCopy(LPNMHDR lParam );
	LRESULT OnListDropeffectMove(LPNMHDR lParam );
	LRESULT OnListCustomDraw(LPNMHDR lParam );
	LRESULT OnListDBClick(LPNMHDR lParam );
	LRESULT OnListKeyDown(LPNMHDR lParam );
	LRESULT OnListItemChanged(LPNMHDR lParam );

protected:
	LRESULT OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);


private:
	VOID OnSearch(EventArgs *pEvt = NULL);
	VOID OnCheckUpdata(EventArgs *pEvt = NULL);


	VOID OnMark_0(EventArgs *pEvt = NULL);
	VOID OnMark_1(EventArgs *pEvt = NULL);
	VOID OnMark_2(EventArgs *pEvt = NULL);
	VOID OnMark_3(EventArgs *pEvt = NULL);
	VOID OnMark_4(EventArgs *pEvt = NULL);
	VOID OnMark_5(EventArgs *pEvt = NULL);
	VOID OnMark_6(EventArgs *pEvt = NULL);
	VOID OnMark_7(EventArgs *pEvt = NULL);

private:
	BOOL GetQueryExtension();
	BOOL GetQueryConditionMask();
	BOOL GetQueryText();
	BOOL GetQueryPath();
	
	VOID SetItemDescription(LPCWSTR lpszFileName);
	BOOL SetCurFileMark( int nMark);
private:
	VOID OnWorkCheckUpdata();
	VOID SetSelectInformation(int nItem);
private:
	BOOL			m_bLayoutInited;

	STabCtrl*		m_pTableCtrl;
	SStatic*		m_pStatusCtrl;

private:
	UTIL::com_ptr<IDiskSearchCli> m_pDiskSearch;
	UTIL::com_ptr<IMsDwordVector> m_pResult;
	CWTLVirtualList* m_pListView;

	DWORD			 m_dwCurResultIndex;
	SearchResult	 m_CurResult;
	CString			m_strExten;
	CString			m_strQueryText;
	CString			m_strQueryPath;
	DWORD			m_dwQueryMask;
};
