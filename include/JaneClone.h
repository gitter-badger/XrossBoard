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
#include <wx/notebook.h>
#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/mstream.h>
#include <wx/url.h>
#include <wx/protocol/http.h>
#include <wx/datstrm.h>
#include <wx/aboutdlg.h>
#include <wx/hashmap.h>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/textfile.h>
#include <wx/convauto.h>

// C 標準ライブラリ
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 自作クラスのヘッダ
#include "ExtractBoardList.h"
#include "SocketCommunication.h"
#include "DataType.h"
#include "SQLiteBundle.h"

// 名前空間
using namespace std;

// ディスクからの読取サイズ
#define S_SIZE (2048)
#define D_SIZE (6144)

// テキストの終端文字が何で終わるのかを定義
#if defined(__WXMSW__)
	#define TextEndLineType wxTextFileType_Dos
#endif
#if defined(__WXGTK__)
	#define TextEndLineType	wxTextFileType_Unix
#endif
#if defined(__WXMAC__)
	#define TextEndLineType wxTextFileType_Mac
#endif

class JaneClone : public wxFrame {

public:
  // begin wxGlade: JaneClone::ids
  // end wxGlade

  JaneClone(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnGetBoardList(wxCommandEvent& event);
  void OnVersionInfo(wxCommandEvent& event);
  void OnMouseClickRight(wxCommandEvent& event);

private:
  // begin wxGlade: JaneClone::methods
  void SetProperties();
  void DoLayout();

  // 板一覧ファイルをダウンロードする処理
  void DownloadBoardList();
  // ダウンロードした板一覧ファイルを解凍する処理
  void DecommpressFile(wxString& , wxString& );
  // ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
  void ConvertSJISToUTF8(wxString& , wxString&);
  // 取得した板一覧ファイルからデータを抽出してレイアウトに反映するメソッド
  void SetBoardList();

protected:
  // URL入力欄のテキスト操作用
  wxTextCtrl* 	textCtlForURL;
  // ステータスバー表示用文字列
  wxStatusBar* 	statusBarStr;
  // ツリーコントロールは内部からならいじれるようにしておく
  wxTreeCtrl* 	m_tree_ctrl;
  wxTreeItemData*	m_treeData;
  wxTreeItemId 	m_rootId;
  wxStaticText* 	label_1;
  wxButton* 		button_1;

  // 左側
  wxSplitterWindow* window_1;
  // 右側
  wxSplitterWindow* window_2;

  // 左側　-- 板一覧がツリーコントロールに入る
  wxPanel* window_1_pane_1;
  // 左側 -- 未使用　デバッグ画面でもつけるか
  wxPanel* window_1_pane_2;

  // 右側上段 -- 板一覧
  wxPanel* window_2_pane_1;
  // 右側下段　-- 個別のスレ
  wxPanel* window_2_pane_2;

  //　ツリーコントロールにクリックした時のイベント
  void OnGetBoardInfo(wxTreeEvent& event);
  // 板名とそのURLを保持するwxHashMap　JaneCloneが起動している間は保持される
  // URLvsBoardNameクラスについてはDataType.h参照
  WX_DECLARE_HASH_MAP( int, URLvsBoardName*, wxIntegerHash, wxIntegerEqual, NameURLHash );
  // HashMapの本体
  NameURLHash retainHash;

  // wxNotebook（上部）が乗るサイザー
  wxBoxSizer* topNote;
  /** 板名のツリーコントロールをクリックした場合表示されるwxNoteBook　*/
  wxNotebook* boardNoteBook;

  // 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
  void SetBoardNameToNoteBook(wxString& boardName, wxString& boardURL);
  // スレッドタイトル一覧の取得メソッド
  wxString DownloadThreadList(wxString& boardURL);
  // 板名のノートブックがクリックされた時スレッド一覧を表示する処理
  void OnClickBoardNote(wxNotebookEvent& event);

  // アプリケーション起動時からSQLiteを管理するオブジェクト
  SQLiteBundle* sqlite;

  // スレッド一覧の情報を保持するwxHashMap　ユーザが板名をクリックするたびに作られる
  // ThreadListクラスについてはDataType.h参照
  WX_DECLARE_HASH_MAP( int, ThreadList*, wxIntegerHash, wxIntegerEqual, ThreadListHash );
  // ThreadListHashの本体
  ThreadListHash threadListHash;

  // スレッド一覧のハッシュマップを作成するメソッド
  void SetThreadList(wxString& inputThreadListdat);

  DECLARE_EVENT_TABLE()
}; // wxGlade: end class

#endif // JANECLONE_H
