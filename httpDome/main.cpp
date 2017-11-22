
#pragma warning(disable:4996)//不要有4996错误
#pragma comment(lib,"ws2_32.lib")//lib动态链接库

#include<WinSock2.h>//socket2.2的版本
#include<stdio.h>
#include<stdlib.h>

WSADATA wsaData;
SOCKET  s;
SOCKADDR_IN ServerAddr;
int     Ret;
int     Port = 8080;


int main()
{
	//1.加载和初始化网络库
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStarup faiked with error. %d\n", Ret);
	}

	//2.创建套接字
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("socket filed with error. %d\n", WSAGetLastError());
		WSACleanup();
	}

	//3.开始链接
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port); //链接的端口
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("116.62.11.154"); // 

  // printf("We are trying to  connect to %s:%d...\n", inet_ntoa(ServerAddr.sin_addr), htons(ServerAddr.sin_port));

																  //4.链接
	if ((connect(s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr))) == SOCKET_ERROR)
	{
		printf("connect failed with error. %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		
	}
	else
	{
		printf("和服务器链接成功!\n");
	}





	//拼接http字符串格式

	char str1[4096];

	memset(str1, 0, 4096);
	strcat(str1, "POST /mobile/list.do HTTP/1.1\n");  //请求行：  |请求方式|空格|你要访问的具体位置|空格|http版本|换行符号|
	strcat(str1, "Host: 116.62.11.154:8080\n");       //请求头：  |key头名字段|冒号|值|换行符号|
	strcat(str1, "mobile: 18717791650\n");
	strcat(str1, "password: 123\n");
	strcat(str1, "ContentType: application/x-www-form-urlencoded; charset=UTF-8\n");
	strcat(str1, "Content-Length: 0");
	strcat(str1, "\r\n\r\n");                          //结尾：   |回车换行回车换行| 











	//5.发送
	if ((Ret = send(s, str1, sizeof(str1), 0)) == SOCKET_ERROR)
	{
		printf("Sent failed with error %d.\n", WSAGetLastError());

		
		closesocket(s);
		WSACleanup();
	}
	else
	{
		printf("已经发送http格式的数据!\n");
	}



	char DataBuffer[10240];//缓存客服端发送的数据
						   //6.用客服端的套接字去接收,客服端的数据,函数“recv”
	if ((Ret = recv(s, DataBuffer, sizeof(DataBuffer), 0)) == SOCKET_ERROR)
	{
		printf("recv failed with error %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
	}
	else
	{
		wchar_t *pwText = NULL;
		DataBuffer[Ret] = '\0';
		DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, DataBuffer, -1, NULL, 0);    //返回原始ASCII码的字符数目       
		pwText = new wchar_t[dwNum];                                              //根据ASCII码的字符数分配UTF8的空间
		MultiByteToWideChar(CP_UTF8, 0, DataBuffer, -1, pwText, dwNum);           //将ASCII码转换成UTF8
		

		int iSize;
		char* pszMultiByte;

		iSize = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL); //iSize =wcslen(pwsUnicode)+1=6
		pszMultiByte = (char*)malloc(iSize * sizeof(char)); //不需要 pszMultiByte = (char*)malloc(iSize*sizeof(char)+1);
		WideCharToMultiByte(CP_ACP, 0, pwText, -1, pszMultiByte, iSize, NULL, NULL);

		printf("http请求成功:\n%s", pszMultiByte);

		
	}


	closesocket(s);
	WSACleanup();

	system("pause");
	return 0;

}