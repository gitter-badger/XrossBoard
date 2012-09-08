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

#ifndef JANECLONEUTIL_H_
#define JANECLONEUTIL_H_

#include <zlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/regex.h>
#include "wx/wxnkf.h"
#include "datatype.hpp"

class JaneCloneUtil {
public:
	/**
	 * gzipファイルを解凍する処理
	 * 引数１は読み込み元gzipファイルのPATH、引数２は解凍先のファイルのPATH
	 * いずれもファイル名までを記述する
	 */
	static void DecommpressFile(wxString& inputPath, wxString& outputPath);
	/**
	 * ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
	 * 引数１は読み込み元のPATH、引数２は出力先ファイルのPATH いずれもファイル名までを記述する
	 */
	static void ConvertSJISToUTF8(wxString& inputPath, wxString& outputPath);
	/**
	 * 指定されたパスにあるHTTPヘッダファイルから取得日時を取得する処理
	 */
	static wxString GetHTTPCommTimeFromHeader(wxString& headerPath);
	/**
	 * 指定された数字からスレッドの作成された時間を計算する処理
	 */
	static wxString CalcThreadCreatedTime(wxString& threadNum);
	/**
	 * 指定されたパスにファイルがあればファイルサイズを返す処理
	 */
	static size_t GetFileSize(const wxString& filePath);
	/**
	 * アンカーで指定されたレスをファイルから読み取ってDOM形式にして送り返す
	 */
	static wxString FindAnchoredResponse(wxString& boardNameAscii,
			wxString& origNumber, wxString& resNumber);
	/**
	 * レス内にURLがあれば<a>タグを付ける
	 */
	static wxString ReplaceURLText(const wxString& responseText);
	/**
	 * 指定された文字列でdatファイルへのファイルパスを組み立てる
	 */
	static wxString AssembleFilePath(wxString& boardNameAscii, wxString& origNumber);

private:
	// ディスクからの読取サイズ
#define S_SIZE (2048)

};

#endif /* JANECLONEUTIL_H_ */