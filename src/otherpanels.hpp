/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
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

// -*- C++ -*- generated by wxGlade 0.6.5 on Wed May 22 03:50:59 2013

#ifndef OTHERPANELS_HPP
#define OTHERPANELS_HPP

#include <utility>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/colordlg.h>
#include <wx/fontdlg.h>
#include <wx/html/htmlwin.h>
#include "enums.hpp"
#include "janecloneutil.hpp"
#include "drawpane.hpp"

// begin wxGlade: ::extracode
// end wxGlade

/**
 * 各種ネットワーク設定用画面
 */
class NetworkSettingPanel: public wxPanel {
public:
     // begin wxGlade: NetworkSettingPanel::ids
     // end wxGlade
     NetworkSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

#ifdef __WXMAC__
     // リソースの更新を行う
     void UpdateResources() {
	  wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
	  proxyUseCheck = new wxCheckBox(panel_6, wxID_ANY, wxT("Proxyを使用する"));
	  proxyCacheUseCheck = new wxCheckBox(panel_6, wxID_ANY, wxT("Proxy使用時にキャッシュを使用しない"));
	  sizer_6->Add(proxyUseCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	  sizer_6->Add(proxyCacheUseCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	  panel_6->SetSizer(sizer_6);
     };
#endif

private:
     // begin wxGlade: NetworkSettingPanel::methods
     void set_properties();
     void do_layout();
     // end wxGlade

#ifdef __WXMAC__
     // メインのスレッドにイベントを送る
     void SendUIUpdateEvent() {
	  wxCommandEvent* event = new wxCommandEvent(wxEVT_UPDATE_UI, ID_NetworkPanelUpdate);
	  wxString ui = wxT("NetworkSettingPanel");
	  event->SetString(ui.c_str());
	  event->SetEventObject(this);

   #if wxCHECK_VERSION(2, 9, 0)
	  wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event->Clone());
   #else
	  this->GetEventHandler()->AddPendingEvent(*event);
   #endif
     };
#endif

protected:
     // begin wxGlade: NetworkSettingPanel::attributes
     wxCheckBox* proxyUseCheck;
     wxCheckBox* proxyCacheUseCheck;
     wxPanel* panel_6;
     wxStaticText* label_6;
     wxStaticText* label_7;
     wxStaticText* label_8;
     wxTextCtrl* recProxyAddrTC;
     wxTextCtrl* recProxyPortTC;
     wxStaticText* label_9;
     wxTextCtrl* sedProxyAddrTC;
     wxTextCtrl* sedProxyPortTC;
     wxStaticText* label_10;
     wxTextCtrl* authSSLAddrTC;
     wxTextCtrl* authSSLPortTC;
     wxStaticText* label_11;
     wxStaticText* label_12;
     wxTextCtrl* basicAuthUserNameTC;
     wxPanel* panel_8;
     wxStaticText* label_13;
     wxTextCtrl* basicAuthPasswordTC;
     wxPanel* panel_9;
     wxPanel* panel_7;
     wxPanel* panel_1;
     wxStaticText* label_4;
     wxTextCtrl* receiveTimeoutTC;
     wxPanel* panel_2;
     wxStaticText* label_5;
     wxTextCtrl* connectTimeoutTC;
     wxPanel* panel_3;
     wxStaticText* label_3;
     wxTextCtrl* boardListURLTC;
     wxPanel* panel_4;
     wxStaticText* label_1;
     wxTextCtrl* receiveBufferSizeTC;
     wxStaticText* label_2;
     wxSpinCtrl* maxConnSC;
     wxPanel* panel_5;
     // end wxGlade
}; // wxGlade: end class

/**
 * 各種パス設定用画面
 */
class PathSettingPanel: public wxPanel {
public:
     // begin wxGlade: PathSettingPanel::ids
     // end wxGlade
     PathSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: PathSettingPanel::methods
     void set_properties();
     void do_layout();
     // end wxGlade
     void OnBrowserCheck(wxCommandEvent& event) {
	  // デフォルトのブラウザを指定する
	  JaneCloneUtil::SetJaneCloneProperties<bool>(wxT("SPECIFY_DEFAULT_BROWSER"), browserCheck->IsChecked());
	  this->set_properties();
     };
     void OnChangeDirPickerBrowser(wxFileDirPickerEvent& event) {
	  // ブラウザのパスを更新する
	  JaneCloneUtil::SetJaneCloneProperties<wxString>(wxT("DEFAULT_BROWSER_PATH"), event.GetPath());
	  this->set_properties();
     };
     void OnChangeDirPickerBoardList(wxFileDirPickerEvent& event) {
	  // 板一覧のパスを更新する
	  JaneCloneUtil::SetJaneCloneProperties<wxString>(wxT("DEFAULT_BOARDLIST_PATH"), event.GetPath());
	  this->set_properties();
     };
     void OnChangeDirPickerSkin(wxFileDirPickerEvent& event) {
	  // スキンのパスを更新する
	  JaneCloneUtil::SetJaneCloneProperties<wxString>(wxT("DEFAULT_SKINFILE_PATH"), event.GetPath());
	  this->set_properties();
     };
     void ClearPathSetting(wxCommandEvent& event) {
	  // パス設定をクリアする
	  const int id = event.GetId();

	  if (id == ID_ClearBrowserPath) {     
	       JaneCloneUtil::DeleteJaneClonePropertyEntry(wxT("DEFAULT_BROWSER_PATH"));
	  } else if (id == ID_ClearBoardListPath) {
	       JaneCloneUtil::DeleteJaneClonePropertyEntry(wxT("DEFAULT_BOARDLIST_PATH"));
	  } else if (id == ID_ClearSkinPath) {
	       JaneCloneUtil::DeleteJaneClonePropertyEntry(wxT("DEFAULT_SKINFILE_PATH"));
	  }
	  this->set_properties();
     };

protected:
     // begin wxGlade: PathSettingPanel::attributes
     wxCheckBox* browserCheck;
     wxStaticText* label_1;
     wxDirPickerCtrl* dirPicker1;
     wxButton* clearButton_1;
     wxStaticText* label_2;
     wxDirPickerCtrl* dirPicker2;
     wxButton* clearButton_2;
     wxStaticText* label_3;
     wxDirPickerCtrl* dirPicker3;
     wxButton* clearButton_3;

#ifdef __WXGTK__
     wxTextCtrl* browserPathTC;
     wxTextCtrl* boardListTC;
     wxTextCtrl* skinPathTC;
#endif

     // end wxGlade
     DECLARE_EVENT_TABLE() 
}; // wxGlade: end class

// begin wxGlade: ::extracode
// end wxGlade

/**
 * 各種動作設定用画面
 */
class BehaviorPanel: public wxPanel {
public:
    // begin wxGlade: BehaviorPanel::ids
    // end wxGlade

    BehaviorPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: BehaviorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: BehaviorPanel::attributes
    wxStaticBox* sizer_7_staticbox;
    wxStaticBox* sizer_5_staticbox;
    wxStaticBox* sizer_3_staticbox;
    wxStaticBox* sizer_2_staticbox;
    wxCheckBox* openBNewTabCheck;
    wxCheckBox* onlyOneCategoryCheck;
    wxPanel* panel_2;
    wxCheckBox* openTNewTabCheck;
    wxComboBox* threadNewTabCombo;
    wxPanel* panel_6;
    wxCheckBox* lastReadThreadPosCheck;
    wxPanel* panel_3;
    wxRadioBox* threadJumpSettingRadio;
    wxCheckBox* redrawNewThreadCheck;
    wxStaticText* label_1;
    wxComboBox* showResponseCombo;
    wxPanel* panel_7;
    wxPanel* panel_4;
    wxCheckBox* favoriteNewTabCheck;
    wxCheckBox* favoriteOnlyOneFolderCheck;
    wxPanel* panel_5;
    // end wxGlade
}; // wxGlade: end class

// begin wxGlade: ::extracode
// end wxGlade

/**
 * ユーザー設定設定用画面
 */
class UserSettingPanel: public wxPanel {
public:
     // begin wxGlade: UserSettingPanel::ids
     // end wxGlade
     UserSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: UserSettingPanel::methods
     void set_properties();
     void do_layout();
     // end wxGlade

protected:
     // begin wxGlade: UserSettingPanel::attributes
     wxStaticBox* sizer_3_staticbox;
     wxStaticBox* sizer_2_staticbox;
     wxHtmlWindow* window_1;
     wxStaticText* label_1;
     wxTextCtrl* maruUserID;
     wxPanel* panel_8;
     wxStaticText* label_2;
     wxTextCtrl* maruUserPassword;
     wxPanel* panel_9;
     wxCheckBox* maruAutoLoginCheck;
     wxButton* maruLogoutButton;
     wxPanel* panel_5;
     wxPanel* panel_2;
     wxHtmlWindow* window_2;
     wxStaticText* label_3;
     wxTextCtrl* beMailAddress;
     wxPanel* panel_6;
     wxStaticText* label_4;
     wxTextCtrl* bePassword;
     wxPanel* panel_7;
     wxPanel* panel_3;
     // end wxGlade
}; // wxGlade: end class

/**
 * 色・フォント設定用画面
 */
class ColorFontSettingPanel: public wxPanel {

public:
     // begin wxGlade: ColorFontSettingPanel::ids
     // end wxGlade
     ColorFontSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

#ifdef __WXMAC__
     // リソースの更新を行う
     void UpdateResources() {

	  // 内部で保存されたIDを元に文字列を生成する
	  const std::string &str = EnumString<JANECLONE_ENUMS>::From( static_cast<JANECLONE_ENUMS>(m_id) );
	  const wxString bgColor = wxString((const char*)str.c_str(), wxConvUTF8);

	  // 背景色を変更する必要があるかどうかのフラグ
	  bool needToChangeBGColor = false;

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
	       JaneCloneUtil::SetJaneCloneProperties(bgColor, colorInfo);
	       needToChangeBGColor = true;
	  }

	  if (needToChangeBGColor)
	  {
	       SetSampleBGColorSetting(m_id);
	  }

	  return;
     };
#endif


private:
     // begin wxGlade: ColorFontSettingPanel::methods
     void set_properties();
     void do_layout();
     // end wxGlade

     // 色選択用ダイアログを表示させる
     void OnClickColorFontSettingButton(wxCommandEvent& event);
     // 各部位のフォントを設定し、プロパティファイルに書き出す
     bool SetEachFontSetting(const wxString& font);
     // 色・フォント設定用画面のサンプル部分のフォントを変更する
     void SetSampleFontSetting(const int id);
     // 各部位の背景色を設定し、プロパティファイルに書き出す
     bool SetEachBGColorSetting(const wxString& bgColor);
     // 色・フォント設定用画面のサンプル部分の背景色を変更する
     void SetSampleBGColorSetting(const int id);

#ifdef __WXMAC__
     // メインのスレッドにイベントを送る
     void SendUIUpdateEvent() {
	  wxCommandEvent* event = new wxCommandEvent(wxEVT_UPDATE_UI, ID_SettingPanelUpdate);
	  wxString ui = wxT("ColorFontSettingPanel");
	  event->SetString(ui.c_str());
	  event->SetEventObject(this);

   #if wxCHECK_VERSION(2, 9, 0)
	  wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event->Clone());
   #else
	  this->GetEventHandler()->AddPendingEvent(*event);
   #endif
     };
#endif


protected:
     // begin wxGlade: ColorFontSettingPanel::attributes

     /**
      * サンプルのテキスト：全部で11ある
      */
     BasicDrawPane* treeSampleLabel;
     BasicDrawPane* threadListSampleLabel;
     BasicDrawPane* extractSampleLabel;
     BasicDrawPane* logWindowSampleLabel;
     BasicDrawPane* threadTitleSampleLabel;
     BasicDrawPane* threadViewSampleLabel;
     BasicDrawPane* kakikoSampleLabel;
     BasicDrawPane* memoSampleLabel;
     BasicDrawPane* hintSampleLabel;
     BasicDrawPane* linkSampleLabel;
     BasicDrawPane* othersSampleLabel;

     wxStaticBox* sizer_6_staticbox;
     wxStaticBox* sizer_5_staticbox;
     wxStaticBox* sizer_4_staticbox;
     wxPanel* panel_7;
     wxPanel* panel_8;
     wxPanel* panel_9;
     wxPanel* panel_4;
     wxButton* treeFontButton;
     wxButton* threadListFontButton;
     wxButton* extractFontButton;
     wxPanel* panel_10;
     wxButton* logWindowFontButton;
     wxButton* threadTitleFontButton;
     wxButton* kakikoFontButton;
     wxButton* memoFontButton;
     wxPanel* panel_11;
     wxButton* hintFontButton;
     wxButton* linkFontButton;
     wxPanel* panel_12;
     wxButton* otherFontButton;
     wxButton* allFontButton;
     wxPanel* panel_5;
     wxButton* treeBGColorButton;
     wxButton* threadListBGColorButton;
     wxButton* logWindowBGColorButton;
     wxButton* threadTitleBGColorButton;
     wxButton* threadViewBGColorButton;
     wxButton* memoBGColorButton;
     wxButton* hintBGColorButton;
     wxButton* popupBGColorButton_copy_1;
     wxPanel* panel_13;
     wxButton* allBGColorButton;
     wxPanel* panel_6;
     wxPanel* panel_2;
     wxStaticText* label_1;
     wxPanel* panel_3;
     // end wxGlade

     // 変数一時保存のため宣言
     int m_id;

     DECLARE_EVENT_TABLE() 
}; // wxGlade: end class

#endif // OTHERPANELS_H
