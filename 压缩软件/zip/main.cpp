#include <stdio.h>
#include <Windows.h>

// ѹ��
typedef NTSTATUS(WINAPI* MyRtlCompressBuffer)(
	USHORT CompressionFormatAndEngine, // ѹ����ʽ����������
	PUCHAR UncompressedBuffer, // Ҫѹ�������ݵĻ�����
	ULONG  UncompressedBufferSize, // Ҫѹ�������ݵĻ������Ĵ�С
	PUCHAR CompressedBuffer, // ѹ��������ݻ�����
	ULONG  CompressedBufferSize, // Ԥ��ѹ��������ݻ������Ĵ�С
	ULONG  UncompressedChunkSize, // ѹ��ʱ�õĿ�Ĵ�С
	PULONG FinalCompressedSize, // ����ѹ��������ݵĴ�С
	PVOID  WorkSpace // ָ�����ռ��ָ��
	);

// ��ѹ
typedef NTSTATUS(WINAPI* MyRtlDecompressBuffer)(
	USHORT CompressionFormat, // ѹ����ʽ
	PUCHAR UncompressedBuffer, // ָ����Ҫ��ѹ�����ݵĻ�������ָ��
	ULONG  UncompressedBufferSize, // �������Ĵ�С
	PUCHAR CompressedBuffer, // ��Ҫ��ѹ�����ݵĻ�����
	ULONG  CompressedBufferSize, // Ԥ�ڽ�ѹ��Ļ�������С
	PULONG FinalUncompressedSize // ���ս�ѹ��Ĵ�С
	);

typedef NTSTATUS(WINAPI* MyRtlGetCompressionWorkSpaceSize)(
	USHORT CompressionFormatAndEngine, // ѹ����ʽ����������
	PULONG CompressBufferWorkSpaceSize, // ѹ����Ҫ�Ĵ�С
	PULONG CompressFragmentWorkSpaceSize // ��ѹ��Ҫ�Ĵ�С
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
		// �����ڴ�
		pWorkSpace = new BYTE[dwWorkSpaceSize];
		if (pWorkSpace == NULL)
		{
			printf("Init pWorkSpace Failed!\n");
			break;
		}
		// ����ڴ�
		RtlZeroMemory(pWorkSpace, dwWorkSpaceSize);
		while (true)
		{
			// �����ڴ�
			pCompressData = new BYTE[dwCompressDataLength];
			if (pCompressData == NULL)
			{
				printf("Init pCompressData Failed!\n");
				break;
			}
			// ����ڴ�
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

	printf("ԭ���ݣ�");
	for (size_t i = 0; i < dwBufferLength; i++)
	{
		printf("%X", szBuffer[i]);
	}
	printf("\nѹ�����ݣ�");
	for (size_t i = 0; i < dwCompressDataLength; i++)
	{
		printf("%X", pCompressData[i]);
	}
	printf("\n��ѹ���ݣ�");
	for (size_t i = 0; i < dwUncompressDataLength; i++)
	{
		printf("%X", pUncompressData[i]);
	}
	system("pause");
	return 0;
}