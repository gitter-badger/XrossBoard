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

#include "JaneCloneUtil.h"

/**
 * gzipファイルを解凍する処理
 * 引数１は読み込み元gzipファイルのPATH、引数２は解凍先のファイルのPATH
 * いずれもファイル名までを記述する
 */
void JaneCloneUtil::DecommpressFile(wxString & inputPath,
		wxString & outputPath) {
	// gzファイルをZlibを使って解凍する
	gzFile infile = gzopen(inputPath.mb_str(), "rb");
	FILE *outfile = fopen(outputPath.mb_str(), "wb");

	char buffer[S_SIZE];
	int num_read = 0;
	while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
		fwrite(buffer, 1, num_read, outfile);
	}

	// ファイルポインタを閉じる
	gzclose(infile);
	fclose(outfile);
}

/**
 * ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
 * 引数１は読み込み元のPATH、引数２は出力先ファイルのPATH いずれもファイル名までを記述する
 */
void JaneCloneUtil::ConvertSJISToUTF8(wxString & inputPath,
		wxString & outputPath) {

	// LibNKFを呼び出してCP932からUTF-8への変換を行う
	wxNKF* nkf = new wxNKF();
	nkf->Convert(inputPath, outputPath, wxT("--ic=CP932 --oc=UTF-8"));
	delete nkf;
}

/**
 * 指定されたパスにあるHTTPヘッダファイルから取得日時を取得する処理
 */
wxString JaneCloneUtil::GetHTTPCommTimeFromHeader(wxString& headerPath) {
	// HTTPヘッダファイルを読み込む
	wxTextFile httpHeaderFile;
	httpHeaderFile.Open(headerPath, wxConvUTF8);
	wxString str;

	// ファイルがオープンされているならば
	if (httpHeaderFile.IsOpened()) {
		for (str = httpHeaderFile.GetFirstLine(); !httpHeaderFile.Eof(); str =
				httpHeaderFile.GetNextLine()) {
			// 上から読み込んで一番最初に当たる日付が取得日時
			if (str.Contains(wxT("Date:")) && str.Contains(wxT("GMT"))) {
				httpHeaderFile.Close();
				return str;
			}
		}
	}
	// ここまで来てしまった場合空文字を返す
	httpHeaderFile.Close();
	return wxEmptyString;
}

/**
 * 指定された数字からスレッドの作成された時間を計算する処理
 */
wxString JaneCloneUtil::CalcThreadCreatedTime(wxString& threadNum) {
	int threadNumInt = wxAtoi(threadNum);
	time_t timeGMT = (time_t) threadNumInt;
	struct tm *date = localtime(&timeGMT);
	char str[64];
	strftime(str, 63, "%x %H:%M", date);
	return wxString(str, wxConvUTF8);
}

/**
 * 指定されたパスにファイルがあればファイルサイズを返す処理
 */
size_t JaneCloneUtil::GetFileSize(const wxString& filePath) {

	/**
	 * メモ：2chのスレッドの容量を測る場合、スレッドはデータ容量が約500KBで規制されるので
	 * fileSizeをsize_tでキャストしても安全？
	 * 符号なしint型の範囲は 0 ～ 65535
	 */

	if (wxFileName::FileExists(filePath)) {

		wxULongLong fileSize = wxFileName::GetSize(filePath);

		if (fileSize == wxInvalidSize) {
			// ファイルが正常に開けなかった場合
			return 0;
		} else {
			// unsigned long型で返却
			return (size_t) fileSize.GetLo();
		}
	} else {
		// ファイルが存在しなかった場合
		return 0;
	}
}
/**
 * アンカーで指定されたレスをファイルから読み取ってDOM形式にして送り返す
 */
wxString JaneCloneUtil::FindAnchoredResponse(wxString& boardNameAscii,
		wxString& origNumber, wxString& resNumber) {

	// ファイルパスの組み立てとファイルの有無確認
	wxDir dir(wxGetCwd());
	wxString filePath = dir.GetName();

#ifdef __WXMSW__
	// Windowsではパスの区切りは"\"
	filePath += wxT("\\dat\\");
	filePath += boardNameAscii;
	filePath += wxT("\\");
	filePath += origNumber;
	filePath += wxT(".dat");
#else
	// それ以外ではパスの区切りは"/"
	filePath += wxT("/dat/");
	filePath += boardNboardNameAscii;
	filePath += wxT("/");
	filePath += origNumber;
	filePath += wxT(".dat");
#endif

	if (!wxFile::Exists(filePath)) {
		// 無ければ空文字リターン
		return wxEmptyString;
	}

	int resNumInt = wxAtoi(resNumber);

	// datファイルを読み込む
	wxTextFile datFile;
	datFile.Open(filePath, wxConvUTF8);
	wxString str;

	if (datFile.IsOpened() && 1 == resNumInt) {
		str = datFile.GetFirstLine();
		datFile.Close();

	} else if (datFile.IsOpened() && resNumInt > 1) {
		datFile.GetFirstLine();

		for (int i = 1; !datFile.Eof(); str = datFile.GetNextLine()) {
			// 上から読み込んでレス番号にあたる行のレスを取得する
			if (i == resNumInt) {
				datFile.Close();
				break;
			}
			++i;
		}
	}

	// HTMLのDOM形式にする
	wxString lumpOfHTML = HTML_HEADER_POPUP;
	lumpOfHTML += wxT("<dt>");
	lumpOfHTML += resNumber;

	wxString default_nanashi, mail, day_and_ID, res;

	// 正規表現でレスの内容を取り出してメモリに展開する
	if (regexThread.IsValid()) {
		if (regexThread.Matches(str)) {
			// マッチさせたそれぞれの要素を取得する
			default_nanashi = regexThread.GetMatch(str, 1);
			mail = regexThread.GetMatch(str, 2);
			day_and_ID = regexThread.GetMatch(str, 3);

			// レスの最初に<table>タグを入れる
			res.Append(wxT("<table border=0 id=\"") + resNumber + wxT("\">"));
			res.Append(regexThread.GetMatch(str, 4));
			res.Append(wxT("</table>"));

			// レス内部のURLに<a>タグをつける
			res = ReplaceURLText(res);
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

	// HTMLソースを加える
	lumpOfHTML += HTML_FOOTER;

	return lumpOfHTML;
}
/**
 * レス内にURLがあれば<a>タグを付ける
 */
wxString JaneCloneUtil::ReplaceURLText(const wxString& responseText) {

	wxString text = responseText;
	wxString tmp, result;
	size_t start, len;

	if (regexURL.IsValid() && regexURL.Matches(responseText)) {
		for (tmp = text; regexURL.Matches(tmp);
				tmp = tmp.SubString(start + len, tmp.Len())) {
			regexURL.GetMatch(&start, &len, 0);
			result += tmp.SubString(0, start - 1);
			result += wxT("<a href=\"");
			result += tmp.SubString(start, start + len - 1);
			result += wxT("\"/>");
			result += tmp.SubString(start, start + len - 1);
			result += wxT("</a>");
		}
		result += tmp;
	} else {
		return responseText;
	}

	return result;
}
