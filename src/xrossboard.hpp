﻿/* XrossBoard - a text board site viewer for open BBS
 * Copyright (C) 2011-2015 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#ifndef XROSSBOARD_HPP_
#define XROSSBOARD_HPP_

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/event.h>
#include <wx/listctrl.h>
#include <wx/srchctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/config.h>
#include <wx/font.h>
#include <wx/menu.h>
#include <wx/html/htmlwin.h>
#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "datatype.hpp"
#include "xrossboardimageviewer.hpp"
#include "xrossboarduiutil.hpp"

#ifdef __WXMAC__
   #include "threadcontentbar.hpp"
   #include "settingwindow.hpp"
#endif

#ifdef __WXMSW__
   #include "threadcontentbar.hpp"
   #include "virtualboardlistctrl.hpp"
#endif

/**
 * XrossBoardのGUI本体部分を構成するクラス
 */
class XrossBoard : public wxFrame {

public:

     XrossBoard(wxWindow* parent, int id, const wxString& title,
	       const wxPoint& pos = wxDefaultPosition, const wxSize& size =
	       wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);

     ~XrossBoard();

     // 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
     void SetBoardNameToNoteBook(wxString& boardName, wxString& boardURL, wxString& boardNameAscii);
     // スレッド一覧をクリックすると、それをスレ表示画面に反映するメソッド
     void SetThreadContentToNoteBook(const wxString&, const wxString&, const wxString&);
     // ウィンドウ内にカーソルが入った場合のイベント通知
     void OnEnterWindow(wxMouseEvent& event);

     // HashMapの本体
     NameURLHash retainHash;
     // 板名のツリーコントロールをクリックした場合表示されるwxNoteBook
     wxAuiNotebook* boardNoteBook;
     // 板名のツリーコントロールをクリックした場合表示されるwxNoteBook
     wxAuiNotebook* threadNoteBook;

     // XrossBoard内部のイメージビューアのインスタンス
     static XrossBoardImageViewer* imageViewer;
     static XrossBoardImageViewer* GetXrossBoardImageViewer();
     
     // 再起動処理のためのフラグ
     static bool restartAppFlag;
     // ユーザーが最後に触ったノートブックのオブジェクトを記憶する
     static wxString userLastAttachedNotebook;
     // メニュー上のイベント
     void OnQuit(wxCommandEvent& event);
     void OnAbout(wxCommandEvent& event);
     void OnRestart(wxCommandEvent& event);
     void WindowMinimize(wxCommandEvent& event);
     void OnGetBoardList(wxCommandEvent& event);
     static void GetBoardListImpl();
     void CheckLogDirectory(wxCommandEvent& event);
     void OnVersionInfo(wxCommandEvent& event);
     void AddThreadFavorite(wxCommandEvent& event);
     void DelThreadFavorite(wxCommandEvent& event);
     void AddBoardFavorite(wxCommandEvent& event);
     void DelBoardFavorite(wxCommandEvent& event);
     void AddAllThreadFavorite(wxCommandEvent& event);
     void MoveNextTab(wxCommandEvent& event);
     void MovePrevTab(wxCommandEvent& event);
     void CallNewBoardAddDialog(wxCommandEvent& event);

     // XrossBoard公式サイトをブラウザで開く
     void OnOpenXrossBoardOfficial(wxCommandEvent& event) {
	  wxLaunchDefaultBrowser(XROSSBOARD_DOWNLOADSITE);
     };
     // ログ出力
     void Logging(wxCommandEvent& event) {
	  *m_logCtrl << event.GetString();
     };
     // メインのスレッドにログとイベントを送る
     void SendLogging(wxString& message) {
	  XrossBoardUiUtil::SendLoggingHelper(message);
     };


     /**
      * ユーザーがカーソルを合わせているウィンドウが変わったあとに実行される処理
      * (※Windowsの場合はフォーカスの合わせ作業も行う)
      */
     void ChangeUserLastAttached(wxCommandEvent& event) {
	  this->userLastAttachedNotebook = event.GetString();

#ifdef __WXMSW__
	  if (this->userLastAttachedNotebook == BOARD_NOTEBOOK) {
	       wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
	       VirtualBoardListCtrl* vbListCtrl = dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardName));
	       if (vbListCtrl) {
		    vbListCtrl->SetFocus();
	       }
	  } else if (this->userLastAttachedNotebook == THREAD_NOTEBOOK) {
               // スレッド内容ウィンドウの処理
	       ThreadContentBar* contentBar = 
		    dynamic_cast<ThreadContentBar*>(threadNoteBook->GetPage(threadNoteBook->GetSelection()));
	  
	       if ( wxPanel* searchBarPanel 
		    = dynamic_cast<wxPanel*>(wxWindow::FindWindowById(ID_ThreadContentSearchBar, contentBar))) {
		    // スレッド内容バーの子ウィンドウを取り出して命令する
		    searchBarPanel->GetNextSibling()->SetFocus();
	       }
	  } else if (this->userLastAttachedNotebook == BOARD_TREE_NOTEBOOK) {
	       m_tree_ctrl->SetFocus();
	  }
#endif
     };

     void ChangeUserLastAttachedEvent(wxString& message) {
	  XrossBoardUiUtil::QueueEventHelper(wxEVT_COMMAND_TEXT_UPDATED, ID_ChangeUserLastAttached, message);
     };

#ifdef __WXMAC__
     /** 
      * 送られてきた情報をもとにGUIを更新するMacOSX特別処理 Macいい加減にしろよ...
      */
     void UpdateXrossBoardUI(wxUpdateUIEvent& event) {
	  wxObject* obj = event.GetEventObject();
	  wxString  ui  = event.GetString();

	  if ( ui == wxT("ThreadContentBar") ) {
	       if ( ThreadContentBar* threadBar = dynamic_cast<ThreadContentBar*>(obj) ) {
		    threadBar->UpdateResources();
		    threadBar->Layout();
	       }
	  } else if ( ui == wxT("ColorFontSettingPanel") ) {
	       if ( ColorFontSettingPanel* colour = dynamic_cast<ColorFontSettingPanel*>(obj) ) {
		    colour->UpdateResources();
	       }
	  }

	  if ( ui == wxT("CommonAuiToolBarUpdate") ) {

	       switch (event.GetId()) {

	       case ID_BoardTreectrl: // 板一覧ツリー
	       {
		    // Sizer
		    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
		    this->CreateCommonAuiToolBar(m_boardTreePanel, vbox, ID_BoardSearchBar);
		    vbox->Add(m_tree_ctrl, 1, wxEXPAND, 0);
		    m_boardTreePanel->SetSizer(vbox);
		    m_boardTreePanel->Layout();
	       }
	       break;

	       case ID_FavsTreectrl: // お気に入りツリー
	       {
		    // Sizer
		    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
		    this->CreateCommonAuiToolBar(m_favoriteTreePanel, vbox, ID_FavsSearchBar);
		    vbox->Add(m_fav_tree_ctrl, 1, wxEXPAND, 0);
		    m_favoriteTreePanel->SetSizer(vbox);
		    m_favoriteTreePanel->Layout();
	       } 
	       break;

	       case ID_NowReadingTreectrl: // 閲覧中ツリー
	       {
		    // Sizer
		    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
		    this->CreateCommonAuiToolBar(m_nowReadingTreePanel, vbox, ID_NowReadingSearchBar);
		    vbox->Add(m_now_reading_tree_ctrl, 1, wxEXPAND, 0);
		    m_nowReadingTreePanel->SetSizer(vbox);
		    m_nowReadingTreePanel->Layout();
	       } 
	       break;
	       
	       }
	  }
     };
#endif

     // ユーザーが最後に閉じた板を開く
     void OnUserLastClosedBoardClick(wxCommandEvent& event);
     // ユーザーが最後に閉じたスレッドを開く
     void OnUserLastClosedThreadClick(wxCommandEvent& event);
     // ユーザーがお気に入り登録したスレッドを開く
     void OnUserFavoriteThreadClick(wxCommandEvent& event);

     // 動的に要素を確保するメニュー
     wxMenu* closeT;
     wxMenu* closeB;
     wxMenu* lookingTB;
     void OnMenuOpen(wxMenuEvent& event);
     // ユーザーが最近閉じた板タブの情報をSQLiteから取得して設定する
     void UserLastClosedBoardMenuUp(wxUpdateUIEvent& event);
     // ユーザーが最近閉じたスレタブの情報をSQLiteから取得して設定する
     void UserLastClosedThreadMenuUp(wxUpdateUIEvent& event);
     void UserLookingTabsMenuUp(wxUpdateUIEvent& event);
     void UserLookingTabsControl(wxUpdateUIEvent& event);
     // 閲覧中ツリーのデータ更新を行う
     void NowReadingTreectrlUpdate(wxUpdateUIEvent& event);

     // スレッド一覧タブ処理
     void OneBoardTabClose(wxCommandEvent& event);
     void ExcepSelTabClose(wxCommandEvent& event);
     void AllBoardTabClose(wxCommandEvent& event);
     void AllLeftBoardTabClose(wxCommandEvent& event);
     void AllRightBoardTabClose(wxCommandEvent& event);
     void OnOpenBoardByBrowser(wxCommandEvent& event);
     void ReloadOneBoard(wxCommandEvent& event);
     void ReloadAllBoard(wxCommandEvent& event);
     void CopyBURLToClipBoard(wxCommandEvent& event);
     void CopyBTitleToClipBoard(wxCommandEvent& event);
     void CopyBBothDataToClipBoard(wxCommandEvent& event);
     void DeleteBSelectedDatFile(wxCommandEvent& event);
     void DeleteBAllDatFile(wxCommandEvent& event);        
     void DeleteBAllDatFileWithoutFav(wxCommandEvent& event);
     void CallResponseWindow(wxCommandEvent& event);
     void OnThreadListSort(wxCommandEvent& event);
     
     // スレタブでの処理
     void OneThreadTabClose(wxCommandEvent& event);
     void ExcepSelThreadTabClose(wxCommandEvent& event);
     void AllThreadTabClose(wxCommandEvent& event);
     void AllLeftThreadTabClose(wxCommandEvent& event);
     void AllRightThreadTabClose(wxCommandEvent& event);
     void OnOpenThreadByBrowser(wxCommandEvent& event);
     void CopyTURLToClipBoard(wxCommandEvent& event);
     void CopyTTitleToClipBoard(wxCommandEvent& event);
     void CopyTBothDataToClipBoard(wxCommandEvent& event);
     void SaveDatFile(wxCommandEvent& event);
     void SaveDatFileToClipBoard(wxCommandEvent& event);
     void DeleteDatFile(wxCommandEvent& event);
     void ReloadThisThread(wxCommandEvent& event);
     void ReloadThreadByName(wxCommandEvent& event);
     void ReloadThread(wxString& title);

     // いろいろなイベント処理
     void OnCloseWindow(wxCloseEvent& event);
     void OnLeftClickAtListCtrl(wxListEvent& event);
     void OnLeftClickAtListCtrl2ch(wxListEvent& event);
     void OnLeftClickAtListCtrlCol(wxListEvent& event);
     void OnChangeBoardTab(wxAuiNotebookEvent& event);
     void OnChangeThreadTab(wxAuiNotebookEvent& event);
     void OnChangedBoardTab(wxAuiNotebookEvent& event);
     void OnChangedThreadTab(wxAuiNotebookEvent& event);
     void OnRightClickBoardNoteBook(wxAuiNotebookEvent& event);
     void OnRightClickThreadNoteBook(wxAuiNotebookEvent& event);
     void OnAboutCloseThreadNoteBook(wxAuiNotebookEvent& event);
     void OnAboutCloseBoardNoteBook(wxAuiNotebookEvent& event);
     void OnCellHover(wxHtmlCellEvent& event);
     void OnClickURLWindowButton(wxCommandEvent& event);
     void OnSetFocus(wxFocusEvent& event);

     // 各種GUI上の設定
     void SetXrossBoardMenuBar();
     void SetProperties();
     void DoLayout();
     void SetAuiPaneInfo();
     void SetPreviousUserLookedTab();
     void InitializeXrossBoard(const wxString& xb);
     // 2ch板一覧ツリーの初期化
     void Initialize2chBoardList();

     // ツリーコントロールを載せるノートブック
     wxAuiNotebook* boardTreeNoteBook;

     /** 2ch板一覧 */
     // 2ch板一覧ツリーコントロールが乗るパネル
     wxPanel* m_boardTreePanel;
     // 2ch板一覧ツリーコントロール
     wxTreeCtrl* m_tree_ctrl;
     // 取得した板一覧ファイルからデータを抽出したのちwxTreeCtrlのインスタンスを返す
     void SetBoardList(const bool updateHash = true);
     // 上記の処理のイベント版
     void SetBoardList(wxCommandEvent& event);

     /** 2chお気に入り */
     wxPanel* m_favoriteTreePanel;
     // 2chお気に入りツリーコントロール
     wxTreeCtrl* m_fav_tree_ctrl;

     /** 閲覧中 */
     wxPanel* m_nowReadingTreePanel;
     // 閲覧中ツリーコントロール
     wxTreeCtrl* m_now_reading_tree_ctrl;


     // 板一覧ツリーの初期化
     void InitializeBoardList();
     // お気に入り一覧ツリーの初期化
     void InitializeFavsList();
     // 閲覧中の一覧ツリーの初期化
     void InitializeNowReadingList();

     // すべてのウィジェットが載るAuiマネージャー
     wxAuiManager m_mgr;
     // ステータスバー表示用文字列
     wxStatusBar* statusBarStr;

     // マウスクリックしているかどうか
     bool isClicking;
     // ドラッグしているかどうか
     bool isDragging;

     /**
      * 画面上部のオブジェクトとメソッド
      */
     // 検索バー
     wxSearchCtrl* m_search_ctrl;
     // URL入力欄が載るパネル
     wxPanel* m_url_input_panel;
     // URL入力欄
     wxTextCtrl* m_url_input;
     // URL入力欄の画像つきボタン
     wxBitmapButton* m_url_input_button;
     // URLを表すString
     wxString m_url_text;

     /**
      * 画面左側のオブジェクトとメソッド
      */
     // ログ出力画面
     wxTextCtrl* m_logCtrl;
     // アイコン付きのツールバー
     wxAuiToolBar* m_floatToolBar;
     // 縦横分割
     bool separateIsX;
     // ペインの数(true:3, false:2)
     bool paneIsThree;
     // GUI右側に表示するウィジェット(true:スレッド一覧, false:スレ欄)
     bool rightIsThreadList;

     // ツリーコントロールの表示・非表示切り替え
     void ShowBoardListTree(wxCommandEvent& event);
     // 縦⇔横分割切り替え
     void SwitchSeparateXY(wxCommandEvent& event);
     // ２⇔３ペイン切り替え
     void SwitchTwoThreePane(wxCommandEvent& event);
     // 右側切り替え
     void SwitchRightPane(wxCommandEvent& event);
     // 設定画面を呼び出す
     void CallSettingWindow(wxCommandEvent& event);
     // 設定画面を呼び出す
     void CallViewerSettingWindow(wxCommandEvent& event);
     // 2ch板一覧ツリーコントロールでクリックした時のイベント
     void OnGetBoardInfo(wxTreeEvent& event);

     /**
      * 右上のオブジェクトとメソッド
      */
     // ノートブックに反映する際のコールバック
     void SetThreadListItemNew( wxString boardName, wxString outputPath, size_t selectedPage, 
				const std::map<wxString,ThreadList>& oldThreadMap);
     void SetThreadListItemUpdate( wxString boardName,  wxString outputPath, size_t selectedPage, 
				   const std::map<wxString,ThreadList>& oldThreadMap);
     void UpdateThreadTabIcons();

     /**
      * 右下のオブジェクトとメソッド
      */

     // ユーザーがタブに保持しているスレッドの情報を保存するHashSet
     ThreadInfoHash tiHash;

public:
     /**
      * 現在XrossBoardが保持しているスレッド情報を取得する
      */
     void GetThreadInfoHash(ThreadInfoHash& threadInfoHash);
     /**
      * 現在XrossBoardが保持しているスレッド情報を設定する
      */
     void SetThreadInfoHash(ThreadInfoHash& threadInfoHash);

private:
     /**
      * その他のオブジェクトとメソッド
      */

     // スレッド一覧の情報を保持するwxHashMap　ユーザが板名をクリックするたびに作られる
     // ThreadListクラスについてはdatatype.hpp参照
     WX_DECLARE_HASH_MAP( int, ThreadList*, wxIntegerHash, wxIntegerEqual, ThreadListHash );

     // wxFileConfigクラスのインスタンス
     wxFileConfig* config;
     // ポップアップウィンドウを出現させる
     void SetPopUpWindow(wxHtmlCellEvent& event, wxString& boardNameAscii,
			 wxString& origNumber, wxString& resNumber, wxPoint& anchorPoint);
     // 取得した情報を元に新しいポップアップウィンドウを出現させる
     void SetPopUpWindowForID(wxHtmlCellEvent& event, wxString& boardNameAscii,
			 wxString& origNumber, wxString& extractId, wxPoint& anchorPoint);
     // 被レス状態を元に新しいポップアップウィンドウを出現させる
     void SetPopUpWindowByIndex(wxHtmlCellEvent& event, wxString& extractIndex, wxPoint& anchorPoint);
     
     /**
      *  フォント読み出し系の処理
      */
     // ウィジェットの名前ごとにコンフィグファイルから設定を取り出す
     wxFont ReadFontInfo(const wxString& widgetName);
     // 現在使用しているフォントを取得する
     wxFont GetCurrentFont();

     /**
      *  検索ボックス系の処理
      */
     // 検索実行
     void ThreadSearchBoxDoSearch(wxCommandEvent& event);
     void BoardSearchBoxDoSearch(wxCommandEvent& event);
     // 板一覧ツリーを検索する
     void SearchBoardTree(const wxString& keyword);
     // スレッド一覧リストを検索する
     void SearchThreadList(const wxString& keyword);
     // 検索ボックスで上に移動
     void SearchBoxUp(wxCommandEvent& event);
     // 検索ボックスで下に移動
     void SearchBoxDown(wxCommandEvent& event);
     // 検索ボックスでコピー実行
     void SearchBoxCopy(wxCommandEvent& event);
     // 検索ボックスで切り取り実行
     void SearchBoxCut(wxCommandEvent& event);
     // 検索ボックスで全て選択
     void SearchBoxSelectAll(wxCommandEvent& event);
     // 検索ボックスをクリア
     void SearchBoxClear(wxCommandEvent& event);
     // 以前検索したキーワードをコンボボックスに補填する
     void SupplySearchWords(wxComboBox* combo, const wxWindowID id);

     /**
      *  よく使う処理のコールバック関数
      */
     // スレタブ上に存在するスレッドのURLを返す
     wxString GetThreadURL(const wxString title,const wxString boardNameAscii,const wxString origNumber);
     // スレッド一覧画面にツールバーを設定する
     wxPanel* CreateAuiToolBar(wxWindow* parent, const wxString& boardName, const wxString& outputPath,
			       const std::map<wxString,ThreadList>& oldThreadMap);
     // 検索用ツールバー設定の共通部分
     void CreateCommonAuiToolBar(wxPanel* panel, wxBoxSizer* vbox, wxWindowID id, const wxString& boardName = wxEmptyString);
     // 検索ボックスを隠す
     void HideSearchBar(wxCommandEvent& event);
     // ペイン更新
     void UpdatePanes(bool immediate = true);

     // ユーザーがさわったコンボボックスを見つける
     wxComboBox* FindUserAttachedCombo(wxCommandEvent& event, wxWindow* parent) {

	  wxComboBox* combo;
	  wxWindowID id = FindUserAttachedWindowId(event, parent);
	  combo = dynamic_cast<wxComboBox*>(wxWindow::FindWindowById(id, parent));

	  return combo;
     };

     // ユーザーがさわったウィンドウのIDを返す
     wxWindowID FindUserAttachedWindowId(wxCommandEvent& event, wxWindow* parent) {

	  if (parent->GetLabel() == BOARD_TREE_SEARCH) {
	       wxString message = wxT("BOARD_TREE_SEARCH");
	       SendLogging(message);
	       return ID_BoardSearchBarCombo;
	  } else if (parent->GetLabel() == THREADLIST_SEARCH) {
	       wxString message = wxT("THREADLIST_SEARCH");
	       SendLogging(message);
	       return ID_ThreadSearchBarCombo;
	  }
	  return wxID_ANY;
     };

     // クリップボードに指定した文字列をコピーする
     void SetStringToClipBoard(const wxString copyString);

#ifdef USE_SHINGETSU /** 新月オプションが有効であればビルドする */
     void OnLeftClickAtListCtrlShingetsu(wxListEvent& event);
     void OnRightClickShingetsuNodeTree(wxTreeEvent& event);

     // 新月の公開ノード用ツリーコントロールが乗るパネル
     wxPanel* m_shingetsuTreePanel;
     // 新月の公開ノード用ツリーコントロール
     wxTreeCtrl* m_shingetsu_tree_ctrl;
     // 新月公開ノードの初期化
     void InitializeShingetsuNodeList();
     // 新月公開ノードツリーコントロールでクリックした時のイベント
     void OnGetShingetsuNodeInfo(wxTreeEvent& event);

     // 新月公開ノードをクリックして、それをノートブックに反映するメソッド
     void SetShingetsuNodeToNoteBook(const wxString& nodeHostname);
     // 新月公開ノード上のスレッド一覧をUIに反映するメソッド
     void SetShingetsuThreadListToNoteBook(const wxString& nodeHostname, wxString& outputFilePath);
     // 新月のスレッド一覧を新たに取得する
     void SetShingetsuThreadListItemNew(const wxString& nodeHostname, const size_t selectedPage
					,wxString& outputFilePath
					,const std::map<wxString,ThreadList>& oldThreadMap);
     // 新月のスレッド一覧を更新する
     void SetShingetsuThreadListItemUpdate(const wxString& nodeHostname, const size_t selectedPage
					   ,wxString& outputFilePath
					   ,const std::map<wxString,ThreadList>& oldThreadMap);

#endif /** USE_SHINGETSU */

public:
     // ショートカットキー(Ctrl+F)のイベント
     void CtrlF(wxKeyEvent& event);
     // ショートカットキー(Enter)のイベント
     bool Enter(wxKeyEvent& event);

     DECLARE_EVENT_TABLE()
};
// wxGlade: end class

#endif // XROSSBOARD_HPP_