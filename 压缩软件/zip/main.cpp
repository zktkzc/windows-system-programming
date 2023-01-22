#include <stdio.h>
#include <Windows.h>

// 压缩
typedef NTSTATUS(WINAPI* MyRtlCompressBuffer)(
	USHORT CompressionFormatAndEngine, // 压缩格式和引擎类型
	PUCHAR UncompressedBuffer, // 要压缩的数据的缓冲区
	ULONG  UncompressedBufferSize, // 要压缩的数据的缓冲区的大小
	PUCHAR CompressedBuffer, // 压缩后的数据缓冲区
	ULONG  CompressedBufferSize, // 预期压缩后的数据缓冲区的大小
	ULONG  UncompressedChunkSize, // 压缩时用的块的大小
	PULONG FinalCompressedSize, // 最终压缩后的数据的大小
	PVOID  WorkSpace // 指向工作空间的指针
	);

// 解压
typedef NTSTATUS(WINAPI* MyRtlDecompressBuffer)(
	USHORT CompressionFormat, // 压缩格式
	PUCHAR UncompressedBuffer, // 指向需要解压的数据的缓冲区的指针
	ULONG  UncompressedBufferSize, // 缓冲区的大小
	PUCHAR CompressedBuffer, // 需要解压的数据的缓冲区
	ULONG  CompressedBufferSize, // 预期解压后的缓冲区大小
	PULONG FinalUncompressedSize // 最终解压后的大小
	);

typedef NTSTATUS(WINAPI* MyRtlGetCompressionWorkSpaceSize)(
	USHORT CompressionFormatAndEngine, // 压缩格式和引擎类型
	PULONG CompressBufferWorkSpaceSize, // 压缩需要的大小
	PULONG CompressFragmentWorkSpaceSize // 解压需要的大小
	);

BOOL CompressData(BYTE* pUncompressData, DWORD dwUncompressDataLength, BYTE** ppCompressData, DWORD* pdwCompressDataLength)
{
	BOOL bRet = FALSE;
	NTSTATUS status = 0;
	HMODULE hModule = NULL;
	MyRtlGetCompressionWorkSpaceSize RtlGetCompressionWorkSpaceSize = NULL;
	MyRtlCompressBuffer RtlCompressBuffer = NULL;
	DWORD dwWorkSpaceSize = 0;
	DWORD dwFragmentWorkSpaceSize = 0;
	BYTE* pWorkSpace = NULL;
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataLength = 4096;
	DWORD dwFinalCompressSize = 0;
	do
	{
		hModule = LoadLibrary("ntdll.dll");
		if (hModule == NULL)
		{
			printf("LoadLibrary Error!\n");
			break;
		}
		RtlGetCompressionWorkSpaceSize = (MyRtlGetCompressionWorkSpaceSize)GetProcAddress(hModule, "RtlGetCompressionWorkSpaceSize");
		RtlCompressBuffer = (MyRtlCompressBuffer)GetProcAddress(hModule, "RtlCompressBuffer");
		if (RtlGetCompressionWorkSpaceSize == NULL || RtlCompressBuffer == NULL)
		{
			printf("Func GetProcAddress Failed!\n");
			break;
		}
		status = RtlGetCompressionWorkSpaceSize(COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_STANDARD, &dwWorkSpaceSize, &dwFragmentWorkSpaceSize);
		if (status != 0)
		{
			printf("Func Get Address of RtlGetCompressionWorkSpaceSize Error!\n");
			break;
		}
		// 申请内存
		pWorkSpace = new BYTE[dwWorkSpaceSize];
		if (pWorkSpace == NULL)
		{
			printf("Init pWorkSpace Failed!\n");
			break;
		}
		// 清空内存
		RtlZeroMemory(pWorkSpace, dwWorkSpaceSize);
		while (true)
		{
			// 申请内存
			pCompressData = new BYTE[dwCompressDataLength];
			if (pCompressData == NULL)
			{
				printf("Init pCompressData Failed!\n");
				break;
			}
			// 清空内存
			RtlZeroMemory(pCompressData, dwCompressDataLength);
			RtlCompressBuffer(COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_STANDARD, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, 4096, &dwFinalCompressSize, (PVOID)pWorkSpace);
			if (dwCompressDataLength < dwFinalCompressSize)
			{
				if (pCompressData)
				{
					delete[] pCompressData;
					pCompressData = NULL;
				}
				dwCompressDataLength = dwFinalCompressSize;
			}
			else
			{
				break;
			}
		}
		*ppCompressData = pCompressData;
		*pdwCompressDataLength = dwFinalCompressSize;
		bRet = TRUE;
	} while (FALSE);
	if (pWorkSpace)
	{
		delete[] pWorkSpace;
		pWorkSpace = NULL;
	}
	if (hModule)
	{
		FreeLibrary(hModule);
	}
	return bRet;
}

BOOL UncompressData(BYTE* pCompressData, DWORD dwCompressDataLength, BYTE** ppUncompressData, DWORD* pdwUncompressDataLength)
{
	BOOL bRet = FALSE;
	HMODULE hModule = NULL;
	MyRtlDecompressBuffer RtlDecompressBuffer = NULL;
	BYTE* pUncompressData = NULL;
	DWORD dwUncompressDataLength = 4096;
	DWORD dwFinalUncompressSize = 0;
	do
	{
		hModule = LoadLibrary("ntdll.dll");
		if (hModule == NULL)
		{
			printf("LoadLibrary Error!\n");
			break;
		}
		RtlDecompressBuffer = (MyRtlDecompressBuffer)GetProcAddress(hModule, "RtlDecompressBuffer");
		if (RtlDecompressBuffer == NULL)
		{
			printf("Func Get Address of RtlDecompressBuffer Error!\n");
			break;
		}
		while (true)
		{
			pUncompressData = new BYTE[dwUncompressDataLength];
			if (pUncompressData == NULL)
			{
				printf("Init pUncompressData Failed!\n");
				break;
			}
			RtlZeroMemory(pUncompressData, dwUncompressDataLength);
			RtlDecompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, &dwFinalUncompressSize);
			if (dwUncompressDataLength < dwFinalUncompressSize)
			{
				if (pUncompressData)
				{
					delete[] pUncompressData;
					pUncompressData = NULL;
				}
				dwUncompressDataLength = dwFinalUncompressSize;
			}
			else
			{
				break;
			}
		}
		*ppUncompressData = pUncompressData;
		*pdwUncompressDataLength = dwUncompressDataLength;
		bRet = TRUE;
	} while (FALSE);
	if (hModule)
	{
		FreeLibrary(hModule);
	}
	return bRet;
}

int main()
{
	BOOL bRet = FALSE;
	CHAR szBuffer[] = "DVBDFBEBREBERBVFVDSFBENRREGREREDBFBGNRTNWRBWVDBFDBQ";
	DWORD dwBufferLength = lstrlen(szBuffer);
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataLength = 0;
	BYTE* pUncompressData = NULL;
	DWORD dwUncompressDataLength = 0;

	CompressData((BYTE*)szBuffer, dwBufferLength, &pCompressData, &dwCompressDataLength);
	UncompressData(pCompressData, dwCompressDataLength, &pUncompressData, &dwUncompressDataLength);

	printf("原数据：");
	for (size_t i = 0; i < dwBufferLength; i++)
	{
		printf("%X", szBuffer[i]);
	}
	printf("\n压缩数据：");
	for (size_t i = 0; i < dwCompressDataLength; i++)
	{
		printf("%X", pCompressData[i]);
	}
	printf("\n解压数据：");
	for (size_t i = 0; i < dwUncompressDataLength; i++)
	{
		printf("%X", pUncompressData[i]);
	}
	system("pause");
	return 0;
}