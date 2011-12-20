// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Nov 01 01:42:24 2011

#ifndef JANECLONE_H
#define JANECLONE_H

// 純粋なwxWidgetsライブラリはここに
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/statusbr.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/xml/xml.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/string.h>
#include <wx/file.h>

// C++ -- boost
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

// C 標準ライブラリ
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 自作関数のヘッダ
#include "ExtractBoardList.h"

// 名前空間
using namespace std;
using namespace boost::asio;
namespace fs = boost::filesystem;

// マクロ
#define SJIS_CHECK_STR	\
strncmp(SJISHEX ,"8" , 1) == 0|| \
strncmp(SJISHEX ,"9" , 1) == 0|| \
strncmp(SJISHEX ,"e" , 1) == 0|| \
strncmp(SJISHEX ,"a" , 1) == 0|| \
strncmp(SJISHEX ,"b" , 1) == 0|| \
strncmp(SJISHEX ,"c" , 1) == 0|| \
strncmp(SJISHEX ,"d" , 1) == 0

#define HANKAKU_CHECK_STR \
strncmp(SJISHEX ,"a" , 1) == 0|| \
strncmp(SJISHEX ,"b" , 1) == 0|| \
strncmp(SJISHEX ,"c" , 1) == 0|| \
strncmp(SJISHEX ,"d" , 1) == 0


class JaneClone : public wxFrame {

public:
    // begin wxGlade: JaneClone::ids
    // end wxGlade

    JaneClone(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnGetBoardList(wxCommandEvent& event);
    void OnVersionInfo(wxCommandEvent& event);

private:
    // begin wxGlade: JaneClone::methods
    void SetProperties();
    void DoLayout();


    /**
    *---OnGetBoardList以下のメソッド
    */
    // 板一覧ファイルをダウンロードする処理
	void DownloadBoardList();
	// ダウンロードした板一覧ファイルを解凍する処理
	void DecommpressFile();
	// ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
	void ConvertSJISToUTF8();
	// 取得した板一覧ファイルからデータを抽出してレイアウトに反映するメソッド
	void SetBoardList();

    /**
    *---OnGetBoardList以下のメソッドおわり
    */


    // URL入力欄のテキスト操作用
    wxTextCtrl* textCtlForURL;
    // ステータスバー表示用文字列
    wxStatusBar* statusBarStr;
    // ツリーコントロールは内部からならいじれるようにしておく
    wxTreeCtrl* m_tree_ctrl;
    wxTreeItemData *m_treeData;
    wxTreeItemId m_rootId;

protected:

    wxStaticText* label_1;
    wxButton* button_1;
    wxPanel* window_1_pane_1;
    wxPanel* window_2_pane_1;
    wxPanel* window_2_pane_2;
    wxSplitterWindow* window_2;
    wxPanel* window_1_pane_2;
    wxSplitterWindow* window_1;
    // end wxGlade

    DECLARE_EVENT_TABLE()
}; // wxGlade: end class


#endif // JANECLONE_H
