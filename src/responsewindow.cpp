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

#include "responsewindow.hpp"
#include "wx/html/m_templ.h"

#ifndef __WXMSW__
#include"../rc/janeclone.xpm"
#endif

// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Sep 11 22:14:45 2012

// begin wxGlade: ::extracode
// end wxGlade

// event table
BEGIN_EVENT_TABLE(ResponseWindow, wxDialog)
   EVT_BUTTON(ID_PostResponse, ResponseWindow::OnPostResponse)
   EVT_BUTTON(ID_QuitResponseWindow, ResponseWindow::QuitResponseWindow)
   EVT_BUTTON(ID_PostConfirmForm, ResponseWindow::PostConfirmForm)
END_EVENT_TABLE()


/** カスタムタグをここで設定する */

// I refered custom tags implementation
// here https://groups.google.com/forum/?fromgroups=#!topic/wx-users/brDgsxuuR0k
// Thanks!

// <form> tags
TAG_HANDLER_BEGIN(FORM, "FORM")
TAG_HANDLER_CONSTR(FORM){}
TAG_HANDLER_PROC(tag) {
     m_WParser->CloseContainer();
     m_WParser->OpenContainer();
     ParseInner(tag);
     return true;
}
TAG_HANDLER_END(FORM)

// <select> tags
TAG_HANDLER_BEGIN(SELECT, "SELECT, OPTION")
     TAG_HANDLER_CONSTR(SELECT) {
     m_pComboBox = NULL;
     m_iNumber = 0;
}

TAG_HANDLER_VARS
wxComboBox *m_pComboBox;
int m_iNumber;

TAG_HANDLER_PROC(tag) {
     if (m_pComboBox && tag.GetName() == wxT("OPTION")) {
	  if (tag.HasParam(wxT("VALUE"))) {
	       wxString strValue = tag.GetParam(wxT("VALUE"));
	       wxString strName;
	       strName = m_WParser->GetSource()->Mid(
		    tag.GetBeginPos(),
		    tag.GetEndPos1()-tag.GetBeginPos());
#if !wxUSE_UNICODE
	       wxCSConv conv(m_WParser->GetInputEncoding());
	       strName = wxString(strName.wc_str(conv), wxConvLocal);
#endif

	       m_WParser->GetEntitiesParser()->Parse(strName);

	       m_pComboBox->Append(strName, &strValue);
	       if (!m_iNumber) m_pComboBox->SetValue(strName);
	       m_iNumber++;
	  }
	  return true;
     } else if (tag.GetName() == wxT("SELECT")) {
	  int fl = 0;
	  wxComboBox *pOldBox = m_pComboBox;
	  int iOldNumber = m_iNumber;

	  m_pComboBox = new wxComboBox(m_WParser->GetWindowInterface()->GetHTMLWindow(), wxID_ANY, _(""), wxPoint(0,0), wxDefaultSize, 0, NULL, wxCB_READONLY  | wxCB_DROPDOWN);
	  m_pComboBox->Show(true);

	  ParseInner(tag);


	  m_WParser->GetContainer()->InsertCell(new wxHtmlWidgetCell(m_pComboBox, fl));

	  m_pComboBox = pOldBox;
	  m_iNumber = iOldNumber;
	  return true;

     }
     return false;
}

TAG_HANDLER_END(SELECT)

// <input> tags
TAG_HANDLER_BEGIN(INPUT, "INPUT")
     TAG_HANDLER_CONSTR(INPUT){}
/**
 * 本来はHTMLのsubmitボタンは一般的なものであるが
 * ここではクッキー情報が得られていない状態での投稿確認ボタン専用とする
 */
TAG_HANDLER_PROC(tag) {
     if (tag.HasParam(wxT("TYPE"))) {
	  if (!tag.GetParam(wxT("TYPE")).CmpNoCase(wxT("SUBMIT"))) {
	       int fl = 0;
	       wxButton *pButton;
	       // 投稿時のボタン専用として使う
	       pButton = new wxButton(m_WParser->GetWindowInterface()->GetHTMLWindow(), ID_PostConfirmForm, tag.GetParam(wxT("VALUE")), wxPoint(0,0), wxDefaultSize);
	       pButton->Show(true);

	       m_WParser->GetContainer()->InsertCell(new wxHtmlWidgetCell(pButton, fl));
	  } else if (!tag.GetParam(wxT("TYPE")).CmpNoCase(wxT("TEXT"))) {
	       int fl = 0;
	       wxTextCtrl *pText;

	       pText = new wxTextCtrl(m_WParser->GetWindowInterface()->GetHTMLWindow(), wxID_ANY, tag.GetParam(wxT("VALUE")), wxPoint(0,0), wxDefaultSize);
	       pText->Show(true);

	       m_WParser->GetContainer()->InsertCell(new wxHtmlWidgetCell(pText, fl));
	  }
     }
     return true;
}

TAG_HANDLER_END(SELECT)

// <form> tags
TAGS_MODULE_BEGIN(Form)

     TAGS_MODULE_ADD(FORM)
     TAGS_MODULE_ADD(SELECT)
     TAGS_MODULE_ADD(INPUT)

TAGS_MODULE_END(Form)

ResponseWindow::ResponseWindow(wxWindow* parent, wxString& title, URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, wxPoint& point):
wxDialog(parent, wxID_ANY, wxEmptyString, point, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {

     // アイコンの設定を行う
#ifdef __WXMSW__
     SetIcon(wxICON(wxicon));
#else
     SetIcon(wxICON(janeclone));
#endif

     // 構造体をローカルに格納する
     m_boardInfo = boardInfoHash;
     m_threadInfo = threadInfoHash;

     // begin wxGlade: ResponseWindow::ResponseWindow
     resNoteBook = new wxNotebook(this, ID_ResponseWindow, wxDefaultPosition, wxDefaultSize, 0);
     settingPane = new wxPanel(resNoteBook, wxID_ANY);
     localRulePane = new wxPanel(resNoteBook, wxID_ANY);
     previewPane = new wxPanel(resNoteBook, wxID_ANY);
     resPane = new wxPanel(resNoteBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
     boardNameTitle = new wxStaticText(resPane, wxID_ANY, wxT("【板名】 - スレッドタイトル"));
     name = new wxStaticText(resPane, wxID_ANY, wxT("名前："));
     const wxString *nameCombo_choices = NULL;
     nameCombo = new wxComboBox(resPane, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, nameCombo_choices, wxCB_DROPDOWN);
     mail = new wxStaticText(resPane, wxID_ANY, wxT("メール"));
     const wxString *mailCombo_choices = NULL;
     mailCombo = new wxComboBox(resPane, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, mailCombo_choices, wxCB_DROPDOWN);
     sageCheck = new wxCheckBox(resPane, wxID_ANY, wxT("sage"));
     text_ctrl_1 = new wxTextCtrl(resPane, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL);
     previewWindow = new wxHtmlWindow(previewPane, wxID_ANY);
     localRuleWindow = new wxHtmlWindow(localRulePane, wxID_ANY);
     settingText = new wxTextCtrl(settingPane, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
     handleCheck = new wxCheckBox(this, wxID_ANY, wxT("コテハン記憶"));
     frontCheck = new wxCheckBox(this, wxID_ANY, wxT("最前面"));
     formatCheck = new wxCheckBox(this, wxID_ANY, wxT("末尾整形"));
     beCheck = new wxCheckBox(this, wxID_ANY, wxT("BE"));
     const wxString *beMailCombo_choices = NULL;
     beMailCombo = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, beMailCombo_choices, wxCB_DROPDOWN);
     proxyCheck = new wxCheckBox(this, wxID_ANY, wxT("Proxy"));
     const wxString *proxyCombo_choices = NULL;
     proxyCombo = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, proxyCombo_choices, wxCB_DROPDOWN);
     space = new wxPanel(this, wxID_ANY);
     postButton = new wxButton(this, ID_PostResponse, wxT("書き込み"));
     quitButton = new wxButton(this, ID_QuitResponseWindow, wxT("やめる"));

     set_properties(title);
     do_layout();
     // end wxGlade
}


void ResponseWindow::set_properties(const wxString& title) {
    // begin wxGlade: ResponseWindow::set_properties
    SetTitle(wxT("『") + title + wxT("』にレス"));
    SetSize(wxSize(640, 480));
    // end wxGlade
}


void ResponseWindow::do_layout() {
    // begin wxGlade: ResponseWindow::do_layout
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hboxOther2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hboxOther1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* settingVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* localRuleVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* previewVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* resVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    resVbox->Add(boardNameTitle, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    hbox1->Add(name, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    hbox1->Add(nameCombo, 1, 0, 0);
    hbox1->Add(mail, 0, wxALIGN_CENTER_VERTICAL, 0);
    hbox1->Add(mailCombo, 0, 0, 0);
    hbox1->Add(sageCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    resVbox->Add(hbox1, 0, wxEXPAND, 0);
    resVbox->Add(text_ctrl_1, 1, wxEXPAND, 0);
    resPane->SetSizer(resVbox);
    previewVbox->Add(previewWindow, 1, wxEXPAND, 0);
    previewPane->SetSizer(previewVbox);
    localRuleVbox->Add(localRuleWindow, 1, wxEXPAND, 0);
    localRulePane->SetSizer(localRuleVbox);
    settingVbox->Add(settingText, 1, wxEXPAND, 0);
    settingPane->SetSizer(settingVbox);
    resNoteBook->AddPage(resPane, wxT("書き込み"));
    resNoteBook->AddPage(previewPane, wxT("プレビュー"));
    resNoteBook->AddPage(localRulePane, wxT("ローカルルール"));
    resNoteBook->AddPage(settingPane, wxT("SETTING.TXT"));
    vbox->Add(resNoteBook, 1, wxEXPAND, 0);
    hboxOther1->Add(handleCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(frontCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(formatCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(beCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(beMailCombo, 0, wxALIGN_CENTER_VERTICAL, 0);
    vbox->Add(hboxOther1, 0, wxEXPAND, 0);
    hboxOther2->Add(proxyCheck, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    hboxOther2->Add(proxyCombo, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    hboxOther2->Add(space, 1, wxEXPAND, 0);
    hboxOther2->Add(postButton, 0, wxALIGN_RIGHT, 0);
    hboxOther2->Add(quitButton, 0, wxALIGN_RIGHT, 0);
    vbox->Add(hboxOther2, 0, wxEXPAND, 0);
    SetSizer(vbox);
    Layout();
    // end wxGlade
}
/**
 * レスを投稿するイベント
 */
void ResponseWindow::OnPostResponse(wxCommandEvent &event) {

     // ソケット通信用のクラスのインスタンスを用意する
     SocketCommunication* socketCommunication = new SocketCommunication();
     socketCommunication->SetLogWindow(m_logCtrl);
     // クッキーの状態チェック
     int cookieStatus = CheckCookie();

     if (cookieStatus == NO_COOKIE) {
	  // クッキーがない状態
	  PostFirstResponse(socketCommunication);
	  delete socketCommunication;
     } else if (cookieStatus == HAS_COOKIE_HIDDEN) {
	  // 最初のレスの後クッキーのみもらった状態
	  PostConfirm(socketCommunication);
	  delete socketCommunication;
     } else if (cookieStatus == HAS_PERN) {
	  // PERNをもらった状態；通常の書き込み
	  PostResponse(socketCommunication);
	  delete socketCommunication;
     }
}
/**
 * クッキーがない状態
 */
void ResponseWindow::PostFirstResponse(SocketCommunication* sock) {

     // 書き込み内容を構造体に設定する
     PostContent* post = new PostContent;
     // NKFの準備
     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     wxNKF* nkf = new wxNKF();
     const std::string stdName = nkf->WxToMultiByte(nameCombo->GetValue(), option);
     const std::string stdMail = nkf->WxToMultiByte(mailCombo->GetValue(), option);
     const std::string stdKakikomi = nkf->WxToMultiByte(text_ctrl_1->GetValue(), option);
     delete nkf;

     // 投稿用の構造体にURLエンコードされた文字列を格納
     post->name = wxString(JaneCloneUtil::UrlEncode(stdName).c_str(), wxConvUTF8);
     post->mail = wxString(JaneCloneUtil::UrlEncode(stdMail).c_str(), wxConvUTF8);
     post->kakikomi = wxString(JaneCloneUtil::UrlEncode(stdKakikomi).c_str(), wxConvUTF8);

     sock->SetPostContent(post);
     wxString result = sock->PostFirstToThread(m_boardInfo, m_threadInfo, NO_COOKIE);

     // m_postContentにデータを設定する
     m_postContent = post;

     if (result.StartsWith(wxT("<html>"))) {
	  // 返り値が<html>タグから始まっていれば書込は失敗
	  // wxHtmlWindowに結果を表示する	  
	  resNoteBook->SetSelection(KAKIKO_PAGE);
	  previewWindow->SetPage(result);
	  // メモリの解放
	  delete post;
	  delete sock;
	  return;
     }
     
     // 失敗でなければ確認画面を表すヘッダファイルへのパスなので
     // ユーザーに確認させるため表示する
     // wxStringにバッファするサイズを計測する
     size_t fileSize = JaneCloneUtil::GetFileSize(result);
     if (fileSize == 0) {
	  // wxHtmlWindowに結果を表示する
	  resNoteBook->SetSelection(KAKIKO_PAGE);
	  previewWindow->SetPage(FAIL_TO_POST);
	  delete post;
	  delete sock;
	  return;
     }
     // 取得サイズ分だけwxStringを確保する
     wxString htmlSource;
     htmlSource.Alloc(fileSize);

     // テキストファイルの読み込み
     wxTextFile confirmFile;
     confirmFile.Open(result, wxConvUTF8);
     wxString str;

     // ファイルがオープンされているならば
     if (confirmFile.IsOpened()) {
	  for (str = confirmFile.GetFirstLine(); !confirmFile.Eof();
	       str = confirmFile.GetNextLine()) {

	       if (str.IsNull() || !str.StartsWith(wxT("<html>"))) {
		    continue;
	       } else {
		    str.Replace(wxT("charset=x-sjis"), wxT("charset=utf-8"));
	       }

	       htmlSource += str;
	  }
     }

     confirmFile.Close();
     // wxHtmlWindowに結果を表示する
     resNoteBook->SetSelection(KAKIKO_PAGE);
     previewWindow->SetPage(htmlSource);
}
/**
 * 最初のレスの後クッキーのみもらった状態
 */
void ResponseWindow::PostConfirm(SocketCommunication* sock) {

     if (m_postContent->kakikomi.IsEmpty()) {
     	  // 内容が無ければエラー
     	  *m_logCtrl << wxT("内部エラー…(ヽ´ん`)…やり直してみて…\n");
     	  return;
     }

     sock->SetPostContent(m_postContent);
     wxString result = sock->PostConfirmToThread(m_boardInfo, m_threadInfo, HAS_COOKIE_HIDDEN);

     if (result.StartsWith(wxT("<html>"))) {
	  // 返り値が<html>タグから始まっていれば書込は失敗
	  // wxHtmlWindowに結果を表示する
	  resNoteBook->SetSelection(KAKIKO_PAGE);
	  previewWindow->SetPage(result);
	  // メモリの解放
	  //delete post;
	  delete sock;
	  return;
     }
     
     // 失敗でなければ確認画面を表すヘッダファイルへのパスなので
     // ユーザーに確認させるため表示する
     // wxStringにバッファするサイズを計測する
     size_t fileSize = JaneCloneUtil::GetFileSize(result);
     if (fileSize == 0) {
	  // wxHtmlWindowに結果を表示する
	  resNoteBook->SetSelection(KAKIKO_PAGE);
	  previewWindow->SetPage(FAIL_TO_POST);
	  //delete post;
	  delete sock;
	  return;
     }
     // 取得サイズ分だけwxStringを確保する
     wxString htmlSource;
     htmlSource.Alloc(fileSize);

     // テキストファイルの読み込み
     wxTextFile confirmFile;
     confirmFile.Open(result, wxConvUTF8);
     wxString str;

     // ファイルがオープンされているならば
     if (confirmFile.IsOpened()) {
	  for (str = confirmFile.GetFirstLine(); !confirmFile.Eof();
	       str = confirmFile.GetNextLine()) {

	       if (str.IsNull() || !str.StartsWith(wxT("<html>"))) {
		    continue;
	       } else {
		    str.Replace(wxT("charset=x-sjis"), wxT("charset=utf-8"));
	       }

	       htmlSource += str;
	  }
     }

     confirmFile.Close();
     // wxHtmlWindowに結果を表示する
     resNoteBook->SetSelection(KAKIKO_PAGE);
     previewWindow->SetPage(htmlSource);
}
/**
 * PERNをもらった状態；通常の書き込み
 */
void ResponseWindow::PostResponse(SocketCommunication* sock) {

     // 書き込み内容を構造体に設定する
     PostContent* post = new PostContent;
     // NKFの準備
     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     wxNKF* nkf = new wxNKF();
     const std::string stdName = nkf->WxToMultiByte(nameCombo->GetValue(), option);
     const std::string stdMail = nkf->WxToMultiByte(mailCombo->GetValue(), option);
     const std::string stdKakikomi = nkf->WxToMultiByte(text_ctrl_1->GetValue(), option);
     delete nkf;

     // 投稿用の構造体にURLエンコードされた文字列を格納
     post->name = wxString(JaneCloneUtil::UrlEncode(stdName).c_str(), wxConvUTF8);
     post->mail = wxString(JaneCloneUtil::UrlEncode(stdMail).c_str(), wxConvUTF8);
     post->kakikomi = wxString(JaneCloneUtil::UrlEncode(stdKakikomi).c_str(), wxConvUTF8);

     sock->SetPostContent(post);
     wxString result = sock->PostResponseToThread(m_boardInfo, m_threadInfo, HAS_PERN);
     delete post;

     if (result.StartsWith(wxT("<html>"))) {
	  // 返り値が<html>タグから始まっていれば書込は失敗
	  // wxHtmlWindowに結果を表示する
	  resNoteBook->SetSelection(KAKIKO_PAGE);
	  previewWindow->SetPage(result);
	  // メモリの解放
	  delete sock;
	  return;
     }
     
     // 失敗でなければ確認画面を表すヘッダファイルへのパスなので
     // ユーザーに確認させるため表示する
     // wxStringにバッファするサイズを計測する
     size_t fileSize = JaneCloneUtil::GetFileSize(result);
     if (fileSize == 0) {
	  // wxHtmlWindowに結果を表示する
	  resNoteBook->SetSelection(KAKIKO_PAGE);
	  previewWindow->SetPage(FAIL_TO_POST);
	  delete sock;
	  return;
     }
     // 取得サイズ分だけwxStringを確保する
     wxString htmlSource;
     htmlSource.Alloc(fileSize);

     // テキストファイルの読み込み
     wxTextFile confirmFile;
     confirmFile.Open(result, wxConvUTF8);
     wxString str;

     // ファイルがオープンされているならば
     if (confirmFile.IsOpened()) {
	  for (str = confirmFile.GetFirstLine(); !confirmFile.Eof();
	       str = confirmFile.GetNextLine()) {

	       if (str.IsNull() || !str.StartsWith(wxT("<html>"))) {
		    continue;
	       } else {
		    str.Replace(wxT("charset=x-sjis"), wxT("charset=utf-8"));
	       }

	       htmlSource += str;
	  }
     }

     confirmFile.Close();
     // wxHtmlWindowに結果を表示する
     resNoteBook->SetSelection(KAKIKO_PAGE);
     previewWindow->SetPage(htmlSource);
}
/**
 * レス用ウィンドウを閉じるイベント
 */
void ResponseWindow::QuitResponseWindow(wxCommandEvent &event) {

     Close(true);
}
/**
 *  投稿確認ボタンイベント
 */
void ResponseWindow::PostConfirmForm(wxCommandEvent &event) {

     // 書き込みイベントに制御を渡す
     OnPostResponse(event);
 }
/**
 * クッキーの状態チェック
 */
int ResponseWindow::CheckCookie() {

     wxString configFile = ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR 
	  + wxFileSeparator + wxT("prop") + wxFileSeparator + COOKIE_CONFIG_FILE;

     if (!wxFile::Exists(configFile))
	  return NO_COOKIE;

     // 内部にある要素を調べる
     wxString hiddenName = wxEmptyString, hiddenVal = wxEmptyString, cookie = wxEmptyString, pern = wxEmptyString;
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenName"), &hiddenName);
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenValue"), &hiddenVal);
     JaneCloneUtil::GetJaneCloneProperties(wxT("Cookie"), &cookie);
     JaneCloneUtil::GetJaneCloneProperties(wxT("PERN"), &pern);

     // もしクッキーと隠し要素１がなければクッキーがないのと同じなのでNO_COOKIEを返す
     if (hiddenName.IsEmpty() || hiddenVal.IsEmpty() || cookie.IsEmpty()) {
	  return NO_COOKIE;
     }
	  
     // PERNがあればHAS_PERN、なければHAS_COOKIE_HIDDEN
     if (!pern.IsEmpty()) {
	  return HAS_PERN;
     }

     return HAS_COOKIE_HIDDEN;
}
