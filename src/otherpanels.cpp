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

#include <wx/xrc/xmlres.h>
#include "otherpanels.hpp"
#include "wx/dynarray.h"
#include "sqliteaccessor.hpp"
#include "socketcommunication.hpp"

/**
 * イベントテーブル
 */
BEGIN_EVENT_TABLE(PathSettingPanel, wxPanel)
   EVT_CHECKBOX(ID_BrowserCheck,                PathSettingPanel::OnBrowserCheck)
   EVT_DIRPICKER_CHANGED(ID_DirPickerBrowser,   PathSettingPanel::OnChangeDirPickerBrowser)
   EVT_DIRPICKER_CHANGED(ID_DirPickerBoardList, PathSettingPanel::OnChangeDirPickerBoardList)
   EVT_DIRPICKER_CHANGED(ID_DirPickerSkin,      PathSettingPanel::OnChangeDirPickerSkin)
   EVT_BUTTON(ID_ClearBrowserPath,              PathSettingPanel::ClearPathSetting)
   EVT_BUTTON(ID_ClearBoardListPath,            PathSettingPanel::ClearPathSetting)
   EVT_BUTTON(ID_ClearSkinPath,                 PathSettingPanel::ClearPathSetting)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ColorFontSettingPanel, wxPanel)
   EVT_BUTTON(ID_TreeFontButton,          ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_ThreadListFontButton,    ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_ExtractFontButton,       ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_LogWindowFontButton,     ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_ThreadTitleFontButton,   ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_KakikoFontButton,        ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_MemoFontButton,          ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_HintFontButton,          ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_LinkFontButton,          ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_OthersFontButton,        ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_AllFontButton,           ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_BoardListBGColorButton,  ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_ThreadListBGColorButton, ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_LogWindowBGColorButton,  ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_ThreadTitleBGColorButton,ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_ThreadViewBGColorButton, ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_MemoBGColorButton,       ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_HintBGColorButton,       ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_PopupBGColorButton,      ColorFontSettingPanel::OnClickColorFontSettingButton) 
   EVT_BUTTON(ID_AllBGColorButton,        ColorFontSettingPanel::OnClickColorFontSettingButton) 
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(TabColorSettingPanel, wxPanel)
   EVT_BUTTON(ID_ActiveTabBGColorButton                , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_DeactiveTabBGColorButton              , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_ThreadTabDefaultFontColorButton       , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_ThreadTabReadingFontColorButton       , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_ThreadTabBrokenFontColorButton        , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_ThreadTabCannotPostFontColorButton    , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_ThreadTabUpdateFontColorButton        , TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_ThreadTabPartialContentFontColorButton, TabColorSettingPanel::OnClickTabColorSettingButton) 	
   EVT_BUTTON(ID_AutoReloadFontColorButton             , TabColorSettingPanel::OnClickTabColorSettingButton) 	
END_EVENT_TABLE()

/**
 * 各種ネットワーク設定用画面
 */
NetworkSettingPanel::NetworkSettingPanel(wxWindow* parent) 
{
     // XRCからレイアウト情報を取得する
     InitWidgetsFromXRC(parent);
     // wxWindowIDを付加する
     this->SetId(ID_NetworkPanel);
     // プロパティファイル情報を設定する
     set_properties();

     // もし板一覧取得URLがなければデフォルトのURLを入力する
     if (boardListURLTC->GetValue().IsEmpty())
     {
	  boardListURLTC->SetValue(DEFAULT_BOARDLIST_URL);
     }

     Layout();
}

void NetworkSettingPanel::InitWidgetsFromXRC(wxWindow* parent)
{
     // XRC の中にある name をキーにしてGUIの型をロードする
     wxXmlResource::Get()->LoadObject(this, parent, wxT("NetworkSettingPanel_Base"), wxT("wxPanel"));

     proxyUseCheck	     = XRCCTRL(*this, "proxyUseCheck"		, wxCheckBox);
     proxyCacheUseCheck      = XRCCTRL(*this, "proxyCacheUseCheck"	, wxCheckBox);
     recProxyAddrTC	     = XRCCTRL(*this, "recProxyAddrTC"		, wxTextCtrl);
     recProxyPortTC	     = XRCCTRL(*this, "recProxyPortTC"		, wxTextCtrl);
     sedProxyAddrTC	     = XRCCTRL(*this, "sedProxyAddrTC"		, wxTextCtrl);
     sedProxyPortTC	     = XRCCTRL(*this, "sedProxyPortTC"		, wxTextCtrl);
     authSSLAddrTC	     = XRCCTRL(*this, "authSSLAddrTC"		, wxTextCtrl);
     authSSLPortTC	     = XRCCTRL(*this, "authSSLPortTC"		, wxTextCtrl);
     basicAuthUserNameTC     = XRCCTRL(*this, "basicAuthUserNameTC"	, wxTextCtrl);
     basicAuthPasswordTC     = XRCCTRL(*this, "basicAuthPasswordTC"	, wxTextCtrl);
     basicAuthPasswordTC->SetWindowStyle(wxTE_PASSWORD); // windows,osxで動くかな -> やっぱ動いてない…
     receiveTimeoutTC	     = XRCCTRL(*this, "receiveTimeoutTC"	, wxTextCtrl);
     connectTimeoutTC	     = XRCCTRL(*this, "connectTimeoutTC"	, wxTextCtrl);
     boardListURLTC	     = XRCCTRL(*this, "boardListURLTC"		, wxTextCtrl);
     receiveBufferSizeTC     = XRCCTRL(*this, "receiveBufferSizeTC"	, wxTextCtrl);
     maxConnSC		     = XRCCTRL(*this, "maxConnSC"		, wxSpinCtrl);
}

void NetworkSettingPanel::set_properties() 
{
     TextCtrlTupleList tupleList;
     tupleList.push_back(TextCtrlTuple(wxT("ID_Receive_Timeout_Sec"),           receiveTimeoutTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_Connection_Timeout_Sec"),        connectTimeoutTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelBasicAuthUserName"), basicAuthUserNameTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelBasicAuthPassword"), basicAuthPasswordTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxyReceiveAddr"),  recProxyAddrTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxyReceivePort"),  recProxyPortTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySendAddr"),     sedProxyAddrTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySendPort"),     sedProxyPortTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySSLAuthAddr"),  authSSLAddrTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySSLAuthPort"),  authSSLPortTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelBoardListURL"),	boardListURLTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelReceiveBufferSize"), receiveBufferSizeTC));
     // maxConnSC
     JC_GET_WIDGETS_PROPERTIES
}

void NetworkSettingPanel::save_properties()
{
     TextCtrlTupleList tupleList;
     tupleList.push_back(TextCtrlTuple(wxT("ID_Receive_Timeout_Sec"),           receiveTimeoutTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_Connection_Timeout_Sec"),        connectTimeoutTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelBasicAuthUserName"), basicAuthUserNameTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelBasicAuthPassword"), basicAuthPasswordTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxyReceiveAddr"),  recProxyAddrTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxyReceivePort"),  recProxyPortTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySendAddr"),     sedProxyAddrTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySendPort"),     sedProxyPortTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySSLAuthAddr"),  authSSLAddrTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelProxySSLAuthPort"),  authSSLPortTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelBoardListURL"),	boardListURLTC));
     tupleList.push_back(TextCtrlTuple(wxT("ID_NetworkPanelReceiveBufferSize"), receiveBufferSizeTC));
     // maxConnSC
     JC_SET_WIDGETS_PROPERTIES
}

/**
 * 各種パス設定用画面
 */
PathSettingPanel::PathSettingPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
wxPanel(parent, ID_PathSettingPanel, pos, size, wxTAB_TRAVERSAL) {

     // begin wxGlade: PathSettingPanel::PathSettingPanel
     browserCheck = new wxCheckBox(this, ID_BrowserCheck, wxT("ブラウザを指定する"));
     label_1 = new wxStaticText(this, wxID_ANY, wxT("ブラウザのパス"));

#ifdef __WXGTK__
     browserPathTC = new wxTextCtrl(this, 
				    ID_BrowserPathTC, 
				    wxEmptyString, 
				    wxDefaultPosition, 
				    wxDefaultSize,
				    wxTE_READONLY);
#endif
     dirPicker1 = new wxDirPickerCtrl(this, ID_DirPickerBrowser);
     clearButton_1 = new wxButton(this, ID_ClearBrowserPath, wxT("設定のクリア"));

     label_2 = new wxStaticText(this, wxID_ANY, wxT("ログとボード一覧のフォルダ（次回起動時から有効）"));
#ifdef __WXGTK__
     boardListTC = new wxTextCtrl(this, 
				  ID_BoardListTC, 
				  wxEmptyString,
				  wxDefaultPosition, 
				  wxDefaultSize,
				  wxTE_READONLY);
#endif
     dirPicker2 = new wxDirPickerCtrl(this, ID_DirPickerBoardList);
     clearButton_2 = new wxButton(this, ID_ClearBoardListPath, wxT("設定のクリア"));

     label_3 = new wxStaticText(this, wxID_ANY, wxT("スキンのフォルダ（次回起動時から有効）"));
#ifdef __WXGTK__
     skinPathTC = new wxTextCtrl(this, 
			       ID_SkinPathTC, 
			       wxEmptyString,
			       wxDefaultPosition, 
			       wxDefaultSize,
			       wxTE_READONLY);
#endif
     dirPicker3 = new wxDirPickerCtrl(this, ID_DirPickerSkin);
     clearButton_3 = new wxButton(this, ID_ClearSkinPath, wxT("設定のクリア"));

     set_properties();
     do_layout();
     // end wxGlade
}


void PathSettingPanel::set_properties() {
     bool specifyBrowser = false;
     wxString browserPath, boardListPath, skinPath;
     XrossBoardUtil::GetXrossBoardProperties(wxT("SPECIFY_DEFAULT_BROWSER"), &specifyBrowser);
     XrossBoardUtil::GetXrossBoardProperties(wxT("DEFAULT_BROWSER_PATH"), &browserPath);
     XrossBoardUtil::GetXrossBoardProperties(wxT("DEFAULT_BOARDLIST_PATH"), &boardListPath);
     XrossBoardUtil::GetXrossBoardProperties(wxT("DEFAULT_SKINFILE_PATH"), &skinPath);
     browserCheck->SetValue(specifyBrowser);

#ifdef __WXGTK__
     browserPathTC->SetValue(browserPath);
     boardListTC->SetValue(boardListPath);
     skinPathTC->SetValue(skinPath);
#else
     dirPicker1->SetPath(browserPath);
     dirPicker2->SetPath(boardListPath);
     dirPicker3->SetPath(skinPath);
#endif
}

/**
 * Linux(GTK) vs Windows & MacOSX で
 * ファイル選択用UIの形が異なるためここで調整をしている
 */
void PathSettingPanel::do_layout() {
    // begin wxGlade: PathSettingPanel::do_layout
#ifdef __WXGTK__
    wxGridSizer* grid_sizer_1 = new wxGridSizer(7, 2, 0, 0);
#else
    wxGridSizer* grid_sizer_1 = new wxGridSizer(7, 3, 0, 0);
#endif
    grid_sizer_1->Add(browserCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(20, 20, 0, wxEXPAND, 0);

#ifdef __WXGTK__
    grid_sizer_1->Add(label_1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(browserPathTC, 0, wxALL|wxEXPAND, 5);
    grid_sizer_1->Add(dirPicker1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(clearButton_1, 2, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
#else
    grid_sizer_1->Add(20, 20, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(dirPicker1, 1, wxALL|wxEXPAND, 5);
    grid_sizer_1->Add(clearButton_1, 2, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
#endif

#ifdef __WXGTK__
    grid_sizer_1->Add(label_2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(boardListTC, 0, wxALL|wxEXPAND, 5);
    grid_sizer_1->Add(dirPicker2, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(clearButton_2, 2, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
#else
    grid_sizer_1->Add(label_2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(dirPicker2, 1, wxALL|wxEXPAND, 5);
    grid_sizer_1->Add(clearButton_2, 2, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
#endif

#ifdef __WXGTK__
    grid_sizer_1->Add(label_3, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(skinPathTC, 0, wxALL|wxEXPAND, 5);
    grid_sizer_1->Add(dirPicker3, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(clearButton_3, 2, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
#else
    grid_sizer_1->Add(label_3, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
    grid_sizer_1->Add(dirPicker3, 1, wxALL|wxEXPAND, 5);
    grid_sizer_1->Add(clearButton_3, 2, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
#endif

    SetSizer(grid_sizer_1);
    grid_sizer_1->Fit(this);
    // end wxGlade
}

/**
 * 各種動作設定用画面
 */
BehaviorPanel::BehaviorPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, ID_BehaviorPanel, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: BehaviorPanel::BehaviorPanel
    panel_5 = new wxPanel(this, wxID_ANY);
    panel_4 = new wxPanel(this, wxID_ANY);
    panel_7 = new wxPanel(panel_4, wxID_ANY);
    panel_3 = new wxPanel(this, wxID_ANY);
    panel_6 = new wxPanel(panel_3, wxID_ANY);
    panel_2 = new wxPanel(this, wxID_ANY);
    sizer_3_staticbox = new wxStaticBox(panel_3, -1, wxT("スレ一覧"));
    sizer_5_staticbox = new wxStaticBox(panel_4, -1, wxT("スレ"));
    sizer_7_staticbox = new wxStaticBox(panel_5, -1, wxT("お気に入り"));
    sizer_2_staticbox = new wxStaticBox(panel_2, -1, wxT("板一覧"));
    openBNewTabCheck = new wxCheckBox(panel_2, ID_OpenBNewTabCheck, wxT("新しいタブで板を開く"));
    onlyOneCategoryCheck = new wxCheckBox(panel_2, ID_OnlyOneCategoryCheck, wxT("カテゴリを一つしか開かない"));
    openTNewTabCheck = new wxCheckBox(panel_6, ID_OpenTNewTabCheck, wxT("新しいタブでスレを開く"));
    const wxString threadNewTabCombo_choices[] = {
        wxT("常に最大化"),
        wxT("常に通常サイズ")
    };
    threadNewTabCombo = new wxComboBox(panel_6, ID_ThreadNewTabCombo, wxT(""), wxDefaultPosition, wxDefaultSize, 2, threadNewTabCombo_choices, wxCB_DROPDOWN|wxCB_READONLY);
    lastReadThreadPosCheck = new wxCheckBox(panel_3, ID_LastReadThreadPosCheck, wxT("最後に見たスレの位置にジャンプ"));
    const wxString threadJumpSettingRadio_choices[] = {
        wxT("見ていたところにジャンプする"),
        wxT("新着にジャンプする")
    };
    threadJumpSettingRadio = new wxRadioBox(panel_4, ID_ThreadJumpSetting, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, threadJumpSettingRadio_choices, 0, wxRA_SPECIFY_ROWS);
    redrawNewThreadCheck = new wxCheckBox(panel_4, ID_RedrawNewThreadCheck, wxT("新着チェック時に既得レスを描き直す"));
    label_1 = new wxStaticText(panel_7, wxID_ANY, wxT("既得スレの表示レス数"));
    const wxString showResponseCombo_choices[] = {
        wxT("全レス表示"),
        wxT("最新50レス"),
        wxT("最新100レス"),
        wxT("最新250レス"),
        wxT("最新500レス")
    };
    showResponseCombo = new wxComboBox(panel_7, ID_ShowResponseCombo, wxT(""), wxDefaultPosition, wxDefaultSize, 5, showResponseCombo_choices, wxCB_DROPDOWN|wxCB_READONLY);
    favoriteNewTabCheck = new wxCheckBox(panel_5, ID_FavoriteNewTabCheck, wxT("新しいタブで開く"));
    favoriteOnlyOneFolderCheck = new wxCheckBox(panel_5, ID_FavoriteOnlyOneFolder, wxT("フォルダを一つしか開かない"));
    set_properties();
    do_layout();
    // end wxGlade
}


void BehaviorPanel::set_properties()
{
     // begin wxGlade: BehaviorPanel::set_properties
     threadNewTabCombo->SetSelection(0);
     threadJumpSettingRadio->SetSelection(0);
     showResponseCombo->SetSelection(-1);
     // end wxGlade
}


void BehaviorPanel::do_layout()
{
     // begin wxGlade: BehaviorPanel::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxStaticBoxSizer* sizer_7 = new wxStaticBoxSizer(sizer_7_staticbox, wxVERTICAL);
     wxStaticBoxSizer* sizer_5 = new wxStaticBoxSizer(sizer_5_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_2 = new wxStaticBoxSizer(sizer_2_staticbox, wxVERTICAL);
     sizer_2->Add(openBNewTabCheck, 1, wxEXPAND, 0);
     sizer_2->Add(onlyOneCategoryCheck, 1, wxEXPAND, 0);
     panel_2->SetSizer(sizer_2);
     sizer_1->Add(panel_2, 0, wxEXPAND, 0);
     sizer_4->Add(openTNewTabCheck, 1, wxEXPAND, 0);
     sizer_4->Add(threadNewTabCombo, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
     panel_6->SetSizer(sizer_4);
     sizer_3->Add(panel_6, 1, wxEXPAND, 0);
     sizer_3->Add(lastReadThreadPosCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
     panel_3->SetSizer(sizer_3);
     sizer_1->Add(panel_3, 0, wxEXPAND, 0);
     sizer_5->Add(threadJumpSettingRadio, 0, 0, 0);
     sizer_5->Add(redrawNewThreadCheck, 0, 0, 0);
     sizer_6->Add(label_1, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
     sizer_6->Add(showResponseCombo, 1, 0, 0);
     panel_7->SetSizer(sizer_6);
     sizer_5->Add(panel_7, 1, wxEXPAND, 0);
     panel_4->SetSizer(sizer_5);
     sizer_1->Add(panel_4, 0, wxEXPAND, 0);
     sizer_7->Add(favoriteNewTabCheck, 0, 0, 0);
     sizer_7->Add(favoriteOnlyOneFolderCheck, 0, 0, 0);
     panel_5->SetSizer(sizer_7);
     sizer_1->Add(panel_5, 0, wxEXPAND, 0);
     SetSizer(sizer_1);
     sizer_1->Fit(this);
     // end wxGlade
}

// begin wxGlade: ::extracode
// end wxGlade

/**
 * 各種操作設定用画面
 */
OperationPanel::OperationPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, ID_OperationPanel, pos, size, wxTAB_TRAVERSAL)
{
     // begin wxGlade: OperationPanel::OperationPanel
     panel_3 = new wxPanel(this, wxID_ANY);
     panel_7 = new wxPanel(panel_3, wxID_ANY);
     panel_14 = new wxPanel(panel_7, wxID_ANY);
     panel_13 = new wxPanel(panel_7, wxID_ANY);
     panel_12 = new wxPanel(panel_7, wxID_ANY);
     panel_6 = new wxPanel(panel_3, wxID_ANY);
     panel_11_copy = new wxPanel(panel_6, wxID_ANY);
     panel_10_copy = new wxPanel(panel_6, wxID_ANY);
     panel_9_copy = new wxPanel(panel_6, wxID_ANY);
     panel_8_copy = new wxPanel(panel_6, wxID_ANY);
     panel_2 = new wxPanel(this, wxID_ANY);
     panel_5 = new wxPanel(panel_2, wxID_ANY);
     panel_4 = new wxPanel(panel_2, wxID_ANY);
     panel_11 = new wxPanel(panel_4, wxID_ANY);
     panel_10 = new wxPanel(panel_4, wxID_ANY);
     panel_9 = new wxPanel(panel_4, wxID_ANY);
     sizer_4_staticbox = new wxStaticBox(panel_4, -1, wxT("板に関する操作"));
     sizer_5_staticbox = new wxStaticBox(panel_5, -1, wxT("スレを裏で開く"));
     sizer_6_staticbox = new wxStaticBox(panel_6, -1, wxT("スレに関する操作"));
     sizer_7_staticbox = new wxStaticBox(panel_7, -1, wxT("リロード制限"));
     panel_8 = new wxPanel(panel_4, wxID_ANY);
     checkbox_1 = new wxCheckBox(this, wxID_ANY, wxT("シングルクリックでカテゴリーを開く"));
     checkbox_2 = new wxCheckBox(this, wxID_ANY, wxT("未読があるスレタブを選択するとレス取得"));
     checkbox_3 = new wxCheckBox(this, wxID_ANY, wxT("お気に入り板が対象でもスレッド一覧を更新する"));
     label_1 = new wxStaticText(panel_8, wxID_ANY, wxT("シングルクリック"));

     // コンボボックスの選択肢
     const wxString combo_box_choices[] = {
	  wxT("無効"),
	  wxT("常にローカル"),
	  wxT("無ければ取得"),
	  wxT("更新チェック")
     };
     
     combo_box_1 = new wxComboBox(panel_8, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_2 = new wxStaticText(panel_9, wxID_ANY, wxT("ダブルクリック"));
     combo_box_2 = new wxComboBox(panel_9, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_3 = new wxStaticText(panel_10, wxID_ANY, wxT("メニュー・リンクバー"));
     combo_box_3 = new wxComboBox(panel_10, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_4 = new wxStaticText(panel_11, wxID_ANY, wxT("その他"));
     combo_box_4 = new wxComboBox(panel_11, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     checkbox_4 = new wxCheckBox(panel_5, wxID_ANY, wxT("スレ覧（シングルクリック）"));
     checkbox_5 = new wxCheckBox(panel_5, wxID_ANY, wxT("お気に入り"));
     checkbox_6 = new wxCheckBox(panel_5, wxID_ANY, wxT("閉じたスレ"));
     checkbox_7 = new wxCheckBox(panel_5, wxID_ANY, wxT("アドレスバー直打ち"));
     checkbox_8 = new wxCheckBox(panel_5, wxID_ANY, wxT("スレ内リンク"));
     checkbox_9 = new wxCheckBox(panel_5, wxID_ANY, wxT("〜をすべて開く"));
     label_1_copy = new wxStaticText(panel_8_copy, wxID_ANY, wxT("シングルクリック"));
     combo_box_1_copy = new wxComboBox(panel_8_copy, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_2_copy = new wxStaticText(panel_9_copy, wxID_ANY, wxT("ダブルクリック"));
     combo_box_2_copy = new wxComboBox(panel_9_copy, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_3_copy = new wxStaticText(panel_10_copy, wxID_ANY, wxT("メニュー・リンクバー"));
     combo_box_3_copy = new wxComboBox(panel_10_copy, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_4_copy = new wxStaticText(panel_11_copy, wxID_ANY, wxT("その他"));
     combo_box_4_copy = new wxComboBox(panel_11_copy, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 4, combo_box_choices, wxCB_DROPDOWN);
     label_5 = new wxStaticText(panel_12, wxID_ANY, wxT("スレッド一覧(秒)"));
     spin_ctrl_1 = new wxSpinCtrl(panel_12, wxID_ANY, wxT("15"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
     label_6 = new wxStaticText(panel_13, wxID_ANY, wxT("レス表示欄(秒)"));
     spin_ctrl_2 = new wxSpinCtrl(panel_13, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
     label_7 = new wxStaticText(panel_14, wxID_ANY, wxT("更新チェック(秒)\n（板数×秒）"));
     spin_ctrl_3 = new wxSpinCtrl(panel_14, wxID_ANY, wxT("15"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);

     set_properties();
     do_layout();
     // end wxGlade
}


void OperationPanel::set_properties()
{
     // begin wxGlade: OperationPanel::set_properties
     // end wxGlade
}


void OperationPanel::do_layout()
{
     // begin wxGlade: OperationPanel::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_7 = new wxStaticBoxSizer(sizer_7_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_14 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_13 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_12 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_6 = new wxStaticBoxSizer(sizer_6_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_11_copy = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_10_copy = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_9_copy = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_8_copy = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_5 = new wxStaticBoxSizer(sizer_5_staticbox, wxVERTICAL);
     wxStaticBoxSizer* sizer_4 = new wxStaticBoxSizer(sizer_4_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_11 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_10 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_9 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
     sizer_1->Add(checkbox_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_1->Add(checkbox_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_1->Add(checkbox_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_8->Add(label_1, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_8->Add(combo_box_1, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_8->SetSizer(sizer_8);
     sizer_4->Add(panel_8, 1, wxEXPAND, 0);
     sizer_9->Add(label_2, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_9->Add(combo_box_2, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_9->SetSizer(sizer_9);
     sizer_4->Add(panel_9, 1, wxEXPAND, 0);
     sizer_10->Add(label_3, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_10->Add(combo_box_3, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_10->SetSizer(sizer_10);
     sizer_4->Add(panel_10, 1, wxEXPAND, 0);
     sizer_11->Add(label_4, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_11->Add(combo_box_4, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_11->SetSizer(sizer_11);
     sizer_4->Add(panel_11, 1, wxEXPAND, 0);
     panel_4->SetSizer(sizer_4);
     sizer_2->Add(panel_4, 1, wxALL|wxEXPAND, 5);
     sizer_5->Add(checkbox_4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(checkbox_5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(checkbox_6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(checkbox_7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(checkbox_8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(checkbox_9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_5->SetSizer(sizer_5);
     sizer_2->Add(panel_5, 1, wxALL|wxEXPAND, 5);
     panel_2->SetSizer(sizer_2);
     sizer_1->Add(panel_2, 1, wxEXPAND, 0);
     sizer_8_copy->Add(label_1_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_8_copy->Add(combo_box_1_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_8_copy->SetSizer(sizer_8_copy);
     sizer_6->Add(panel_8_copy, 1, wxEXPAND, 0);
     sizer_9_copy->Add(label_2_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_9_copy->Add(combo_box_2_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_9_copy->SetSizer(sizer_9_copy);
     sizer_6->Add(panel_9_copy, 1, wxEXPAND, 0);
     sizer_10_copy->Add(label_3_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_10_copy->Add(combo_box_3_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_10_copy->SetSizer(sizer_10_copy);
     sizer_6->Add(panel_10_copy, 1, wxEXPAND, 0);
     sizer_11_copy->Add(label_4_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_11_copy->Add(combo_box_4_copy, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_11_copy->SetSizer(sizer_11_copy);
     sizer_6->Add(panel_11_copy, 1, wxEXPAND, 0);
     panel_6->SetSizer(sizer_6);
     sizer_3->Add(panel_6, 1, wxALL|wxEXPAND, 5);
     sizer_12->Add(label_5, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_12->Add(spin_ctrl_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     panel_12->SetSizer(sizer_12);
     sizer_7->Add(panel_12, 1, wxEXPAND, 0);
     sizer_13->Add(label_6, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_13->Add(spin_ctrl_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     panel_13->SetSizer(sizer_13);
     sizer_7->Add(panel_13, 1, wxEXPAND, 0);
     sizer_14->Add(label_7, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_14->Add(spin_ctrl_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     panel_14->SetSizer(sizer_14);
     sizer_7->Add(panel_14, 1, wxEXPAND, 0);
     panel_7->SetSizer(sizer_7);
     sizer_3->Add(panel_7, 1, wxALL|wxEXPAND, 5);
     panel_3->SetSizer(sizer_3);
     sizer_1->Add(panel_3, 1, wxEXPAND, 0);
     SetSizer(sizer_1);
     sizer_1->Fit(this);
     // end wxGlade
}

// begin wxGlade: ::extracode
// end wxGlade

/**
 * 書き込み設定用画面
 */
KakikomiPanel::KakikomiPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, ID_KakikomiPanel, pos, size, wxTAB_TRAVERSAL)
{
     // begin wxGlade: KakikomiPanel::KakikomiPanel
     panel_3 = new wxPanel(this, wxID_ANY);
     panel_6 = new wxPanel(panel_3, wxID_ANY);
     panel_8 = new wxPanel(panel_6, wxID_ANY);
     panel_10 = new wxPanel(panel_8, wxID_ANY);
     panel_7 = new wxPanel(panel_6, wxID_ANY);
     panel_9 = new wxPanel(panel_7, wxID_ANY);
     panel_5 = new wxPanel(panel_3, wxID_ANY);
     panel_4 = new wxPanel(panel_3, wxID_ANY);
     panel_2 = new wxPanel(this, wxID_ANY);
     checkbox_1 = new wxCheckBox(panel_2, wxID_ANY, wxT("書き込み履歴をkakikomi.txtに保存する"));
     checkbox_2 = new wxCheckBox(panel_2, wxID_ANY, wxT("失敗時も保存する"));
     checkbox_3 = new wxCheckBox(panel_2, wxID_ANY, wxT("書き込みが成功した気がするときには閉じる"));
     checkbox_4 = new wxCheckBox(panel_2, wxID_ANY, wxT("スレッドタイトルパネル表示"));
     checkbox_5 = new wxCheckBox(panel_2, wxID_ANY, wxT("タスクバーを使用する"));
     checkbox_6 = new wxCheckBox(panel_2, wxID_ANY, wxT("デフォルトでsageチェック"));
     checkbox_7 = new wxCheckBox(panel_2, wxID_ANY, wxT("ステータスバー非表示"));
     checkbox_8 = new wxCheckBox(panel_2, wxID_ANY, wxT("デフォルトでBEチェック"));
     checkbox_9 = new wxCheckBox(panel_2, wxID_ANY, wxT("Shift+Enterでの書き込みを無効にする"));
     checkbox_10 = new wxCheckBox(panel_2, wxID_ANY, wxT("コテハン警告"));
     checkbox_11 = new wxCheckBox(panel_2, wxID_ANY, wxT("誤爆警告"));
     checkbox_12 = new wxCheckBox(panel_2, wxID_ANY, wxT("BEログイン警告"));
     checkbox_13 = new wxCheckBox(panel_2, wxID_ANY, wxT("Samba警告"));
     label_1 = new wxStaticText(panel_4, wxID_ANY, wxT("「これにレス」の記号"));
     text_ctrl_1 = new wxTextCtrl(panel_4, wxID_ANY, wxT(">>"));
     label_2 = new wxStaticText(panel_4, wxID_ANY, wxT("カウント補正(秒)"));
     spin_ctrl_1 = new wxSpinCtrl(panel_4, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
     label_3 = new wxStaticText(panel_5, wxID_ANY, wxT("AAListの形式"));
     const wxString combo_box_choices[] = {
	  wxT("リスト形式"),
	  wxT("メニュー形式")
     };
     
     combo_box_1 = new wxComboBox(panel_5, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 2, combo_box_choices, wxCB_DROPDOWN);
     label_4 = new wxStaticText(panel_9, wxID_ANY, wxT("名前"));
     checkbox_14 = new wxCheckBox(panel_9, wxID_ANY, wxT("１行目をデフォルト"));
     text_ctrl_2 = new wxTextCtrl(panel_7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL);
     label_5 = new wxStaticText(panel_10, wxID_ANY, wxT("メール"));
     checkbox_15 = new wxCheckBox(panel_10, wxID_ANY, wxT("１行目をデフォルト"));
     text_ctrl_3 = new wxTextCtrl(panel_8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL);

     set_properties();
     do_layout();
     // end wxGlade
}


void KakikomiPanel::set_properties()
{
     // begin wxGlade: KakikomiPanel::set_properties
     // end wxGlade
}


void KakikomiPanel::do_layout()
{
     // begin wxGlade: KakikomiPanel::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_8 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_9 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_6 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
     wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(7, 2, 0, 0);
     grid_sizer_1->Add(checkbox_1, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_2, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_3, 1, wxALL, 5);
     grid_sizer_1->Add(20, 20, 0, wxEXPAND, 0);
     grid_sizer_1->Add(checkbox_4, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_5, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_6, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_7, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_8, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_9, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_10, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_11, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_12, 1, wxALL, 5);
     grid_sizer_1->Add(checkbox_13, 1, wxALL, 5);
     panel_2->SetSizer(grid_sizer_1);
     sizer_1->Add(panel_2, 1, wxEXPAND, 0);
     sizer_3->Add(label_1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     sizer_3->Add(text_ctrl_1, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_3->Add(label_2, 1, wxALIGN_CENTER_VERTICAL, 0);
     sizer_3->Add(spin_ctrl_1, 1, wxALIGN_CENTER_VERTICAL, 0);
     panel_4->SetSizer(sizer_3);
     sizer_2->Add(panel_4, 0, wxEXPAND, 0);
     sizer_4->Add(label_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_4->Add(combo_box_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_5->SetSizer(sizer_4);
     sizer_2->Add(panel_5, 0, wxEXPAND, 0);
     sizer_7->Add(label_4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(checkbox_14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_9->SetSizer(sizer_7);
     sizer_6->Add(panel_9, 0, wxEXPAND, 0);
     sizer_6->Add(text_ctrl_2, 1, wxALL|wxEXPAND, 5);
     panel_7->SetSizer(sizer_6);
     sizer_5->Add(panel_7, 1, wxEXPAND, 0);
     sizer_9->Add(label_5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_9->Add(checkbox_15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_10->SetSizer(sizer_9);
     sizer_8->Add(panel_10, 0, wxEXPAND, 0);
     sizer_8->Add(text_ctrl_3, 1, wxALL|wxEXPAND, 5);
     panel_8->SetSizer(sizer_8);
     sizer_5->Add(panel_8, 1, wxEXPAND, 0);
     panel_6->SetSizer(sizer_5);
     sizer_2->Add(panel_6, 1, wxEXPAND, 0);
     panel_3->SetSizer(sizer_2);
     sizer_1->Add(panel_3, 1, wxEXPAND, 0);
     SetSizer(sizer_1);
     sizer_1->Fit(this);
     // end wxGlade
}

/**
 * スレ表示欄の設定画面
 */
DoePanel::DoePanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, ID_DoePanel, pos, size, wxTAB_TRAVERSAL)
{
     // begin wxGlade: DoePanel::DoePanel
     panel_8 = new wxPanel(this, wxID_ANY);
     panel_7 = new wxPanel(this, wxID_ANY);
     panel_6 = new wxPanel(this, wxID_ANY);
     panel_5 = new wxPanel(this, wxID_ANY);
     panel_4 = new wxPanel(this, wxID_ANY);
     panel_3 = new wxPanel(this, wxID_ANY);
     panel_9 = new wxPanel(panel_3, wxID_ANY);
     sizer_3_staticbox = new wxStaticBox(panel_3, -1, wxT("スクロール"));
     panel_2 = new wxPanel(this, wxID_ANY);
     label_1 = new wxStaticText(panel_2, wxID_ANY, wxT("上下スクロール時に残す行数"));
     text_ctrl_1 = new wxTextCtrl(panel_2, wxID_ANY, wxEmptyString);
     radio_btn_1 = new wxRadioButton(panel_3, wxID_ANY, wxT("次の行数ごとスクロール"));
     panel_10 = new wxPanel(panel_9, wxID_ANY);
     text_ctrl_2 = new wxTextCtrl(panel_9, wxID_ANY, wxEmptyString);
     label_2 = new wxStaticText(panel_9, wxID_ANY, wxT("スムーズネス"));
     spin_ctrl_1 = new wxSpinCtrl(panel_9, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
     label_3 = new wxStaticText(panel_9, wxID_ANY, wxT("フレームレート"));
     spin_ctrl_2 = new wxSpinCtrl(panel_9, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
     radio_btn_2 = new wxRadioButton(panel_3, wxID_ANY, wxT("１ページごとスクロール"));
     checkbox_1 = new wxCheckBox(panel_3, wxID_ANY, wxT("オートスクロールを有効にする"));
     checkbox_2 = new wxCheckBox(panel_4, wxID_ANY, wxT("キャレットを表示する"));
     checkbox_3 = new wxCheckBox(panel_5, wxID_ANY, wxT("スクロールに同期する"));
     label_4 = new wxStaticText(panel_6, wxID_ANY, wxT("ブラウザのフォントサイズ（最小→最大）"));
     text_ctrl_3 = new wxTextCtrl(panel_7, wxID_ANY, wxEmptyString);
     text_ctrl_4 = new wxTextCtrl(panel_7, wxID_ANY, wxEmptyString);
     text_ctrl_5 = new wxTextCtrl(panel_7, wxID_ANY, wxEmptyString);
     text_ctrl_6 = new wxTextCtrl(panel_7, wxID_ANY, wxEmptyString);
     text_ctrl_7 = new wxTextCtrl(panel_7, wxID_ANY, wxEmptyString);
     label_5 = new wxStaticText(panel_8, wxID_ANY, wxT("ハイライトの色"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
     button_1 = new wxButton(panel_8, wxID_ANY, wxT("..."));

     set_properties();
     do_layout();
     // end wxGlade
}


void DoePanel::set_properties()
{
     // begin wxGlade: DoePanel::set_properties
     label_5->SetMinSize(wxSize(250, 19));
     // end wxGlade
}


void DoePanel::do_layout()
{
     // begin wxGlade: DoePanel::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_6 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_5 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_4 = new wxBoxSizer(wxVERTICAL);
     wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
     wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(3, 2, 0, 0);
     wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
     sizer_2->Add(label_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_2->Add(text_ctrl_1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_2->SetSizer(sizer_2);
     sizer_1->Add(panel_2, 0, wxEXPAND, 0);
     sizer_3->Add(radio_btn_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     grid_sizer_1->Add(panel_10, 1, wxEXPAND, 0);
     grid_sizer_1->Add(text_ctrl_2, 0, 0, 0);
     grid_sizer_1->Add(label_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     grid_sizer_1->Add(spin_ctrl_1, 0, 0, 0);
     grid_sizer_1->Add(label_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     grid_sizer_1->Add(spin_ctrl_2, 0, 0, 0);
     panel_9->SetSizer(grid_sizer_1);
     sizer_3->Add(panel_9, 1, wxEXPAND, 0);
     sizer_3->Add(radio_btn_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_3->Add(checkbox_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_3->SetSizer(sizer_3);
     sizer_1->Add(panel_3, 0, wxALL|wxEXPAND, 5);
     sizer_4->Add(checkbox_2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_4->SetSizer(sizer_4);
     sizer_1->Add(panel_4, 0, wxEXPAND, 0);
     sizer_5->Add(checkbox_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_5->SetSizer(sizer_5);
     sizer_1->Add(panel_5, 0, wxEXPAND, 0);
     sizer_6->Add(label_4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_6->SetSizer(sizer_6);
     sizer_1->Add(panel_6, 0, wxEXPAND, 0);
     sizer_7->Add(text_ctrl_3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(text_ctrl_4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(text_ctrl_5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(text_ctrl_6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(text_ctrl_7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     panel_7->SetSizer(sizer_7);
     sizer_1->Add(panel_7, 0, wxEXPAND, 0);
     sizer_8->Add(label_5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_8->Add(button_1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxFIXED_MINSIZE, 5);
     panel_8->SetSizer(sizer_8);
     sizer_1->Add(panel_8, 0, wxEXPAND, 0);
     SetSizer(sizer_1);
     sizer_1->Fit(this);
     // end wxGlade
}

/**
 * その他の設定画面１
 */
const wxString OtherSettingPanelOne::checkboxlabels[] = {
     wxT("最新バージョンが公開されたら通知する"),
     wxT("起動時に板一覧を更新する"),
     wxT("メニューに板一覧を展開する"),
     wxT("メニューにお気に入りを展開する"),
     wxT("お気に入りメニューで「このフォルダを開く」を使用する"),
     wxT("お気に入りの重複を許可する"),
     wxT("「お気に入りに追加」をフォルダの末尾で行う"),
     wxT("終了時に開いていたスレ・板を次回起動時に開く"),
     wxT("既読にする"),
     wxT("最小化時はタスクトレイに格納する"), // ここまでで10個
     wxT("「一つ前のレスを貼り付け」のショートカットキー（Shift+BackSpace）を無効にする"),
     wxT("最後までスクロールしたスレを既読にする"),
     wxT("再描画する"),
     wxT("すべて開く系動作時に更新チェックを同時に行う"),
     wxT("「すべてのタブの新着チェック」で未読のあるタブの文字色を変更しない"),
     wxT("トレースにベンチマークを表示する")  // 16個
};

OtherSettingPanelOne::OtherSettingPanelOne(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, ID_OtherSettingPanelOne, pos, size, wxTAB_TRAVERSAL)
{
     for ( size_t i = 0; i < XrossBoardUtil::ArrayLength(checkboxlabels); i++ )
     {
	  panels[i]      = new wxPanel(this, wxID_ANY);
	  checkboxes[i]  = new wxCheckBox(panels[i] , wxID_ANY, checkboxlabels[i]);
     }
     
     restPanels[0] = new wxPanel(this, wxID_ANY);
     retainBoardST = new wxStaticText(restPanels[0], wxID_ANY, wxT("解放しないで保持する板情報の数"));
     retainBoardSC = new wxSpinCtrl(restPanels[0], wxID_ANY);

     restPanels[1] = new wxPanel(this, wxID_ANY);
     glanceSC      = new wxStaticText(restPanels[1], wxID_ANY, wxT("「ちょっと見る」の範囲"));
     glanceTC      = new wxTextCtrl(restPanels[1], wxID_ANY, wxT(""));

     restPanels[2] = new wxPanel(this, wxID_ANY);
     autoSaveST = new wxStaticText(restPanels[2], wxID_ANY, wxT("秒ごとに開いている板・スレを保存する"));
     autoSaveSC = new wxSpinCtrl(restPanels[2], wxID_ANY);

     set_properties();
     do_layout();
}

void OtherSettingPanelOne::set_properties()
{
}

void OtherSettingPanelOne::do_layout()
{

     wxBoxSizer* sizer_0  = new wxBoxSizer(wxVERTICAL);

     // チェックボックス
     const size_t checkboxsize = 16;
     wxBoxSizer* sizers[checkboxsize] = {};

     for ( size_t i = 0; i < checkboxsize; i++ )
     {
	  sizers[i] = new wxBoxSizer(wxVERTICAL);
	  sizer_0->Add(panels[i], 0, wxALL|wxEXPAND, 0);
	  sizers[i]->Add(checkboxes[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	  panels[i]->SetSizer(sizers[i]);
     }

     /**
      * 残りのwidgets
      */

     // 解放しないで保持する板情報の数
     wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
     sizer_0->Add(restPanels[0], 0, wxALL|wxEXPAND, 0);
     box1->Add(retainBoardST, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     box1->Add(retainBoardSC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     restPanels[0]->SetSizer(box1);

     // 「ちょっと見る」の範囲
     wxBoxSizer* box2 = new wxBoxSizer(wxHORIZONTAL);
     sizer_0->Add(restPanels[1], 0, wxALL|wxEXPAND, 0);
     box2->Add(glanceSC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     box2->Add(glanceTC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     restPanels[1]->SetSizer(box2);

     // 秒ごとに開いている板・スレを保存する
     wxBoxSizer* box3 = new wxBoxSizer(wxHORIZONTAL);
     sizer_0->Add(restPanels[2], 0, wxALL|wxEXPAND, 0);
     box3->Add(autoSaveSC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     box3->Add(autoSaveST, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
     restPanels[2]->SetSizer(box3);

     SetSizer(sizer_0);
     sizer_0->Fit(this);
}

/**
 * 色・フォント設定用画面
 */
// begin wxGlade: ::extracode
// end wxGlade
ColorFontSettingPanel::ColorFontSettingPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, ID_ColorFontSettingPanel, pos, size, wxTAB_TRAVERSAL)
{
     // begin wxGlade: ColorFontSettingPanel::ColorFontSettingPanel
     panel_3 = new wxPanel(this, wxID_ANY);
     panel_2 = new wxPanel(this, wxID_ANY);
     panel_6 = new wxPanel(panel_2, wxID_ANY);
     panel_13 = new wxPanel(panel_6, wxID_ANY);
     panel_5 = new wxPanel(panel_2, wxID_ANY);
     panel_12 = new wxPanel(panel_5, wxID_ANY);
     panel_11 = new wxPanel(panel_5, wxID_ANY);
     panel_10 = new wxPanel(panel_5, wxID_ANY);
     panel_4 = new wxPanel(panel_2, wxID_ANY);
     panel_9 = new wxPanel(panel_4, wxID_ANY);
     panel_8 = new wxPanel(panel_4, wxID_ANY);
     sizer_4_staticbox = new wxStaticBox(panel_4, -1, wxT("プレビュー"));
     sizer_5_staticbox = new wxStaticBox(panel_5, -1, wxT("フォント"));
     sizer_6_staticbox = new wxStaticBox(panel_6, -1, wxT("背景色"));
     panel_7 = new wxPanel(panel_4, wxID_ANY);

#if wxCHECK_VERSION(2, 9, 1)
     treeSampleLabel = new wxGenericStaticText(panel_4, ID_TreeSampleLabel, wxT("ツリー"));
     threadListSampleLabel = new wxGenericStaticText(panel_7, ID_ThreadListSampleLabel, wxT("スレ覧"));
     extractSampleLabel = new wxGenericStaticText(panel_7, ID_ExtractSampleLabel, wxT("抽出"));
     logWindowSampleLabel = new wxGenericStaticText(panel_4, ID_LogWindowSampleLabel, wxT("ログ出力画面"));
     threadTitleSampleLabel = new wxGenericStaticText(panel_4, ID_ThreadTitleSampleLabel, wxT("スレタイトル"));
     threadViewSampleLabel = new wxGenericStaticText(panel_4, ID_ThreadViewSampleLabel, wxT("スレビュー"));
     kakikoSampleLabel = new wxGenericStaticText(panel_8, ID_KakikoSampleLabel, wxT("書込み"));
     memoSampleLabel = new wxGenericStaticText(panel_8, ID_MemoSampleLabel, wxT("メモ欄"));
     hintSampleLabel = new wxGenericStaticText(panel_9, ID_HintSampleLabel, wxT("ヒント"));
     linkSampleLabel = new wxGenericStaticText(panel_9, ID_LinkSampleLabel, wxT("Link"));
     othersSampleLabel = new wxGenericStaticText(panel_4, ID_OthersSampleLabel, wxT("その他"));
#else
     treeSampleLabel = new BasicDrawPane(panel_4, ID_TreeSampleLabel, wxT("ツリー"));
     threadListSampleLabel = new BasicDrawPane(panel_7, ID_ThreadListSampleLabel, wxT("スレ覧"));
     extractSampleLabel = new BasicDrawPane(panel_7, ID_ExtractSampleLabel, wxT("抽出"));
     logWindowSampleLabel = new BasicDrawPane(panel_4, ID_LogWindowSampleLabel, wxT("ログ出力画面"));
     threadTitleSampleLabel = new BasicDrawPane(panel_4, ID_ThreadTitleSampleLabel, wxT("スレタイトル"));
     threadViewSampleLabel = new BasicDrawPane(panel_4, ID_ThreadViewSampleLabel, wxT("スレビュー"));
     kakikoSampleLabel = new BasicDrawPane(panel_8, ID_KakikoSampleLabel, wxT("書込み"));
     memoSampleLabel = new BasicDrawPane(panel_8, ID_MemoSampleLabel, wxT("メモ欄"));
     hintSampleLabel = new BasicDrawPane(panel_9, ID_HintSampleLabel, wxT("ヒント"));
     linkSampleLabel = new BasicDrawPane(panel_9, ID_LinkSampleLabel, wxT("Link"));
     othersSampleLabel = new BasicDrawPane(panel_4, ID_OthersSampleLabel, wxT("その他"));
#endif

     treeFontButton = new wxButton(panel_5, ID_TreeFontButton, wxT("ツリー"));
     threadListFontButton = new wxButton(panel_10, ID_ThreadListFontButton, wxT("スレ欄"));
     extractFontButton = new wxButton(panel_10, ID_ExtractFontButton, wxT("抽出"));
     logWindowFontButton = new wxButton(panel_5, ID_LogWindowFontButton, wxT("ログ出力画面"));
     threadTitleFontButton = new wxButton(panel_5, ID_ThreadTitleFontButton, wxT("スレタイトル"));
     kakikoFontButton = new wxButton(panel_11, ID_KakikoFontButton, wxT("書込み"));
     memoFontButton = new wxButton(panel_11, ID_MemoFontButton, wxT("メモ"));
     hintFontButton = new wxButton(panel_12, ID_HintFontButton, wxT("ヒント"));
     linkFontButton = new wxButton(panel_12, ID_LinkFontButton, wxT("Link"));
     otherFontButton = new wxButton(panel_5, ID_OthersFontButton, wxT("その他"));
     allFontButton = new wxButton(panel_5, ID_AllFontButton, wxT("すべて"));
     treeBGColorButton = new wxButton(panel_6, ID_BoardListBGColorButton, wxT("ツリー"));
     threadListBGColorButton = new wxButton(panel_6, ID_ThreadListBGColorButton, wxT("スレ覧"));
     logWindowBGColorButton = new wxButton(panel_6, ID_LogWindowBGColorButton, wxT("ログ出力画面"));
     threadTitleBGColorButton = new wxButton(panel_6, ID_ThreadTitleBGColorButton, wxT("スレタイトル"));
     threadViewBGColorButton = new wxButton(panel_6, ID_ThreadViewBGColorButton, wxT("スレビュー"));
     memoBGColorButton = new wxButton(panel_6, ID_MemoBGColorButton, wxT("メモ欄"));
     hintBGColorButton = new wxButton(panel_13, ID_HintBGColorButton, wxT("ヒント"));
     popupBGColorButton_copy_1 = new wxButton(panel_13, ID_PopupBGColorButton, wxT("ポップアップ"));
     allBGColorButton = new wxButton(panel_6, ID_AllBGColorButton, wxT("すべて"));
     label_1 = new wxStaticText(panel_3, wxID_ANY, wxT("※スレビューのフォントの種類はスキン、大きさは表示メニューで設定"));

     set_properties();
     do_layout();
     // end wxGlade
}


void ColorFontSettingPanel::set_properties()
{
     // ウィジェットとウィジェット名のペアを作成
     static const int STATIC_TEXT_NUMBER = 11;
     
     /**
      * フォントを設定する
      */
#if wxCHECK_VERSION(2, 9, 1)
     std::pair <wxString, wxGenericStaticText*> *pFontArray = new std::pair<wxString, wxGenericStaticText*>[STATIC_TEXT_NUMBER];
#else
     std::pair <wxString, BasicDrawPane*> *pFontArray = new std::pair<wxString, BasicDrawPane*>[STATIC_TEXT_NUMBER];
#endif
     pFontArray[0]  = std::make_pair(wxT("ID_TreeFontButton"),        treeSampleLabel);
     pFontArray[1]  = std::make_pair(wxT("ID_ThreadListFontButton"),  threadListSampleLabel);  
     pFontArray[2]	= std::make_pair(wxT("ID_ExtractFontButton"),	  extractSampleLabel);	 
     pFontArray[3]	= std::make_pair(wxT("ID_LogWindowFontButton"),	  logWindowSampleLabel);	 
     pFontArray[4]	= std::make_pair(wxT("ID_ThreadTitleFontButton"), threadTitleSampleLabel);
     pFontArray[5]	= std::make_pair(wxT("ID_KakikoFontButton"),	  threadViewSampleLabel); 
     pFontArray[6]	= std::make_pair(wxT("ID_MemoFontButton"),	  kakikoSampleLabel);	 
     pFontArray[7]	= std::make_pair(wxT("ID_HintFontButton"),	  memoSampleLabel);	 
     pFontArray[8]	= std::make_pair(wxT("ID_LinkFontButton"),	  hintSampleLabel);	 
     pFontArray[9]	= std::make_pair(wxT("ID_OthersFontButton"),	  linkSampleLabel);	 
     pFontArray[10] = std::make_pair(wxT("ID_AllFontButton"),	  othersSampleLabel);	 

     for (int i = 0; i < STATIC_TEXT_NUMBER; i++ )
     {
	  wxString widgetsName = pFontArray[i].first;
	  wxString widgetsInfo = wxEmptyString;
	  XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);

	  if (widgetsInfo != wxEmptyString) 
	  {
	       wxFont font;
	       bool ret = font.SetNativeFontInfoUserDesc(widgetsInfo);
#if wxCHECK_VERSION(2, 9, 1)
	       if(ret) pFontArray[i].second->SetFont(font);
#else
	       if(ret) pFontArray[i].second->SetInnerFont(font);
#endif
	  }
     }

     delete[] pFontArray;

     /**
      * 背景色を設定する
      */
#if wxCHECK_VERSION(2, 9, 1)
     std::pair <wxString, wxGenericStaticText*> *pBGArray = new std::pair<wxString, wxGenericStaticText*>[STATIC_TEXT_NUMBER];
#else
     std::pair <wxString, BasicDrawPane*> *pBGArray = new std::pair<wxString, BasicDrawPane*>[STATIC_TEXT_NUMBER];
#endif

     pBGArray[0]  = std::make_pair(wxT("ID_BoardListBGColorButton"),   treeSampleLabel);
     pBGArray[1]  = std::make_pair(wxT("ID_ThreadListBGColorButton"),  threadListSampleLabel);  
     pBGArray[2]  = std::make_pair(wxT("ID_ThreadListBGColorButton"),  extractSampleLabel);	 
     pBGArray[3]  = std::make_pair(wxT("ID_LogWindowBGColorButton"),   logWindowSampleLabel);	 
     pBGArray[4]  = std::make_pair(wxT("ID_ThreadTitleBGColorButton"), threadTitleSampleLabel);
     pBGArray[5]  = std::make_pair(wxT("ID_ThreadViewBGColorButton"),  threadViewSampleLabel); 
     pBGArray[6]  = std::make_pair(wxT("ID_MemoBGColorButton"),	       kakikoSampleLabel);	 
     pBGArray[7]  = std::make_pair(wxT("ID_MemoBGColorButton"),	       memoSampleLabel);	 
     pBGArray[8]  = std::make_pair(wxT("ID_HintBGColorButton"),	       hintSampleLabel);	 
     pBGArray[9]  = std::make_pair(wxT("ID_PopupBGColorButton"),       linkSampleLabel);	 
     pBGArray[10] = std::make_pair(wxT("ID_AllFontButton"),	       othersSampleLabel);

     for (int i = 0; i < STATIC_TEXT_NUMBER; i++ )
     {
	  wxString widgetsName = pBGArray[i].first;
	  wxString widgetsInfo = wxEmptyString;
	  XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);

	  if (widgetsInfo != wxEmptyString) 
	  {
	       wxColour bgColor;
	       bool ret = bgColor.Set(widgetsInfo);
#if wxCHECK_VERSION(2, 9, 1)
	       if(ret) pBGArray[i].second->SetBackgroundColour(bgColor);
#else
	       if(ret) pBGArray[i].second->SetInnerBGColor(bgColor);
#endif
	  }
     }

     // 更新されたリソースの反映
     this->Refresh();
     this->Update();

     delete[] pBGArray;
}


void ColorFontSettingPanel::do_layout()
{
     // begin wxGlade: ColorFontSettingPanel::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_6 = new wxStaticBoxSizer(sizer_6_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_13 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_5 = new wxStaticBoxSizer(sizer_5_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_12 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_11 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_10 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_4 = new wxStaticBoxSizer(sizer_4_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_9 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
     sizer_4->Add(treeSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_7->Add(threadListSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_7->Add(extractSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     panel_7->SetSizer(sizer_7);
     sizer_4->Add(panel_7, 1, wxEXPAND, 0);
     sizer_4->Add(logWindowSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_4->Add(threadTitleSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_4->Add(threadViewSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_8->Add(kakikoSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_8->Add(memoSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     panel_8->SetSizer(sizer_8);
     sizer_4->Add(panel_8, 1, wxEXPAND, 0);
     sizer_9->Add(hintSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     sizer_9->Add(linkSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     panel_9->SetSizer(sizer_9);
     sizer_4->Add(panel_9, 1, wxEXPAND, 0);
     sizer_4->Add(othersSampleLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5);
     panel_4->SetSizer(sizer_4);
     sizer_3->Add(panel_4, 1, wxEXPAND, 0);
     sizer_5->Add(treeFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_10->Add(threadListFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_10->Add(extractFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_10->SetSizer(sizer_10);
     sizer_5->Add(panel_10, 1, wxEXPAND, 0);
     sizer_5->Add(logWindowFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(threadTitleFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_11->Add(kakikoFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_11->Add(memoFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_11->SetSizer(sizer_11);
     sizer_5->Add(panel_11, 1, wxEXPAND, 0);
     sizer_12->Add(hintFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_12->Add(linkFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_12->SetSizer(sizer_12);
     sizer_5->Add(panel_12, 1, wxEXPAND, 0);
     sizer_5->Add(otherFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(allFontButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_5->SetSizer(sizer_5);
     sizer_3->Add(panel_5, 1, wxEXPAND, 0);
     sizer_6->Add(treeBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_6->Add(threadListBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_6->Add(logWindowBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_6->Add(threadTitleBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_6->Add(threadViewBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_6->Add(memoBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_13->Add(hintBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_13->Add(popupBGColorButton_copy_1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_13->SetSizer(sizer_13);
     sizer_6->Add(panel_13, 1, wxEXPAND, 0);
     sizer_6->Add(allBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_6->SetSizer(sizer_6);
     sizer_3->Add(panel_6, 1, wxEXPAND, 0);
     panel_2->SetSizer(sizer_3);
     sizer_1->Add(panel_2, 1, wxEXPAND, 0);
     sizer_2->Add(label_1, 0, 0, 0);
     panel_3->SetSizer(sizer_2);
     sizer_1->Add(panel_3, 0, wxEXPAND, 0);
     SetSizer(sizer_1);
     sizer_1->Fit(this);
     // end wxGlade
}

/*
 * 色選択/フォント選択用ダイアログを表示させる
 */
void ColorFontSettingPanel::OnClickColorFontSettingButton(wxCommandEvent& event)
{
     // イベントで取得したIDはなにか
     // switch文で書きたいけどうまく行かないのでif文
     const int id = event.GetId();

     if (id == ID_TreeFontButton        ||	// ツリーフォント設定ボタン	     
	 id == ID_ThreadListFontButton	||	// スレ欄フォント設定ボタン	     
	 id == ID_ExtractFontButton	||	// 抽出フォント設定ボタン		 
	 id == ID_LogWindowFontButton	||	// ログ出力画面フォント設定ボタン    
	 id == ID_ThreadTitleFontButton ||	// スレッドタイトルフォント設定ボタン
	 id == ID_KakikoFontButton	||	// 書き込みフォント設定ボタン	     
	 id == ID_MemoFontButton        ||	// メモ欄フォント設定ボタン	     
	 id == ID_HintFontButton	||	// ヒントフォント設定ボタン	     
	 id == ID_LinkFontButton	||	// リンクフォント設定ボタン	     
	 id == ID_OthersFontButton      ||	// その他フォント設定ボタン	     
	 id == ID_AllFontButton			// すべてフォント設定ボタン
	  ) {
          /**		  
	   * フォント設定 
	   */
	  const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	  const wxString font = wxString((const char*)str.c_str(), wxConvUTF8);
	  bool needToChangeFont = this->SetEachFontSetting(font);

	  if (needToChangeFont)
	  {
	       SetSampleFontSetting(id);
	  }

     } else if (
	  id == ID_BoardListBGColorButton   ||	// スレ欄背景色設定ボタン	    
	  id == ID_ThreadListBGColorButton  ||	// 抽出背景色設定ボタン		    
	  id == ID_LogWindowBGColorButton   ||	// ログ出力画面背景色設定ボタン	    
	  id == ID_ThreadTitleBGColorButton ||	// スレッドタイトル背景色設定ボタン 
	  id == ID_ThreadViewBGColorButton  ||	// スレビュー背景色設定ボタン	    
	  id == ID_MemoBGColorButton        ||	// メモ欄背景色設定ボタン	    
	  id == ID_HintBGColorButton	    ||	// ヒント背景色設定ボタン	    
	  id == ID_PopupBGColorButton	    ||	// ポップアップ背景色設定ボタン	    
	  id == ID_AllBGColorButton		// すべての背景色設定ボタン	    
	  ) {
	  /**
	   * 背景色設定
	   */
	  const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	  const wxString bgColor = wxString((const char*)str.c_str(), wxConvUTF8);

#ifndef __WXMAC__
	  bool needToChangeBGColor = this->SetEachBGColorSetting(bgColor);

	  if (needToChangeBGColor)
	  {
	       SetSampleBGColorSetting(id);
	  }

     }
     
#else
     // メインスレッドに更新してもらう
     m_id = id;
     SendUIUpdateEvent();

     }

#endif
}
/**
 * 各部位のフォントを設定し、プロパティファイルに書き出す
 * @param  各部位の名称の文字列型
 * @return 変更があればtrueを返す
 */
bool ColorFontSettingPanel::SetEachFontSetting(const wxString& font)
{
     wxFontData data;
     wxFont canvasFont;
     data.SetInitialFont(canvasFont);
     wxFontDialog dialog(this, data);

     if ( wxID_OK == dialog.ShowModal() ) 
     {
     	  // フォント設定用データを用意する
     	  wxFontData retData = dialog.GetFontData();
     	  canvasFont = retData.GetChosenFont();

     	  // 結果を受け取る
     	  const wxString fontInfo  = canvasFont.GetNativeFontInfoUserDesc();
     	  // プロパティファイルに格納
     	  XrossBoardUtil::SetXrossBoardProperties(font, fontInfo);
     	  return true;
     }

     return false;
}
/**
 * 色・フォント設定用画面のサンプル部分のフォントを変更する
 */
void ColorFontSettingPanel::SetSampleFontSetting(const int id)
{

     int wannaChangeID = -1;

     if (id == ID_TreeFontButton) {
	  wannaChangeID = ID_TreeSampleLabel;	  
     } else if (id == ID_ThreadListFontButton) {
	  wannaChangeID = ID_ThreadListSampleLabel;	  
     } else if (id == ID_ExtractFontButton) {
	  wannaChangeID = ID_ExtractSampleLabel;	  
     } else if (id == ID_LogWindowFontButton) {
	  wannaChangeID = ID_LogWindowSampleLabel;	  
     } else if (id == ID_ThreadTitleFontButton) {
	  wannaChangeID = ID_ThreadTitleSampleLabel;	  
     } else if (id == ID_KakikoFontButton) {
	  wannaChangeID = ID_KakikoSampleLabel;	  
     } else if (id == ID_MemoFontButton) {
	  wannaChangeID = ID_MemoSampleLabel;	  
     } else if (id == ID_HintFontButton) {
	  wannaChangeID = ID_HintSampleLabel;	  
     } else if (id == ID_LinkFontButton) {
	  wannaChangeID = ID_LinkSampleLabel;
     } else if (id == ID_OthersFontButton) {
	  wannaChangeID = ID_OthersSampleLabel;	  
     } else if (id == ID_AllFontButton) {
	  wannaChangeID = ID_AllSampleLabel;
     }

     if (wannaChangeID < 0)
	  return;

     wxWindow* window = FindWindowById(static_cast<long>(wannaChangeID), this);

#if wxCHECK_VERSION(2, 9, 1)
     if ( wxGenericStaticText* bdp = dynamic_cast<wxGenericStaticText*>(window))
#else
     if ( BasicDrawPane* bdp = dynamic_cast<BasicDrawPane*>(window))
#endif
     {
	  wxString widgetsName = wxEmptyString;
	  wxString widgetsInfo = wxEmptyString;
	  const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	  widgetsName = wxString((const char*)str.c_str(), wxConvUTF8);
	  XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);

	  if (widgetsInfo != wxEmptyString)
	  {
	       wxFont font;
	       bool ret = font.SetNativeFontInfoUserDesc(widgetsInfo);

#if wxCHECK_VERSION(2, 9, 1)
	       if(ret) bdp->SetFont(font);
#else
	       if(ret) bdp->SetInnerFont(font);
#endif

	       this->Refresh();
	       this->Update();
	  }

     } else {
	  wxMessageBox(wxT("内部エラー, フォントの変更に失敗しました."), wxT("設定画面"), wxICON_ERROR);
	  return;
     }
}
/**
 * 各部位の背景色を設定し、プロパティファイルに書き出す
 * @param  各部位の名称を文字列で
 * @return 変更があればtrueを返す
 */
bool ColorFontSettingPanel::SetEachBGColorSetting(const wxString& bgColor)
{
     wxColourData data;
     data.SetChooseFull(true);
     for (int i = 0; i < 16; i++)
     {
	  wxColour colour(i*16, i*16, i*16);
	  data.SetCustomColour(i, colour);
     }

     wxColourDialog dialog(this, &data);
     if (dialog.ShowModal() == wxID_OK)
     {
	  wxColourData retData = dialog.GetColourData();
	  wxColour col = retData.GetColour();
	  // 結果を受け取る
	  const wxString colorInfo = col.GetAsString();
	  // フォント,色情報 の順でプロパティファイルに格納
	  XrossBoardUtil::SetXrossBoardProperties(bgColor, colorInfo);
	  return true;
     }

     return false;
}
/**
 * 色・フォント設定用画面のサンプル部分の背景色を変更する
 */
void ColorFontSettingPanel::SetSampleBGColorSetting(const int id)
{
     wxArrayInt iArray;

     if (id == ID_BoardListBGColorButton) {
	  iArray.Add(ID_TreeSampleLabel);	  
     } else if (id == ID_ThreadListBGColorButton) {
	  iArray.Add(ID_ThreadListSampleLabel);
	  iArray.Add(ID_ExtractSampleLabel);
     } else if (id == ID_LogWindowBGColorButton) {
	  iArray.Add(ID_LogWindowSampleLabel);	  
     } else if (id == ID_ThreadTitleBGColorButton) {
	  iArray.Add(ID_ThreadTitleSampleLabel);	  
     } else if (id == ID_MemoBGColorButton) {
	  iArray.Add(ID_KakikoSampleLabel);
	  iArray.Add(ID_MemoSampleLabel);	  
     } else if (id == ID_HintBGColorButton) {
	  iArray.Add(ID_HintSampleLabel);	  
     } else if (id == ID_PopupBGColorButton) {
	  iArray.Add(ID_LinkSampleLabel);	  
     } else if (id == ID_AllFontButton) {
	  iArray.Add(ID_TreeSampleLabel);
	  iArray.Add(ID_ThreadListSampleLabel);
	  iArray.Add(ID_ExtractSampleLabel);
	  iArray.Add(ID_LogWindowSampleLabel);
	  iArray.Add(ID_ThreadTitleSampleLabel);
	  iArray.Add(ID_ThreadViewSampleLabel);
	  iArray.Add(ID_KakikoSampleLabel);
	  iArray.Add(ID_MemoSampleLabel);
	  iArray.Add(ID_HintSampleLabel);
	  iArray.Add(ID_LinkSampleLabel);
	  iArray.Add(ID_OthersSampleLabel);
     }

     for (unsigned int i = 0; i < iArray.size(); i++ )
     {
	  wxWindow* window = FindWindowById(static_cast<long>(iArray[i]), this);

#if wxCHECK_VERSION(2, 9, 1)
	  if ( wxGenericStaticText* bdp = dynamic_cast<wxGenericStaticText*>(window))
#else
	  if ( BasicDrawPane* bdp = dynamic_cast<BasicDrawPane*>(window))
#endif
	  {
	       wxString widgetsName = wxEmptyString;
	       wxString widgetsInfo = wxEmptyString;
	       const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	       widgetsName = wxString((const char*)str.c_str(), wxConvUTF8);
	       XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);

	       if (widgetsInfo != wxEmptyString)
	       {
		    wxColour bgColor;
		    bool ret = bgColor.Set(widgetsInfo);
#if wxCHECK_VERSION(2, 9, 1)
		    if(ret) bdp->SetBackgroundColour(bgColor);
#else
		    if(ret) bdp->SetInnerBGColor(bgColor);
#endif
	       }

	  } else {
	       wxMessageBox(wxT("内部エラー, 背景色の変更に失敗しました."), wxT("設定画面"), wxICON_ERROR);
	       return;
	  }

	  // 更新されたリソースの反映
	  this->Refresh();
	  this->Update();
     }
}

/**
 * タブ色設定用画面
 */
TabColorSettingPanel::TabColorSettingPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, ID_TabColorSettingPanel, pos, size, wxTAB_TRAVERSAL)
{
     // begin wxGlade: TabColorSettingPanel::TabColorSettingPanel
     panel_2 = new wxPanel(this, wxID_ANY);
     panel_3 = new wxPanel(panel_2, wxID_ANY);
     panel_5 = new wxPanel(panel_2, wxID_ANY);
     panel_8 = new wxPanel(panel_5, wxID_ANY);
     panel_7 = new wxPanel(panel_5, wxID_ANY);
     panel_6 = new wxPanel(panel_5, wxID_ANY);
     sizer_6_staticbox = new wxStaticBox(panel_7, -1, wxT("非アクティブタブ"));
     sizer_5_staticbox = new wxStaticBox(panel_6, -1, wxT("アクティブタブ"));
     threadTabColorCheck = new wxCheckBox(this, ID_ThreadTabColorCheck, wxT("スレタブの色を変更する"));
     activeTabBGColorButton = new wxButton(panel_6, ID_ActiveTabBGColorButton, wxT("背景色"));

#if wxCHECK_VERSION(2, 9, 1)
     defaultActiveTabSampleLabel = new wxGenericStaticText(panel_6, ID_DefaultActiveTabSampleLabel, wxT("Default"));
     readingActiveTabSampleLabel = new wxGenericStaticText(panel_6, ID_ReadingActiveTabSampleLabel, wxT("読み込み中"));
     brokenActiveTabSampleLabel = new wxGenericStaticText(panel_6, ID_BrokenActiveTabSampleLabel, wxT("ログ破損時"));
     cannotPostActiveTabSampleLabel = new wxGenericStaticText(panel_6, ID_CannotPostActiveTabSampleLabel, wxT("書き込み不可"));
     updateActiveTabSampleLabel = new wxGenericStaticText(panel_6, ID_UpdateActiveTabSampleLabel, wxT("更新あり"));
     partialContentActiveTabSampleLabel = new wxGenericStaticText(panel_6, ID_PartialContentActiveTabSampleLabel, wxT("新着あり"));
     defaultDeactiveTabSampleLabel = new wxGenericStaticText(panel_7, ID_DefaultDeactiveTabSampleLabel, wxT("Default"));
     readingDeactiveTabSampleLabel = new wxGenericStaticText(panel_7, ID_ReadingDeactiveTabSampleLabel, wxT("読み込み中"));
     brokenDeactiveTabSampleLabel = new wxGenericStaticText(panel_7, ID_BrokenDeactiveTabSampleLabel, wxT("ログ破損時"));
     cannotPostDeactiveTabSampleLabel = new wxGenericStaticText(panel_7, ID_CannotPostDeactiveTabSampleLabel, wxT("書き込み不可"));
     updateDeactiveTabSampleLabel = new wxGenericStaticText(panel_7, ID_UpdateDeactiveTabSampleLabel, wxT("更新あり"));
     partialContentDeactiveTabSampleLabel = new wxGenericStaticText(panel_7, ID_PartialContentDeactiveTabSampleLabel, wxT("新着あり"));
#else
     defaultActiveTabSampleLabel = new BasicDrawPane(panel_6, ID_DefaultActiveTabSampleLabel, wxT("Default"));
     readingActiveTabSampleLabel = new BasicDrawPane(panel_6, ID_ReadingActiveTabSampleLabel, wxT("読み込み中"));
     brokenActiveTabSampleLabel = new BasicDrawPane(panel_6, ID_BrokenActiveTabSampleLabel, wxT("ログ破損時"));
     cannotPostActiveTabSampleLabel = new BasicDrawPane(panel_6, ID_CannotPostActiveTabSampleLabel, wxT("書き込み不可"));
     updateActiveTabSampleLabel = new BasicDrawPane(panel_6, ID_UpdateActiveTabSampleLabel, wxT("更新あり"));
     partialContentActiveTabSampleLabel = new BasicDrawPane(panel_6, ID_PartialContentActiveTabSampleLabel, wxT("新着あり"));
     defaultDeactiveTabSampleLabel = new BasicDrawPane(panel_7, ID_DefaultDeactiveTabSampleLabel, wxT("Default"));
     readingDeactiveTabSampleLabel = new BasicDrawPane(panel_7, ID_ReadingDeactiveTabSampleLabel, wxT("読み込み中"));
     brokenDeactiveTabSampleLabel = new BasicDrawPane(panel_7, ID_BrokenDeactiveTabSampleLabel, wxT("ログ破損時"));
     cannotPostDeactiveTabSampleLabel = new BasicDrawPane(panel_7, ID_CannotPostDeactiveTabSampleLabel, wxT("書き込み不可"));
     updateDeactiveTabSampleLabel = new BasicDrawPane(panel_7, ID_UpdateDeactiveTabSampleLabel, wxT("更新あり"));
     partialContentDeactiveTabSampleLabel = new BasicDrawPane(panel_7, ID_PartialContentDeactiveTabSampleLabel, wxT("新着あり"));
#endif
     deactiveTabBGColorButton = new wxButton(panel_7, ID_DeactiveTabBGColorButton, wxT("背景色"));
     panel_9 = new wxPanel(panel_8, wxID_ANY);
     threadTabDefaultFontColorButton = new wxButton(panel_8, ID_ThreadTabDefaultFontColorButton, wxT("デフォルト"));
     threadTabReadingFontColorButton = new wxButton(panel_8, ID_ThreadTabReadingFontColorButton, wxT("読込中"));
     threadTabBrokenFontColorButton = new wxButton(panel_8, ID_ThreadTabBrokenFontColorButton, wxT("ログ破損"));
     threadTabCannotPostFontColorButton = new wxButton(panel_8, ID_ThreadTabCannotPostFontColorButton, wxT("書込不可"));
     threadTabUpdateFontColorButton = new wxButton(panel_8, ID_ThreadTabUpdateFontColorButton, wxT("更新あり"));
     threadTabPartialContentFontColorButton = new wxButton(panel_8, ID_ThreadTabPartialContentFontColorButton, wxT("新着あり"));
     panel_4 = new wxPanel(panel_3, wxID_ANY);

#if wxCHECK_VERSION(2, 9, 1)
     autoReloadSampleLabel = new wxGenericStaticText(panel_3, ID_AutoReloadSampleLabel, wxT("オートリロード中"));
#else
     autoReloadSampleLabel = new BasicDrawPane(panel_3, ID_AutoReloadSampleLabel, wxT("オートリロード中"));
#endif
     autoReloadFontColorButton = new wxButton(panel_3, ID_AutoReloadFontColorButton, wxT("オートリロード中"));

     set_properties();
     do_layout();
     // end wxGlade
}


void TabColorSettingPanel::set_properties()
{
     // begin wxGlade: TabColorSettingPanel::set_properties
     threadTabColorCheck->SetValue(1);
     // end wxGlade

     // 背景色の設定
     this->SetSampleBGColorSetting(ID_ActiveTabBGColorButton);
     this->SetSampleBGColorSetting(ID_DeactiveTabBGColorButton);

     // フォント色の設定
     wxArrayInt iArray;
     iArray.Add(ID_ThreadTabDefaultFontColorButton);
     iArray.Add(ID_ThreadTabReadingFontColorButton);
     iArray.Add(ID_ThreadTabBrokenFontColorButton);
     iArray.Add(ID_ThreadTabCannotPostFontColorButton);
     iArray.Add(ID_ThreadTabUpdateFontColorButton);
     iArray.Add(ID_ThreadTabPartialContentFontColorButton);
     iArray.Add(ID_AutoReloadFontColorButton);

     for (unsigned int i = 0; i < iArray.size(); i++ )
     {
	  this->SetSampleFontColorSetting(iArray[i]);
     }
}

void TabColorSettingPanel::do_layout()
{
     // begin wxGlade: TabColorSettingPanel::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_7 = new wxBoxSizer(wxVERTICAL);
     wxStaticBoxSizer* sizer_6 = new wxStaticBoxSizer(sizer_6_staticbox, wxVERTICAL);
     wxStaticBoxSizer* sizer_5 = new wxStaticBoxSizer(sizer_5_staticbox, wxVERTICAL);
     sizer_1->Add(threadTabColorCheck, 0, 0, 5);
     sizer_5->Add(activeTabBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_5->Add(defaultActiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_5->Add(readingActiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_5->Add(brokenActiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_5->Add(cannotPostActiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_5->Add(updateActiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_5->Add(partialContentActiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     panel_6->SetSizer(sizer_5);
     sizer_4->Add(panel_6, 1, wxEXPAND, 0);
     sizer_6->Add(deactiveTabBGColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_6->Add(defaultDeactiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_6->Add(readingDeactiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_6->Add(brokenDeactiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_6->Add(cannotPostDeactiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_6->Add(updateDeactiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     sizer_6->Add(partialContentDeactiveTabSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5);
     panel_7->SetSizer(sizer_6);
     sizer_4->Add(panel_7, 1, wxEXPAND, 0);
     sizer_7->Add(panel_9, 1, wxEXPAND, 0);
     sizer_7->Add(threadTabDefaultFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(threadTabReadingFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(threadTabBrokenFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(threadTabCannotPostFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(threadTabUpdateFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     sizer_7->Add(threadTabPartialContentFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_8->SetSizer(sizer_7);
     sizer_4->Add(panel_8, 1, wxEXPAND, 0);
     panel_5->SetSizer(sizer_4);
     sizer_2->Add(panel_5, 1, wxEXPAND, 0);
     sizer_3->Add(panel_4, 1, wxEXPAND, 0);
     sizer_3->Add(autoReloadSampleLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
     sizer_3->Add(autoReloadFontColorButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
     panel_3->SetSizer(sizer_3);
     sizer_2->Add(panel_3, 0, wxEXPAND, 0);
     panel_2->SetSizer(sizer_2);
     sizer_1->Add(panel_2, 1, wxEXPAND, 0);
     SetSizer(sizer_1);
     sizer_1->Fit(this);
     // end wxGlade
}
/**
 * タブ色設定用画面のボタンイベントを処理する
 */
void TabColorSettingPanel::OnClickTabColorSettingButton(wxCommandEvent& event)
{

     const int id = event.GetId();
     
     if ( id == ID_ActiveTabBGColorButton  ||
	  id ==	ID_DeactiveTabBGColorButton )
     {
	  /**
	   * 背景色設定
	   */
	  const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	  const wxString bgColor = wxString((const char*)str.c_str(), wxConvUTF8);

	  bool needToChangeBGColor = this->SetEachBGColorSetting(bgColor);

	  if (needToChangeBGColor)
	  {
	       SetSampleBGColorSetting(id);
	  }
     } else if ( id == ID_ThreadTabDefaultFontColorButton	 || 
		 id == ID_ThreadTabReadingFontColorButton	 || 
		 id == ID_ThreadTabBrokenFontColorButton	 || 
		 id == ID_ThreadTabCannotPostFontColorButton	 || 
		 id == ID_ThreadTabUpdateFontColorButton	 || 
		 id == ID_ThreadTabPartialContentFontColorButton || 
		 id == ID_AutoReloadFontColorButton )
     {
          /**		  
	   * 文字色設定
	   */
	  const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	  const wxString fontColor = wxString((const char*)str.c_str(), wxConvUTF8);
	  bool needToChangeFontColor = this->SetEachFontColorSetting(fontColor);

	  if (needToChangeFontColor)
	  {
	       SetSampleFontColorSetting(id);
	  }
     }
}
/**
 * 各部位の背景色を設定し、プロパティファイルに書き出す
 * @param  各部位の名称を文字列で
 * @return 変更があればtrueを返す
 */
bool TabColorSettingPanel::SetEachBGColorSetting(const wxString& bgColor)
{
     wxColourData data;
     data.SetChooseFull(true);
     for (int i = 0; i < 16; i++)
     {
	  wxColour colour(i*16, i*16, i*16);
	  data.SetCustomColour(i, colour);
     }

     wxColourDialog dialog(this, &data);
     if (dialog.ShowModal() == wxID_OK)
     {
	  wxColourData retData = dialog.GetColourData();
	  wxColour col = retData.GetColour();
	  // 結果を受け取る
	  const wxString colorInfo = col.GetAsString();
	  // フォント,色情報 の順でプロパティファイルに格納
	  XrossBoardUtil::SetXrossBoardProperties(bgColor, colorInfo);
	  return true;
     }

     return false;
}
/**
 * 色・フォント設定用画面のサンプル部分の背景色を変更する
 */
void TabColorSettingPanel::SetSampleBGColorSetting(const int id)
{
     wxArrayInt iArray;

     if (id == ID_ActiveTabBGColorButton) {
	  iArray.Add(ID_DefaultActiveTabSampleLabel         ); 	  
	  iArray.Add(ID_ReadingActiveTabSampleLabel	    ); 
	  iArray.Add(ID_BrokenActiveTabSampleLabel	    ); 
	  iArray.Add(ID_CannotPostActiveTabSampleLabel	    ); 
	  iArray.Add(ID_UpdateActiveTabSampleLabel	    ); 
	  iArray.Add(ID_PartialContentActiveTabSampleLabel  ); 
	  iArray.Add(ID_AutoReloadSampleLabel		    ); 
     } else if (id == ID_DeactiveTabBGColorButton) {
	  iArray.Add(ID_DefaultDeactiveTabSampleLabel	    ); 
	  iArray.Add(ID_ReadingDeactiveTabSampleLabel	    ); 
	  iArray.Add(ID_BrokenDeactiveTabSampleLabel	    ); 
	  iArray.Add(ID_CannotPostDeactiveTabSampleLabel    ); 
	  iArray.Add(ID_UpdateDeactiveTabSampleLabel	    ); 
	  iArray.Add(ID_PartialContentDeactiveTabSampleLabel); 
     }

     for (unsigned int i = 0; i < iArray.size(); i++ )
     {
	  wxWindow* window = FindWindowById(static_cast<long>(iArray[i]), this);

#if wxCHECK_VERSION(2, 9, 1)
	  if ( wxGenericStaticText* bdp = dynamic_cast<wxGenericStaticText*>(window))
#else
	  if ( BasicDrawPane* bdp = dynamic_cast<BasicDrawPane*>(window))
#endif
	  {
	       wxString widgetsName = wxEmptyString;
	       wxString widgetsInfo = wxEmptyString;
	       const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	       widgetsName = wxString((const char*)str.c_str(), wxConvUTF8);
	       XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);

	       if (widgetsInfo != wxEmptyString)
	       {
		    // プロパティファイルに設定がある場合
		    wxColour bgColor;
		    bool ret = bgColor.Set(widgetsInfo);
#if wxCHECK_VERSION(2, 9, 1)
		    if(ret) bdp->SetBackgroundColour(bgColor);
#else
		    if(ret) bdp->SetInnerBGColor(bgColor);
#endif
	       } else {
		    // プロパティファイルに設定がない場合
		    if ( id == ID_ActiveTabBGColorButton )
		    {
#if wxCHECK_VERSION(2, 9, 1)
			 bdp->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
#else
			 bdp->SetInnerBGColor(wxColour(wxT("rgb(255,255,255)")));
#endif
			 widgetsInfo = wxT("rgb(255,255,255)");
			 XrossBoardUtil::SetXrossBoardProperties(widgetsName, widgetsInfo);
		    } else if (id == ID_DeactiveTabBGColorButton) {
#if wxCHECK_VERSION(2, 9, 1)
			 bdp->SetBackgroundColour(wxColour(wxT("rgb(212,208,200)")));
#else
			 bdp->SetInnerBGColor(wxColour(wxT("rgb(212,208,200)")));
#endif
			 widgetsInfo = wxT("rgb(212,208,200)");
			 XrossBoardUtil::SetXrossBoardProperties(widgetsName, widgetsInfo);
		    }
	       }
	       

	  } else {
	       wxMessageBox(wxT("内部エラー, 背景色の変更に失敗しました."), wxT("設定画面"), wxICON_ERROR);
	       return;
	  }

	  // 更新されたリソースの反映
	  this->Refresh();
	  this->Update();
     }
}
/**
 * 各部位のフォントを設定し、プロパティファイルに書き出す
 * @param  各部位の名称の文字列型
 * @return 変更があればtrueを返す
 */
bool TabColorSettingPanel::SetEachFontColorSetting(const wxString& fontColor)
{
     return SetEachBGColorSetting(fontColor);
}
/**
 * タブ色設定用画面のサンプル部分のフォントを変更する
 */
void TabColorSettingPanel::SetSampleFontColorSetting(const int id)
{

     wxArrayInt iArray;
     wxString defaultRGB = wxEmptyString;

     if ( id == ID_ThreadTabDefaultFontColorButton ) 
     {
	  defaultRGB = wxT("rgb(0,0,0)");
	  iArray.Add(ID_DefaultActiveTabSampleLabel);
	  iArray.Add(ID_DefaultDeactiveTabSampleLabel);
     }
     else if ( id == ID_ThreadTabReadingFontColorButton	)
     {
	  defaultRGB = wxT("rgb(255,188,0)");
	  iArray.Add(ID_ReadingActiveTabSampleLabel);
	  iArray.Add(ID_ReadingDeactiveTabSampleLabel);
     }
     else if ( id == ID_ThreadTabBrokenFontColorButton )
     {
	  defaultRGB = wxT("rgb(255,0,0)");
	  iArray.Add(ID_BrokenActiveTabSampleLabel);
	  iArray.Add(ID_BrokenDeactiveTabSampleLabel);
     }
     else if ( id == ID_ThreadTabCannotPostFontColorButton )
     {
	  defaultRGB = wxT("rgb(128,128,128)");
	  iArray.Add(ID_CannotPostActiveTabSampleLabel);
	  iArray.Add(ID_CannotPostDeactiveTabSampleLabel);
     }
     else if ( id == ID_ThreadTabUpdateFontColorButton )
     {
	  defaultRGB = wxT("rgb(30,144,255)");
	  iArray.Add(ID_UpdateActiveTabSampleLabel);
	  iArray.Add(ID_UpdateDeactiveTabSampleLabel);
     }
     else if ( id == ID_ThreadTabPartialContentFontColorButton )
     {
	  defaultRGB = wxT("rgb(0,0,255)");
	  iArray.Add(ID_PartialContentActiveTabSampleLabel);
	  iArray.Add(ID_PartialContentDeactiveTabSampleLabel);
     }
     else if ( id == ID_AutoReloadFontColorButton )
     {
	  defaultRGB = wxT("rgb(224,255,255)");
	  iArray.Add(ID_AutoReloadSampleLabel);
     }


     for (unsigned int i = 0; i < iArray.size(); i++ )
     {	  
	  wxWindow* window = FindWindowById(static_cast<long>(iArray[i]), this);
#if wxCHECK_VERSION(2, 9, 1)
	  if ( wxGenericStaticText* bdp = dynamic_cast<wxGenericStaticText*>(window))
#else
	  if ( BasicDrawPane* bdp = dynamic_cast<BasicDrawPane*>(window))
#endif
	  {
	       wxString widgetsName = wxEmptyString;
	       wxString widgetsInfo = wxEmptyString;
	       const std::string &str = EnumString<XROSSBOARD_ENUMS>::From( static_cast<XROSSBOARD_ENUMS>(id) );
	       widgetsName = wxString((const char*)str.c_str(), wxConvUTF8);
	       XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);

	       if (widgetsInfo != wxEmptyString)
	       {
		    // プロパティファイルに設定がある場合
		    wxColour bgColor;
		    bool ret = bgColor.Set(widgetsInfo);
#if wxCHECK_VERSION(2, 9, 1)
		    if(ret) bdp->SetForegroundColour(bgColor);
#else
		    if(ret) bdp->SetInnerTextForeGroundColor(bgColor);
#endif
	       } else {
		    // プロパティファイルに設定がない場合
#if wxCHECK_VERSION(2, 9, 1)
		    bdp->SetForegroundColour(wxColour(defaultRGB));
#else
		    bdp->SetInnerTextForeGroundColor(wxColour(defaultRGB));
#endif
		    XrossBoardUtil::SetXrossBoardProperties(widgetsName, defaultRGB);
	       }

	  } else {
	       wxMessageBox(wxT("内部エラー, 背景色の変更に失敗しました."), wxT("設定画面"), wxICON_ERROR);
	       return;
	  }

	  // 更新されたリソースの反映
	  this->Refresh();
	  this->Update();
     }
}


TabControlSettingPanel::TabControlSettingPanel(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, ID_TabControlSettingPanel, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: TabControlSettingPanel::TabControlSettingPanel
    panel_5 = new wxPanel(this, wxID_ANY);
    panel_7 = new wxPanel(panel_5, wxID_ANY);
    panel_4 = new wxPanel(this, wxID_ANY);
    sizer_3_staticbox = new wxStaticBox(panel_4, wxID_ANY, wxT("タブを追加する位置"));
    sizer_4_staticbox = new wxStaticBox(panel_5, wxID_ANY, wxT("タブを閉じたとき"));
    panel_6 = new wxPanel(panel_4, wxID_ANY);
    label_1 = new wxStaticText(panel_6, wxID_ANY, wxT("通常"));
    const wxString choice_1_choices[] = {
        wxT("先頭"),
        wxT("アクティブの左"),
        wxT("アクティブの右"),
        wxT("最後")
    };
    choice_1 = new wxChoice(panel_6, ID_TabControlSettingPanelOpenTabChoice, wxDefaultPosition, wxDefaultSize, 4, choice_1_choices, 0);
    label_2 = new wxStaticText(panel_6, wxID_ANY, wxT("タブから開く時"));
    const wxString choice_2_choices[] = {
        wxT("先頭"),
        wxT("アクティブの左"),
        wxT("アクティブの右"),
        wxT("最後")
    };
    choice_2 = new wxChoice(panel_6, ID_TabControlSettingPanelOpenTabFromOtherTabChoice, wxDefaultPosition, wxDefaultSize, 4, choice_2_choices, 0);
    label_3 = new wxStaticText(panel_7, wxID_ANY, wxT("スレビュー"));
    const wxString choice_3_choices[] = {
        wxT("左をアクティブ"),
        wxT("右をアクティブ"),
        wxT("アクティブにした順")
    };
    choice_3 = new wxChoice(panel_7, ID_TabControlSettingPanelCloseThreadViewTabChoice, wxDefaultPosition, wxDefaultSize, 3, choice_3_choices, 0);
    label_4 = new wxStaticText(panel_7, wxID_ANY, wxT("スレ覧"));
    const wxString choice_4_choices[] = {
        wxT("左をアクティブ"),
        wxT("右をアクティブ")
    };
    choice_4 = new wxChoice(panel_7, ID_TabControlSettingPanelCloseThreadTabChoice, wxDefaultPosition, wxDefaultSize, 2, choice_4_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade
}


void TabControlSettingPanel::set_properties()
{
    // begin wxGlade: TabControlSettingPanel::set_properties
    choice_1->SetSelection(0);
    choice_2->SetSelection(0);
    choice_3->SetSelection(0);
    choice_4->SetSelection(0);
    // end wxGlade
}


void TabControlSettingPanel::do_layout()
{
    // begin wxGlade: TabControlSettingPanel::do_layout
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    sizer_4_staticbox->Lower();
    wxStaticBoxSizer* sizer_4 = new wxStaticBoxSizer(sizer_4_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_4 = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_3_staticbox->Lower();
    wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_3 = new wxFlexGridSizer(2, 2, 0, 0);
    grid_sizer_3->Add(label_1, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_3->Add(choice_1, 0, 0, 0);
    grid_sizer_3->Add(label_2, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_3->Add(choice_2, 0, 0, 0);
    panel_6->SetSizer(grid_sizer_3);
    sizer_3->Add(panel_6, 1, wxEXPAND, 0);
    panel_4->SetSizer(sizer_3);
    sizer_2->Add(panel_4, 0, 0, 0);
    grid_sizer_4->Add(label_3, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_4->Add(choice_3, 0, 0, 0);
    grid_sizer_4->Add(label_4, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_4->Add(choice_4, 0, 0, 0);
    panel_7->SetSizer(grid_sizer_4);
    sizer_4->Add(panel_7, 1, wxEXPAND, 0);
    panel_5->SetSizer(sizer_4);
    sizer_2->Add(panel_5, 0, 0, 0);
    SetSizer(sizer_2);
    sizer_2->Fit(this);
    // end wxGlade
}

/**
 * 外部板登録ダイアログ
 * もしDBに更新あれば返り値は1, 無ければ0
 */
NewBoardAddDialog::NewBoardAddDialog(wxWindow* parent, int id, const wxString& title, 
				     const wxPoint& pos, const wxSize& size, long style):
     wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP)
{
     // begin wxGlade: NewBoardAddDialog::NewBoardAddDialog
     // イベントテーブル書くのがダルいのでConnectする
     panel_1 = new wxPanel(this, wxID_ANY);
     panel_3 = new wxPanel(this, wxID_ANY);
     panel_6 = new wxPanel(panel_3, wxID_ANY);
     panel_5 = new wxPanel(panel_3, wxID_ANY);
     sizer_3_staticbox = new wxStaticBox(panel_3, -1, wxEmptyString);
     panel_4 = new wxPanel(panel_3, wxID_ANY);

     // 板URLは入力されたあとフォーカスがなくなったらイベントを起こす
     label_1 = new wxStaticText(panel_4, wxID_ANY, wxT("板URL　"));
     text_ctrl_1 = new wxTextCtrl(panel_4, wxID_ANY, wxEmptyString);
     text_ctrl_1->Connect(wxID_ANY,					      
			  wxEVT_KILL_FOCUS,
			  wxCommandEventHandler(NewBoardAddDialog::CheckBoardUrl),
			  NULL, this);					      

     label_2 = new wxStaticText(panel_5, wxID_ANY, wxT("板名　　"));
     text_ctrl_2 = new wxTextCtrl(panel_5, wxID_ANY, wxEmptyString);
     button_3 = new wxButton(panel_5, wxID_ANY, wxT("板名取得"));
     button_3->Connect(wxID_ANY,
		       wxEVT_COMMAND_BUTTON_CLICKED,
		       wxCommandEventHandler(NewBoardAddDialog::GetShitarabaBoardInfo),
		       NULL, this);					       

     label_3 = new wxStaticText(panel_6, wxID_ANY, wxT("カテゴリ"));

     const wxArrayString categoryList = SQLiteAccessor::GetCategoryList();
     combo_box_1 = new wxComboBox(panel_6, wxID_ANY, wxT("外部板"), 
				  wxDefaultPosition, 
				  wxDefaultSize,
				  categoryList,
				  wxCB_DROPDOWN);

     panel_7 = new wxPanel(panel_6, wxID_ANY);
     spacePanel = new wxPanel(panel_1, wxID_ANY);

     button_1 = new wxButton(panel_1, wxID_ANY, wxT("OK"));
     button_1->Connect(wxID_ANY,
		       wxEVT_COMMAND_BUTTON_CLICKED,
		       wxCommandEventHandler(NewBoardAddDialog::OnClickOK),
		       NULL, this);

     button_2 = new wxButton(panel_1, wxID_ANY, wxT("キャンセル"));
     button_2->Connect(wxID_ANY,					
		       wxEVT_COMMAND_BUTTON_CLICKED,				
		       wxCommandEventHandler(NewBoardAddDialog::OnClickCancel),
		       NULL, this);

     set_properties();
     do_layout();
     // end wxGlade
}

void NewBoardAddDialog::set_properties()
{
     // begin wxGlade: NewBoardAddDialog::set_properties
     SetTitle(wxT("新規板登録"));
     SetSize(wxSize(640, 200));
     button_3->Enable(false);
     // end wxGlade
}


void NewBoardAddDialog::do_layout()
{
     // begin wxGlade: NewBoardAddDialog::do_layout
     wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
     wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
     sizer_4->Add(label_1, 0, wxALIGN_CENTER_VERTICAL, 1);
     sizer_4->Add(text_ctrl_1, 1, wxALIGN_CENTER_VERTICAL, 1);
     panel_4->SetSizer(sizer_4);
     sizer_3->Add(panel_4, 1, wxEXPAND, 0);
     sizer_5->Add(label_2, 0, wxALIGN_CENTER_VERTICAL, 0);
     sizer_5->Add(text_ctrl_2, 1, wxALIGN_CENTER_VERTICAL, 1);
     sizer_5->Add(button_3, 0, wxALIGN_CENTER_VERTICAL, 0);
     panel_5->SetSizer(sizer_5);
     sizer_3->Add(panel_5, 1, wxEXPAND, 0);
     sizer_6->Add(label_3, 0, wxALIGN_CENTER_VERTICAL, 0);
     sizer_6->Add(combo_box_1, 4, wxALIGN_CENTER_VERTICAL, 0);
     sizer_6->Add(panel_7, 1, wxEXPAND, 0);
     panel_6->SetSizer(sizer_6);
     sizer_3->Add(panel_6, 1, wxEXPAND, 0);
     panel_3->SetSizer(sizer_3);
     sizer_1->Add(panel_3, 1, wxALL|wxEXPAND, 0);
     sizer_2->Add(spacePanel, 5, wxEXPAND, 0);
     sizer_2->Add(button_1, 2, wxEXPAND, 0);
     sizer_2->Add(button_2, 2, wxEXPAND, 0);
     panel_1->SetSizer(sizer_2);
     sizer_1->Add(panel_1, 0, 0, 0);
     SetSizer(sizer_1);
     Layout();
     // end wxGlade
}

void NewBoardAddDialog::OnClickOK(wxCommandEvent& event)
{
     wxString url       = text_ctrl_1->GetValue();
     wxString boardName = text_ctrl_2->GetValue();
     wxString category  = combo_box_1->GetValue();

     if ( url       == wxEmptyString ||
	  boardName == wxEmptyString ||
	  category  == wxEmptyString ) 
     {
	  // 必須項目が抜けている
	  wxMessageBox(wxT("エラー, 新規追加板のURL, 板名, カテゴリを入力してください."), 
		       wxT("新規追加板の登録"), wxICON_ERROR);
	  Close(true);
     }
     else
     {
	  // 入力項目をデータベースに格納する
	  SQLiteAccessor::SetOutSideBoardInfo(url, boardName, category);
     }

     Close(true);
}

void NewBoardAddDialog::OnClickCancel(wxCommandEvent& event)
{
     Close(true);
}
/**
 * 対象がしたらば掲示板か
 */
void NewBoardAddDialog::CheckBoardUrl(wxCommandEvent& event)
{

     wxString url = text_ctrl_1->GetValue();
     if ( url == wxEmptyString )
     {
	  button_3->Enable(false);
	  return;
     }

     PartOfURI partOfUri;
     if ( XrossBoardUtil::SubstringURI(url, &partOfUri))
     {
	  if ( partOfUri.hostname.Contains(wxT("jbbs.shitaraba.net")) ||
	       partOfUri.hostname.Contains(wxT("jbbs.livedoor.jp")) )
	  {
	       // 入力されたURLはしたらば掲示板なので板名取得ボタンを有効化
	       button_3->Enable(true);
	       return;
	  }
     }

     button_3->Enable(false);
     return;
}
/**
 * したらば掲示板の掲示板情報取得APIを叩く
 */
void NewBoardAddDialog::GetShitarabaBoardInfo(wxCommandEvent& event)
{
     wxString url = text_ctrl_1->GetValue();
     if ( url == wxEmptyString )
     {
	  button_3->Enable(false);
	  wxMessageBox(wxT("エラー, したらば掲示板の板情報取得に失敗しました."), 
		       wxT("新規追加板の登録"), wxICON_ERROR);	  
	  return;
     }

     PartOfURI partOfUri;
     if ( XrossBoardUtil::SubstringURI(url, &partOfUri))
     {
	  if ( partOfUri.hostname.Contains(wxT("jbbs.shitaraba.net")) ||
	       partOfUri.hostname.Contains(wxT("jbbs.livedoor.jp")) )
	  {
	       // 入力されたURLはしたらば掲示板なので板名取得APIを叩く
	       wxString boardName, category;
	       std::unique_ptr<SocketCommunication> sock(new SocketCommunication());

	       if (sock->GetShitarabaBoardInfo(partOfUri.path, boardName, category)) {
		    // カテゴリ設定
		    text_ctrl_2->SetValue(boardName);
		    combo_box_1->SetValue(category);
		    return;
	       }
	  }
     }

     wxMessageBox(wxT("エラー, したらば掲示板の板情報取得に失敗しました."), 
		  wxT("新規追加板の登録"), wxICON_ERROR);	  

     button_3->Enable(false);
     return;     
}
