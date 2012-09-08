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

#include "threadcontentwindow.hpp"

IMPLEMENT_DYNAMIC_CLASS(ThreadContentWindow, wxHtmlWindow)

// event table
BEGIN_EVENT_TABLE(ThreadContentWindow, wxHtmlWindow)
EVT_RIGHT_DOWN(ThreadContentWindow::OnRightClickHtmlWindow)
EVT_MENU(ID_CopyFromHtmlWindow, ThreadContentWindow::CopyFromHtmlWindow)
EVT_MENU(ID_CopyURLFromHtmlWindow, ThreadContentWindow::CopyURLFromHtmlWindow)
EVT_MENU(ID_SelectAllTextHtmlWindow, ThreadContentWindow::SelectAllTextHtmlWindow)
END_EVENT_TABLE()

/**
 * 通常のコンストラクタ
 */
ThreadContentWindow::ThreadContentWindow(wxWindow* parent, const wxString& threadContentPath):
wxHtmlWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO) {

	// 指定されたパスからHTMLファイルを読み出す
	wxString htmlSource = GetConvertedDatFile(threadContentPath);
	// メモリに読み込んだHTMLを表示する
	this->SetPage(htmlSource);
}
/**
 * 指定されたパスからHTMLファイルを読み出し、2ch形式に加工する
 */
const wxString ThreadContentWindow::GetConvertedDatFile(
		const wxString& threadContentPath) {

	// wxStringにバッファするサイズを計測する
	size_t fileSize = JaneCloneUtil::GetFileSize(threadContentPath);

	if (fileSize == 0)
		// 読み込みに失敗した場合
		return FAIL_TO_READ_PAGE;

	// 取得サイズ分だけwxStringを確保する
	wxString htmlSource;
	htmlSource.Alloc(fileSize);
	htmlSource += HTML_HEADER;

	// テキストファイルの読み込み
	wxTextFile datfile;
	datfile.Open(threadContentPath, wxConvUTF8);
	wxString str;
	int number = 1;

	// ファイルがオープンされているならば
	if (datfile.IsOpened()) {
		for (str = datfile.GetFirstLine(); !datfile.Eof();
				str = datfile.GetNextLine()) {

			if (str.IsNull())
				continue;

			if (number == 1) {
				// 最初の１行目の処理
				htmlSource += ProcessFirstResponse(str);
			} else {
				// それ以降の処理
				htmlSource += ProcessRestResponse(str, number);
			}

			number++;
		}
	}

	htmlSource += HTML_FOOTER;
	datfile.Close();

	return htmlSource;
}
/**
 * スレッドの最初の行を処理するメソッド・スレの１には最後の「<>」の後にスレッドタイトルがつく
 */
wxString ThreadContentWindow::ProcessFirstResponse(wxString& threadRecord) {

	// スレッドのそれぞれの要素
	wxString default_nanashi, mail, day_and_ID, res;

	// ひとかたまりのHTMLソースにまとめる
	wxString lumpOfHTML = wxT("<dt>");
	wxString num = wxT("1");
	lumpOfHTML += num;

	// 正規表現でレスの内容を取り出してメモリに展開する
	if (regexThreadFst.IsValid()) {
		if (regexThreadFst.Matches(threadRecord)) {
			// マッチさせたそれぞれの要素を取得する
			default_nanashi = regexThreadFst.GetMatch(threadRecord, 1);
			mail = regexThreadFst.GetMatch(threadRecord, 2);
			day_and_ID = regexThreadFst.GetMatch(threadRecord, 3);

			// レスの最初に<table>タグを入れる
			res.Append(wxT("<table border=0 id=\"") + num + wxT("\">"));
			res.Append(regexThreadFst.GetMatch(threadRecord, 4));
			res.Append(wxT("</table>"));

			// レス内部のURLに<a>タグをつける
			res = JaneCloneUtil::ReplaceURLText(res);
			// レスの最後に改行
			res.Append(wxT("<br>"));
		}
	}

	if (mail != wxEmptyString) {
		// もしメ欄になにか入っているならば
		lumpOfHTML += wxT(" 名前：<a href=\"mailto:");
		lumpOfHTML += mail;
		lumpOfHTML += wxT("\"><b>");
		lumpOfHTML += default_nanashi;
		lumpOfHTML += wxT("</b></a>");
		lumpOfHTML += day_and_ID;
		lumpOfHTML += wxT("<dd>");
		lumpOfHTML += res;
	} else {
		// 空の場合
		lumpOfHTML += wxT(" 名前：<font color=green><b>");
		lumpOfHTML += default_nanashi;
		lumpOfHTML += wxT("</b></font>");
		lumpOfHTML += day_and_ID;
		lumpOfHTML += wxT("<dd>");
		lumpOfHTML += res;
	}

	return lumpOfHTML;
}
/**
 * スレッドの１以降を処理するメソッド
 */
wxString ThreadContentWindow::ProcessRestResponse(wxString& threadRecord, int number) {

	// スレッドのそれぞれの要素
	wxString default_nanashi, mail, day_and_ID, res;

	// ひとかたまりのHTMLソースにまとめる
	wxString lumpOfHTML = wxT("<dt>");
	wxString num;
	num << number;
	lumpOfHTML += num;

	// 正規表現でレスの内容を取り出してメモリに展開する
	if (regexThread.IsValid()) {
		if (regexThread.Matches(threadRecord)) {
			// マッチさせたそれぞれの要素を取得する
			default_nanashi = regexThread.GetMatch(threadRecord, 1);
			mail = regexThread.GetMatch(threadRecord, 2);
			day_and_ID = regexThread.GetMatch(threadRecord, 3);

			// レスの最初に<table>タグを入れる
			res.Append(wxT("<table border=0 id=\"") + num + wxT("\">"));
			res.Append(regexThread.GetMatch(threadRecord, 4));
			res.Append(wxT("</table>"));

			// レス内部のURLに<a>タグをつける
			res = JaneCloneUtil::ReplaceURLText(res);
			// レスの最後に改行
			res.Append(wxT("<br>"));
		}
	}

	if (mail != wxEmptyString) {
		// もしメ欄になにか入っているならば
		lumpOfHTML += wxT(" 名前：<a href=\"mailto:");
		lumpOfHTML += mail;
		lumpOfHTML += wxT("\"><b>");
		lumpOfHTML += default_nanashi;
		lumpOfHTML += wxT("</b></a>");
		lumpOfHTML += day_and_ID;
		lumpOfHTML += wxT("<dd>");
		lumpOfHTML += res;
	} else {
		// 空の場合
		lumpOfHTML += wxT(" 名前：<font color=green><b>");
		lumpOfHTML += default_nanashi;
		lumpOfHTML += wxT("</b></font>");
		lumpOfHTML += day_and_ID;
		lumpOfHTML += wxT("<dd>");
		lumpOfHTML += res;
	}

	return lumpOfHTML;
}
/**
 * スレッドのHtmlWindowで右クリックした場合の処理
 */
void ThreadContentWindow::OnRightClickHtmlWindow(wxMouseEvent& event) {

	wxMenu* copy = new wxMenu();
	copy->Append(ID_CopyFromHtmlWindow, wxT("コピー"));
	copy->Append(ID_CopyURLFromHtmlWindow, wxT("リンクをコピー"));
	copy->Enable(ID_CopyURLFromHtmlWindow, false); // デフォルトでは使用不能
	copy->Append(ID_SelectAllTextHtmlWindow, wxT("全て選択"));

	if (this->SelectionToText() != wxEmptyString) {
		// テキストを選択している場合は追加で選択肢を表示する
		copy->AppendSeparator();
		copy->Append(wxID_ANY, wxT("選択範囲をYahooで検索"));
		copy->Append(wxID_ANY, wxT("選択範囲をGoogleで検索"));
		copy->Append(wxID_ANY, wxT("選択範囲をamazonで検索"));
		copy->Append(wxID_ANY, wxT("選択範囲でスレッドタイトル検索"));
		copy->AppendSeparator();
		copy->Append(wxID_ANY, wxT("抽出ポップアップ"));
		copy->Append(wxID_ANY, wxT("この文にレス"));
		copy->Append(wxID_ANY, wxT("メモ欄に追加"));
		copy->Append(wxID_ANY, wxT("この文にメモ欄でレス"));
		copy->Append(wxID_ANY, wxT("選択単語でレス抽出"));
		wxMenu *ng = new wxMenu;
		ng->Append(wxID_ANY, wxT("NGWordに追加"));
		ng->Append(wxID_ANY, wxT("NGNameに追加"));
		ng->AppendSeparator();
		ng->Append(wxID_ANY, wxT("選択単語であぼ～ん"));
		ng->Append(wxID_ANY, wxT("選択単語で透明あぼ～ん"));
		copy->AppendSubMenu(ng, wxT("NG処理"));
		copy->AppendSeparator();
	}

	int x, y;
	CalcUnscrolledPosition(event.m_x, event.m_y, &x, &y);
	wxHtmlCell *cell = GetInternalRepresentation()->FindCellByPos(x, y);
	m_linkInfo = NULL;

	if(cell)
		m_linkInfo = cell->GetLink();

	if (m_linkInfo) {
		if (m_linkInfo->GetHref() != wxEmptyString) {
			// リンク上で右クリックしたのでなければ使用不能に
			copy->Enable(ID_CopyURLFromHtmlWindow, true);
		}
	}

	// ポップアップメニューを表示させる
	PopupMenu(copy);
}
/**
 * HtmlWindowで選択しているテキストをクリップボードにコピーする
 */
void ThreadContentWindow::CopyFromHtmlWindow(wxCommandEvent& event) {

	wxString selectText = this->SelectionToText();

	if (wxTheClipboard->Open()) {
		wxTheClipboard->Clear();
		wxTheClipboard->SetData(new wxTextDataObject(selectText));
		wxTheClipboard->Close();
	}
}
/*
 * HtmlWindowで選択しているURLをクリップボードにコピーする
 */
void ThreadContentWindow::CopyURLFromHtmlWindow(wxCommandEvent& event) {

	wxString url;

	if (m_linkInfo) {
		url = m_linkInfo->GetHref();
	}

	if (wxTheClipboard->Open()) {
		wxTheClipboard->Clear();
		wxTheClipboard->SetData(new wxTextDataObject(url));
		wxTheClipboard->Close();
	}
}
/*
 * HtmlWindowでテキストを全て選択する
 */
void ThreadContentWindow::SelectAllTextHtmlWindow(wxCommandEvent& event) {
	this->SelectAll();
}